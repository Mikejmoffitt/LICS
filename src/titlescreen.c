#include "titlescreen.h"
#include <genesis.h>
#include "pal.h"
#include "gfx.h"
#include "music.h"
#include "sprites.h"
#include "system.h"
#include "music.h"
#include "map.h"
#include "state.h"
#include "bg.h"

#define TITLE_PLANE_W 64
#define TITLE_PLANE_H 64

static void title_init(void)
{

	// For the intro, we're going to use a 512x512 plane size for scrolling in
	// the bottom ground area.
	VDP_setEnable(0);
	VDP_setBPlanAddress(0xC000);
	VDP_setWindowAddress(0xA000);
	VDP_setAPlanAddress(0xA000);
	VDP_setSpriteListAddress(0xF000);

	VDP_setPlanSize(TITLE_PLANE_W, TITLE_PLANE_H);
	VDP_clearPlan(VDP_PLAN_A, 1);
	VDP_waitDMACompletion();
	VDP_clearPlan(VDP_PLAN_B, 1);
	VDP_waitDMACompletion();
	
	VDP_setScreenWidth320();
	system_set_h_split(0, 0, NULL);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	VDP_setHilightShadow(0);

	VDP_setVerticalScroll(PLAN_B, 16);
	VDP_setVerticalScroll(PLAN_A, 16);


	VDP_setVerticalScroll(PLAN_B, 256);
	VDP_setVerticalScroll(PLAN_A, 256);

	VDP_setHorizontalScroll(PLAN_B, -30);
	VDP_setHorizontalScroll(PLAN_A, -120);

	sprites_init();
	map_init();
	map_load_tileset(MAP_SET_OUTSIDE1);
	state.current_room = map_by_id(1);
	state.current_map = (u8 *)&(state.current_room->map_data);

	map_draw_full(624, 0);
	system_wait_v();
	// DMA graphic tile assets - overwrites totally unused enemy graphics. fine.
	VDP_doVRamDMA((u32)gfx_bogologo, BOGOLOGO_VRAM_SLOT * 32, BOGOLOGO_VRAM_LEN * 16);
	bg_load(255);
	bg_load(1);
	map_dma();
	VDP_setEnable(1);

}

static inline void place_bogologo(s16 lx, s16 ly)
{
	for (u16 y = 0; y < 6; y++)
	{
		for (u16 x = 0; x < 4; x++)
		{
			sprite_put(lx + (x * 32), ly + (y * 8), SPRITE_SIZE(4, 1), TILE_ATTR_FULL(
				2, 1, 0, 0,
				BOGOLOGO_VRAM_SLOT + (x * 4) + (y * 16)));


		}
	}
}

static inline void scroll_movement(fix16 *scroll_dy, fix16 *scroll_y, u16 *logo_falling)
{
	*scroll_dy = fix16Add(*scroll_dy, system_ntsc ? FIX16(0.2) : FIX16(0.24));
	*scroll_y = fix16Add(*scroll_y, *scroll_dy);

	// The bouncing
	if (*scroll_y > FIX16(360))
	{
		if (*scroll_dy > FIX16(2))
		{
			*scroll_dy = -(*scroll_dy >> 1);
			playsound(SFX_CUBE_EXPLODE);
			*scroll_y = FIX16(360);
		}
		else if (*scroll_dy <= FIX16(2) && *scroll_dy > FIX16(0))
		{
			*scroll_dy = FIX16(0);
			playsound(SFX_CUBE_EXPLODE);
			*logo_falling = 0;
			*scroll_y = FIX16(360);
		}
	}
}

static void title_draw_bogologo(u16 i, fix16 *scroll_y)
{
	// Logo flashes for 60 frames
	if ((i >= 60 || (i % 4 < 2)) && *scroll_y <= FIX16(240))
	{
		place_bogologo(96, 88 - fix16ToInt(*scroll_y));
	}
}

static void bogologo_palette(u16 i)
{
	if ((i >> 3) % 2 == 0)
	{
		VDP_doCRamDMA((u32)pal_bogologo, 64, 16);
	}
	else
	{
		VDP_doCRamDMA((u32)pal_bogologo+32, 64, 16);
	}
}

static void title_bg_scroll(fix16 scroll_y)
{
	s16 calc_scroll = 256 + fix16ToInt(scroll_y) - (system_ntsc ? 88 : 105);
	if (scroll_y < FIX16(96))
	{
		calc_scroll = 256;
	}
	VDP_setVerticalScroll(PLAN_A, calc_scroll);
	if (!system_ntsc && scroll_y >= FIX16(96))
	{
		calc_scroll += 16;
	}
	VDP_setVerticalScroll(PLAN_B, calc_scroll);

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
		// Logo slides up after a delay
		if (i == (system_ntsc ? 120 : 100))
		{
			logo_falling = 1;
		}

		if (logo_falling)
		{
			scroll_movement(&scroll_dy, &scroll_y, &logo_falling);
		}

		title_draw_bogologo(i, &scroll_y);

		system_wait_v();
		title_bg_scroll(scroll_y);
		if (scroll_y <= FIX16(240))
		{
			bogologo_palette(i);
		}

		sprites_dma_simple();
		i++;
	}
	// Logo slides up as scene slides down
}
