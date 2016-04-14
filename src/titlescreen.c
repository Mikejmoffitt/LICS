#include "titlescreen.h"
#include <genesis.h>
#include "pal.h"
#include "gfx.h"
#include "music.h"
#include "sprites.h"
#include "system.h"

static void title_setup(void)
{
	// DMA graphic tile assets
	VDP_doVRamDMA((u32)gfx_bogologo, BOGOLOGO_VRAM_SLOT * 32, BOGOLOGO_VRAM_LEN * 16);
}

static void place_bogologo(s16 x, s16 y)
{
	for (u16 y = 0; y < 6; y++)
	{
		for (u16 x = 0; x < 4; x++)
		{
			sprite_put(x, y, SPRITE_SIZE(4, 1), TILE_ATTR_FULL(
				0, 1, 0, 0,
				BOGOLOGO_VRAM_SLOT + (x * 4) + (y * 16)));
		}
	}
}

void title_play_intro(void)
{
	title_setup();
	// Blank wait
	for (u16 i = 0; i < 200; i++)
	{
		place_bogologo(64, 64);
		
		system_wait_v();
		sprites_dma_simple();
	}
}
