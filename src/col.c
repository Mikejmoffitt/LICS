#include "col.h"

void col_init(void)
{
	VDP_doVRamDMA(gfx_font,32*COL_VRAM_SLOT,16 * COL_VRAM_LEN);

	VDP_setHorizontalScroll(PLAN_B,4);
	VDP_setVerticalScroll(PLAN_B,16);
}

void col_puts40(u16 x, u16 y, char *s)
{
	u16 orig_x = x;
	while(*s)
	{
		if (*s == '\n')
		{	
			y++;
			x = orig_x;
		}
		else if (*s == '\t')
		{
			x = x + 4;
		}
		else
		{
			VDP_setTileMapXY(
				VDP_PLAN_A,
				TILE_ATTR_FULL(0,1,0,0,
				(*s + (COL_VRAM_SLOT))),
				x,y);	
			x++;
		}
		s++;
	}
}

void col_puthex(u16 x, u16 y, u32 val)
{
	col_puts40(x,y,"0x");
	x += 9;
	for (int i = 8; i != 0; i--)
	{
		char num[2];
		num[1] = '\0';
		switch (val & 0xF)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
				num[0] = '0' + (val & 0xF);
				break;
			default:
				num[0] = 'A' + (val & 0xF) - 0xA;
				break;
		}
		val = val >> 4;

		col_puts40(x,y,num);
		x--;
	}
}

void col_puts(u16 x, u16 y, char *s)
{
	u16 orig_x = x;
	register flip = x % 2;
	while(*s)
	{
		if (*s == '\n')
		{	
			flip = orig_x % 2;
			y++;
			x = orig_x;
		}
		else if (*s == '\t')
		{
			x = x + 4;
		}
		else
		{
			VDP_setTileMapXY(
				(flip) ? VDP_PLAN_B : VDP_PLAN_A,
				TILE_ATTR_FULL(0,1,0,0,
				(*s + COL_SEL + (COL_VRAM_SLOT))),
				x >> 1,y);	
			x++;
			flip = (flip) ? 0 : 1;
		}
		s++;
	}
}

/* vim: set noet: */
