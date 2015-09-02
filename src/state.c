#include "state.h"

#include "col.h"

gamestate state;

static s16 sx_memo;
static s16 sy_memo;

void state_load_room(u8 roomnum)
{
	if (roomnum == state.current_id)
	{
		return;
	}
	state.cam_x = 65535;
	state.cam_y = 65535;
	sx_memo = 65535;
	sy_memo = 65535;
	state.current_room = map_by_id(roomnum);
	state.current_map = (u8 *)&(state.current_room->map_data);

	if (state.current_music != state.current_room->music)
	{
		state.current_music = state.current_room->music;
		// Play music if it isn't already
	}
	state.current_id = roomnum;

	// Set scrolling scheme

	state.vs_en = (state.current_room->h != 1);
	state.hs_en = (state.current_room->w != 1);

	if (state.current_room->w == 1 && state.current_room->h == 1)
	{
		VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	}
	else if (state.current_room->w == 1 && state.current_room->h != 1)
	{
		VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_2TILE);
	}
	else
	{
		VDP_setScrollingMode(HSCROLL_TILE, VSCROLL_PLANE);
	}
}

// Scrolling support functions
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
		for (u16 i = 0; i < STATE_PLANE_H; i++)
		{
			state.xscroll_vals[i] = amt;
		}
		state.xscroll_cmd = STATE_SCROLL_DMA;
	}
}

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
		for (u16 i = 0; i < STATE_PLANE_W / 2; i++)
		{
			state.yscroll_vals[i] = amt;
		}
		state.yscroll_cmd = STATE_SCROLL_DMA;
	}
}

u16 state_update_scroll(u16 px, u16 py)
{
	state.xscroll_cmd = 0;
	state.yscroll_cmd = 0;
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
	}

	if (state.cam_x != sx_memo)
	{
		state_scroll_fgx(state.cam_x);
	}

	return (state.xscroll_cmd ? STATE_MOVED_X : 0) | (state.yscroll_cmd ? STATE_MOVED_Y : 0);
}

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
}

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
