#include "titlescreen.h"
#include <genesis.h>
#include "pal.h"
#include "gfx.h"
#include "music.h"
#include "sprites.h"
#include "system.h"
#include "music.h"

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
	u16 i = 0;
	fix16 scroll_y = FIX16(0);
	fix16 scroll_dy = 0;
	u16 logo_falling = 0;
	title_setup();
	// Blank wait
	for (i = 0; i < 30; i++)
	{
		sprites_dma_simple();
		system_wait_v();
	}
	// Logo appears; slides up
	playsound(SFX_BOGOLOGO);
	while (i < 400)
	{

		// Logo slides up after frame 120
		if (i == 120)
		{
			logo_falling = 1;
		}

		if (logo_falling)
		{
			scroll_dy = fix16Add(scroll_dy, FIX16(0.2));
			scroll_y = fix16Add(scroll_y, scroll_dy);

			// The bouncing
			if (scroll_y > FIX16(360))
			{
				if (scroll_dy > FIX16(2))
				{
					playsound(SFX_CUBEBOUNCE);
					scroll_dy = -(scroll_dy >> 1);
				}
				else if (scroll_dy <= FIX16(2) && scroll_dy > FIX16(0))
				{
					scroll_dy = FIX16(0);
					logo_falling = 0;
				}
			}
		}



		// Logo flashes for 60 frames
		if (i >= 60 || (i % 8 < 4))
		{
			place_bogologo(96, 88 - fix16ToInt(scroll_y), (i >> 3) % 2);
		}
		system_wait_v();
		sprites_dma_simple();
		i++;
	}
	// Logo slides up as scene slides down
}
