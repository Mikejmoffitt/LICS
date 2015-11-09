#include "gameloop.h"
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
#include "projectiles.h"

static void puts(const char *s, u16 x, u16 y)
{
	while (*s)
	{
		VDP_setTileMapXY(VDP_getWindowPlanAddress(), TILE_ATTR_FULL(1, 1, 0, 0, 0x500 + *s), x, y);
		x++;
		s++;
	}
}

static void message_screen(const char *s)
{
	u16 i = 60 * 5;
	puts(s, 8, 8);
//	VDP_drawTextBG(VDP_getWindowPlanAddress(), s, TILE_ATTR(1, 1, 0, 0), 4, 6);
	VDP_setReg(0x12, 0x1E);
	while (i--)
	{
		system_wait_v();
		sprites_dma_simple();
	}
	VDP_setReg(0x12, 0x00);
}

void gameloop_logic(void)
{
	player_run();
	enemy_run();
	cubes_run();
	powerup_run();
	projectiles_run();
	particles_run();
	sfx_counters();
	if (pl.input & BUTTON_Z)
	{
		message_screen("Oh my lordy!!! It's not broken.");
		message_screen("Could it really be a success?");
		message_screen("Is that even real?");
	}
	state_update_progress();

}

void gameloop_gfx(void)
{
	/* BG updates for scrolling */
	map_draw_diffs(state_update_scroll());

	/* Place sprites */
	hud_draw_health(sram.max_hp,pl.hp);
	if (sram.have_phantom)
	{
		hud_draw_cp(pl.cp + 1 + ((pl.cp + 1) >> 1)); // CP scaled 32 --> 48
	}
	cubes_draw();
	particles_draw();
	player_draw();
	enemy_draw();
	projectiles_draw();
	powerup_draw();
	system_debug_cpu_meter();

}

void gameloop_dma(void)
{
	map_dma();
	sprites_dma_simple();
	state_dma_scroll();	
	player_dma();
}

static void gameloop_room_setup(u16 transition)
{	
	// Blank the display
	system_wait_v();
	VDP_setEnable(0);
	system_ntsc = (!IS_PALSYSTEM);

	// Reset object lists, gameplay variables, etc.
	cubes_init();
	enemy_init();
	particles_init();
	projectiles_init();
	powerup_init();
	player_init_soft();
	pause_init();

	// Depending on room entry, do some things
	if (transition == STATE_TRANSITION_UP)
	{
		player_do_jump();
	}

	// Load the next room
	state_load_room(state.next_id);

	// Locate entry position for player
	player_set_xy_fix32(state_get_entrance_x(), state_get_entrance_y());

	// DMA needed graphics for this room
	map_load_tileset(state.current_room->tileset);

	// Re-load the usuals in case something else is in VRAM there
	particles_dma_tiles();
	projectiles_dma_tiles();
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
	gameloop_logic();

	gameloop_gfx();
	gameloop_dma();

	music_play(state.current_room->music);

	system_wait_v();
	VDP_setEnable(1);
}

void gameloop_main(void)
{
	u16 transition = 0;
	state.next_id = 1;
	state.next_entrance = 0;
	state.current_id = 64;

	player_init();

	pl.input = JOY_readJoypad(JOY_1);
	if (pl.input & BUTTON_Z)	
	{
		save_clear();
	}
	if (pl.input & BUTTON_X)	
	{
		save_clear();
		sram.have_lift = 1;
		sram.have_jump = 1;
		sram.have_kick = 1;
		sram.have_phantom = 1;
		sram.have_map = 1;
	}
	// Game is in progress
	while (1)
	{
		gameloop_room_setup(transition);
		do
		{
			/* Run one frame of engine logic */
			gameloop_logic();
			gameloop_gfx();

			/* Wait for VBlank. */
			system_wait_v();

			gameloop_dma();
			if ((pl.input & BUTTON_START) && (!(pl.input_prev & BUTTON_START)))
			{
				pause_screen_loop();	
			}
		}
		while (!(transition = state_watch_transitions()));
	}
}
