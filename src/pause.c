#include "pause.h"
#include "player.h"
#include "vramslots.h"
#include "gfx.h"
#include "pal.h"
#include "system.h"
#include "pausemap.h"
#include "sprites.h"
#include "save.h"
#include "hud.h"
#include "state.h"
#include "music.h"
#include "gameloop.h"

static void pause_pal_cycle(void)
{

	const u16 tele_colors[] =
	{
		0xEEE, 0x8EE, 0x0EE, 0x0CE,
		0x0AE, 0x0CE, 0x0EE, 0x8EE
	};

	const u16 special_colors[] =
	{
		0xEEE, 0xECC, 0xE88, 0xE66,
		0xE44, 0xE66, 0xE88, 0xECC
	};

	if (system_osc % 16 >= 8)
	{
		VDP_setPaletteColor(59, 0x0E00);
	}
	else
	{
		VDP_setPaletteColor(59, 0x0800);
	}

	VDP_setPaletteColor(56, special_colors[(system_osc / 16) % 8]);
	VDP_setPaletteColor(55, tele_colors[(system_osc / 8) % 8]);
}

static void pause_place_sprites(void)
{
	if (sram.have_map)
	{
		// Flashing sequence for the "Here" indicator
		switch (system_osc % 32)
		{
			case 0:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 11:
				break;

			default:
			sprite_put(8 * (PAUSE_MAP_X + state.world_x) - 5,
					   8 * (PAUSE_MAP_Y + state.world_y) - 14,
					   SPRITE_SIZE(2,2),
					   TILE_ATTR_FULL(PAUSE_PALNUM, 1, 0, 0, PAUSE_VRAM_SLOT + 0xA8));
			sprite_put(8 * (PAUSE_MAP_X +  state.world_x),
					   8 * (PAUSE_MAP_Y + state.world_y),
					   SPRITE_SIZE(1,1),
					   TILE_ATTR_FULL(PAUSE_PALNUM, 1, 0, 0, PAUSE_VRAM_SLOT + 0xC0));
		}
	}

	// Abilities

	// Cube lifting / tossing
	if (sram.have_lift)
	{
		sprite_put(PAUSE_POWER_X, PAUSE_POWER_Y, SPRITE_SIZE(2,2), TILE_ATTR_FULL(PAUSE_PALNUM, 1, 0, 0, PAUSE_VRAM_SLOT + 0xE4));
	}

	// Cube jumping
	if (sram.have_jump)
	{
		sprite_put(PAUSE_POWER_X + 28, PAUSE_POWER_Y, SPRITE_SIZE(2,2), TILE_ATTR_FULL(PAUSE_PALNUM, 1, 0, 0, PAUSE_VRAM_SLOT + 0xE8));
	}

	// CP power levels
	// Order is:
	// 		have_phantom
	//		have_double_phantom
	//		have_fast_phantom
	//		have_cheap_phantom
	if (sram.have_phantom)
	{
		u16 phantom_x = PAUSE_POWER_X + 60;
		sprite_put(phantom_x, PAUSE_POWER_Y, SPRITE_SIZE(2,2), TILE_ATTR_FULL(PAUSE_PALNUM, 1, 0, 0, PAUSE_VRAM_SLOT + 0xF4));

		// Show level counter
		if (sram.have_cheap_phantom)
		{
			sprite_put(phantom_x + 1, PAUSE_POWER_Y + 18, SPRITE_SIZE(2,1), TILE_ATTR_FULL(PAUSE_PALNUM, 1, 0, 0, PAUSE_VRAM_SLOT + 0xB6));
		}
		else if (sram.have_fast_phantom)
		{
			sprite_put(phantom_x + 1, PAUSE_POWER_Y + 18, SPRITE_SIZE(2,1), TILE_ATTR_FULL(PAUSE_PALNUM, 1, 0, 0, PAUSE_VRAM_SLOT + 0xB4));

		}
		else if (sram.have_double_phantom)
		{
			sprite_put(phantom_x + 1, PAUSE_POWER_Y + 18, SPRITE_SIZE(2,1), TILE_ATTR_FULL(PAUSE_PALNUM, 1, 0, 0, PAUSE_VRAM_SLOT + 0xB2));
		}
		else
		{
			sprite_put(phantom_x + 1, PAUSE_POWER_Y + 18, SPRITE_SIZE(2,1), TILE_ATTR_FULL(PAUSE_PALNUM, 1, 0, 0, PAUSE_VRAM_SLOT + 0xB0));
		}
	}

	// Kicking
	if (sram.have_kick)
	{
		sprite_put(PAUSE_POWER_X + 92, PAUSE_POWER_Y, SPRITE_SIZE(2,2), TILE_ATTR_FULL(PAUSE_PALNUM, 1, 0, 0, PAUSE_VRAM_SLOT + 0xEC));
	}

	// Orange cube lifting
	if (sram.have_orange)
	{
		sprite_put(PAUSE_POWER_X + 120, PAUSE_POWER_Y, SPRITE_SIZE(2,2), TILE_ATTR_FULL(PAUSE_PALNUM, 1, 0, 0, PAUSE_VRAM_SLOT + 0xF0));
	}
}

// Set the given window plane tile
static void window_tile_set(u16 x, u16 y, u16 tile)
{
	vu32 *plctrl;
	vu16 *pwdata;
	u32 vaddr = VDP_getWindowAddress() + (x * 2) + (y << 7);

	plctrl = (u32 *)GFX_CTRL_PORT;
	pwdata = (u16 *)GFX_DATA_PORT;

	*plctrl = GFX_WRITE_VRAM_ADDR(vaddr);
	*pwdata = tile;
}

// Set all unexplored areas to be blank
static void map_progress_cover(void)
{
	u16 y, x;
	for (y = 0; y < SAVE_MAP_H; y++)
	{
		for (x = 0; x < SAVE_MAP_W; x++)
		{
			if (sram.map[y][x] == 0 || !sram.have_map)
			{
				window_tile_set(PAUSE_MAP_X + x, PAUSE_MAP_Y + y, 0xe220);
			}
		}
	}
	if (!sram.have_map)
	{
		for (x = 0; x < 5; x++)
		{
		window_tile_set(PAUSE_MAP_X + 10 + x, PAUSE_MAP_Y + 6,
			TILE_ATTR_FULL(3, 1, 0, 0, 0x1db + x));
		window_tile_set(PAUSE_MAP_X + 10 + x, PAUSE_MAP_Y + 7,
			TILE_ATTR_FULL(3, 1, 0, 0, 0x1eb + x));
		}
	}
}

// --------------------

void pause_init(void)
{
	// Get rid of the window plane
	VDP_setReg(0x12,0x00);
}

void pause_dma_tiles(void)
{
	VDP_doVRamDMA((u32)gfx_pause, PAUSE_VRAM_SLOT * 32, PAUSE_VRAM_LEN * 16);
}

static void pause_intro_anim(void)
{
	u16 win_v = 0x9F;
	while (win_v > 0x82)
	{
		win_v-=1;
		// Window plane vertical
		VDP_setReg(0x12,win_v);
		gameloop_gfx();
		system_wait_v();
		// Set the palette at the window seam so Lyle and Co don't look weird
		if (win_v < 0x9E)
		{
			system_set_h_split((win_v - 128 - 2) * 8, 3, (u16 *)pal_pause);
		}
		VDP_doCRamDMA((u32)pal_lyle, PLAYER_PALNUM * 32, 16);
		sprites_dma_simple();
	}
}

static void pause_exit_anim(void)
{
	u16 win_v = 0x20;
	while (win_v > 0x02)
	{
		win_v-=1;
		VDP_setReg(0x12,win_v);
		gameloop_gfx();
		system_wait_v();
		// Set the palette at the window seam so Lyle and Co don't look weird
		system_set_h_split((win_v - 2) * 8, 3, (u16 *)pal_lyle);
		VDP_doCRamDMA((u32)pal_pause, PLAYER_PALNUM * 32, 16);
		sprites_dma_simple();
	}
}

void pause_setup(void)
{
	// Copy the layout information for the window plane
	system_wait_v();

	VDP_doVRamDMA((u32)pausemap_layout, VDP_getWindowAddress(), (64 * 32));

	// Not sure why the DMA seems to miss this, so this is a manual fix.
	window_tile_set(0,0, TILE_ATTR_FULL(3, 1, 0, 0, PAUSE_VRAM_SLOT));

	// Play the "pause sound" which mutes three FM channels
	stopsound();
	playsound(SFX_PAUSE);

	// Cover unexplored areas on the map
	map_progress_cover();

	pause_intro_anim();

	// Bring in the colors
	VDP_doCRamDMA((u32)pal_pause, PAUSE_PALNUM * 32, 16);

	// Fullscreen Window plane
	VDP_setReg(0x12,0x1E);

	// Save the player's progress, why not?
	save_write();
}

void pause_exit(void)
{
	playsound(SFX_PAUSE);
	pause_exit_anim();
	// no Window plane here
	VDP_setReg(0x12,0x00);

	// Bring in the colors
	VDP_doCRamDMA((u32)pal_lyle, PAUSE_PALNUM * 32, 16);
	system_set_h_split(0, 0, NULL);
	stopsound();
}

void pause_screen_loop(void)
{
	u16 in_prog = 1;
	pause_setup();
	system_set_h_split(0, 0, NULL);
	while (in_prog)
	{
		if ((buttons & BUTTON_START) && (!(buttons_prev & BUTTON_START)))
		{
			in_prog = 0;
		}
		pause_place_sprites();
		gameloop_gfx();
		system_wait_v();
		sprites_dma_simple();
		pause_pal_cycle();
	}
	pause_exit();
}
