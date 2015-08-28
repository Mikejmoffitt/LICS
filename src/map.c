#include "map.h"

#include "pal.h"
#include "gfx.h"
#include "vramslots.h"

void map_load_tileset(register num)
{
	u32 src_ptr;
	switch (num)
	{
		default:
		case MAP_SET_OUTSIDE1:
			src_ptr = (u32)gfx_outside1;
			break;
		case MAP_SET_OUTSIDE2:
			src_ptr = (u32)gfx_outside2;
			break;
		case MAP_SET_INSIDE1:
			src_ptr = (u32)gfx_inside1;
			break;
	}
	VDP_doVRamDMA(src_ptr,MAP_FG_VRAM_SLOT * 32,MAP_FG_VRAM_LEN * 16);
}

void map_load_palette(register num)
{
	u32 src_ptr;
	switch (num)
	{
		default:
		case MAP_SET_OUTSIDE1:
			src_ptr = (u32)pal_outside1;
			break;
		case MAP_SET_OUTSIDE2:
			src_ptr = (u32)pal_outside2;
			break;
		case MAP_SET_INSIDE1:
			src_ptr = (u32)pal_inside1;
			break;
	}
	VDP_doCRamDMA(src_ptr,MAP_FG_PALNUM * 32, 16);
}
