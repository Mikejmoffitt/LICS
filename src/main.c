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
#include "particles.h"
#include "hud.h"

#define DEBUG_BGCOL(x) if (debug_bgcol) { VDP_setPaletteColor(0,x); }

u16 debug_bgcol;

void room_setup(player *pl)
{	
	int i;
	// Blank the display
	VDP_setEnable(0);
	state_load_room(state.next_id);
	player_init_soft(pl);
	particles_init();
	cubes_init();
	// First entry to a room needs some extra processing
	pl->y = intToFix32(64);
	pl->x = intToFix32(64);

	u16 px = fix32ToInt(pl->x);
	u16 py = fix32ToInt(pl->y);

	// One-time graphics DMA parts
	map_load_tileset(state.current_room->tileset);
	particles_dma_tiles();
	cube_dma_tiles();
	hud_dma_tiles();

	for (i = 0; i < 6; i++)
	{
		cube_spawn(128 + (32 * i),(240 - 33 + 8), CUBE_BLUE, CUBE_STATE_IDLE, 0, 0);
	}

	// First graphical commit
	state_update_scroll(px, py);
	player_draw(pl);
	state_dma_scroll();
	system_wait_v();
	system_wait_v();
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
		do
		{
			/* Run one frame of engine logic */
			DEBUG_BGCOL(0x200);
			player_run(&pl);
			DEBUG_BGCOL(0x204);
			cubes_run(&pl);
			particles_run();
			px = fix32ToInt(pl.x);
			py = fix32ToInt(pl.y);
			DEBUG_BGCOL(0x8E8);

			/* Updating scroll and sprite caches, drawn top-down */
			DEBUG_BGCOL(0x444);
			map_draw_diffs(state_update_scroll(px,py),pl.dx,pl.dy);
			DEBUG_BGCOL(0x282);
			hud_draw_health(8,pl.hp); 
			hud_draw_cp(pl.cp + 1 + ((pl.cp + 1) >> 1)); // CP scaled 32 --> 48
			particles_draw();
			player_draw(&pl);
			cubes_draw();
			
			/* Wait for VBlank. */
			DEBUG_BGCOL(0x000);
			system_wait_v();

			/* Data hauls ass to VRAM with precious little DMA bandwidth */
			DEBUG_BGCOL(0x00E);
			map_dma();
			DEBUG_BGCOL(0x28E);
			sprites_dma_simple();
			DEBUG_BGCOL(0xE0E);
			state_dma_scroll();	
			DEBUG_BGCOL(0x0EE);
			player_dma(&pl);
			// Enable the VDP here at the end. This is to hide frame 0
			VDP_setEnable(1);

			if (pl.input & KEY_A)
			{
				pl.cp = 30;
			}

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
