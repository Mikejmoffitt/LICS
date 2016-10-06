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

static void message_init(void)
{
	system_wait_v();
	VDP_setReg(0x12,0x1E);
	map_clear();
	VDP_doVRamDMA((u32)pausemap_layout, VDP_getWindowAddress(), (64 * 32));
}

static void message_loop(const char *s)
{
	u16 show_prompt_timeout = 100;
	// Fullscreen Window plane
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
	}
}

void message_screen(const char *s)
{
	u16 orig_pal[64];
	stopsound();
	playsound(SFX_PAUSE);

	// Back up the palette
	VDP_getPalette(0, &orig_pal[0]);
	VDP_getPalette(1, &orig_pal[16]);
	VDP_getPalette(2, &orig_pal[32]);
	VDP_getPalette(3, &orig_pal[48]);

	fade_out();

	message_init();
	system_wait_v();

	// New palette
	VDP_doCRamDMA((u32)pal_pause, 0, 16);
	VDP_doCRamDMA((u32)&orig_pal[16], 32, 16);
	VDP_doCRamDMA((u32)&orig_pal[32], 64, 16);
	VDP_doCRamDMA((u32)pal_pause, 96, 16);
	
	fade_in();

	message_loop(s);

	fade_out();

	system_wait_v();
	// Kill the window plane
	VDP_setReg(0x12, 0x00);
	// Restore palette
	VDP_doCRamDMA((u32)orig_pal, 0, 64);
	fade_in();
	system_set_h_split(0, 0, NULL);
}
