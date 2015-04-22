#include <genesis.h>
#include <stdio.h>
#include "col.h"
#include "mpad.h"
#include "sprites.h"

u16 hint_val = 1;
u16 col = 0;
u16 col_off = 0;
u16 phrase_num = 0;


void lp(u16 x, u16 y, u16 val)
{
	val += 128;
	VDP_setTileMapXY(
				VDP_PLAN_A,
				TILE_ATTR_FULL(0,1,0,0,
				val + (COL_FONT_VRAM_OFFSET/32)),x,y);
}

void putCSHLogo(u16 x, u16 y)
{
	// top line
	lp(x,y,4); // corner TL
	for (u16 i = 1; i < 17; i++)
	{
		lp(x + i,y,5);
	}
	lp(x + 17,y,6);
	lp(x + 20,y,4);
	lp(x + 21,y,5);
	lp(x + 22,y,6);
	// line 2
	y++;
	lp(x,y,7);
	lp(x+17,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 3
	y++;
	lp(x,y,7);
	lp(x+2,y,4);
	for (int i = 3; i < 15; i++)
	{
		lp(x+i,y,5);
	}
	lp(x+15,y,6);
	lp(x+17,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 4
	y++;
	lp(x,y,7);
	lp(x+2,y,7);
	lp(x+15,y,7);
	lp(x+17,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 5
	y++;
	lp(x,y,7);
	lp(x+2,y,7);
	lp(x+15,y,7);
	lp(x+17,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 6
	y++;
	lp(x,y,7);
	lp(x+2,y,7);
	lp(x+5,y,4);
	for (int i = 6; i < 12; i++)
	{
		lp(x+i,y,5);
	}
	lp(x+12,y,6);
	lp(x+15,y,8);
	lp(x+16,y,5);
	lp(x+17,y,9);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 7
	y++;
	lp(x,y,7);
	lp(x+2,y,7);
	lp(x+5,y,7);
	lp(x+12,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 8
	y++;
	lp(x,y,7);
	lp(x+2,y,7);
	lp(x+5,y,7);
	lp(x+7,y,4);
	lp(x+8,y,5);
	lp(x+9,y,5);
	lp(x+10,y,6);

	lp(x+12,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 9
	y++;
	lp(x,y,7);
	lp(x+2,y,7);
	lp(x+5,y,7);
	lp(x+7,y,7);
	lp(x+10,y,8);
	lp(x+11,y,5);
	lp(x+12,y,9);

	lp(x+15,y,4);
	lp(x+16,y,5);
	lp(x+17,y,6);
	
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 10
	y++;
	lp(x,y,7);
	lp(x+2,y,7);
	lp(x+5,y,7);
	lp(x+7,y,7);

	lp(x+15,y,7);
	lp(x+17,y,7);

	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 11
	y++;
	lp(x,y,7);
	lp(x+2,y,7);

	lp(x+5,y,7);
	lp(x+7,y,8);
	lp(x+8,y,5);
	lp(x+9,y,5);
	lp(x+10,y,5);
	lp(x+11,y,5);
	lp(x+12,y,6);

	lp(x+15,y,7);
	lp(x+17,y,8);
	lp(x+18,y,5);
	lp(x+19,y,5);
	lp(x+20,y,9);
	lp(x+22,y,7);
	// line 12
	y++;
	lp(x,y,7);
	lp(x+2,y,7);

	lp(x+5,y,7);
	lp(x+12,y,7);
	lp(x+15,y,7);
	lp(x+22,y,7);
	// line 13
	y++;
	lp(x,y,7);
	lp(x+2,y,7);

	lp(x+5,y,8);
	lp(x+6,y,5);
	lp(x+7,y,5);
	lp(x+8,y,5);
	lp(x+9,y,5);
	lp(x+10,y,6);
	lp(x+12,y,7);
	lp(x+15,y,7);
	lp(x+17,y,4);
	lp(x+18,y,5);
	lp(x+19,y,5);
	lp(x+20,y,6);
	lp(x+22,y,7);
	// line 14
	y++;
	lp(x,y,7);
	lp(x+2,y,7);

	lp(x+10,y,7);
	lp(x+12,y,7);
	lp(x+15,y,7);
	lp(x+17,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 15
	y++;
	lp(x,y,7);
	lp(x+2,y,7);

	lp(x+5,y,4);
	lp(x+6,y,5);
	lp(x+7,y,6);
	lp(x+10,y,7);
	lp(x+12,y,7);
	lp(x+15,y,8);
	lp(x+16,y,5);
	lp(x+17,y,9);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 16
	y++;
	lp(x,y,7);
	lp(x+2,y,7);

	lp(x+5,y,7);
	lp(x+7,y,8);
	lp(x+8,y,5);
	lp(x+9,y,5);
	lp(x+10,y,9);
	lp(x+12,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 17
	y++;
	lp(x,y,7);
	lp(x+2,y,7);

	lp(x+5,y,7);
	lp(x+12,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 18
	y++;
	lp(x,y,7);
	lp(x+2,y,7);

	lp(x+5,y,8);
	lp(x+6,y,5);
	lp(x+7,y,5);
	lp(x+8,y,5);
	lp(x+9,y,5);
	lp(x+10,y,5);
	lp(x+11,y,5);
	lp(x+12,y,9);
	lp(x+15,y,4);
	lp(x+16,y,5);
	lp(x+17,y,6);
	lp(x+20,y,7);
	lp(x+22,y,7);
	
	
	// line 19
	y++;
	lp(x,y,7);
	lp(x+2,y,7);
	lp(x+15,y,7);
	lp(x+17,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	// line 19
	y++;
	lp(x,y,7);
	lp(x+2,y,7);
	lp(x+15,y,7);
	lp(x+17,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);


	// line 20
	y++;
	lp(x,y,7);
	lp(x+2,y,8);
	for (int i = 3; i < 15; i++)
	{
		lp(x+i,y,5);
	}
	lp(x+15,y,9);
	lp(x+17,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	
	

	// line 21
	y++;
	lp(x,y,7);
	lp(x+17,y,7);
	lp(x+20,y,7);
	lp(x+22,y,7);
	
	
	// Line 22
	
	y++;
	lp(x,y,8); // corner TL
	for (u16 i = 1; i < 17; i++)
	{
		lp(x + i,y,5);
	}
	lp(x + 17,y,9);
	lp(x + 20,y,8);
	lp(x + 21,y,5);
	lp(x + 22,y,9);



}

_voidCallback *v_int(void)
{
	//VDP_setPaletteColor(0,col);	
	//col_puts(0,1,"Got int...");
	col = col_off;
	return;
}	

_voidCallback *h_int(void)
{
	VDP_setPaletteColor(14,col);
	col += 0x002;
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
}

void print_phrase(u16 p)
{
	col_puts(0,26,"                                                                                        ");
	switch(p)
	{
		case 0:
			col_puts(0,26,"         Getting more done after 2:00 AM than most people do all day.");
			break;
		case 1:
			col_puts(0,26,"                         Social versus Technical!  ");
			break;
		case 2:
			col_puts(0,26,"             68K blows the '88 away! Welcome to Motorola Zone!     ");
			break;
		case 3:
			col_puts(0,26,"                        I said hey! What's going on?!       ");
			break;
		case 4:
			col_puts(0,26,"      Why buy something when you can do it yourself for twice the cost? ");
			break;
		case 5:
			col_puts(0,26,"                       We're working on drink, we swear!   ");
			break;
		case 6:
			col_puts(0,26,"         Lean synergy-driven agile development to leverage our product.  ");
			break;
		case 7:
			col_puts(0,26,"                          Can you hear my microphone? ");
			break;
		case 8:
			col_puts(0,26,"                         Microsoft wouldn't do that... ");
			break;

	}
}

#define NUM_STARS 64

typedef struct star star;
struct star
{
	u16 x;
	u16 y;
	u16 size;
	u8 speed;
	u8 offset;
};

int main(void)
{
	u16 assholes = 0;
	u16 delay_mod = 4;
	setup();
	VDP_setPaletteColor(0,0x000);
	VDP_setPaletteColor(1,0x444);
	VDP_setPaletteColor(2,0x888);
	VDP_setPaletteColor(3,0xEEE);
	VDP_setHIntCounter(hint_val);
	volatile u8 p1 = pad_read(0);
	VDP_setHInterrupt(0);
	sprites_init();

	col_puts40(8,0,"Computer Science House");

	putCSHLogo(8, 2);

	star stars[NUM_STARS];
	u16 i = 0;
	for (i = 0; i < NUM_STARS; i++)
	{
		stars[i].x = i * 5;
		stars[i].y = 128 + i * 8;
		stars[i].speed = (i % 7) + 1;
		stars[i].offset = (3 * i) % 4;
		stars[i].size = (i % 4 == 0) ? ('.' + COL_FONT_VRAM_OFFSET/32) : (128 + '.' + COL_FONT_VRAM_OFFSET/32);
	}

	print_phrase(8);
	u16 delay = 0;
	u8 dir = 0;
	VDP_setHInterrupt(1);
	for (;;)
	{
		VDP_waitVSync();
		VDP_setHInterrupt(0);
		sprites_dma(NUM_STARS + 1);
		VDP_setHInterrupt(1);
		delay++;

		/*
		if (delay % 128 == 0)
		{
			delay_mod = delay_mod >> 1;
			if (delay_mod == 1)
			{

				delay_mod = 32;
			}
		}
		*/
		if (delay == 2048)
		{
			if (phrase_num == 8)
			{
				phrase_num = 0;
			}
			hint_val++;
			if (hint_val == 12)
			{
				hint_val = 2;
			}
			delay_mod = delay_mod >> 1;
			if (delay_mod == 0)
			{
				delay_mod = delay_mod = 4;
			}
			VDP_setHIntCounter(hint_val);
			VDP_setHInterrupt(0);
			print_phrase(phrase_num);
			VDP_setHInterrupt(1);
			phrase_num++;

			delay = 0;
		}		
		if (delay % delay_mod == 0)
		{
			col_off += 0x002;
		}


		for (i = 0; i < NUM_STARS; i++)
		{
			stars[i].x = stars[i].x + stars[i].speed;
			if (stars[i].x >= 320)
			{
				stars[i].x = 0;
				stars[i].y = stars[i].y + stars[i].offset;
			}
			if (stars[i].y > 256)
			{
				stars[i].y-=256;
			}
			sprite_set(i, stars[i].x, stars[i].y,  SPRITE_SIZE(1,1), stars[i].size,i + 1);
		}

	}
	return 0;	
}

/* vim: set noet: */
