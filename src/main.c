#include <genesis.h>
#include "col.h"
#include "mpad.h"
#include "sprites.h"
#include "player.h"

u16 hint_val = 1;
u16 col = 0;
u16 col_off = 0;
u16 phrase_num = 0;

player pl;

u16 expal[] = {
	0xEEE, 0xCCC, 0xAAA, 0x888,
	0x666, 0x444, 0x222, 0x000,
	0x026, 0x0EE, 0x000, 0x000,
	0x000, 0x008, 0x080, 0x800
};

u16 lylepal[] = {
	0x000, 0xEEE, 0x888, 0x444,
	0x24A, 0x0E8, 0xC44, 0x622,
	0x026, 0x0EE, 0x000, 0x000,
	0x000, 0x000, 0x000, 0x000
};


_voidCallback *v_int(void)
{
	return;
}	

_voidCallback *h_int(void)
{
	return; 
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
	SYS_enableInts();
	VDP_setScreenWidth320();
	VDP_setPlanSize(64,32);
	VDP_setScreenHeight240();
}

int main(void)
{
	u16 delay_mod = 4;
	setup();
	col_puts40(12,2,"Lyle sprite test");
	col_puts(3,4,"Lyle's \"kicking\" animation has shadow next to his head, and\nhighlight above his foot.\n");
	col_puts(4,6,"This little block has priority bit set...\n          \n          \n          ");
	col_puts(3,26,"Color 0x3E and 0x3F should mask sprites, and HL should NOT \nmake a 0xEEE BG brighter.");
		pl.x = fix16Add(pl.x,pl.dx);
		pl.y = fix16Add(pl.y,pl.dy);
	VDP_setPalette(0,&expal);
	VDP_setPalette(3,&lylepal);
	VDP_setHIntCounter(128);
	volatile u8 p1 = pad_read(0);
	VDP_setHInterrupt(0);
	VDP_setHilightShadow(1);


	sprites_init();
	u8 i = 0;

	u8 size;

	u16 delay = 0;
	u8 dir = 0;
	VDP_setHInterrupt(1);
	u8 pad = 0;

	fix16 fzero;
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
		u16 tile = 256 + (9 * i);
		sprite_put(16 + (12 * i), 112 + (i % 2 == 0 ? 32 : 0), size, TILE_ATTR(3,i%2,0,1) + tile);
		player_dma(i,32*(256 + 9*i));	
	}
	
	for (;;)
	{	
		p1 = pad_read(0);
		if (p1 & KEY_RIGHT)
		{
			pl.dx = fix16Add(pl.dx,PLAYER_X_ACCEL);
		}
		else if (p1 & KEY_LEFT)
		{
			pl.dx = fix16Sub(pl.dx,PLAYER_X_ACCEL);
		}
		else
		{
			if (pl.dx > fzero)
			{
				pl.dx = fix16Sub(pl.dx,PLAYER_X_DECEL);	
				if (fix16ToInt(pl.dx) < fzero)
				{
					pl.dx = fzero;
				}
			}
			else
			{
				pl.dx = fix16Add(pl.dx,PLAYER_X_ACCEL);				
				if (pl.dx > fzero)
				{
					pl.dx = fzero;
				}
			}
		} 
		if (pl.dx > PLAYER_DX_MAX)
		{
			pl.dx = PLAYER_DX_MAX;
		}
		else if (pl.dx < PLAYER_DX_MIN)
		{
			pl.dx = PLAYER_DX_MIN;
		}
		if (fix16ToRoundedInt(pl.y) < 150)
		{
			if (p1 & KEY_C && pl.dy < fzero)
			{
				pl.dy = fix16Add(pl.dy,PLAYER_Y_ACCEL_WEAK);
			}
			else
			{
				pl.dy = fix16Add(pl.dy,PLAYER_Y_ACCEL);
			}
		}
		
		if (fix16ToInt(pl.y) >= 150 && fix16ToInt(pl.dy) >= 0)
		{
			if (p1 & KEY_C)
			{
				pl.dy = PLAYER_JUMP_DY;
			}
			else
			{
				pl.dy = intToFix16(0);
				pl.y = intToFix16(150);
			}
		}
		pl.x = fix16Add(pl.x,pl.dx);
		pl.y = fix16Add(pl.y,pl.dy);
		sprite_set(0,fix16ToRoundedInt(pl.x),fix16ToRoundedInt(pl.y),SPRITE_SIZE(3,3),TILE_ATTR(3,1,0,0) + 256 + (9 * 0x17),1);
		VDP_waitVSync();
		sprites_dma(32);

	}
	return 0;	
}

/* vim: set noet: */
