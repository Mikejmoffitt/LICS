#include "state.h"

gamestate state;

void state_load_room(u8 roomnum)
{
	state.cam_x = 0;
	state.cam_y = 0;
	state.current_room = map_by_id(roomnum);
	state.current_map = (u8 *)&(state.current_room->map_data);
	map_load_tileset(state.current_room->tileset);
	
}

static void state_scroll_fgx(s16 amt)
{
	amt = amt * -1;
	for (int i = 0; i < 32; i++)
	{
		VDP_setHorizontalScrollTile(PLAN_A, i, &amt, 1, 0);
	}
}

static void state_scroll_fgy(s16 amt)
{
	for (int i = 0; i < 32; i++)
	{
		VDP_setVerticalScrollTile(PLAN_A, i, &amt, 1, 0);
	}
}

void state_update_scroll(u16 px, u16 py)
{
	px += STATE_SC_OFFX;
	// Horizontal scrolling
	if (state.current_room->w <= 1)
	{
		state.cam_x = 0;
	}
	else if (px >= (state.current_room->w * STATE_SC_W) - STATE_SC_SEAMX)
	{
		state.cam_x = (state.current_room->w * STATE_SC_W) - STATE_SC_SEAMX*2;
	}
	else if (px > STATE_SC_SEAMX)
	{
		state.cam_x = (px - STATE_SC_SEAMX);
	}
	else
	{
		state.cam_x = 0;
	}
	state_scroll_fgx(state.cam_x);

	// Vertical scrolling
	if (state.current_room->h <= 1)
	{
		// Scroll down half a tile, since our screen height is 224
		state.cam_y = 8;
	}
	else if (py >= (state.current_room->h * STATE_SC_H) - STATE_SC_SEAMY)
	{
		state.cam_y = (state.current_room->h * STATE_SC_H) - STATE_SC_SEAMY*2;
	}
	else if (py > STATE_SC_SEAMY)
	{
		state.cam_y = (py - STATE_SC_SEAMY);
	}
	else
	{
		state.cam_y = 0;
	}
	state_scroll_fgy(state.cam_y);

}
