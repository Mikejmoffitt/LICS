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
	0x026, 0xEE0, 0x000, 0x000,
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
	u16 i = 0;

	u16 delay = 0;
	u8 dir = 0;
	VDP_setHInterrupt(0);
	u8 pad = 0;
	u8 sprnum = 0;
	for (;;)
	{	
		u8 size;
		p1 = pad_read(0);
		if (p1 & KEY_A)
		{
			if (pad == 0)
			{
				sprnum++;
				pad = 1;
			}
		}
		else
		{
			pad = 0;
		}
		if (sprnum < 0x10)
		{
			size = SPRITE_SIZE(2,3);	
		}
		else if (sprnum < 0x14)
		{
			size = SPRITE_SIZE(3,2);
		}
		else
		{
			size = SPRITE_SIZE(3,3);
		}
		if (sprnum == 0x18)
		{
			sprnum = 0;
		}

		sprite_put(160,112, size, 256);
		VDP_waitVSync();

		player_dma(sprnum,32*256);
		
		sprites_dma_simple();

	}
	return 0;	
}

/* vim: set noet: */
