#include "system.h"
// ----------------
#include "sprites.h"
#include "save.h"
#include "music.h"
#include "cdaudio.h"
#include "vramslots.h"
#include "pausemap.h"

static vu16 vbl_active;
u16 system_osc;

static u16 debug_en;

// For doing a palette write at a certain line
static u16 hsplit_line;
static u16 hsplit_num;
static u16 *hsplit_pal;

void system_set_h_split(u16 line, u16 num, u16 *p)
{
	if (line)
	{
		hsplit_line = line;
		hsplit_pal = p;
		hsplit_num = num;
		VDP_setHInterrupt(1);
		VDP_setHIntCounter(line);
	}
	else
	{
		hsplit_line = 0;
		VDP_setHInterrupt(0);
	}
}

static _voidCallback *v_int(void)
{
	vbl_active = 1;
	// Poke Sega CD if we're doing that
	if (segacd_bios_addr)
	{
		volatile unsigned short *reg = (volatile unsigned short *)SEGACD_REG_CPU;
		*reg |= 0x0100;
	}
	if (hsplit_line)
	{
		VDP_setHInterrupt(1);
	}
	return NULL;
}

static _voidCallback *h_int(void)
{
	VDP_doCRamDMA((u32)hsplit_pal, hsplit_num << 5, 16);
	VDP_setHInterrupt(0);
	return NULL;
}

void system_init(void)
{
	VDP_init();
	// Configure interrupts
	SYS_disableInts();
	VDP_setHInterrupt(0);
	SYS_setVIntCallback((_voidCallback *)v_int);
	SYS_setHIntCallback((_voidCallback *)h_int);
	SYS_setInterruptMaskLevel(0);
	SYS_enableInts();

	// Set up basic VDP settings
	VDP_setPlanSize(SYSTEM_PLANE_W,SYSTEM_PLANE_H);
	VDP_setScreenWidth320();
	VDP_setScreenHeight240();
	VDP_setHilightShadow(0);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

	// Set base addresses for tables
	VDP_setBPlanAddress(0xC000);
	VDP_setWindowAddress(0xD000);
	VDP_setAPlanAddress(0xE000);
	VDP_setSpriteListAddress(0xF000);

	sprites_init();
	save_load();
	system_set_debug(0);
	// Allow holding A to force 480i
	if ((JOY_readJoypad(JOY_1) & BUTTON_A))
	{
		sram.opt_interlace = SAVE_OPT_INTERLACE_ENABLED;
	}
	// Allow holding C to force 240p
	if ((JOY_readJoypad(JOY_1) & BUTTON_C))
	{
		sram.opt_interlace = SAVE_OPT_INTERLACE_NORMAL;
	}
	// Hold Z for debug
	if ((JOY_readJoypad(JOY_1) & BUTTON_Z))
	{
		system_set_debug(1);
	}
	if (sram.opt_interlace == SAVE_OPT_INTERLACE_ENABLED)
	{
		VDP_setScanMode(INTERLACED_MODE1);
	}
	else
	{
		VDP_setScanMode(INTERLACED_NONE);
	}

	// Set up sound stuff engine
	music_init();
	cdaudio_init();
	system_set_h_split(0, 0, NULL);

	//VDP_setReg(0x11,0x88);
}

void system_wait_v(void)
{
	system_osc++;
	while (!vbl_active)
	{
		// Twiddle thumbs
	}
	vbl_active = 0;
}

void system_set_debug(u16 val)
{
	debug_en = val;
}	

void system_debug_cpu_meter(void)
{
	if (debug_en)
	{
		u16 i = GET_VCOUNTER;
		while (i < 240)
		{
			sprite_put(-4, i, SPRITE_SIZE(1,1), HUD_VRAM_SLOT + 14);
			i+=8;
		}
	}
}
