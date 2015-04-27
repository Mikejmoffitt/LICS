#include <genesis.h>
#include <stdio.h>
#include "col.h"
#include "mpad.h"
#include "sprites.h"
#include "player.h"

u16 hint_val = 1;
u16 col = 0;
u16 col_off = 0;
u16 phrase_num = 0;

_voidCallback *v_int(void)
{
	return;
}	

_voidCallback *h_int(void)
{
	return; 
}

u16 lylepal[] = {
	0x000, 0xEEE, 0x888, 0x444,
	0x24A, 0x0E8, 0xC44, 0x622,
	0x026, 0x0EE, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000
};

void setup(void)
{
	VDP_init();
	col_init();

	SYS_disableInts();
	SYS_setInterruptMaskLevel(0);
	SYS_setVIntCallback(v_int);
	VDP_setHInterrupt(0);
	SYS_setHIntCallback(h_int);
	SYS_enableInts();
	VDP_setScreenWidth320();
	VDP_setPlanSize(64,32);
}

int main(void)
{
	u16 assholes = 0;
	u16 delay_mod = 4;
	setup();
	col_puts40(8,3,"Lyle sprite test");
	VDP_setPalette(0,&lylepal);
	VDP_setHIntCounter(hint_val);
	volatile u8 p1 = pad_read(0);
	VDP_setHInterrupt(0);


	sprites_init();
	u8 i = 0;

	u8 size;

	u16 delay = 0;
	u8 dir = 0;
	VDP_setHInterrupt(0);
	u8 pad = 0;
	for (i = 0; i < 0x18; i++)
	{
		if (i < 0x10)
		{
			size = SPRITE_SIZE(2,3);	
		}
		else if (i < 0x14)
		{
			size = SPRITE_SIZE(3,2);
		}
		else
		{
			size = SPRITE_SIZE(3,3);
		}
		if (i == 0x18)
		{
			i = 0;
		}
		sprite_put(16 + (12 * i), 112 + (i % 2 == 0 ? 32 : 0), size, 256 + (9 * i));
		player_dma(i,32*(256 + 9*i));	
	}


	for (;;)
	{	
		VDP_waitVSync();
		sprites_dma(32);

	}
	return 0;	
}

/* vim: set noet: */
