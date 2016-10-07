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

static inline void screen_clear(void)
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
			*pwdata = (u16)(TILE_ATTR_FULL(1, 1, 0, 0, (0x500 + ' ')));
		}
	}
}

static void message_init(void)
{
	system_wait_v();
	VDP_setReg(0x12,0x1E);

	// Clear the whole screen
	screen_clear();

	// Copy in just the very top of the pause screen layout, to get the Pause
	// text along the top.
	VDP_doVRamDMA((u32)pausemap_layout, VDP_getWindowAddress(), (64 * 4));

	// Get the opaque font in there
	VDP_doVRamDMA((u32)gfx_font_opaque, 0xA400, 1536);

}

static void print_message(const char *s)
{

	// Draw text box
	w_puts("~________________________________~", 3, 5);
	w_puts("|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n", 3, 6);
	w_puts("|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n|\n", 36, 6);
	w_puts("~________________________________~", 3, 23);

	// Print message
	w_puts(s, 5, 7);
}

static void wait_for_buttons(void)
{
	u16 show_prompt_timeout = 100;
	for (;;)
	{
		if (show_prompt_timeout > 0)
		{
			show_prompt_timeout--;
			if (show_prompt_timeout == 0)
			{
				w_puts("Push button to continue", 9, 25);
			}
		}
		else
		{
			if (buttons & (BUTTON_A | BUTTON_B | BUTTON_C | BUTTON_X | BUTTON_Y | BUTTON_Z | BUTTON_START))
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
	u16 new_pal[64];
	stopsound();
	playsound(SFX_PAUSE);

	// Back up the palette
	VDP_getPalette(0, &orig_pal[0]);
	VDP_getPalette(1, &orig_pal[16]);
	VDP_getPalette(2, &orig_pal[32]);
	VDP_getPalette(3, &orig_pal[48]);

	fade_out();

	message_init();
	print_message(s);
	system_wait_v();

	// New palette
	for (u16 i = 0; i < 16; i++)
	{
		u16 *pal_1 = (u16 *)pal_pause;
		u16 *pal_2 = (u16 *)&orig_pal[16];
		u16 *pal_3 = (u16 *)&orig_pal[32];
		u16 *pal_4 = (u16 *)pal_pause;
		new_pal[i]    = pal_1[i];
		new_pal[i+16] = pal_2[i];
		new_pal[i+32] = pal_3[i];
		new_pal[i+48] = pal_4[i];
	}

	fade_in_to(new_pal);

	wait_for_buttons();

	fade_out();

	system_wait_v();
	// Kill the window plane
	VDP_setReg(0x12, 0x00);
	// Restore palette
	fade_in_to(orig_pal);
	system_set_h_split(0, 0, NULL);
}
