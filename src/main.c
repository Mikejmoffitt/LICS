#include <genesis.h>
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
#include "music.h"
#include "bg.h"
#include "cdaudio.h"

#define DEBUG_BGCOL(x) if (debug_bgcol) { VDP_setPaletteColor(0,x); }

u16 debug_bgcol;

player pl;

static inline void loop_logic(void)
{
	player_run(&pl);
	cubes_run(&pl);
	particles_run();
	sfx_counters();
}

static inline void loop_gfx(void)
{
	/* BG updates for scrolling */
	map_draw_diffs(state_update_scroll(pl.px,pl.py),pl.dx,pl.dy);

	/* Place sprites */
	hud_draw_health(8,pl.hp); 
	hud_draw_cp(pl.cp + 1 + ((pl.cp + 1) >> 1)); // CP scaled 32 --> 48
	player_draw(&pl);
	particles_draw();
	cubes_draw();
}

static inline void loop_dma(void)
{
	map_dma();
	sprites_dma_simple();
	state_dma_scroll();	
	player_dma(&pl);
}

void room_setup(void)
{	
	// Blank the display
	system_wait_v();

	VDP_clearPlan(VDP_PLAN_A,1);
	VDP_waitDMACompletion();
	VDP_clearPlan(VDP_PLAN_B,1);
	VDP_waitDMACompletion();

	cubes_init();
	particles_init();
	hud_draw_health(8,pl.hp); 
	hud_draw_cp(pl.cp + 1 + ((pl.cp + 1) >> 1)); // CP scaled 32 --> 48
	sprites_dma_simple();
	player_init_soft(&pl);

	state_load_room(state.next_id);
	// First entry to a room needs some extra processing
	pl.x = state_get_entrance_x();
	pl.y = state_get_entrance_y();

	// One-time graphics DMA parts
	map_load_tileset(state.current_room->tileset);
	particles_dma_tiles();
	cube_dma_tiles();
	hud_dma_tiles();
	bg_load(state.current_room->background);

	// First graphical commit
	loop_logic();
	loop_gfx();
	loop_dma();
}

void room_loop(void)
{
	state.next_id = 1;
	state.next_entrance = 0;
	state.current_id = 64;

	player_init(&pl);
	// Game is in progress
	while (1)
	{
		room_setup();
		do
		{
			/* Run one frame of engine logic */
			loop_logic();
			loop_gfx();

			/* Wait for VBlank. */
			system_wait_v();

			loop_dma();
		}
		while (!state_watch_transitions(pl.px,pl.py,pl.dx,pl.dy));
	}
}

int main(void)
{
	system_init();
	room_loop();
	return 0;	
}
