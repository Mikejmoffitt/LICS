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
	VDP_doCRamDMA((u32)pal_bogologo, 0, 32);
}

static void place_bogologo(s16 lx, s16 ly, u16 pal)
{
	for (u16 y = 0; y < 6; y++)
	{
		for (u16 x = 0; x < 4; x++)
		{
			sprite_put(lx + (x * 32), ly + (y * 8), SPRITE_SIZE(4, 1), TILE_ATTR_FULL(
				pal, 1, 0, 0,
				BOGOLOGO_VRAM_SLOT + (x * 4) + (y * 16)));
		}
	}
}

void title_play_intro(void)
{
	title_setup();
	// Blank wait
	for (u16 i = 0; i < 30; i++)
	{
		sprites_dma_simple();
		system_wait_v();
	}
	for (u16 i = 0; i < 200; i++)
	{
		place_bogologo(96, 88, (i >> 3) % 2);
		system_wait_v();
		sprites_dma_simple();
	}
}
