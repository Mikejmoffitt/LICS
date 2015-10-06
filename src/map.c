#include "map.h"

#include "pal.h"
#include "gfx.h"
#include "vramslots.h"
#include "mapdata.h"
#include "state.h"

#define MAP_DMA_QUEUE_MAX 96
static u32 map_dma_src_queue[MAP_DMA_QUEUE_MAX];
static u16 map_dma_dest_queue[MAP_DMA_QUEUE_MAX];
static u16 map_dma_len_queue[MAP_DMA_QUEUE_MAX];
static u16 map_dma_queue_depth;

#define MAP_DMA_H_QUEUE_MAX 48

static u16 map_dma_h_src_queue[MAP_DMA_H_QUEUE_MAX];
static u16 map_dma_h_dest[2];
static u16 map_dma_h_len[2];

void map_dma_queue(u32 src, u16 dest, u16 len)
{
	if (map_dma_queue_depth == MAP_DMA_QUEUE_MAX)
	{
		return;
	}
	map_dma_src_queue[map_dma_queue_depth] = src;
	map_dma_dest_queue[map_dma_queue_depth] = dest;
	map_dma_len_queue[map_dma_queue_depth] = len;
	map_dma_queue_depth++;
}

// Unordered list of all maps
static const map_file *maplist[] = {
	(map_file *)&mapdata_roomzero,
	(map_file *)&mapdata_startroom,
	(map_file *)&mapdata_sidesquare,
	(map_file *)&mapdata_teleroom,
	(map_file *)&mapdata_lefttall,
	(map_file *)&mapdata_plantroom,
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
		case MAP_SET_SANDY1:
			tsrc_ptr = (u32)gfx_sandy1;
			psrc_ptr = (u32)pal_sandy1;
			break;
		case MAP_SET_TELEPORTER:
			tsrc_ptr = (u32)gfx_teleporter;
			psrc_ptr = (u32)pal_teleporter;
			break;
	}
	VDP_doVRamDMA(tsrc_ptr,MAP_FG_VRAM_SLOT * 32,MAP_FG_VRAM_LEN * 16);
	VDP_doCRamDMA(psrc_ptr,MAP_FG_PALNUM * 32, 16);
	map_dma_queue_depth = 0;
	map_dma_h_len[0] = 0;
	map_dma_h_len[1] = 0;
}

// Maps are referred to by number so we never have to call them by name
map_file *map_by_id(u8 num)
{
	int i;
	for (i = 0; i < 255; i++)
	{
		map_file *tf = (map_file *)maplist[i];
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
	return NULL;
}

void map_draw_horizontal(u16 cam_x, u16 cam_y, u16 right_side)
{
	int i;
	// Useful values for sourcing and plotting
	// Map width, in tiles * 2 (actual address in VRAM)
	u16 map_width = state.current_room->w * (2 * (STATE_SC_W / 8));

	// VRAM address at which the vertical seam occurs
	u16 seam_vaddr = STATE_PLANE_H * STATE_PLANE_W * 2;

	// Width of the screen in double-tiles (actual address)
	const u16 vis_width = (STATE_SC_W / 8);

	// X and Y components of the source address (top-left visible corner)
	u16 src_xcomp = (2 * (cam_x / 8));
	src_xcomp += (right_side?2 * (STATE_SC_W / 8):0);
	u16 src_ycomp = (map_width * (cam_y / 8));

	// What is the position of the tile shown at cam_x, cam_y?
	u16 plot_x = (cam_x % (STATE_PLANE_W * 8))/8;
	u16 plot_y = (cam_y % (STATE_PLANE_H * 8))/8;

	// Copy destination
	u16 dma_dest = (2 * plot_x) + ((STATE_PLANE_W * 2) * plot_y);

	// Copy source
	u32 dma_src = (u32)state.current_map + src_xcomp + src_ycomp;

	if (right_side)
	{
		// Add almost one screen width's
		dma_dest += (STATE_SC_W / 8) * 2; 
		//dma_dest += 2 * (((STATE_SC_W / 8) - plot_x));
		// Horizontal seam
		if (((plot_x + vis_width) >= (STATE_PLANE_W)))
		{
			dma_dest -= (STATE_PLANE_W * 2);
		}
	}

	u16 current_dma = 0;
	map_dma_h_dest[0] = VDP_getAPlanAddress() + dma_dest;
	map_dma_h_len[0] = 0;
	map_dma_h_len[1] = 0;

	for (i = 0; i < STATE_PLANE_H; i++)
	{
		map_dma_h_src_queue[i] = *((u16 *)dma_src);
		dma_src += map_width;
		map_dma_h_len[current_dma]++;
		dma_dest += STATE_PLANE_W * 2;
		// Have we crossed the vertical seam?
		if (dma_dest >= seam_vaddr)
		{
			// Loop back around.
			dma_dest -= seam_vaddr;
			current_dma++;
			map_dma_h_dest[current_dma] = VDP_getAPlanAddress() + dma_dest;
		}
	}
}

void map_draw_vertical(u16 cam_x, u16 cam_y, u16 bottom_side)
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
	u16 dma_len_0;
	u16 dma_len_1;

	// Copy destinations
	u16 dma_dest_0;
	u16 dma_dest_1;

	// Copy sources
	u32 dma_src_0 = (u32)state.current_map + src_xcomp + src_ycomp;
	u32 dma_src_1;

	// Will we cross the horizontal seam?
	if ((plot_x + vis_width + 1) >= (STATE_PLANE_W))
	{
		// DMA zero is mostly normal, but it cuts short at the seam
		dma_len_0 = STATE_PLANE_W - plot_x;
		dma_dest_0 = (2 * plot_x) + ((STATE_PLANE_W * 2) * plot_y);
		
		// DMA 1 fills in the rest, on the "same row" near the left
		dma_len_1 = (STATE_SC_W / 8) - dma_len_0 + 1;
		dma_src_1 = dma_src_0 + (2 * dma_len_0);
		dma_dest_1 = ((STATE_PLANE_W * 2) * plot_y);
	}
	else
	{
		dma_len_0 = vis_width + 1;
		dma_dest_0 = (2 * plot_x) + ((STATE_PLANE_W * 2) * plot_y);
		dma_src_1 = 0;
		dma_dest_1 = 0;
		dma_len_1 = 0;
	}

	// Work on the bottom of the screen instead of the top
	if (bottom_side)
	{
		dma_src_0 += map_width * 28;
		dma_dest_0 += STATE_PLANE_W * 2 * 28;
		if (VDP_getScreenHeight() == 240)
		{
			dma_src_0 += map_width * 2;
			dma_dest_0 += STATE_PLANE_W * 2 * 2;
		}

		// Have we crossed a vertical seam?
		while (dma_dest_0 >= seam_vaddr)
		{
			// Loop back around.
			dma_dest_0 -= seam_vaddr;
		}


		if (dma_len_1)
		{

			if (VDP_getScreenHeight() == 240)
			{
				dma_src_1 += map_width * 2;
				dma_dest_1 += STATE_PLANE_W * 2 * 2;
			}
			dma_src_1 += map_width * 28;
			dma_dest_1 += STATE_PLANE_W * 2 * 28;

			while (dma_dest_1 >= seam_vaddr)
			{
				// Loop back around.
				dma_dest_1 -= seam_vaddr;
			}
		}
	}

	map_dma_queue(dma_src_0,VDP_getAPlanAddress() + dma_dest_0, dma_len_0);
	if (dma_len_1)
	{
		map_dma_queue(dma_src_1,VDP_getAPlanAddress() + dma_dest_1, dma_len_1);
	}
}

void map_draw_full(u16 cam_x, u16 cam_y)
{
	int y = 0;
	u16 num_rows = STATE_PLANE_H;
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

	if (VDP_getScreenHeight() == 224)
	{
		num_rows -=2;
	}
	for (y = 0; y < num_rows; y++)
	{		
		// DMA 1
//		VDP_doVRamDMA(dma_src[0],VDP_getAPlanAddress() + dma_dest[0],dma_len[0]);
		map_dma_queue(dma_src[0],VDP_getAPlanAddress() + dma_dest[0], dma_len[0]);
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
		//VDP_doVRamDMA(dma_src[1],VDP_getAPlanAddress() + dma_dest[1],dma_len[1]);
		map_dma_queue(dma_src[1],VDP_getAPlanAddress() + dma_dest[1], dma_len[1]);
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

void map_draw_diffs(u16 moved, fix16 dx, fix16 dy)
{
	if (state.fresh_room)
	{
		map_draw_full(state.cam_x, state.cam_y);
		state.fresh_room = 0;
		return;
	}
	if (moved & STATE_MOVED_Y)
	{
		map_draw_vertical(state.cam_x, state.cam_y, dy > FIX16(0.0));
	}
	if (moved & STATE_MOVED_X)
	{
		map_draw_horizontal(state.cam_x, state.cam_y, dx > FIX16(0.0));
	}
}

void map_dma(void)
{
	unsigned int i = map_dma_queue_depth;
	while (i--)
	{
		VDP_doVRamDMA(map_dma_src_queue[i],
			(u32)map_dma_dest_queue[i],
			(map_dma_len_queue[i]));
	}
	map_dma_queue_depth = 0;
	if (map_dma_h_len[0])
	{
		VDP_doDMAEx(VDP_DMA_VRAM,
			(u32)map_dma_h_src_queue,
			map_dma_h_dest[0],
			map_dma_h_len[0],
			(STATE_PLANE_W * 2));
	}
	
	if (map_dma_h_len[1])
	{
		VDP_doDMAEx(VDP_DMA_VRAM,
			(u32)&map_dma_h_src_queue[map_dma_h_len[0]],
			map_dma_h_dest[1],
			map_dma_h_len[1],
			(STATE_PLANE_W * 2));
		map_dma_h_len[1] = 0;
	}
	map_dma_h_len[0] = 0;

}

u16 map_collision(u16 px, u16 py)
{
	u16 check_addr = ((py / 8) * (state.current_room->w * 40)) + (px / 8);
	u16 *m = (u16 *)(&state.current_map[0] + (2 * check_addr));
	return ((*m >= 0x80) && (*m < 0xE0));
}

u16 map_hurt(u16 px, u16 py)
{
	u16 check_addr = ((py / 8) * (state.current_room->w * 40)) + (px / 8);
	u16 *m = (u16 *)(&state.current_map[0] + (2 * check_addr));
	return ((*m >= 0xE0) && (*m <= 0xFF));
}
