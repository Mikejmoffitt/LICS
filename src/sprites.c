#include "sprites.h"

u8 next_spr;
u16 sprite_addr;
u16 sprite_table[NUM_SPRITES * 4];

void sprites_init(void)
{
	sprite_addr = VDP_getSpriteListAddress();
	next_spr = 0;
}

void sprites_dma(register spr_num)
{
	VDP_doVRamDMA(sprite_table,sprite_addr,spr_num << 2);
}

void sprites_dma_simple()
{
	VDP_doVRamDMA(sprite_table,sprite_addr,next_spr << 2);
	next_spr = 0;
}

void sprite_set(u8 num, u16 x, u16 y, u8 size, u16 attr, u8 link)
{
	// Sprite table only holds 80 sprites
	if (num >= NUM_SPRITES)
	{
		return;
	}
	u16 *addr = &sprite_table[num << 2];
	*addr = 128 + y;
	*(addr + 1) = (size << 8) + link;
	*(addr + 2) = attr;
	*(addr + 3) = 128 + x;
}

void sprite_put(u16 x, u16 y, u8 size, u16 attr)
{
	sprite_set(next_spr,x,y,size,attr,next_spr+1);
	next_spr++;
}
