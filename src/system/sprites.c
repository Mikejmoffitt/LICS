#include "sprites.h"

u16 next_spr;
u16 sprite_addr;
u16 sprite_table[NUM_SPRITES * 4];

void sprites_init(void)
{
	sprite_addr = VDP_getSpriteListAddress();
	next_spr = 0;
}

void sprites_dma(u32 spr_num)
{
	VDP_doVRamDMA((u32)sprite_table,sprite_addr,spr_num << 2);
}

void sprites_clamp_link(u8 num)
{
	u16 *addr = &sprite_table[num << 2];
	addr++;
	*addr = *addr & 0xFF00; // Set next link to 0 to terminate list
}

void sprites_dma_simple()
{
	// No sprites placed, don't bother
	if (next_spr == 0)
	{
		// Place a dummy sprite invisibly
		sprite_put(-64,-64,0,0);
		sprites_clamp_link(0);

		// Just transfer sprite zero to terminate list
		VDP_doVRamDMA((u32)sprite_table, sprite_addr, 4);
		return;
	}
	// Terminate the list at the last sprite
	sprites_clamp_link(next_spr - 1);
	VDP_doVRamDMA((u32)sprite_table,sprite_addr,(u16)(next_spr << 2));
	next_spr = 0;
}

u8 sprites_get_next_sprite(void)
{
	return next_spr;
}

u16 sprites_get_sprite_addr(void)
{
	return sprite_addr;
}

u16 *sprites_get_table(void)
{
	return &sprite_table[0];
}
