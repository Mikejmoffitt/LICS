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
#include "save.h"
#include "cubes.h"
#include "particles.h"
#include "hud.h"
#include "music.h"
#include "bg.h"
#include "enemy.h"

static inline void loop_logic(void)
{
	DEBUG_BGCOL(0x22E);
	player_run();
	DEBUG_BGCOL(0x2E2);
	enemy_run();
	DEBUG_BGCOL(0xE2E);
	cubes_run();
	DEBUG_BGCOL(0xE22);
	particles_run();
	DEBUG_BGCOL(0x444);
	sfx_counters();
}

static inline void loop_gfx(void)
{
	/* BG updates for scrolling */
	DEBUG_BGCOL(0xAA0);
	map_draw_diffs(state_update_scroll());

	DEBUG_BGCOL(0xAAA);
	/* Place sprites */
	hud_draw_health(sram.max_hp,pl.hp);
	DEBUG_BGCOL(0x666);
	if (sram.have_phantom)
	{
		hud_draw_cp(pl.cp + 1 + ((pl.cp + 1) >> 1)); // CP scaled 32 --> 48
	}
	DEBUG_BGCOL(0x00A);
	player_draw();
	DEBUG_BGCOL(0x0A0);
	enemy_draw();
	DEBUG_BGCOL(0xA00);
	particles_draw();
	DEBUG_BGCOL(0xA0A);
	cubes_draw();
	DEBUG_BGCOL(0x000);
	system_debug_cpu_meter();

}

static inline void loop_dma(void)
{
	map_dma();
	sprites_dma_simple();
	state_dma_scroll();	
	player_dma();
}

void room_setup(void)
{	
	// Blank the display
	system_wait_v();
	VDP_setEnable(0);
	cubes_init();
	enemy_init();
	particles_init();
	player_init_soft();

	state_load_room(state.next_id);
	// First entry to a room needs some extra processing
	pl.x = state_get_entrance_x();
	pl.y = state_get_entrance_y();

	// One-time graphics DMA parts
	map_load_tileset(state.current_room->tileset);
	particles_dma_tiles();
	cube_dma_tiles();
	hud_dma_tiles();
	enemy_dma_tiles();
	bg_load(state.current_room->background);

	// First graphical commit
	loop_logic();

	loop_gfx();
	loop_dma();

	music_play(state.current_room->music);

	system_wait_v();
	VDP_setEnable(1);
}

void main_game_loop(void)
{
	state.next_id = 1;
	state.next_entrance = 0;
	state.current_id = 64;

	player_init();
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
		while (!state_watch_transitions());
	}
}

int main(void)
{
	system_init();
	main_game_loop();
	return 0;	
}
