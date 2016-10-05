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
	map_clear();
	VDP_doVRamDMA((u32)pausemap_layout, VDP_getWindowAddress(), (64 * 32));
	stopsound();
	playsound(SFX_PAUSE);
	VDP_setReg(0x12,0x1E);
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
	// Palette to fade from
	u16 orig_pal[64];
	u16 orig_pal_mod[64];
	u16 fade = 0;

	// Back up the palette
	VDP_getPalette(0, &orig_pal[0]);
	VDP_getPalette(1, &orig_pal[16]);
	VDP_getPalette(2, &orig_pal[32]);
	VDP_getPalette(3, &orig_pal[48]);

	u16 new_pal[64];

	for (int i = 0; i < 64; i++)
	{
		orig_pal_mod[i] = orig_pal[i];
	}

	// Fade to black
	fade = 0;
	while (fade < 16)
	{
		palette_att(&orig_pal_mod[0], 1);
		palette_att(&orig_pal_mod[16], 1);
		palette_att(&orig_pal_mod[32], 1);
		palette_att(&orig_pal_mod[48], 1);
		system_wait_v();
		VDP_doCRamDMA((u32)orig_pal_mod, 0, 64);
		fade++;
	}

	message_init();

	// Fade in new screen
	fade = 0;
	while (fade < 16)
	{
		for (int i = 0; i < 64; i++)
		{
			u16 *pal_pause_16 = (u16 *)pal_pause;
			new_pal[i]    = pal_pause_16[i & 0x0F];
		}
		palette_att(&new_pal[0], 15 - fade);
		palette_att(&new_pal[16], 15 - fade);
		palette_att(&new_pal[32], 15 - fade);
		palette_att(&new_pal[48], 15 - fade);
		system_wait_v();
		VDP_doCRamDMA((u32)new_pal, 0, 64);
		fade++;
	}

	message_loop(s);

	// Fade to black
	fade = 0;
	while (fade < 16)
	{
		palette_att(&new_pal[0], 1);
		palette_att(&new_pal[16], 1);
		palette_att(&new_pal[32], 1);
		palette_att(&new_pal[48], 1);
		system_wait_v();
		VDP_doCRamDMA((u32)new_pal, 0, 64);
		fade++;
	}

	system_wait_v();
	// Kill the window plane
	VDP_setReg(0x12, 0x00);
	system_set_h_split(0, 0, NULL);

	// Fade in original palette
	fade = 0;
	while (fade < 16)
	{
		for (int i = 0; i < 64; i++)
		{
			new_pal[i] = orig_pal[i];
		}
		palette_att(&new_pal[0], 15 - fade);
		palette_att(&new_pal[16], 15 - fade);
		palette_att(&new_pal[32], 15 - fade);
		palette_att(&new_pal[48], 15 - fade);
		system_wait_v();
		VDP_doCRamDMA((u32)new_pal, 0, 64);
		fade++;
	}
}
