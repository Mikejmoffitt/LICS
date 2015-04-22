#include "sprites.h"

u16 sprite_addr;
u16 sprite_table[64 * 4];

void sprites_init(void)
{
	sprite_addr = VDP_getSpriteListAddress();
}

void sprites_dma(register spr_num)
{
	VDP_doVRamDMA(sprite_table,sprite_addr,spr_num << 2);
}
void sprite_set(u8 num, u16 x, u16 y, u8 size, u16 attr, u8 link)
{
	u16 *addr = &sprite_table[num << 2];
	*addr = 128 + y;
	addr++;
	*addr = (size << 8) + link;
	addr++;
	*addr = attr;
	addr++;
	*addr = 128 + x;
};
