/*
	Minimal Example
		by Haroldo de Oliveira pinheiro
*/
/*
 *
 * A Plan Adr = c000
 * window adr = b000
 * B Plan adr = e000
 * sprite adr = bc00
 * HScrol adr = b800
 *
 */

#include "genesis.h"
#include "utils.h"
#include "psgsound.h"
#include "rdc.h"
#include "rpgwalk.h"

uint blocks_pal[16]={
0x000,
0x444,
0x888,
0xEEE,
0x004,
0x008,
0x00E,
0x040,
0x080,
0x0E0,
0x400,
0x800,
0xE00,
0x044,
0x088,
0x0EE
};

uint bkg_pal[16]={
0xfff,
0xe50,
0xfa8,
0xf96,
0xfba,
0xfa8,
0xf97,
0xfcb,
0xfba,
0xfcc,
0xf87,
0xfaa,
0xf99,
0xf88,
0xe22,
0x000
};

uint frame_pal[16]={
0x000,
0x420,
0x862,
0xE84,
0x004,
0x008,
0x00E,
0x040,
0x080,
0x0E0,
0x400,
0x800,
0xE00,
0x044,
0x088,
0x0EE
};

uint char1_pal[]={
0x220,
0x000,
0x888,
0x444,
0x48C,
0x048,
0x004,
0x0A0,
0x060,
0x840,
0xAA2,
0x0AC,
0x40C,
0x00A,
0x400,
0xCCC
};

uint  plyx,plyy;

/*************    Variables  *************/
int xwrite1, xwrite2;   

extern uint Ding1[];

unsigned char temp_buf[4096];

/*************  Main program  ***********/
void main()
{
    register uint i,j;
    register uint *pw;
	ulong tmp,tmp2;
    register uint joy;
	walker plywlk;

    /* Initialize Genesis GFX registers */
    init_GFX();
	init_text_mode();
    
/*    init_PSG();*/
    
    set_colors(0, blocks_pal);
    set_colors(1, char1_pal);
/*	init_options(); */

    /* some correct */
/*    pw = (uint *) GFXCNTL; */

/*    *pw = 0x8700; */  /* reg. 7 - Background Color number*/
/*    *pw = 0x9001; */  /* reg 16 - scrl screen v&h size */
/*    extWIDTH = 32;*/


    /* Wait until GFX reg are initialized */
    wait_sync();

	/* Inicializa o Sega Genesis File System */
	init_GFS();

	/* Carrega o sprite de teste */
/*	get_GFS_fileaddr(&tmp, "CHAR1.TIL");
    dma_vram_copy(tmp, 0, 256*32);*/

	get_GFS_fileaddr(&tmp, "CHAR1.RDC");
	RDC_Decompress(tmp, temp_buf);
    dma_vram_copy(temp_buf, 256*32, 256*32);

	get_GFS_fileaddr(&tmp, "FONT.CHR");

	loadfont((uchar *)tmp,0,256,15,0);

	putch('T'); putch('E'); putch('S'); putch('T'); putch('\n');
	putch('\t'); putch('T'); putch('E'); putch('S'); putch('X'); putch('\b'); putch('T');
	cputs("isto eh um teste\n");
	write_uint(12345,0);
	putch('\n');
	write_ulong(1234567890,0);
	putch('\n');
	write_uint(123,1);
	putch('\n');
	write_uint(123,5);

	init_walker(&plywlk,(uint)256);

	/* Loop principal */
	while(TRUE){
/*	    init_GFX();*/
		while(TRUE){
/*			def_sprite(1,8,8,0x0700,BUILD_ATTR(1,0,0,0)|0);*/
			draw_walker(&plywlk);
			show_sprite(1,1);
			handle_walker(&plywlk);

			wait_sync();

			joy = read_joypad1();
			if(joy & BUTTON_A){
				start_sound(1,Ding1);
				while(joy & BUTTON_A){
					joy = read_joypad1();
				}
			}
			if(joy & BUTTON_B){
				get_GFS_file(&tmp, &tmp2, "YEAH.RAW");
				play_sample(tmp, tmp2);
				while(joy & BUTTON_B){
					joy = read_joypad1();
				}
			}
			if(!plywlk.stepcnt){
				if(joy&JOY_UP){
					walker_walkto(&plywlk,RWK_FC_UP,1);
				}
				if(joy&JOY_DOWN){
					walker_walkto(&plywlk,RWK_FC_DOWN,1);
				}
				if(joy&JOY_LEFT){
					walker_walkto(&plywlk,RWK_FC_LEFT,1);
				}
				if(joy&JOY_RIGHT){
					walker_walkto(&plywlk,RWK_FC_RIGHT,1);
				}
			}
		}
	}
};

/************ init_tiles sub-program  **********/
void init_tiles()
{
#ifdef NEVERDEF
    register uint x, y;
    
    dma_vram_copy(FONT_BIN+16, 0, 256*32);
    dma_vram_copy(BLOCKS_BIN, 256*32, 64*32);
    dma_vram_copy(FRAME_BIN, (256+64)*32, 64*32);
    wait_sync();

    /* Define the palettes (now we can see the fields) */
    set_colors(1, blocks_pal);
    set_colors(2, frame_pal);
#endif
}

/*************  test_joy() sub-program  ***********/
void test_joy()
{
    register uint joy;

    joy= read_joypad1();

}



/*************  redraw sub-program  ***********/
void redraw()
{
	wait_sync();
}
