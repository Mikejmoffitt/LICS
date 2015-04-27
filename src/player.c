#include "player.h"
#include "gfx.h"

void player_dma(u16 num, u16 dest)
{
	u16 offset = 0;
	if (num < LYLE_3x3_CUTOFF)
	{
		offset = num * 6;
	}
	else
	{
		num -= 0x14;
		offset = 120 + (9 * num);
	}

	u16 size = (num < LYLE_3x3_CUTOFF) ? (9 * 16) : (9 * 16);

	VDP_doVRamDMA(gfx_lyle + (32 *offset),dest,size);
}
