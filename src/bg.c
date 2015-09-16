#include "bg.h"
#include "gfx.h"
#include "pal.h"
#include "vramslots.h"
#include "bgmaps.h"
#include "state.h"
#include "bgcoefs.h"

static u16 current_bg;

static u16 bg_xscroll_vals[STATE_PLANE_H];
static u16 bg_yscroll_vals[STATE_PLANE_W / 2];
static u16 bg_xscroll_cmd;
static u16 bg_yscroll_cmd;

/* BG scroll coefficient tables */
static const fix16 *coeff_tables[] = 
{
	0,
	bgcoef_bg1,
	bgcoef_bg2,
};

// Set up the called-upon BG number's tiles, palette, and mapping
void bg_load(u16 num)
{
	if (num == current_bg)
	{
		return;
	}
	u32 pal_src;
	u32 gfx_src;
	u16 gfx_len;
	u32 map_src;
	switch (num)
	{
		default:
		case 1:
			pal_src = (u32)pal_bg1;
			gfx_src = (u32)gfx_bg1;
			map_src = (u32)map_bg1;
			gfx_len = 64 * 16;
			break;
		case 2:
			pal_src = (u32)pal_bg2;
			gfx_src = (u32)gfx_bg2;
			map_src = (u32)map_bg2;
			gfx_len = 32 * 16;
			break;
	}
	VDP_doCRamDMA(pal_src, 32 * BG_PALNUM, 16);
	VDP_doVRamDMA(gfx_src, 32 * BG_VRAM_SLOT, gfx_len);
	VDP_doVRamDMA(map_src, VDP_getBPlanAddress(), 64 * 32);
	current_bg = num;
}

void bg_scroll_x(u16 amt)
{
	if (state.current_room->w == 1)
	{
		amt = 0;
	}
	else
	{
		amt = amt * -1;
	}
	if (VDP_getHorizontalScrollingMode() == HSCROLL_PLANE)
	{
		bg_xscroll_vals[0] = amt >> coeff_tables[current_bg][0];
		bg_xscroll_cmd = STATE_SCROLL_SINGLE;
	}
	else
	{
		int i = STATE_PLANE_H;
		while (i--)
		{
			bg_xscroll_vals[i] = amt >> coeff_tables[current_bg][i];
		}
		bg_xscroll_cmd = STATE_SCROLL_DMA;
	}
}

void bg_scroll_y(u16 amt)
{
	if (!state.vs_en)
	{
		bg_yscroll_vals[0] = 16;
		bg_yscroll_cmd = STATE_SCROLL_SINGLE;
	}
	else if (VDP_getVerticalScrollingMode() == VSCROLL_PLANE)
	{
		bg_yscroll_vals[0] = amt >> coeff_tables[current_bg][0];
		bg_yscroll_cmd = STATE_SCROLL_SINGLE;
	}
	else
	{
		int i = STATE_PLANE_W / 2;
		while (i--)
		{
			bg_yscroll_vals[i] = amt >> coeff_tables[current_bg][i];
		}
		bg_yscroll_cmd = STATE_SCROLL_DMA;
	}
}

// Transfer scrolling information to VRAM as needed
void bg_dma_scroll(void)
{
	if (bg_xscroll_cmd == STATE_SCROLL_DMA)
	{
	 	VDP_setHorizontalScrollTile(PLAN_B, 0, bg_xscroll_vals, STATE_PLANE_H, 1);
	}
	else if (bg_xscroll_cmd)
	{
	 	VDP_setHorizontalScroll(PLAN_B, bg_xscroll_vals[0]);
	}
	if (bg_yscroll_cmd == STATE_SCROLL_DMA)
	{
		VDP_setVerticalScrollTile(PLAN_B, 0, bg_yscroll_vals, STATE_PLANE_H, 1);
	}
	else if (bg_yscroll_cmd)
	{
		VDP_setVerticalScroll(PLAN_B, bg_yscroll_vals[0]);
	}
}