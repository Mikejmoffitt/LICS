#include "bg.h"
#include "gfx.h"
#include "pal.h"
#include "vramslots.h"
#include "bgmaps.h"
#include "state.h"

static u16 bg_xscroll_vals[STATE_PLANE_H];
static u16 bg_yscroll_vals[STATE_PLANE_W / 2];
static u16 bg_xscroll_cmd;
static u16 bg_yscroll_cmd;

void bg_load(u16 num)
{
	VDP_doCRamDMA((u32)pal_bg1, 32 * BG_PALNUM, 16);
	VDP_doVRamDMA((u32)gfx_bg1, 32 * BG_VRAM_SLOT, 64 * 16);
	VDP_doVRamDMA((u32)map_bg1, VDP_getBPlanAddress(), 64 * 32);
}

void bg_scroll_x(u16 amt)
{
	amt = amt * -1;
	amt = amt / 2;
	if (VDP_getHorizontalScrollingMode() == HSCROLL_PLANE)
	{
		bg_xscroll_vals[0] = amt;
		bg_xscroll_cmd = STATE_SCROLL_SINGLE;
	}
	else
	{
		int i = STATE_PLANE_H;
		while (i--)
		{
			bg_xscroll_vals[i] = amt;
		}
		bg_xscroll_cmd = STATE_SCROLL_DMA;
	}
}

void bg_scroll_y(u16 amt)
{
	amt = amt / 2;
	if (VDP_getVerticalScrollingMode() == VSCROLL_PLANE)
	{
		bg_yscroll_vals[0] = amt;
		bg_yscroll_cmd = STATE_SCROLL_SINGLE;
	}
	else
	{
		int i = STATE_PLANE_W / 2;
		while (i--)
		{
			bg_yscroll_vals[i] = amt;
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
