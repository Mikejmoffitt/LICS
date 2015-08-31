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
		u16 px;
		u16 py;
		state.next_id++;
		do
		{
			player_eval_grounded(&pl);
			player_input(&pl);
			player_cp(&pl);
			player_accel(&pl);
			player_jump(&pl);
			player_move(&pl);

			player_eval_grounded(&pl);
			player_calc_anim(&pl);
			px = fix32ToInt(pl.x);
			py = fix32ToInt(pl.y);

			state_update_scroll(px,py);
			player_draw(&pl);
			
			system_wait_v();
			if (pl.input & KEY_A && state.current_id == 0)
			{
				state_load_room(state.next_id);
			}
			if (pl.input & KEY_A)
			{
				map_draw_full(state.cam_x,state.cam_y);
			}
			state_watch_transitions(px,py,pl.dx,pl.dy);
			player_dma(&pl);
			sprites_dma_simple();

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
