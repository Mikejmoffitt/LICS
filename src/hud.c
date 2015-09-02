#include "hud.h"
#include "sprites.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"

void hud_dma_tiles(void)
{
	VDP_doVRamDMA(gfx_hud,HUD_VRAM_SLOT * 32, HUD_VRAM_LEN * 16);
}

void hud_draw_health(u16 max, u16 val)
{
	// HP icon
	sprite_put(HUD_HP_X, HUD_HP_Y, SPRITE_SIZE(2,2), 
		TILE_ATTR_FULL(HUD_PALNUM, 1, 0, 0, HUD_VRAM_SLOT + 4));

	// Build the bar
	for (u16 i = 1; i < max + 1; i++)
	{
		u16 attr = TILE_ATTR_FULL(HUD_PALNUM, 1, 0, 0, HUD_VRAM_SLOT + (val > i ? 8 : 10));
			sprite_put(HUD_HP_X, HUD_HP_Y + 8 + (8 * i),
				SPRITE_SIZE(2,1), attr);

	}
/*	for (; max != 0; max--)
	{
		u16 attr = TILE_ATTR_FULL(HUD_PALNUM, 1, 0, 0, HUD_VRAM_SLOT);
		attr += (val >= max) ? 6 : 8;
		sprite_put(HUD_HP_X, HUD_HP_Y + 8 + ((max) * (HUD_HP_SPACING), 
			SPRITE_SIZE(2,1), attr);
	}*/
}

void hud_draw_cp(u16 val)
{
	// CP icon
	sprite_put(HUD_CP_X, HUD_CP_Y, SPRITE_SIZE(2,2), 
		TILE_ATTR_FULL(HUD_PALNUM, 1, 0, 0, HUD_VRAM_SLOT));

	// Top of the bar
	sprite_put(HUD_CP_X, HUD_CP_Y - HUD_CP_BAR_HEIGHT - 10, SPRITE_SIZE(2,1),
		TILE_ATTR_FULL(HUD_PALNUM, 1, 0, 0, HUD_VRAM_SLOT + 12));
	// Bottom of the bar
	sprite_put(HUD_CP_X, HUD_CP_Y - 2, SPRITE_SIZE(2,1),
		TILE_ATTR_FULL(HUD_PALNUM, 1, 1, 0, HUD_VRAM_SLOT + 12));
	// Build center of bar
	for (u16 i = 0; i < (HUD_CP_BAR_HEIGHT / 8); i++)
	{
		// Default to "full" tile
		u16 attr = TILE_ATTR_FULL(HUD_PALNUM, 1, 0, system_osc & 1, HUD_VRAM_SLOT + 14);
		u16 py = HUD_CP_Y - 10 - (i * 8);
		if (val == HUD_CP_BAR_HEIGHT)
		{
			sprite_put(HUD_CP_X, py, SPRITE_SIZE(2,1), attr);
			continue;
		}
		if (val < (i + 1) * 8 && val >= (i * 8))
		{
			attr += 2 + ((val % 8) * 2);
		}
		else if (val < (i * 8))
		{
			attr += 2;
		}

		// Calculate Y for this tile
		sprite_put(HUD_CP_X, py, SPRITE_SIZE(2,1), attr);
	}


}
