#include <genesis.h>
#include "mpad.h"
#include "sprites.h"
#include "player.h"
#include "system.h"
#include "gfx.h"
#include "pal.h"
#include "map.h"
#include "mapdata.h"
#include "vramslots.h"
#include "state.h"
#include "col.h"

void room_loop(void)
{
	player pl;
	state.next_id = 1;
	state.current_id = 64;
	
	player_init(&pl);	
	// Game is in progress
	while (1)
	{
		state_load_room(state.next_id);
		player_init_soft(&pl);	
		pl.y = intToFix32(64);
		pl.x = intToFix32(64);

		u16 px = fix32ToInt(pl.x);
		u16 py = fix32ToInt(pl.y);
		// Entrance to a room requires a full redraw
		state_update_scroll(px, py);
		map_draw_full(state.cam_x, state.cam_y);
		if (state.current_room->id == 0)
		{
			col_init();
			col_puts40(7,12,"Big trouble in cube sector!");
			col_puts40(6,13,"Could not load the next room.");
			col_puts40(4,15,"Your save data will be fine, but");
			col_puts40(4,16,"you will need to reset the game.");
		}
		state.next_id++;
		do
		{
			VDP_setPaletteColor(0, 0x200);
			player_eval_grounded(&pl);
			player_input(&pl);
			player_cp(&pl);
			player_accel(&pl);
			player_jump(&pl);
			VDP_setPaletteColor(0, 0x220);
			player_move(&pl);
			VDP_setPaletteColor(0, 0x260);

			player_eval_grounded(&pl);
			player_calc_anim(&pl);
			VDP_setPaletteColor(0, 0x280);
			px = fix32ToInt(pl.x);
			py = fix32ToInt(pl.y);
			VDP_setPaletteColor(0, 0x8E8);
			state_update_scroll(px,py);
			player_draw(&pl);
			VDP_setPaletteColor(0, 0x000);
			
			system_wait_v();
			VDP_setPaletteColor(0, 0x00E);
			map_draw_vertical(state.cam_x,state.cam_y);
			VDP_setPaletteColor(0, 0x04E);
			sprites_dma_simple();
			VDP_setPaletteColor(0, 0x08C);
			player_dma(&pl);
			VDP_setPaletteColor(0, 0x000);

		}
		while (!state_watch_transitions(px,py,pl.dx,pl.dy));
	}
}

int main(void)
{
	system_init();

	room_loop();
	return 0;	
}
