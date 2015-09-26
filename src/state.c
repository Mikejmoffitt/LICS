#include "state.h"
#include "bg.h"
#include "music.h"
#include "cdaudio.h"

gamestate state;

static s16 sx_memo;
static s16 sy_memo;

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
}

// Set up a room by the specified ID.
void state_load_room(u8 roomnum)
{
	if (roomnum == state.current_id)
	{
		return;
	}
	state.cam_x = STATE_SCROLL_INVALID;
	state.cam_y = STATE_SCROLL_INVALID;
	sx_memo = STATE_SCROLL_INVALID;
	sy_memo = STATE_SCROLL_INVALID;
	state.current_room = map_by_id(roomnum);
	state.current_map = (u8 *)&(state.current_room->map_data);
	state.fresh_room = 1;

	state.current_id = roomnum;

	state_config_scrolling();
	bg_load(state.current_room->background);

	if (cdaudio_is_active())
	{
		cdaudio_play_once(state.current_room->music);
	}
	else
	{
		music_play(state.current_room->music);
	}
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
		int i = STATE_PLANE_H;
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
		int i = STATE_PLANE_W / 2;
		while (i--)
		{
			state.yscroll_vals[i] = amt;
		}
		state.yscroll_cmd = STATE_SCROLL_DMA;
	}
}

// Update scrolling camera coordinates based on player real-world coords
// Returns a bitfield describing if the camera moved, and on which axes
u16 state_update_scroll(u16 px, u16 py)
{
	state.xscroll_cmd = 0;
	state.yscroll_cmd = 0;
	py -= 16; // Offset by half of lyle's height plus a bit
	// Horizontal scrolling
	if (!state.hs_en)
	{
		// No need to scroll, single-screen room
		state.cam_x = 0;
	}
	else if (px >= (state.current_room->w * STATE_SC_W) - STATE_SC_SEAMX)
	{
		// Far right side
		state.cam_x = (state.current_room->w * STATE_SC_W) - STATE_SC_SEAMX*2;
	}
	else if (px > STATE_SC_SEAMX)
	{	
		// Between edges
		state.cam_x = (px - STATE_SC_SEAMX);
	}
	else
	{
		// Far left side
		state.cam_x = 0;
	}

	// Vertical scrolling TODO: Make euro mode less ghetto looking
	if (!state.vs_en)
	{
		state.cam_y = 240 - VDP_getScreenHeight();
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

	if (state.cam_x != sx_memo)
	{
		state_scroll_fgx(state.cam_x);
		bg_scroll_x(state.cam_x);
	}

	return (state.xscroll_cmd ? STATE_MOVED_X : 0) | (state.yscroll_cmd ? STATE_MOVED_Y : 0);
}

// Transfer scrolling information to VRAM as needed
void state_dma_scroll(void)
{
	if (state.xscroll_cmd == STATE_SCROLL_DMA)
	{
	 	VDP_setHorizontalScrollTile(PLAN_A, 0, state.xscroll_vals, STATE_PLANE_H, 1);
	}
	else if (state.xscroll_cmd)
	{
	 	VDP_setHorizontalScroll(PLAN_A, state.xscroll_vals[0]);
	}
	if (state.yscroll_cmd == STATE_SCROLL_DMA)
	{
		VDP_setVerticalScrollTile(PLAN_A, 0, state.yscroll_vals, STATE_PLANE_H, 1);
	}
	else if (state.yscroll_cmd)
	{
		VDP_setVerticalScroll(PLAN_A, state.yscroll_vals[0]);
	}
	bg_dma_scroll();
}

// Watch for the player entering/exiting a room.
u16 state_watch_transitions(u16 px, u16 py, fix16 dx, fix16 dy)
{
	if (dx == FZERO && dy == FZERO)
	{
		return 0;
	}
	else if ((px < STATE_TRANSITION_MARGIN) && (dx < FZERO))
	{
		return 1;
	}
	else if ((px > ((state.current_room->w * 320) - STATE_TRANSITION_MARGIN)) && (dx > FZERO))
	{
		return 1;
	}
	else if ((py < STATE_TRANSITION_MARGIN) && (dy < FZERO))
	{
		return 1;
	}
	else if ((py > ((state.current_room->h * 240) - STATE_TRANSITION_MARGIN)) && (dx > FZERO))
	{
		return 1;
	}
	return 0;
}
