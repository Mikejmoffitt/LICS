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
#include "powerups.h"
#include "pause.h"

static inline void loop_logic(void)
{
	DEBUG_BGCOL(0x22E);
	player_run();
	DEBUG_BGCOL(0x2E2);
	enemy_run();
	DEBUG_BGCOL(0xE2E);
	cubes_run();
	DEBUG_BGCOL(0xE22);
	powerup_run();
	DEBUG_BGCOL(0x2EE);
	particles_run();
	DEBUG_BGCOL(0x444);
	sfx_counters();
	DEBUG_BGCOL(0x222);
	state_update_progress();
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
	powerup_draw();
	DEBUG_BGCOL(0x0AA);
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

	// Reset object lists, gameplay variables, etc.
	cubes_init();
	enemy_init();
	particles_init();
	powerup_init();
	player_init_soft();
	pause_init();

	// Load the next room
	state_load_room(state.next_id);

	// Locate entry position for player
	player_set_xy_fix32(state_get_entrance_x(), state_get_entrance_y());

	// DMA needed graphics for this room
	map_load_tileset(state.current_room->tileset);

	// Re-load the usuals in case something else is in VRAM there
	particles_dma_tiles();
	cube_dma_tiles();
	hud_dma_tiles();
	pause_dma_tiles();
	enemy_dma_tiles();
	powerup_dma_tiles();

	// Set up the far backdrop
	bg_load(state.current_room->background);

	// Save player's progress for frequent auto-save
	save_write();

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



	if (pl.input & BUTTON_A)	
	{
		save_clear();
	}
	// Game is in progress
	while (1)
	{
		room_setup();
		do
		{
			if (pl.input & BUTTON_START)
			{
				pause_screen_loop();	
			}
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
