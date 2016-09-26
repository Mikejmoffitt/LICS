#include "messagescreen.h"
#include <genesis.h>
#include "system.h"
#include "sprites.h"
#include "pause.h"
#include "pal.h"
#include "gfx.h"
#include "hud.h"
#include "pausemap.h"
#include "state.h"
#include "save.h"
#include "music.h"
#include "vramslots.h"

static void map_clear(void)
{
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
			*pwdata = (u16)(0x500 + ' ');
		}
	}
}

void message_screen(const char *s)
{
	u16 show_prompt_timeout = 60 * 2;

	// Copy the layout information for the window plane
	system_wait_v();
	sprites_dma_simple();
	VDP_doVRamDMA((u32)pausemap_layout, VDP_getWindowAddress(), (64 * 32));

	stopsound();
	playsound(SFX_PAUSE);

	map_clear();

	// Bring in the colors
	VDP_doCRamDMA((u32)pal_pause, PAUSE_PALNUM * 32, 16);

	// Fullscreen Window plane
	VDP_setReg(0x12,0x1E);

	VDP_waitDMACompletion();
	w_puts(s, 3, 8);
	for (;;)
	{
		if (show_prompt_timeout > 0)
		{
			show_prompt_timeout--;
			if (show_prompt_timeout == 0)
			{
				w_puts("Press Start", 14, 24);
			}
		}
		else
		{
			if (buttons & (BUTTON_START))
			{
				break;
			}
		}
		system_wait_v();
		sprites_dma_simple();
	}
	system_wait_v();
	sprites_dma_simple();
	VDP_setReg(0x12, 0x00);
	playsound(SFX_PAUSE);
	
	// no Window plane here
	VDP_setReg(0x12,0x00);

	// Bring in the colors
	VDP_doCRamDMA((u32)pal_lyle, PAUSE_PALNUM * 32, 16);
	system_set_h_split(0, 0, NULL);
}
