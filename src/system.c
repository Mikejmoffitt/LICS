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
	// Configure interrupts
	SYS_disableInts();
	SYS_setInterruptMaskLevel(0);
	SYS_setVIntCallback(v_int);
	SYS_enableInts();

	// Set up basic VDP settings
	VDP_init();
	VDP_setPlanSize(SYSTEM_PLANE_W,SYSTEM_PLANE_H);
	VDP_setScreenWidth320();
	VDP_setScreenHeight224();
	VDP_setHilightShadow(0);
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
