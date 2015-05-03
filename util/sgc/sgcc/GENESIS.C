/************************************************************************
 *                      GENESIS.C                                       *
 *                                                                      *
 *                         by Kaneda                                    *
 *                                                                      *
 *                                                                      *
 * History:                                                             *
 *      ?????98         First release                                   *
 *      20Nov98         Make it more readable :)                        *
 *      26Dec98         Put OUTTEXT_XY in CONIO.C                       *
 *      09Mar99         Add WAIT_PRESS (useful, no?)                    *
 *      16Aug99         Add Flipping in SHOW_TILES                      *
 *      31Aug99         Work on Scroll functions to add field B         *
 *                      Convert ScrollA functions in ScrollAB           *
 *                                                                      *
 ************************************************************************/

#include "genesis.h"
#include "conio.h"

extern uint extWIDTH;


/*************    Variables  *************/

ulong vtimer;           /* VBL timer */
int  sprites_def[80*5]; /* 80 sprites max */
uint  pal_black[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; /* black palette */


/************* DMA_VRAM_COPY ************
 * Use the DMA to copy the map data to  *
 * the GFX RAM.                         *
 * source = array of ulong              *
 * dest   = first number of the tiles   *
 *              (32*existing_tiles)     *
 * len    = length of ulong             *
 *              (32*nbr_tiles)          *
 *                                      *
 *   Made by P.Lee                      *
 ****************************************/
void dma_vram_copy(source, dest, len)
ulong source;
uint dest;
uint len;
{
    register uint *pw;
    register ulong *pl;

    /* Halt the Z80 for DMA */
    pw = (uint *) Z80_HALT;
    *pw = 0x100;

    /* Point to GFX register */
    pw = (uint *) GFXCNTL;

    /* Setup DMA length */
    len >>= 1;
    *pw = 0x9300L + (len & 0xff);
    *pw = 0x9400L + ((len >> 8) & 0xff);

    /* Setup DMA address */
    source >>= 1;
    *pw = 0x9500L + (source & 0xff);

    source >>= 8;
    *pw = 0x9600L + (source & 0xff);

    source >>= 8;
    *pw = 0x9700L + (source & 0xff);

    /* Enable DMA */
    pl = (ulong *) GFXCNTL;
    *pl = GFX_DMA_ADDR((long) dest);

    /* Enable Z80 */
    pw = (uint *) Z80_HALT;
    *pw = 0;
}


/**************** SHOW_TILES ********************
 * Show the tiles by writing the tile           *
 * number (1 to...) to the GFX memory.          *
 *     priority flipV           tile number     *
 *            |  |  _____________|              *
 * TileDef =  0000 0000 0000 0000               *
 *             ||  |                            *
 *        palette  flipH                        *
 *                                              *
 * fieldAdr = adress of plan of dest            *
 * pri = priority                               *
 *                                              *
 *      Made by P.Lee                           *
 *      Modified by Kaneda to allow arg         *
 *              allow Flipping                  *
 *              assume you define the WIDTH     *
 ************************************************/
void show_tiles(from, to, pal, posx, posy, fieldAdr, pri, flipV, flipH)
uint from;
uint to;
uint pal;
uint posx;
uint posy;
uint fieldAdr;
uint pri;
uint flipV;
uint flipH;
{
    register ulong *pl;
    register uint *pw;
    register ulong i;
    register ulong TileDef;


    /* Point to the coordinate we want (WIDTH=Plane width defined in .H) */
    i = fieldAdr + (  2 * (posx + (extWIDTH*posy) ) ); 

    pl = (ulong *) GFXCNTL;
    *pl = GFX_WRITE_ADDR(i);

    pw = (uint *) GFXDATA;

   /* We make TileDef (see up)*/
    TileDef = (flipH << 11) + (flipV << 12) + (pal << 13) + ( pri << 15);

    for (i=from;i < to+1;i++)
        *pw = (TileDef + i);

}

/************* LOAD_TILES_SP ********************
 * Load the tile data for sprite.               *
 * source = array of ulong                      *
 * nbre   = number of tile to load              *
 *                                              *
 * I prefer used DMA_VRAM_COPY (Kaneda)         *
 *                                              *
 *      Made by P.Lee                           *
 ************************************************/
void load_tiles_sp(source, nbre)
ulong source[];
uint  nbre;
{
        register ulong *pl;
        register uint i;

        nbre++;
        pl = (ulong *) GFXCNTL;
        *pl = GFX_WRITE_ADDR(0);

        pl = (ulong *) GFXDATA;

        for (i = 0; i < nbre * 8; i++)
                *pl = source[i];

}


/***************** DEF_SPRITE *******************
 * Definition of the sprite                     *
 * number = number of the sprite                *
 * posx, posy =position of the sprite           *
 * size = in hexa                               *
 *        ex: 0xb00 = 3*4 (b=1011)              *
 * from =                                       *
 *      bit 15= priority                        *          
 *      bit 14-13 = palette                     *          
 *      bit 12 = vertical flipping              *          
 *      bit 11 = horizontal flipping            *          
 *      bit 10-0 = tile number                  *
 *                                              *
 *      Made by Kaneda                          *
 ************************************************/
void def_sprite(number,posx,posy,size,from)
uint number;
int posx;
int posy;
uint size;
uint from;
{
        /* As you can see, all is based on a uint array         */
        /* It's my choice, but you can find something else      */
        
        uint loc;

        loc=5*(number-1);
        sprites_def[loc]=posy;
        sprites_def[loc+1]=size;
        sprites_def[loc+2]=number;
        sprites_def[loc+3]=from;
        sprites_def[loc+4]=posx;
}

/*********************** SHOW_SPRITE ********************
 *  Show the sprites                                    *
 *  from = number of the first sprite to show           *
 *  to   =   "    "   "  last    "    "   "             *
 *                                                      *
 *    Made by P.Lee                                     *
 *    Modified by Kaneda to use SPRITES_DEF[]           *
 ********************************************************/
void show_sprite(from,to)
uint from;
uint to;
{
    register ulong *pl;
    register uint *pw;
    register uint i;

    /* Sprite table begins at (see GENESIS.H)  */
    /* Note: We set this address using         */
    /*       reg. 85 int the init_gfx routine. */
    pl = (ulong *) GFXCNTL;
    *pl = GFX_WRITE_ADDR(SLIST);

    pw = (uint *) GFXDATA;

    for ( i = (from-1)*5 ; i < (to*5) ; i=i+5 )
    {

    /* Set the y position */
    *pw = 0x80 + sprites_def[i];

    /* We need to set the size and number of the */
    /* sprite we want to show.  0=last, 1=first, */
    /* 2=second, etc...                          */
    /* 0x500 = 2x2 tile sprite                   */
    /* 0 = last sprite                           */
    *pw = sprites_def[i+1] | sprites_def[i+2]; 

    /* bit 15= priority                          */
    /* bit 14-13 = palette                       */
    /* bit 12 = vertical flipping                */
    /* bit 11 = horizontal flipping              */
    /* bit 10-0 = tile number                    */
    /* The genesis shows sprites x then y.       */
    /* Note: Look at the tile data.              */
    *pw = sprites_def[i+3];

    /* x position */
    *pw = 0x80+sprites_def[i+4];
    }

}

/********************* RESET_SPRITES ********************
 * Reset of the nbr first sprites                       *
 *                                                      *
 *  Made by Kaneda                                      *
 ********************************************************/
void reset_sprites(nbr)
uint nbr;
{
        register ulong *pl;
        register uint i;

        /* Need comments ? */

        pl = (ulong *) GFXCNTL;
        *pl = GFX_WRITE_ADDR(SLIST);

        pl = (ulong *) GFXDATA;

        for (i= 0 ; i < (nbr+1)*(4*8)*(4*8) ; i++)
                *pl = 0x00000000;

}


/***************** SET_COLORS *******************
 * Set the color in the palette number (0-3).   *
 *                                              *
 *   Made by P.Lee                              *
 *   Modified by Kaneda to use arg and any pal  *
 ************************************************/
void set_colors(number,pal)
uint  number;
uint  *pal;
{
        register ulong *pl;
        register uint *pw;
        register uint x;

        register ulong Decal;

        /* Pal 1 is 32bytes, 16colors of 2bytes each, after Pal 0 */
        Decal = number * 32;

        /* Point on adress memory of the pal we want to modif/create */
        pl = (ulong *) GFXCNTL;
        *pl = GFX_COLOR_WRITE_ADDR(Decal);

        pw = (uint *) GFXDATA;

        for (x = 0; x < 16; x++)
                *pw=pal[x];
}

/********************** FADE_IN *************************
 * Fade in the palette PALNUM to the new palette PALDEST*
 *                                                      *
 * Made by P.Lee                                        *
 * Mod. by Kaneda                                       *
 *   fade to something else than the black pal          *
 ********************************************************/
void fade_in(palDest, palNum)
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
        tmp_pal[i] = 0;

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
            /* increment BLUE till reaching target color */
            c = tmp_pal[j] & 0xf00;
            if (c < (*pal_ptr & 0xf00))
                tmp_pal[j] += 0x100;
            /* check GREEN */
            c = tmp_pal[j] & 0xf0;
            if (c < (*pal_ptr & 0xf0))
                tmp_pal[j] += 0x10;
            /* check RED */
            c = tmp_pal[j] & 0xf;
            if (c < (*pal_ptr++ & 0xf))
                tmp_pal[j] += 0x1;

            /* Store color */
            *pw = tmp_pal[j];
        }

        /* Wait a sec... */
        for (j = 0; j < 4; j++)
            wait_sync();
    }
}


/********************* INIT_JOYPADS *************
 *  Initialize Joypads                          *
 *                                              *
 *   Made by P.Lee                              *
 ************************************************/
void init_joypad()
{
    register uchar *pb;

    pb = (uchar *) 0xa10009;
    *pb = 0x40;
    pb += 2;
    *pb = 0x40;
    pb += 2;
    *pb = 0x40;
}

/*********************** READ_JOYPAD1 *******************
 *  Read joypad #1                                      *
 *  Return a byte like this :                           *
 *                              76543210                *
 *                              SACBRLDU                *
 *  A = button A                                        *
 *  B = button B                                        *
 *  C = button C                                        *
 *  D = Down                                            *
 *  L = Left                                            *
 *  R = Right                                           *
 *  U = Up                                              *
 *  S = Start                                           *
 *                                                      *
 *  Made by P.Lee                                       *
 *      to be modified for 6 buttons use                *
*********************************************************/
uint read_joypad1()
{
    register uchar *pb;
    register uint i, j;

    pb = (uchar *) 0xa10003;

    *pb = 0x40;        /* check joypad */
    asm("nop");
    asm("nop");
    i = *pb & 0x3f;

    *pb = 0;           /* check buttons */
    asm("nop");
    asm("nop");
    j = (*pb & 0x30) << 2;

    return( ~(i|j) );
}

/************************** READ_JOYPAD2 ****************
 * see read_joypad1 for more details                    *
 ********************************************************/
uint read_joypad2()
{
    register uchar *pb;
    register uint i, j;

    pb = (uchar *) 0xa10005;

    *pb = 0x40;        /* check joypad */
    asm("nop");
    asm("nop");
    i = *pb & 0x3f;

    *pb = 0;           /* check buttons */
    asm("nop");
    asm("nop");
    j = (*pb & 0x30) << 2;

    return(~(i | j));
}


/********************** WAIT_PRESS **********************
 *  Wait for a button to be pressed                     *
 *                                                      *
 *  Button can be A, B, C or Start not direction!!      *
 *                                                      *
 *  Made by Kaneda                                      *
 ********************************************************/
void wait_press()
{
    uint tag;
    uint i;


    init_joypad();

    tag=0;

    while(tag == 0)
    {
        i=read_joypad1();

        if (i)
        {
                switch(i&0xff)
                {
                        case BUTTON_A:
                                tag=1;
                                break;
                        case BUTTON_B:
                                tag=1;
                                break;
                        case BUTTON_C:
                                tag=1;
                                break;
                        case BUTTON_S:
                                tag=1;
                                break;
                }
        }
    }
}


/********************** PLAY_SAMPLE *********************
 *  Play sample                                         *
 *  addr = address of the sample                        *
 *  len  = length of the sample                         *
 *                                                      *
 *  While the sound is playing, game continue           *
 *                                                      *
 *  Made by P.Lee                                       *
 ********************************************************/
void play_sample(addr, len)
ulong addr;
ulong len;
{
    register uint *pw;
    register uchar *pb;

    /* Halt the Z80 for DMA */
    pw = (uint *) Z80_HALT;
    *pw = 0x100;

    /* Point to Z80 RAM */
    pb = (uchar *) 0xa00039;

    /* Flag to begin playing */
    *pb++ = 1;

    /* Store address of sample */
    *pb++ = addr & 0xff;
    *pb++ = (addr >> 8) & 0xff;
    *pb++ = (addr >> 16) & 0xff;

    /* Store length of sample */
    *pb++ = len & 0xff;
    *pb++ = (len >> 8) & 0xff;
    *pb++ = (len >> 16) & 0xff;

    /* Enable Z80 and begin playing */
    pw = (uint *) Z80_HALT;
    *pw = 0;

}


/*********************** SCROLLV_A **********************
 * Scroll the mapfield A                                *
 * pos < 0 , scroll down                                *
 * pos > 0 , scroll up                                  *
 *                                                      *
 * Made by P.Lee                                        *
 * Modified by Kaneda                                   *
 ********************************************************/
void scrollv_A(pos)
int pos;
{
    register int *pw;
    register ulong *pl;

    /* Setup GFX pointers */
    pw = (int *) GFXDATA;
    pl = (ulong *) GFXCNTL;

    /* GFX scroll plane A */
    *pl = GFX_VERT_SCROLL(0);
    *pw = pos;

    wait_sync();
}

/*********************** SCROLLV_A **********************
 * Scroll the mapfield A                                *
 * pos < 0 , scroll down                                *
 * pos > 0 , scroll up                                  *
 *                                                      *
 * Made by P.Lee                                        *
 * Modified by Kaneda                                   *
 ********************************************************/
void scrollv(posA, posB)
int posA;
int posB;
{
    register int *pw;
    register ulong *pl;

    /* Setup GFX pointers */
    pw = (int *) GFXDATA;
    pl = (ulong *) GFXCNTL;

    /* GFX scroll plane A */
    *pl = GFX_VERT_SCROLL(0); /* 0 if all scroll else, the number of col */
    *pw = posA;
    *pw = posB;

    /* wait_sync(); */
}




/************************* SCROLLH **********************
 * Scroll mapfield A and B                              *
 * pos < 0 , scroll left                                *
 * pos > 0 , scroll right                               *
 *                                                      *
 * Made by Kaneda                                       *
 ********************************************************/
void scrollh(posA, posB)
int posA;
int posB;
{
    register int *pw;
    register ulong *pl;

    pl = (ulong *) GFXCNTL;
    *pl = GFX_WRITE_ADDR(HSCRL);

    pw = (int *) GFXDATA;
    *pw = posA;
    *pw = posB;
    /* you must scroll fieldA before fieldB....(?) */

    wait_sync(); 
}



/*********************** RAZ ****************************
 * RAZ of fields, sprites, palettes, scrolls            *
 *                                                      *
 *  Made by Kaneda                                      *
 ********************************************************/
void RAZ()
{
   uint i;

   set_colors(0, pal_black);
   set_colors(1, pal_black);
   set_colors(2, pal_black);
   set_colors(3, pal_black);

   clrscr();

   reset_sprites(1);

   scrollv_A(0);
   scrollh(0,0);

   wait_sync();
}

/************************** WAIT_SYNC *******************
 *  Wait for VBL.                                       *
 *                                                      *
 *  Made by P.Lee                                       *
 ********************************************************/
void wait_sync()
{
    register ulong x;

    x = vtimer;
    while (x == vtimer);
}
