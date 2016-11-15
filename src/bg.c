#include "bg.h"
#include "gfx.h"
#include "pal.h"
#include "vramslots.h"
#include "bgmaps.h"
#include "state.h"
#include "bgcoefs.h"

static u16 current_bg;

// The way scroll values are set up, row scrolling is only okay for 64x32 (ingame).
static s16 bg_xscroll_vals[STATE_PLANE_H];
static s16 bg_yscroll_vals[STATE_PLANE_W / 2];
static u16 bg_xscroll_cmd;
static u16 bg_yscroll_cmd;

/* BG scroll coefficient tables */
static const s16 *coeff_tables[] =
{
	0,
	bgcoef_bg1,
	bgcoef_bg2,
	bgcoef_bg3,
	bgcoef_bg4,
	bgcoef_bg5,
	bgcoef_bg1, // BGMAP 1 coefs are reused
	bgcoef_bg7,
	0, // DUMMY
	0, // DUMMY
	bgcoef_bg10,
	0, // DUMMY
	bgcoef_bg12,
	bgcoef_bg13,
	bgcoef_bg13 // BGMAP 13 coefs are reused
};

// Set up the called-upon BG number's tiles, palette, and mapping
void bg_load(u16 num)
{
	if (num == current_bg)
	{
		return;
	}

	current_bg = num;
	VDP_clearPlan(VDP_PLAN_B, 1);
	// Source data for CRAM palette (first 8 entries)
	u32 pal_src;
	// Source data for VRAM tiles
	u32 gfx_src;
	// Number of tiles
	u16 gfx_len;
	// Source data for graphics mapping
	u32 map_src;
	switch (num)
	{
		default:
			// Do nothing for an invalid backdrop
		case 0:
			current_bg = 0;
			VDP_waitDMACompletion();
			VDP_doCRamDMA((u32)pal_bg_common, (32 * BG_PALNUM) + 16, 8);
			return;
		case 1:
			pal_src = (u32)pal_bg1;
			gfx_src = (u32)gfx_bg1;
			map_src = (u32)map_bg1;
			gfx_len = 64;
			break;
		case 2:
			pal_src = (u32)pal_bg2;
			gfx_src = (u32)gfx_bg2;
			map_src = (u32)map_bg2;
			gfx_len = 32;
			break;
		case 3:
			pal_src = (u32)pal_bg3;
			gfx_src = (u32)gfx_bg3;
			map_src = (u32)map_bg3;
			gfx_len = 16;
			break;
		case 4:
			pal_src = (u32)pal_bg4;
			gfx_src = (u32)gfx_bg4;
			map_src = (u32)map_bg4;
			gfx_len = 8;
			break;
		case 5:
			pal_src = (u32)pal_bg5;
			gfx_src = (u32)gfx_bg5;
			map_src = (u32)map_bg5;
			gfx_len = 16;
			break;
		case 6:
			// Pal and gfx from bg1 are reused
			pal_src = (u32)pal_bg1;
			gfx_src = (u32)gfx_bg1;
			map_src = (u32)map_bg6;
			gfx_len = 64;
			break;
		case 7:
			pal_src = (u32)pal_bg7;
			gfx_src = (u32)gfx_bg7;
			map_src = (u32)map_bg7;
			gfx_len = 36;
			break;

		case 10:
			pal_src = (u32)pal_bg10;
			gfx_src = (u32)gfx_bg10;
			map_src = (u32)map_bg10;
			gfx_len = 88;
			break;

		case 12:
			// Pal and gfx from bg10 are reused
			pal_src = (u32)pal_bg10;
			gfx_src = (u32)gfx_bg10;
			map_src = (u32)map_bg12;
			gfx_len = 88;
			break;
		case 13:
			pal_src = (u32)pal_bg13;
			gfx_src = (u32)gfx_bg13;
			map_src = (u32)map_bg13;
			gfx_len = 24;
			break;

		case 14:
			// Pal and gfx from bg13 are reused
			pal_src = (u32)pal_bg13;
			gfx_src = (u32)gfx_bg13;
			map_src = (u32)map_bg14;
			gfx_len = 24;
	}
	// Multiply length by 16 for number of words per tile
	gfx_len = gfx_len << 4;
	// Only 8 words are loaded for the BG palette, since the other 8 are common
	// Load the common 8 words
	VDP_waitDMACompletion();
	VDP_doVRamDMA(gfx_src, 32 * BG_VRAM_SLOT, gfx_len);
	VDP_doVRamDMA(map_src, VDP_getBPlanAddress(), 64 * 32);
	VDP_doCRamDMA(pal_src, 32 * BG_PALNUM, 8);
	VDP_doCRamDMA((u32)pal_bg_common, (32 * BG_PALNUM) + 16, 8);
}

void bg_scroll_x(u16 amt)
{
	if (!state.hs_en)
	{
		bg_xscroll_vals[0] = 0;
		bg_xscroll_cmd = STATE_SCROLL_SINGLE;
	}
	else if (VDP_getHorizontalScrollingMode() == HSCROLL_PLANE)
	{
		bg_xscroll_vals[0] = (amt * -1) >> coeff_tables[current_bg][0];
		bg_xscroll_cmd = STATE_SCROLL_SINGLE;
	}
	else
	{
		amt = amt * -1;
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
		// Hack for BG2 to look good on a single-height screen
		if (current_bg == 2)
		{
			bg_yscroll_vals[0] += 80;
		}
		bg_yscroll_cmd = STATE_SCROLL_SINGLE;
	}
	else if (VDP_getVerticalScrollingMode() == VSCROLL_PLANE)
	{
		bg_yscroll_vals[0] = amt >> coeff_tables[current_bg][0];
		bg_yscroll_cmd = STATE_SCROLL_SINGLE;
	}
	else
	{
		int i = VDP_getPlanWidth() / 2;
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
	 	VDP_setHorizontalScrollTile(PLAN_B, 0, bg_xscroll_vals, VDP_getPlanHeight(), 1);
	}
	else if (bg_xscroll_cmd)
	{
	 	VDP_setHorizontalScroll(PLAN_B, bg_xscroll_vals[0]);
	}
	if (bg_yscroll_cmd == STATE_SCROLL_DMA)
	{
		VDP_setVerticalScrollTile(PLAN_B, 0, bg_yscroll_vals, VDP_getPlanHeight(), 1);
	}
	else if (bg_yscroll_cmd)
	{
		VDP_setVerticalScroll(PLAN_B, bg_yscroll_vals[0]);
	}
}
