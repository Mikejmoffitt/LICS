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

void map_draw_vertical(u16 cam_x, u16 cam_y)
{
	// Useful values for sourcing and plotting
	// Map width, in tiles * 2 (actual address in VRAM)
	u16 map_width = state.current_room->w * (2 * (STATE_SC_W / 8));

	// VRAM address at which the vertical seam occurs
	u16 seam_vaddr = STATE_PLANE_H * STATE_PLANE_W * 2;

	// Width of the screen in double-tiles (actual address)
	const u16 vis_width = (STATE_SC_W / 8);

	// X and Y components of the source address
	u16 src_xcomp = (2 * (cam_x / 8));
	u16 src_ycomp = (map_width * (cam_y / 8));

	// What is the position of the tile shown at cam_x, cam_y?
	u16 plot_x = (cam_x % (STATE_PLANE_W * 8))/8;
	u16 plot_y = (cam_y % (STATE_PLANE_H * 8))/8;

	// DMA transfer lengths
	u16 dma_len[2];
	dma_len[0] = 0;
	dma_len[1] = 0;

	// Copy destinations
	u16 dma_dest[2];
	dma_dest[0] = 0;
	dma_dest[1] = 0;

	// Copy sources
	u32 dma_src[2];
	dma_src[0] = 0;
	dma_src[1] = 0;

	// Will we cross the horizontal seam?
	if ((plot_x + vis_width + 1) >= (STATE_PLANE_W))
	{
		// DMA zero is mostly normal, but it cuts short at the seam
		dma_len[0] = STATE_PLANE_W - plot_x;
		dma_src[0] = (u32)state.current_map + src_xcomp + src_ycomp;
		dma_dest[0] = (2 * plot_x) + ((STATE_PLANE_W * 2) * plot_y);
		
		// DMA 1 fills in the rest, on the "same row" near the left
		dma_len[1] = (STATE_SC_W / 8) - dma_len[0] + 1;
		dma_src[1] = dma_src[0] + (2 * dma_len[0]);
		dma_dest[1] = ((STATE_PLANE_W * 2) * plot_y);
		//dma_src[1] = state.current_map + (2 * (cam_x / 8)) + dma_len[0] - map_width;
	}
	else
	{
		dma_len[0] = vis_width + 1;
		dma_src[0] = (u32)state.current_map + src_xcomp + src_ycomp;
		dma_dest[0] = (2 * plot_x) + ((STATE_PLANE_W * 2) * plot_y);
	}

	// Reduced from 32 to 28 iterations, since only 28 rows are visible on-screen anyway.
	for (int y = 0; y < STATE_PLANE_H - 2; y++)
	{		
		if (y == 2)
		{
			dma_src[0] += map_width * (27 - 2);
			dma_dest[0] += STATE_PLANE_W * 2 * (27 - 2);
			if (dma_len[1])
			{
				dma_src[1] += map_width * (27 - 2);
				dma_dest[1] += STATE_PLANE_W * 2 * (27 - 2);
			}
			y = 26;
			continue;
		}
		// DMA 1
		VDP_doVRamDMA(dma_src[0],VDP_getAPlanAddress() + dma_dest[0],dma_len[0]);
		dma_src[0] += map_width;
		dma_dest[0] += STATE_PLANE_W * 2;
		// Have we crossed the vertical seam?
		if (dma_dest[0] >= seam_vaddr)
		{
			// Loop back around.
			dma_dest[0] -= seam_vaddr;
		}

		// DMA 2
		if (dma_len[1] == 0)
		{
			continue;
		}
			VDP_doVRamDMA(dma_src[1],VDP_getAPlanAddress() + dma_dest[1],dma_len[1]);
		dma_src[1] += map_width;
		dma_dest[1] += STATE_PLANE_W * 2;
		// Have we crossed the vertical seam?
		if (dma_dest[1] >= seam_vaddr)
		{
			// Loop back around.
			dma_dest[1] -= seam_vaddr;
		}
	}
}

void map_draw_full(u16 cam_x, u16 cam_y)
{
	// Useful values for sourcing and plotting
	// Map width, in tiles * 2 (actual address in VRAM)
	u16 map_width = state.current_room->w * (2 * (STATE_SC_W / 8));

	// VRAM address at which the vertical seam occurs
	u16 seam_vaddr = STATE_PLANE_H * STATE_PLANE_W * 2;

	// Width of the screen in double-tiles (actual address)
	const u16 vis_width = (STATE_SC_W / 8);

	// X and Y components of the source address
	u16 src_xcomp = (2 * (cam_x / 8));
	u16 src_ycomp = (map_width * (cam_y / 8));

	// What is the position of the tile shown at cam_x, cam_y?
	u16 plot_x = (cam_x % (STATE_PLANE_W * 8))/8;
	u16 plot_y = (cam_y % (STATE_PLANE_H * 8))/8;

	// DMA transfer lengths
	u16 dma_len[2];
	dma_len[0] = 0;
	dma_len[1] = 0;

	// Copy destinations
	u16 dma_dest[2];
	dma_dest[0] = 0;
	dma_dest[1] = 0;

	// Copy sources
	u32 dma_src[2];
	dma_src[0] = 0;
	dma_src[1] = 0;

	// Will we cross the horizontal seam?
	if ((plot_x + vis_width + 1) >= (STATE_PLANE_W))
	{
		// DMA zero is mostly normal, but it cuts short at the seam
		dma_len[0] = STATE_PLANE_W - plot_x;
		dma_src[0] = (u32)state.current_map + src_xcomp + src_ycomp;
		dma_dest[0] = (2 * plot_x) + ((STATE_PLANE_W * 2) * plot_y);
		
		// DMA 1 fills in the rest, on the "same row" near the left
		dma_len[1] = (STATE_SC_W / 8) - dma_len[0] + 1;
		dma_src[1] = dma_src[0] + (2 * dma_len[0]);
		dma_dest[1] = ((STATE_PLANE_W * 2) * plot_y);
		//dma_src[1] = state.current_map + (2 * (cam_x / 8)) + dma_len[0] - map_width;
	}
	else
	{
		dma_len[0] = vis_width + 1;
		dma_src[0] = (u32)state.current_map + src_xcomp + src_ycomp;
		dma_dest[0] = (2 * plot_x) + ((STATE_PLANE_W * 2) * plot_y);
	}

	// Reduced from 32 to 28 iterations, since only 28 rows are visible on-screen anyway.
	for (int y = 0; y < STATE_PLANE_H - 2; y++)
	{		
		// DMA 1
		VDP_doVRamDMA(dma_src[0],VDP_getAPlanAddress() + dma_dest[0],dma_len[0]);
		dma_src[0] += map_width;
		dma_dest[0] += STATE_PLANE_W * 2;
		// Have we crossed the vertical seam?
		if (dma_dest[0] >= seam_vaddr)
		{
			// Loop back around.
			dma_dest[0] -= seam_vaddr;
		}

		// DMA 2
		if (dma_len[1] == 0)
		{
			continue;
		}
		VDP_doVRamDMA(dma_src[1],VDP_getAPlanAddress() + dma_dest[1],dma_len[1]);
		dma_src[1] += map_width;
		dma_dest[1] += STATE_PLANE_W * 2;
		// Have we crossed the vertical seam?
		if (dma_dest[1] >= seam_vaddr)
		{
			// Loop back around.
			dma_dest[1] -= seam_vaddr;
		}
	}
}

u16 map_collision(u16 px, u16 py)
{
	u16 check_addr = ((py / 8) * (state.current_room->w * 40)) + (px / 8);
	u16 *m = (u16 *)(&state.current_map[0] + (2 * check_addr));
	return *m & 0xFF80;
}
