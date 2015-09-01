#include "system.h"
// ----------------
#include "sprites.h"

static vu16 vbl_active;

static _voidCallback *v_int(void)
{
	vbl_active = 1;
	return;
}

static _voidCallback *h_int(void)
{
	return;
}

void system_init(void)
{
	VDP_init();
	// Configure interrupts
	SYS_disableInts();
	VDP_setHInterrupt(1);
	VDP_setHIntCounter(223 - 6);
	SYS_setVIntCallback(v_int);
	SYS_setHIntCallback(h_int);
	SYS_enableInts();

	// Set up basic VDP settings
	VDP_setPlanSize(SYSTEM_PLANE_W,SYSTEM_PLANE_H);
	VDP_setScreenWidth320();
	VDP_setScreenHeight224();
	VDP_setHilightShadow(0);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	sprites_init();

}

void system_wait_v(void)
{
	while (!vbl_active)
	{
		// Twiddle thumbs
	}
	vbl_active = 0;
}
