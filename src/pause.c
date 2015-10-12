#include "pause.h"
#include "player.h"
#include "vramslots.h"
#include "gfx.h"
#include "pal.h"
#include "system.h"
#include "pausemap.h"
#include "sprites.h"
#include "save.h"
#include "hud.h"

void pause_init(void)
{
	// Get rid of the window plane
	VDP_setReg(0x12,0x00);
}

void pause_dma_tiles(void)
{
	VDP_doVRamDMA((u32)gfx_pause, PAUSE_VRAM_SLOT * 32, PAUSE_VRAM_LEN * 16);
}

void pause_setup(void)
{
	// Black out the screen during drawing
	VDP_doCRamDMA((u32)pal_black, PAUSE_PALNUM * 32, 16);

	// Copy the layout information for the window plane
	VDP_doVRamDMA((u32)pausemap_layout, VDP_getWindowAddress(), (64 * 32));

	// Fullscreen Window plane
	VDP_setReg(0x12,0x1E);

	// Bring in the colors
	VDP_doCRamDMA((u32)pal_pause, PAUSE_PALNUM * 32, 16);
}

void pause_exit(void)
{
	// no Window plane here
	VDP_setReg(0x12,0x00);

	// Give lyle back his palette
	VDP_doCRamDMA((u32)pal_lyle, PLAYER_PALNUM * 32, 16);
}

void pause_screen_loop(void)
{
	u16 in_prog = 1;
	u16 pad;
	pause_setup();
	while (in_prog)
	{
		pad = JOY_readJoypad(JOY_1); 
		if (pad & BUTTON_C)
		{
			in_prog = 0;
		}
		hud_draw_health(sram.max_hp,pl.hp);
		if (sram.have_phantom)
		{
			hud_draw_cp(pl.cp + 1 + ((pl.cp + 1) >> 1)); // CP scaled 32 --> 48
		}
		system_wait_v();
		sprites_dma_simple();
	}
	pause_exit();
}
