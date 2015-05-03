#ifndef _INC_GENESIS_

/*
 * genesis.h
 *
 * Copyright (c) 1996 by Paul W. Lee
 *
 * Permission is granted to anyone to use this software for any purpose
 * on any computer system, and to redistribute it freely, with the
 * following restrictions:
 * 1) No charge may be made other than reasonable charges for reproduction.
 * 2) Modified versions must be clearly marked as such.
 * 3) The authors are not responsible for any harmful consequences
 *    of using this software, even if they result from defects in it.
 */

/*
 * Modified by Haroldo de Oliveira Pinheiro 2003
 *
 * Increased number of sprites to 80 (was 40)
 * Added various new functions (see end of this include)
 * Added a new macro (BUILD_ATTR)
 * 
 * Note that all the new functions are in GENESISB.C
 *
 *
 *
 *
 *
 *
 */

/*
 *      GFX Registers
 */
#define GFXDATA 0xc00000
#define GFXCNTL 0xc00004

#define Z80_HALT  0xa11100
#define Z80_RESET 0xa11200
/*
 *      Joypad definitions
 */
#define JOY_UP          1
#define JOY_DOWN        2
#define JOY_LEFT        4
#define JOY_RIGHT       8
#define BUTTON_A        0x40
#define BUTTON_B        0x10
#define BUTTON_C        0x20
#define BUTTON_S        0x80

/*
 *      GFX address macros
 */
#define GFX_WRITE_ADDR(x) ((0x4000 + (x & 0x3fff)) << 16) + (x >> 14)
#define GFX_DMA_ADDR(x)   ((0x4000 + (x & 0x3fff)) << 16) + ((x >> 14) | 0x80)
#define GFX_COLOR_WRITE_ADDR(x) ((0xc000 + (x & 0x3fff)) << 16) + (x >> 14)

#define GFX_VERT_SCROLL(x) ((0x4000 + (x & 0x3fff)) << 16) + ((x >> 14) | 0x10)
#define GFX_HORZ_SCROLL(x) ((0x4000 + (x & 0x3fff)) << 16) + (x >> 14)   

/*
 *  Field Adr define
 */
#define APLAN 0xC000
#define WPLAN 0xB000
#define BPLAN 0xE000
/*#define SLIST 0xBE00*/
#define SLIST 0xBC00
#define HSCRL 0xB800

/*
 * (Haroldo/2003) Builds a tile attribute word
 */
#define BUILD_ATTR(pal,pri,flipV,flipH) ((flipH << 11) + (flipV << 12) + (pal << 13) + (pri << 15))

/*
 *  Modify this else it could bug :)
 *      WIDTH  = Width of the screen mod by reg #16
 *      HEIGHT = Height of the screen mod by reg #16
 *
 */

#define HEIGHT  64


/*
 *      Misc defines
 */
#define FALSE   0
#define TRUE    1
#define NULL    0

#define uchar   unsigned char
#define uint    unsigned int
#define ulong   unsigned long

/******* Function prototypes in GENESIS.C ************/
void dma_vram_copy(source, dest, len);
void show_tiles(from, to, pal, posx, posy, fieldAdr, pri, flipV, flipH);
void load_tiles_sp(source, nbre);
void def_sprite(number,posx,posy,size,from);
void show_sprite(from,to);
void reset_sprites(nbr);
void set_colors(number,pal);
void fade_in(palDest, palNum);
void init_joypad();
uint read_joypad1();
uint read_joypad2();
void wait_press();
void play_sample(addr, len);
void scrollv_A(pos);
void scrollv(posA, posB);
void scrollh(posA, posB);
void wait_sync();
void RAZ();

/******* Function prototypes in GENESISB.C (by Haroldo) ************/

extern uint  extWIDTH;

void init_GFX();
void put_tile(tile, attr, posx, posy, fieldAdr);
void set_bkg_data(first,count,data);
void set_bkg_tiles(X,Y,W,H,data,fieldAddr,basetile);
void fill_bkg_rect(X,Y,W,H,fieldAddr,basetile);
void fade_from_to(palSrc, palDest, palNum);

#define _INC_GENESIS_

#endif
