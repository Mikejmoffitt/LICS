#include "state.h"
#include "bg.h"
#include "music.h"
#include "cubes.h"
#include "player.h"
#include "system.h"
#include "vramslots.h"
#include "pal.h"
#include "save.h"
#include "enemy.h"
#include "enemy_types.h"
#include "items.h"

gamestate state;

static s16 sx_memo;
static s16 sy_memo;

static inline entrance *state_entrance_by_num(u16 num)
{
	return &state.entrances[num];
}

// Go through the object list, doing whatever setup is required
static void state_parse_objects(void)
{
	u16 i = STATE_NUM_ENTRANCES;
	while (i--)
	{
		state.entrances[i].x = 65535;
		state.entrances[i].y = 65535;
	}
	i = MAP_NUM_OBJS;
	map_list_obj *o = &(state.current_room->objects[0]);
	entrance *d;
	en_generic *e;
	(void)e;
	while (i--)
	{
		o = &(state.current_room->objects[i]);
		switch (o->type)
		{
			case ENEMY_NULL:
				continue;
			case ENEMY_ENTRANCE:
				d = &state.entrances[o->data & 0x000F];
				d->x = o->x;
				d->y = o->y;
				d->to_num = (o->data & 0x00F0) >> 4;
				d->to_roomid = (o->data & 0xFF00) >> 8;
				break;
			case ENEMY_CUBE:
				cube_spawn(o->x - CUBE_LEFT, o->y - CUBE_TOP, o->data, CUBE_STATE_IDLE, 0, 0);
				break;
			default:
				enemy_place(o->x, o->y, o->type, o->data);
				break;
		}
	}
}

static void state_config_scrolling(void)
{
	state.vs_en = (state.current_room->h != 1);
	state.hs_en = (state.current_room->w != 1);
	// 1 x many rooms - 2tile V scroll
	if (state.current_room->w == 1 && state.current_room->h != 1)
	{
		VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_2TILE);
	}
	// many x 1 rooms - tile H scroll
	else if (state.current_room->w > 1 && state.current_room->h == 1)
	{
		VDP_setScrollingMode(HSCROLL_TILE, VSCROLL_PLANE);
	}
	// 1x1, or many x many rooms - just plane scroll
	else
	{
		VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	}

	state.bg_cam_x_offset = 0;

	VDP_setReg(0x0B, VDP_getReg(0x0B) | 0x80);
}

// Set up a room by the specified ID.
void state_load_room(u8 roomnum)
{
	state.cam_x = STATE_SCROLL_INVALID;
	state.cam_y = STATE_SCROLL_INVALID;
	sx_memo = STATE_SCROLL_INVALID;
	sy_memo = STATE_SCROLL_INVALID;

	state.current_room = map_by_id(roomnum);
	state.current_map = (u8 *)&(state.current_room->map_data);
	state.fresh_room = 1;
	state.current_id = roomnum;

	state_config_scrolling();
	state_parse_objects();

	state.bound_x = state.current_room->w * 320;
	state.bound_y = state.current_room->h * 240;
}

// Set the entire H scroll foreground table to amt
static void state_scroll_fgx(s16 amt)
{
	sx_memo = amt;
	amt = amt * -1;
	if (VDP_getHorizontalScrollingMode() == HSCROLL_PLANE)
	{
		state.xscroll_vals[0] = amt;
		state.xscroll_cmd = STATE_SCROLL_SINGLE;
	}
	else
	{
		int i = VDP_getPlanHeight();
		while (i--)
		{
			state.xscroll_vals[i] = amt;
		}
		state.xscroll_cmd = STATE_SCROLL_DMA;
	}
}

// Set the entire foreground Y scroll table to amt
static void state_scroll_fgy(s16 amt)
{
	sy_memo = amt;
	if (VDP_getVerticalScrollingMode() == VSCROLL_PLANE)
	{
		state.yscroll_vals[0] = amt;
		state.yscroll_cmd = STATE_SCROLL_SINGLE;
	}
	else
	{
		int i = VDP_getPlanWidth() / 2;
		while (i--)
		{
			state.yscroll_vals[i] = amt;
		}
		state.yscroll_cmd = STATE_SCROLL_DMA;
	}
}

// Update scrolling camera coordinates based on player real-world coords
// Returns a bitfield describing if the camera moved, and on which axes
u16 state_update_scroll()
{
	state.xscroll_cmd = 0;
	state.yscroll_cmd = 0;
	u16 py = pl.py -16; // Offset by half of lyle's height plus a bit
	// Horizontal scrolling
	if (!state.hs_en)
	{
		// No need to scroll, single-screen room
		state.cam_x = 0;
	}
	else if (pl.px >= (state.current_room->w * STATE_SC_W) - STATE_SC_SEAMX)
	{
		// Far right side
		state.cam_x = (state.current_room->w * STATE_SC_W) - STATE_SC_SEAMX*2;
	}
	else if (pl.px > STATE_SC_SEAMX)
	{
		// Between edges
		state.cam_x = (pl.px - STATE_SC_SEAMX);
	}
	else
	{
		// Far left side
		state.cam_x = 0;
	}

	// Vertical scrolling TODO: Make euro mode less ghetto looking
	if (!state.vs_en)
	{
		state.cam_y = (system_ntsc) ? 16 : 0;
	}
	else if (py >= (state.current_room->h * STATE_SC_H) - (VDP_getScreenHeight() / 2))
	{
		state.cam_y = (state.current_room->h * STATE_SC_H) - (VDP_getScreenHeight());
	}
	else if (py > VDP_getScreenHeight() / 2)
	{
		state.cam_y = (py - VDP_getScreenHeight() / 2);
	}
	else
	{
		state.cam_y = 0;
	}
	if (state.cam_y != sy_memo)
	{
		state_scroll_fgy(state.cam_y);
		bg_scroll_y(state.cam_y);
	}

	if (state.cam_x != sx_memo || state.bg_cam_x_offset != 0)
	{
		state_scroll_fgx(state.cam_x);
		bg_scroll_x(state.cam_x + state.bg_cam_x_offset);
	}

	return (state.xscroll_cmd ? STATE_MOVED_X : 0) | (state.yscroll_cmd ? STATE_MOVED_Y : 0);
}

// Transfer scrolling information to VRAM as needed
void state_dma_scroll(void)
{
	if (state.xscroll_cmd == STATE_SCROLL_DMA)
	{
	 	VDP_setHorizontalScrollTile(PLAN_A, 0, state.xscroll_vals, VDP_getPlanHeight(), 1);
	}
	else if (state.xscroll_cmd)
	{
	 	VDP_setHorizontalScroll(PLAN_A, state.xscroll_vals[0]);
	}
	if (state.yscroll_cmd == STATE_SCROLL_DMA)
	{
		VDP_setVerticalScrollTile(PLAN_A, 0, state.yscroll_vals, VDP_getPlanHeight(), 1);
	}
	else if (state.yscroll_cmd)
	{
		VDP_setVerticalScroll(PLAN_A, state.yscroll_vals[0]);
	}
	bg_dma_scroll();
}

fix32 state_get_entrance_x(void)
{
	return (intToFix32(state_entrance_by_num(state.next_entrance)->x + 8));
}

fix32 state_get_entrance_y(void)
{
	return (intToFix32(state_entrance_by_num(state.next_entrance)->y + 31));
}

// Watch for the player entering/exiting a room.
u16 state_watch_transitions()
{
	if (state.teleflag)
	{
		state.teleflag = 0;
		return STATE_TRANSITION_TELE;
	}
	else if (pl.dx == FZERO && pl.dy == FZERO)
	{
		return STATE_TRANSITION_NONE;
	}
	else if ((pl.px < STATE_TRANSITION_MARGIN) && (pl.dx < FZERO))
	{
		return STATE_TRANSITION_LEFT;
	}
	else if ((pl.px > (state.bound_x - STATE_TRANSITION_MARGIN)) && (pl.dx > FZERO))
	{
		return STATE_TRANSITION_RIGHT;
	}
	else if ((pl.py - PLAYER_HEIGHT < STATE_TRANSITION_MARGIN ) && (pl.dy < FZERO))
	{
		return STATE_TRANSITION_UP;
	}
	else if ((pl.py > (state.bound_y - STATE_TRANSITION_MARGIN - 8)) && (pl.dy > FZERO))
	{
		return STATE_TRANSITION_DOWN;
	}
	return 0;
}

static u16 crap_division(u16 num, u16 den)
{
	u16 ret = 0;
	while (num >= den)
	{
		ret++;
		num -= den;
	}
	return ret;
}

// Indicator on pause map
void state_update_progress(void)
{
	// No need to save progress on a room that shouldn't exist
	if (state.current_id == 0)
	{
		return;
	}
	state.world_x = state.current_room->map_x + crap_division(pl.px, STATE_SC_W);
	state.world_y = state.current_room->map_y + crap_division(pl.py, STATE_SC_H);

	// Record progress
	sram.map[state.world_y][state.world_x] = 1;
}
