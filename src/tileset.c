#include "tileset.h"

void tileset_load_fg(u16 set)
{
	u16 *pal;
	u32 from;
	u16 to = TILESET_FG_VRAM_SLOT * 32;
	u16 len = TILESET_FG_DMA_SIZE;
	switch(set)
	{
		case TILESET_FG_OUTSIDE1:
			pal = (u16 *)&pal_outside1;
			from = (u32)gfx_outside1;
			break;
		case TILESET_FG_OUTSIDE2:
			pal = (u16 *)&pal_outside2;
			from = (u32)gfx_outside2;
			break;
		case TILESET_FG_INSIDE1:
			pal = (u16 *)&pal_inside1;
			from = (u32)gfx_inside1;
			break;
	}
	VDP_setPalette(TILESET_FG_PAL_NUM,pal);
	VDP_doVRamDMA(from,to,len);
}
