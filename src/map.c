#include "map.h"

#include "pal.h"
#include "gfx.h"
#include "vramslots.h"
#include "mapdata.h"
#include "state.h"
#include "player.h"
#include "sprites.h"
#include "system.h"

#define MAP_DMA_QUEUE_MAX 96
static u32 map_dma_src_queue[MAP_DMA_QUEUE_MAX];
static u16 map_dma_dest_queue[MAP_DMA_QUEUE_MAX];
static u16 map_dma_len_queue[MAP_DMA_QUEUE_MAX];
static u16 map_dma_queue_depth;

#define MAP_DMA_H_QUEUE_MAX 48

static u16 map_dma_h_src_queue[MAP_DMA_H_QUEUE_MAX];
static u16 map_dma_h_dest[2];
static u16 map_dma_h_len[2];

static u8 map_current_tileset;

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
	(map_file *)&mapdata_earlycorridor,
	(map_file *)&mapdata_cownspikes,
	(map_file *)&mapdata_liftget,
	(map_file *)&mapdata_earlybumps,
	(map_file *)&mapdata_smallchamber,
	(map_file *)&mapdata_zigzag,
	(map_file *)&mapdata_jumpget,
	(map_file *)&mapdata_lavatower,
	(map_file *)&mapdata_ballhall,
	(map_file *)&mapdata_kickblock,
	(map_file *)&mapdata_pillatower,
	(map_file *)&mapdata_buggozone,
	(map_file *)&mapdata_poweruptower,
	(map_file *)&mapdata_boingotele,
	(map_file *)&mapdata_undersand,
	(map_file *)&mapdata_spikeshelf,
	(map_file *)&mapdata_kickget,
	(map_file *)&mapdata_cowzone,
	(map_file *)&mapdata_smallghetto,
	(map_file *)&mapdata_flargycolumn,
	(map_file *)&mapdata_bigghetto,
	(map_file *)&mapdata_elevatorroom,
	(map_file *)&mapdata_topleft,
	(map_file *)&mapdata_dogtown,
	(map_file *)&mapdata_pyramid,
	(map_file *)&mapdata_basketball,
	(map_file *)&mapdata_treesand,
	(map_file *)&mapdata_purplezone,
	(map_file *)&mapdata_littlepurple,
	(map_file *)&mapdata_orangeget,
	(map_file *)&mapdata_phantomget,
	(map_file *)&mapdata_boss1,
	(map_file *)&mapdata_rooftop,
	(map_file *)&mapdata_roofroom,
	0
};

void map_debug_chooser(void)
{
// Clear screen
	u16 y, x;
	for (y = 4; y < 32; y++)
	{
		for (x = 0; x < 40; x++)
		{
			vu32 *plctrl;
			vu16 *pwdata;
			u32 vaddr = VDP_getWindowAddress() + (x * 2) + (y << 7);

			plctrl = (u32 *)GFX_CTRL_PORT;
			pwdata = (u16 *)GFX_DATA_PORT;

			*plctrl = GFX_WRITE_VRAM_ADDR(vaddr);
			*pwdata = (u16)(TILE_ATTR_FULL(1, 1, 0, 0, (0x500 + ' ')));
		}
	}

	// Show window plane
	system_wait_v();
	VDP_setReg(0x12,0x1E);

	u16 room_choice = 0;

	while (buttons & BUTTON_START)
	{
		system_wait_v();
	}

goto do_render;

	for (;;)
	{
		// Select a room
		if ((room_choice != 0) && (buttons & BUTTON_UP) && !(buttons_prev & BUTTON_UP))
		{
			room_choice--;
		}
		else if ((buttons & BUTTON_DOWN) && !(buttons_prev & BUTTON_DOWN))
		{
			room_choice++;
			while (maplist[room_choice] == 0)
			{
				room_choice--;
			}
		}
		else
		{
			goto skip_print;
		}

do_render:
		// Clear screen
		system_wait_v();
		VDP_doVRamDMAFill(VDP_getWindowAddress(), 40 * 64, 0);
		VDP_waitDMACompletion();
		for (s16 i = room_choice - 8; i < room_choice + 8; i++)
		{
			if (i < 0)
			{
				continue;
			}
			if (i >= (sizeof(maplist) / sizeof(map_file *) - 1))
			{
				continue;
			}

			if (i == room_choice)
			{
				w_puts("->", 1, 7+(i-room_choice));
			}

			// Print current selection
			const map_file *map = maplist[i];
			char num_chr[3];
			num_chr[2] = '\0';
			u8 room_high = (i & 0xF0) >> 4;

			if ((i & 0xF) < 0xA)
			{
				num_chr[1] = '0' + (i & 0xF);
			}
			else
			{
				num_chr[1] = 'A' + (i & 0xF) - 0xA;
			}

			if (room_high < 0xA)
			{
				num_chr[0] = '0' + room_high;
			}
			else
			{
				num_chr[0] = 'A' + (room_high - 0xA);
			}

			w_puts(num_chr, 4, 7+(i-room_choice));
			w_puts(map->name, 7, 7+(i-room_choice));
		}

		const map_file *map = maplist[room_choice];
skip_print:
		// If a button is pressed, latch map and exit
		if (buttons & (BUTTON_A | BUTTON_B | BUTTON_C | BUTTON_START))
		{
			state.next_id = map->id;
			state.next_entrance = 0;
			return;
		}

		system_wait_v();
		w_puts(map->name, 7, 7);
		sprites_dma_simple();
	}

	// Hide window plane
	VDP_setReg(0x12,0x1E);
}

void map_init(void)
{
	u16 i = MAP_DMA_QUEUE_MAX;
	while (i--)
	{
		map_dma_src_queue[i] = 0;
		map_dma_dest_queue[i] = 0;
		map_dma_len_queue[i] = 0;
		map_dma_queue_depth = 0;
	}
	i = MAP_DMA_H_QUEUE_MAX;
	while (i--)
	{
		map_dma_h_src_queue[i] = 0;
	}
	map_dma_h_dest[0] = 0;
	map_dma_h_dest[1] = 0;
	map_dma_h_len[0] = 0;
	map_dma_h_len[1] = 0;

	map_current_tileset = MAP_SET_INVALID;
}

u8 map_get_current_tileset(void)
{
	return map_current_tileset;
}

void map_load_tileset(u8 num)
{
	if (num == map_current_tileset)
	{
		return;
	}
	map_current_tileset = num;
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
		case MAP_SET_OUTSIDE3:
			tsrc_ptr = (u32)gfx_outside3;
			psrc_ptr = (u32)pal_outside3;
			break;
		case MAP_SET_PURPLEZONE:
			tsrc_ptr = (u32)gfx_purplezone;
			psrc_ptr = (u32)pal_purplezone;
			break;
		case MAP_SET_ROOFTOP:
			tsrc_ptr = (u32)gfx_rooftop;
			psrc_ptr = (u32)pal_rooftop;
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
	u16 pw = VDP_getPlanWidth();
	u16 ph = VDP_getPlanHeight();

	// Useful values for sourcing and plotting
	// Map width, in tiles * 2 (actual address in VRAM)
	u16 map_width = state.current_room->w * (2 * (STATE_SC_W / 8));

	// VRAM address at which the vertical seam occurs
	u16 seam_vaddr = ph * pw * 2;

	// Width of the screen in double-tiles (actual address)
	const u16 vis_width = (STATE_SC_W / 8);

	// X and Y components of the source address (top-left visible corner)
	u16 src_xcomp = (2 * (cam_x / 8));
	src_xcomp += (right_side?2 * (STATE_SC_W / 8):0);
	u16 src_ycomp = (map_width * (cam_y / 8));

	// What is the position of the tile shown at cam_x, cam_y?
	u16 plot_x = (cam_x % (pw * 8))/8;
	u16 plot_y = (cam_y % (ph * 8))/8;

	// Copy destination
	u16 dma_dest = (2 * plot_x) + ((pw * 2) * plot_y);

	// Copy source
	u32 dma_src = (u32)state.current_map + src_xcomp + src_ycomp;

	if (right_side)
	{
		// Add almost one screen width's
		dma_dest += (STATE_SC_W / 8) * 2;
		// Horizontal seam
		if (((plot_x + vis_width) >= (pw)))
		{
			dma_dest -= (pw * 2);
		}
	}

	u16 current_dma = 0;
	map_dma_h_dest[0] = VDP_getAPlanAddress() + dma_dest;
	map_dma_h_len[0] = 0;
	map_dma_h_len[1] = 0;

	for (i = 0; i < ph; i++)
	{
		map_dma_h_src_queue[i] = *((u16 *)dma_src);
		dma_src += map_width;
		map_dma_h_len[current_dma]++;
		dma_dest += pw * 2;
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
	u16 pw = VDP_getPlanWidth();
	u16 ph = VDP_getPlanHeight();
	// Useful values for sourcing and plotting
	// Map width, in tiles * 2 (actual address in VRAM)
	u16 map_width = state.current_room->w * (2 * (STATE_SC_W / 8));

	// VRAM address at which the vertical seam occurs
	u16 seam_vaddr = ph * pw * 2;

	// Width of the screen in double-tiles (actual address)
	const u16 vis_width = (STATE_SC_W / 8);

	// X and Y components of the source address
	u16 src_xcomp = (2 * (cam_x / 8));
	u16 src_ycomp = (map_width * (cam_y / 8));

	// What is the position of the tile shown at cam_x, cam_y?
	u16 plot_x = (cam_x % (pw * 8))/8;
	u16 plot_y = (cam_y % (ph * 8))/8;

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
	if ((plot_x + vis_width + 1) >= (pw))
	{
		// DMA zero is mostly normal, but it cuts short at the seam
		dma_len_0 = pw - plot_x;
		dma_dest_0 = (2 * plot_x) + ((pw * 2) * plot_y);

		// DMA 1 fills in the rest, on the "same row" near the left
		dma_len_1 = (STATE_SC_W / 8) - dma_len_0 + 1;
		dma_src_1 = dma_src_0 + (2 * dma_len_0);
		dma_dest_1 = ((pw * 2) * plot_y);
	}
	else
	{
		dma_len_0 = vis_width + 1;
		dma_dest_0 = (2 * plot_x) + ((pw * 2) * plot_y);
		dma_src_1 = 0;
		dma_dest_1 = 0;
		dma_len_1 = 0;
	}

	// Work on the bottom of the screen instead of the top
	if (bottom_side)
	{
		dma_src_0 += map_width * 28;
		dma_dest_0 += pw * 2 * 28;
		if (!system_ntsc)
		{
			dma_src_0 += map_width * 2;
			dma_dest_0 += pw * 2 * 2;
		}

		// Have we crossed a vertical seam?
		while (dma_dest_0 >= seam_vaddr)
		{
			// Loop back around.
			dma_dest_0 -= seam_vaddr;
		}


		if (dma_len_1)
		{

			if (!system_ntsc)
			{
				dma_src_1 += map_width * 2;
				dma_dest_1 += pw * 2 * 2; // Add 2 cells for 240p mode (PAL)
			}
			dma_src_1 += map_width * 28;
			dma_dest_1 += pw * 2 * 28; // 28 cells for 224p mode (NTSC)

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
	u16 pw = VDP_getPlanWidth();
	u16 ph = VDP_getPlanHeight();
	u16 num_rows = 32; // Only one screen's worth of rows.
	// Useful values for sourcing and plotting
	// Map width, in tiles * 2 (actual address in VRAM)
	u16 map_width = state.current_room->w * (2 * (STATE_SC_W / 8));

	// VRAM address at which the vertical seam occurs
	u16 seam_vaddr = ph * pw * 2;

	// Width of the screen in double-tiles (actual address)
	const u16 vis_width = (STATE_SC_W / 8);

	// X and Y components of the source address
	u16 src_xcomp = (2 * (cam_x / 8));
	u16 src_ycomp = (map_width * (cam_y / 8));

	// What is the position of the tile shown at cam_x, cam_y?
	u16 plot_x = (cam_x % (pw * 8))/8;
	u16 plot_y = (cam_y % (ph * 8))/8;

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
	if ((plot_x + vis_width + 1) >= (pw))
	{
		// DMA zero is mostly normal, but it cuts short at the seam
		dma_len[0] = pw - plot_x;
		dma_src[0] = (u32)state.current_map + src_xcomp + src_ycomp;
		dma_dest[0] = (2 * plot_x) + ((pw * 2) * plot_y);

		// DMA 1 fills in the rest, on the "same row" near the left
		dma_len[1] = (STATE_SC_W / 8) - dma_len[0] + 1;
		dma_src[1] = dma_src[0] + (2 * dma_len[0]);
		dma_dest[1] = ((pw * 2) * plot_y);
		//dma_src[1] = state.current_map + (2 * (cam_x / 8)) + dma_len[0] - map_width;
	}
	else
	{
		dma_len[0] = vis_width + 1;
		dma_src[0] = (u32)state.current_map + src_xcomp + src_ycomp;
		dma_dest[0] = (2 * plot_x) + ((pw * 2) * plot_y);
	}

	if (VDP_getScreenHeight() == 224)
	{
		num_rows -=2;
	}
	for (y = 0; y < num_rows; y++)
	{
		// DMA 1
		map_dma_queue(dma_src[0],VDP_getAPlanAddress() + dma_dest[0], dma_len[0]);
		dma_src[0] += map_width;
		dma_dest[0] += pw * 2;
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
		map_dma_queue(dma_src[1],VDP_getAPlanAddress() + dma_dest[1], dma_len[1]);
		dma_src[1] += map_width;
		dma_dest[1] += pw * 2;
		// Have we crossed the vertical seam?
		if (dma_dest[1] >= seam_vaddr)
		{
			// Loop back around.
			dma_dest[1] -= seam_vaddr;
		}
	}
}

void map_draw_diffs(u16 moved)
{
	if (state.fresh_room)
	{
		map_draw_full(state.cam_x, state.cam_y);
		state.fresh_room = 0;
		return;
	}
	if (moved & STATE_MOVED_Y)
	{
		map_draw_vertical(state.cam_x, state.cam_y, pl.dy > FIX16(0.0));
	}
	if (moved & STATE_MOVED_X)
	{
		map_draw_horizontal(state.cam_x, state.cam_y, pl.dx > FIX16(0.0));
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
			(VDP_getPlanWidth() * 2));
	}

	if (map_dma_h_len[1])
	{
		VDP_doDMAEx(VDP_DMA_VRAM,
			(u32)&map_dma_h_src_queue[map_dma_h_len[0]],
			map_dma_h_dest[1],
			map_dma_h_len[1],
			(VDP_getPlanWidth() * 2));
		map_dma_h_len[1] = 0;
	}
	map_dma_h_len[0] = 0;

}

u16 map_collision(u16 px, u16 py)
{
	u16 check_addr = ((py / 8) * (state.current_room->w * 40)) + (px / 8);
	u16 m = *(u16 *)(&state.current_map[0] + (2 * check_addr)) & 0x7FFF;
	return ((m >= 0x80) && (m < 0xE0));
}

u16 map_hurt(u16 px, u16 py)
{
	u16 check_addr = ((py / 8) * (state.current_room->w * 40)) + (px / 8);
	u16 m = *(u16 *)(&state.current_map[0] + (2 * check_addr)) & 0x7FFF;
	return ((m >= 0xE0) && (m <= 0xFF));
}
