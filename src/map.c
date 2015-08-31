#include "map.h"

#include "pal.h"
#include "gfx.h"
#include "vramslots.h"
#include "mapdata.h"
#include "state.h"
#include "col.h"

// Unordered list of all maps
static const map_file *maplist[] = {
	(map_file *)&mapdata_roomzero,
	(map_file *)&mapdata_startroom,
	(map_file *)&mapdata_sidesquare,
	(map_file *)&mapdata_talltest,
	0
};

void map_load_tileset(u8 num)
{
	// Black the screen first so no funny wrong blocks show up
	VDP_doCRamDMA(pal_black,0,64);
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
			return map_by_id(0);
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
	u16 plot_x = (cam_x % (STATE_PLANE_W * 8))/8;
	u16 plot_y = (cam_y % (STATE_PLANE_H * 8))/8;
	u32 copy_src = state.current_map + (2 * (cam_x / 8));
	u16 map_width = state.current_room->w * (2 * (STATE_SC_W / 8));
	copy_src += (map_width * (cam_y / 8));
	u32 copy_dest = (2 * plot_x) + ((STATE_PLANE_W * 2) * plot_y);

	u16 dma_post_len = (STATE_PLANE_W) - plot_x;
	u16 dma_pre_len = plot_x;

	for (int y = 0; y < STATE_PLANE_H; y++)
	{
		VDP_doVRamDMA(copy_src,VDP_getAPlanAddress() + copy_dest,41);
		copy_src += map_width;
		copy_dest += STATE_PLANE_W * 2;
		// Crossing vertical seam
		if (copy_dest >= STATE_PLANE_H * STATE_PLANE_W * 2)
		{
			copy_dest -= (STATE_PLANE_H * STATE_PLANE_W * 2);
		}
	}
}

u16 map_collision(u16 px, u16 py)
{
	u16 check_addr = ((py / 8) * (state.current_room->w * 40)) + (px / 8);
	u16 *m = (u16 *)(&state.current_map[0] + (2 * check_addr));
	return *m & 0xFF80;
}
