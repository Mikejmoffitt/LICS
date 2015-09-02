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
#include "save.h"
#include "cubes.h"

#define DEBUG_BGCOL(x) if (debug_bgcol) { VDP_setPaletteColor(0,x); }

u16 debug_bgcol;

void room_setup(player *pl)
{
	cube_dma_tiles();
	hud_dma_tiles();
	state_load_room(state.next_id);
	player_init_soft(pl);

	// First entry to a room needs some extra processing
	pl->y = intToFix32(64);
	pl->x = intToFix32(64);

	u16 px = fix32ToInt(pl->x);
	u16 py = fix32ToInt(pl->y);

	// First room entry needs a redraw
	state_update_scroll(px, py);
	player_draw(&pl);
	map_draw_full(state.cam_x, state.cam_y);
	state_dma_scroll();

}

void room_loop(void)
{
	state.next_id = 1;
	state.current_id = 64;

	player pl;
	
	player_init(&pl);
	// Game is in progress
	while (1)
	{
		room_setup(&pl);
		state.next_id++;
		u16 px;
		u16 py;
		u16 i;
		do
		{
			i++;
			if (i == 48)
			{
				i = 0;
			}
			/* Engine processing */
			DEBUG_BGCOL(0x200);
			player_special_counters(&pl);
			player_input(&pl);
			player_cp(&pl);
			player_accel(&pl);
			player_jump(&pl);
			DEBUG_BGCOL(0x220);
			player_move(&pl);
			DEBUG_BGCOL(0x260);
			player_toss_cubes(&pl);
			player_lift_cubes(&pl);

			player_eval_grounded(&pl);
			player_calc_anim(&pl);
			player_dma_setup(&pl);
			DEBUG_BGCOL(0x280);
			px = fix32ToInt(pl.x);
			py = fix32ToInt(pl.y);
			DEBUG_BGCOL(0x8E8);

			/* Updating scroll and sprite caches */
			// hud_draw_health(sram.max_hp,pl.hp); 
			hud_draw_health(8,pl.hp); 
			DEBUG_BGCOL(0xE88);
			hud_draw_cp(pl.cp + (pl.cp >> 1)); 
			u16 moved = state_update_scroll(px,py);
			player_draw(&pl);
			DEBUG_BGCOL(0x444);
			if (moved & STATE_MOVED_Y) { map_draw_vertical(state.cam_x,state.cam_y,pl.dy > FZERO); }
			if (moved & STATE_MOVED_X) { map_draw_horizontal(state.cam_x,state.cam_y,pl.dx > FZERO); }
			if (pl.input & KEY_A) { debug_bgcol = 1; }
			DEBUG_BGCOL(0x000);
			
			/* Once VBlank begins, transfer all our data! */
			system_wait_v();

			// Let's DMA, quick
			DEBUG_BGCOL(0x00E);
			map_dma();
			DEBUG_BGCOL(0x28E);
			sprites_dma_simple();
			DEBUG_BGCOL(0xE0E);
			state_dma_scroll();	
			DEBUG_BGCOL(0x0EE);
			player_dma(&pl);

		}
		while (!state_watch_transitions(px,py,pl.dx,pl.dy));
	}
}

int main(void)
{
	system_init();
	save_clear();
	room_loop();
	return 0;	
}
