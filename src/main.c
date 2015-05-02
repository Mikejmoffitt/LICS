#include <genesis.h>
#include "col.h"
#include "mpad.h"
#include "sprites.h"
#include "player.h"
#include "globalsys.h"
player pl;

u16 pal0[] = {
	0x000, 0x402, 0x60A, 0x60E,
	0x666, 0x444, 0x222, 0x000,
	0x026, 0x0EE, 0x000, 0x000,
	0x000, 0x008, 0x080, 0x800
};

u16 pal3[] = {
	0x000, 0xEEE, 0x888, 0x444,
	0x24A, 0x0E8, 0xC44, 0x622,
	0x026, 0x0EE, 0x000, 0xE88,
	0x084, 0xCCE, 0x00E, 0xC0E
};


_voidCallback *v_int(void)
{
	vbl_ok = 1;
	VDP_setPaletteColor(0,0x000);
	return;
}	

_voidCallback *h_int(void)
{
	return; 
}

void wait_v(void)
{
	while (!vbl_ok)
	{

	}
	vbl_ok = 0;
}


void setup(void)
{
	VDP_init();
	col_init();
	SYS_disableInts();
	SYS_setInterruptMaskLevel(0);
	SYS_setVIntCallback(v_int);
	VDP_setHInterrupt(0);
	SYS_setHIntCallback(h_int);
	VDP_setScreenWidth320();
	VDP_setPlanSize(64,32);
	VDP_setScreenHeight224();
	VDP_setHIntCounter(112);
	VDP_setHilightShadow(0);
	SYS_enableInts();
	sprites_init();
}

void player_test(void)
{
	player_init(&pl);	
	pl.y = 64;
	pl.x = intToFix32(64);
	for (;;)
	{
		player_eval_grounded(&pl);
		player_input(&pl);
		player_cp(&pl);
		player_accel(&pl);
		player_jump(&pl);
		player_move(&pl);

		player_eval_grounded(&pl);
		player_calc_anim(&pl);
		
		player_draw(&pl);
		
		wait_v();
		
		player_dma(pl.anim_frame, PLAYER_VRAM_SLOT);
		sprites_dma_simple();
	}

}

int main(void)
{
	u16 delay_mod = 4;
	setup();
	col_puts40(9,2,"Lyle in Cube Sector!?");
	col_puts(21,3,"More Blast Processing than you think!");
	col_puts40(0,21,"________________________________________");
	VDP_setPalette(0,&pal0);
	VDP_setPalette(3,&pal3);
	VDP_setHInterrupt(0);

	player_test();
	
	return 0;	
}
