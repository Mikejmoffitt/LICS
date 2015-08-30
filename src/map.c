#include "map.h"

#include "pal.h"
#include "gfx.h"
#include "vramslots.h"
#include "mapdata.h"
#include "state.h"

// Unordered list of all maps
static const map_file *maplist[] = {
	(map_file *)&mapdata_testroom,
	(map_file *)&mapdata_startroom,
	0
};



void map_load_tileset(u8 num)
{
	u32 tsrc_ptr;
	u32 psrc_ptr;
	switch (num)
	{
		default:
		case MAP_SET_OUTSIDE1:
			tsrc_ptr = (u32)gfx_outside1;
			psrc_ptr = (u32)pal_outside1;
			break;
		case MAP_SET_OUTSIDE2:
			tsrc_ptr = (u32)gfx_outside2;
			psrc_ptr = (u32)pal_outside2;
			break;
		case MAP_SET_INSIDE1:
			tsrc_ptr = (u32)gfx_inside1;
			psrc_ptr = (u32)pal_inside1;
			break;
	}
	VDP_doVRamDMA(tsrc_ptr,MAP_FG_VRAM_SLOT * 32,MAP_FG_VRAM_LEN * 16);
	VDP_doCRamDMA(psrc_ptr,MAP_FG_PALNUM * 32, 16);
}

// Maps are referred to by number so we never have to call them by name
map_file *map_by_id(u8 num)
{
	for (int i = 0; i < 255; i++)
	{
		map_file *tf = maplist[i];
		// List termination; room not found
		if (!tf)
		{
			return NULL;
		}

		// Found the room, return it
		if (tf->id == (u8)num)
		{
			return tf;
		}
	}
}

void map_draw_full(u16 cam_x, u16 cam_y)
{
	
}
