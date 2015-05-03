/************************************************************************
 *                      GENESISB.C                                      *
 *                                                                      *
 *                         by Haroldo de Oliveira Pinheiro              *
 ************************************************************************/

#include "genesis.h"

uint  extWIDTH;

/************** INIT_GFX ****************
 * Initializes the graphics registers   *
 *                                      *
 * A Plan Adr = c000                    *
 * window adr = b000                    *
 * B Plan adr = e000                    *
 * sprite adr = bc00                    *
 * HScrol adr = b800                    *
 *                                      *
 * Mode : 320x224                       *
 * Bkg  : 64x64 tiles (both)            *
 *                                      *
 *   Made by Haroldo O. Pinheiro        *
 ****************************************/
void init_GFX()
{
    register uint *pw;

    pw = (uint *) GFXCNTL;

    *pw = 0x8016;   /* reg. 0 - Enable HBL */
    *pw = 0x8174;   /* reg. 1 - Enable display, VBL, DMA + VCell size */
    *pw = 0x8230;   /* reg. 2 - Plane A =$30*$400=$C000 */
    *pw = 0x832C;   /* reg. 3 - Window  =$2C*$400=$B000 */
    *pw = 0x8407;   /* reg. 4 - Plane B =$7*$2000=$E000 */
    *pw = 0x855E;   /* reg. 5 - sprite table begins at $BC00=$5E*$200 */
    *pw = 0x8600;   /* reg. 6 - not used */
    *pw = 0x8700;   /* reg. 7 - Background Color number*/
    *pw = 0x8800;   /* reg. 8 - not used */
    *pw = 0x8900;   /* reg. 9 - not used */
    *pw = 0x8a01;   /* reg 10 - HInterrupt timing */
    *pw = 0x8b00;   /* reg 11 - $0000abcd a=extr.int b=vscr cd=hscr */
    *pw = 0x8c01;   /* reg 12 - hcell mode + shadow/highight + interlaced mode (40 cell, no shadow, no interlace)*/
    *pw = 0x8d2E;   /* reg 13 - HScoll Table = $B800 */
    *pw = 0x8e00;   /* reg 14 - not used */
    *pw = 0x8f02;   /* reg 15 - auto increment data */
    *pw = 0x9011;   /* reg 16 - scrl screen v&h size (64x64) */
    
    extWIDTH = 64;
    
    *pw = 0x9100;   /* reg 17 - window hpos */
    *pw = 0x92ff;   /* reg 18 - window vpos */
};

/*************** PUT_TILE ***************
 * Puts a single tile on the screen.    *
 *                                      *
 * tile   = tile number                 *
 * attr   = tile attribute              *
 * posx   = X position                  *
 * posy   = Y position                  *
 * fieldAdr = adress of destination     *
 *              (ex: APLAN, BPLAN)      *
 *                                      *
 *   Made by Haroldo O. Pinheiro        *
 ****************************************/
void put_tile(tile, attr, posx, posy, fieldAdr)
/* baseada na fun‡Æo show_tiles, de genesis.h */
uint tile;
uint attr;
uint posy;
uint posx;
uint fieldAdr;
{
    register ulong *pl;
    register uint *pw;
    register ulong i;

    /* Point to the coordinate we want (WIDTH=Plane width defined in .H) */
    i = fieldAdr + (  2 * (posx + (extWIDTH*posy) ) ); 

    pl = (ulong *) GFXCNTL;
    *pl = GFX_WRITE_ADDR(i);

    pw = (uint *) GFXDATA;

   /* Grava caracter+atributo */
    *pw = tile | attr;

}

/************* SET_BKG_DATA *************
 * Loads a tile set to the video memory *
 *                                      *
 * first  = first tile to replace       *
 * count  = number of tiles to load     *
 * data   = pointer to tile data        *
 *                                      *
 *   Made by Haroldo O. Pinheiro        *
 ****************************************/
void set_bkg_data(first,count,data)
uint first;
uint count;
uint *data;
{
	register uint i;

	/* Copia os caracteres da imagem, 32 de cada vez */
	for(i = first*32; count; i += 32*32){
		if(count > 32){
			dma_vram_copy(data, i, 32*32);
			count -= 32;
		}else{
			dma_vram_copy(data, i, count*32);
			count = 0;
		}
		data += 32*32/2;
	}
}

/************ SET_BKG_TILES *************
 * Draws a rectangle of tiles on the    *
 * background map.                      *
 *                                      *
 * X, Y     = position on the bkg       *
 * W, H     = size of the rectangle     *
 * data     = pointer to the map data   *
 * fieldAdr = adress of destination     *
 *              (ex: APLAN, BPLAN)      *
 * basetile = this will be added to the *
 *            map's tile number         *
 *                                      *
 *   Made by Haroldo O. Pinheiro        *
 ****************************************/
void set_bkg_tiles(X,Y,W,H,data,fieldAddr,basetile)
uint X,Y;
uint W,H;
uint *data;
uint fieldAddr;
uint basetile;
{
    register ulong *pl;
    register uint *pw;
    register ulong l;
	register uint i, j;

	for(i = 0; i != H; i++){
		/* Aponta para o endere‡o inicial da linha */
	    l = fieldAddr + (  2 * (X + (extWIDTH*Y) ) ); 

	    pl = (ulong *) GFXCNTL;
	    *pl = GFX_WRITE_ADDR(l);

	    pw = (uint *) GFXDATA;

		/* Grava a linha */
		for(j = 0; j != W; j++){
			*pw = (*data)+basetile;
			data++;
		}

		Y++;
	}
}

/************ FILL_BKG_RECT *************
 * Fills a rectangular area of the bkg  *
 * with a single character/attribute    *
 *                                      *
 * X, Y     = position on the bkg       *
 * W, H     = size of the rectangle     *
 * fieldAdr = adress of destination     *
 *              (ex: APLAN, BPLAN)      *
 * basetile = the rectangle will be     *
 *            filled with this          *
 *                                      *
 *   Made by Haroldo O. Pinheiro        *
 ****************************************/
void fill_bkg_rect(X,Y,W,H,fieldAddr,basetile)
uint X,Y;
uint W,H;
uint fieldAddr;
uint basetile;
{
    register ulong *pl;
    register uint *pw;
    register ulong l;
	register uint i, j;

	for(i = 0; i != H; i++){
		/* Aponta para o endere‡o inicial da linha */
	    l = fieldAddr + (  2 * (X + (extWIDTH*Y) ) ); 

	    pl = (ulong *) GFXCNTL;
	    *pl = GFX_WRITE_ADDR(l);

	    pw = (uint *) GFXDATA;

		/* Grava a linha */
		for(j = 0; j != W; j++){
			*pw = basetile;
		}

		Y++;
	}
}

/************ FADE_FROM_TO **************
 * Fades from one palette to another    *
 *                                      *
 * palSrc   = source palette            *
 * palDest  = destination palette       *
 * palNum   = # of the palette (0..2)   *
 *                                      *
 *   Made by Haroldo O. Pinheiro        *
 ****************************************/
void fade_from_to(palSrc, palDest, palNum)
uint *palSrc;
uint *palDest;
uint palNum;
{
    uint tmp_pal[16];
    register ulong *pl;
    register uint *pw;
    register uint *pal_ptr;
    register uint i, j, c;
    register ulong Decal;

    /* see set_colors() */
    Decal = (32 * palNum);

    /* Initialize a temporary palette */
    for (i = 0; i < 16; i++)
        tmp_pal[i] = palSrc[i];

    /* Fade in loop */
    for (i = 0; i < 16; i++)
    {
        /* Pointer to palette we want */
        pal_ptr = palDest;
    
        /* Setup GFX to save colors */
        pl = (ulong *) GFXCNTL;
        *pl = GFX_COLOR_WRITE_ADDR(Decal);

        /* Setup GFX data register */
        pw = (uint *) GFXDATA;
        
        /* Increment one color B,G,R at a time */
        for (j = 0; j < 16; j++)
        {
            /* increment OR decrement BLUE till reaching target color */
            c = tmp_pal[j] & 0xf00;
            if (c < (*pal_ptr & 0xf00)){
                tmp_pal[j] += 0x100;
			}else if (c > (*pal_ptr & 0xf00)){
                tmp_pal[j] -= 0x100;
			}
            /* check GREEN */
            c = tmp_pal[j] & 0xf0;
            if (c < (*pal_ptr & 0xf0)){
                tmp_pal[j] += 0x10;
			}else if (c > (*pal_ptr & 0xf0)){
                tmp_pal[j] -= 0x10;
			}
            /* check RED */
            c = tmp_pal[j] & 0xf;
            if (c < (*pal_ptr & 0xf)){
                tmp_pal[j] += 0x1;
			}else if (c > (*pal_ptr & 0xf)){
                tmp_pal[j] -= 0x1;
			}
			pal_ptr++;

            /* Store color */
            *pw = tmp_pal[j];
        }

        /* Wait a sec... */
        for (j = 0; j < 4; j++)
            wait_sync();
    }
}
