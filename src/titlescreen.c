#include "titlescreen.h"
#include <genesis.h>
#include "pal.h"
#include "gfx.h"
#include "music.h"
#include "sprites.h"
#include "system.h"
#include "music.h"

#define TITLE_PLANE_W 64
#define TITLE_PLANE_H 64

static void title_init(void)
{
	// DMA graphic tile assets
	VDP_doVRamDMA((u32)gfx_bogologo, BOGOLOGO_VRAM_SLOT * 32, BOGOLOGO_VRAM_LEN * 16);
	VDP_doCRamDMA((u32)pal_bogologo, 0, 32);

	// For the intro, we're going to use a 512x512 plane size for scrolling in
	// the bottom ground area.
	VDP_setPlanSize(TITLE_PLANE_W, TITLE_PLANE_H);
	VDP_clearPlan(VDP_PLAN_A, 1);
	VDP_waitDMACompletion();
	VDP_clearPlan(VDP_PLAN_B, 1);
	VDP_waitDMACompletion();
	
	VDP_setScreenWidth320();
	system_set_h_split(0, 0, NULL);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	VDP_setHilightShadow(0);

	sprites_init();
}

static inline void place_bogologo(s16 lx, s16 ly, u16 pal)
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

static inline void scroll_movement(fix16 *scroll_dy, fix16 *scroll_y, u16 *logo_falling)
{
	*scroll_dy = fix16Add(*scroll_dy, FIX16(0.2));
	*scroll_y = fix16Add(*scroll_y, *scroll_dy);

	// The bouncing
	if (*scroll_y > FIX16(360))
	{
		if (*scroll_dy > FIX16(2))
		{
			*scroll_dy = -(*scroll_dy >> 1);
			playsound(SFX_CUBE_EXPLODE);
		}
		else if (*scroll_dy <= FIX16(2) && *scroll_dy > FIX16(0))
		{
			*scroll_dy = FIX16(0);
			playsound(SFX_CUBE_EXPLODE);
			*logo_falling = 0;
		}
	}
}

static void title_draw_bogologo(u16 i, fix16 *scroll_y)
{
	// Logo flashes for 60 frames
	if ((i >= 60 || (i % 4 < 2)) && *scroll_y <= FIX16(240))
	{
		place_bogologo(96, 88 - fix16ToInt(*scroll_y), (i >> 3) % 2);
	}
}

void title_play_intro(void)
{
	u16 i = 0;
	title_init();
	fix16 scroll_y = FIX16(0);
	fix16 scroll_dy = 0;
	u16 logo_falling = 0;
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
			scroll_movement(&scroll_dy, &scroll_y, &logo_falling);
		}

		title_draw_bogologo(i, &scroll_y);

		system_wait_v();
		sprites_dma_simple();
		i++;
	}
	// Logo slides up as scene slides down
}
