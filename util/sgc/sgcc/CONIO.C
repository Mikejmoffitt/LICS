/************************************************************************
 *                      CONIO.C                                         *
 *                                                                      *
 *                         by Kaneda                                    *
 *                         major improvements by Haroldo O. Pinheiro    *
 *                                                                      *
 *  Initially, simulate Borland text video funcs for GPP                *
 *  copyright (c) 1991 J. Alan Eldridge                                 *
 *                                                                      *
 *  27Oct91    J. Alan Eldridge       created                           *
 *  25Dec98    Kaneda                 Rewrite all for Genesis           *
 *  10Jan99    Kaneda                 Add CLRSCR                        *
 *                                    Mod OOUTTEXTXY= incr TEXT_pos/y   *
 *  01Avr99    Kaneda                 Add PUTCHAR                       *
 *                                                                      *
 *  01Apr03    Haroldo O. Pinheiro    Rewrote everything, trying to     *
 *                                    make it more compatible with      *
 *                                    the standard conio.h, and, at the *
 *                                    same time, trying to make better  *
 *                                    use of the capabilities of Sega   *
 *                                    Genesis. Added some extras, too.  *
 *                                                                      *
 ************************************************************************/

#include "genesis.h"
#include "conio.h"
#include "string.h"

text_info_rec text_info;

/*********************GOTOXY*********************
 * Sets the cursor position                     *
 *                                              *
 * Made by Kaneda                               *
 * Modified by Haroldo O. Pinheiro              *
 ************************************************/
void    gotoxy(posx, posy)
uint posx, posy;
{
		if( (posx < 1 )                                          ||
	 		(posx > text_info.winright - text_info.winleft + 1 ) || 
		    (posy < 1 )                                          || 
		    (posy > text_info.wintop - text_info.winbottom + 1 )    ){
			return;
        }

        text_info.curx = posx;
        text_info.cury = posy;
}


/*********************WHEREX*********************
 * Returns cursor column number.                *
 *                                              *
 * Made by Kaneda                               *
 * Modified by Haroldo O. Pinheiro              *
 ************************************************/
uint     wherex()
{
        return text_info.curx;
}


/*********************WHEREY*********************
 * Returns cursor line number.                  *
 *                                              *
 * Made by Kaneda                               *
 * Modified by Haroldo O. Pinheiro              *
 ************************************************/
uint     wherey()
{
        return text_info.cury;
}

/******************* CLRSCR *****************************
 * RAZ of fields                                        *
 *                                                      *
 *  Made by Kaneda                                      *
 *  Modified by Haroldo O. Pinheiro                     *
 ********************************************************/
void clrscr()
{
    register ulong *pl;
    register uint *pw;
    register uint i;

	/* clear plane A */
    pl = (ulong *) GFXCNTL;
    *pl = GFX_WRITE_ADDR(APLAN);

    pw = (uint *) GFXDATA;

	for (i= 0; i < extWIDTH*HEIGHT; i++)
		*pw = 0;

	/* clear plane B */
    pl = (ulong *) GFXCNTL;
    *pl = GFX_WRITE_ADDR(BPLAN);

    pw = (uint *) GFXDATA;

	for (i= 0; i < extWIDTH*HEIGHT; i++)
		*pw = 0;

	/* Reset cursor */
	gotoxy(1,1); 

}


/**************** PUTCH ************************        
 * Prints an character to the screen.          *
 *                                             *
 *      Made by Kaneda                         *
 *      Modified by Haroldo O. Pinheiro        *
 *                                             *
 ***********************************************/
uint putch(_c)
uint _c;
{
    register ulong *pl;
    register uint *pw;
    register ulong i;
    register ulong TileDef;
	register uint TEXT_posx, TEXT_posy;

	TEXT_posx = text_info.curx + text_info.winleft - 1;
	TEXT_posy = text_info.cury + text_info.wintop - 1;

    /* Point to the coordinate we want*/        
    i = APLAN + ( 2 * (TEXT_posx + (extWIDTH*TEXT_posy) ) );

    pl = (ulong *) GFXCNTL;
    *pl = GFX_WRITE_ADDR(i);

    /* TileDef */

    pw = (uint *) GFXDATA;
                        /* let 0 for a next use */
/*    TileDef = 0x8000 + (0 << 13);*/
    TileDef = 0x0000;

	switch(_c){
		case '\n':
       		text_info.curx = 1;
   			text_info.cury++;
		break;

		case '\b':
			if(text_info.curx != 1){
       			text_info.curx--;
			}
		break;

		case '\t':
			text_info.curx += 8;
		    if (text_info.curx > (text_info.winright-text_info.winleft+1)) {
        		text_info.curx -= (text_info.winright-text_info.winleft+1);
	    	}
		break;

		default:
    		*pw = (TileDef + _c);
		    if (text_info.curx++ > (text_info.winright-text_info.winleft+1)) {
        		text_info.curx = 1;
    			text_info.cury++;
	    	}
	}

    return _c;

}

/************** INIT_TEXT_MODE *****************        
 * Initializes the default text mode.          *
 * It's up to the programmer to have the       *
 * appropriate font loaded. See font.c         *
 *                                             *
 *      Made by Haroldo de Oliveira Pinheiro   *
 *                                             *
 ***********************************************/
void init_text_mode(){
	text_info.winleft   = 0;
	text_info.wintop    = 0;
	text_info.winright  = extWIDTH - 1;
	text_info.winbottom = HEIGHT - 1;

	text_info.screenwidth  = extWIDTH;
	text_info.screenheight = HEIGHT;

	clrscr();
}

/******************* CPUTS *********************
 * Writes a string to the screen.              *
 *                                             *
 *      Made by Haroldo de Oliveira Pinheiro   *
 *                                             *
 ***********************************************/
int cputs(str)
char *str;
{
	register int cnt;

	cnt = 0;
	while(*str){
		putch(*str);
		str++;
		cnt++;
	}

	return cnt;
}

/******************* WINDOW ********************
 * Sets the coordinates of the text window.    *
 *                                             *
 *      Made by Haroldo de Oliveira Pinheiro   *
 *                                             *
 ***********************************************/
void window(left, top, right, bottom)
int left;
int top;
int right;
int bottom;
{
	text_info.winleft = left;
	text_info.wintop  = top;

	text_info.winright  = right;
	text_info.winbottom = bottom;
}

/****************** OUT_TEXT *******************
 * Raw text output routine.                    *
 *                                             *
 *      Made by Haroldo de Oliveira Pinheiro   *
 *                                             *
 ***********************************************/
void out_text_XY(X,Y,pal,fieldAddr,str)
uint X;
uint Y;
uint pal;
uint fieldAddr;
char *str;
{
    register ulong *pl;
    register uint *pw;
    register ulong i;
    register uint attr,chr;

	attr = BUILD_ATTR(pal,0,0,0);

    /* Point to the coordinate we want (WIDTH=Plane width defined in .H) */
    i = fieldAddr + (  2 * (X + (extWIDTH*Y) ) ); 

    pl = (ulong *) GFXCNTL;
    *pl = GFX_WRITE_ADDR(i);

    pw = (uint *) GFXDATA;

   /* Grava caracter+atributo */
   while(*str){
      *pw = *str | attr;
      str++;
   }
}

/***************** WRITE_UINT ******************
 * Writes an unsigned integer number.          *
 *                                             *
 *      Made by Haroldo de Oliveira Pinheiro   *
 *                                             *
 ***********************************************/
void write_uint(number,pad)
uint number;
int pad;
{
	register char *ptr;
	char buffer[6];

	if(pad > 5){
		pad = 5;
	}

	if(number){
		buffer[5] = 0;
		ptr = &(buffer[5]);

		while(number){
			ptr--;
			pad--;
			*ptr = '0' + (number % 10);
			number /= 10;
		}
	}else{
		ptr--;
		pad--;
		*ptr = '0';
	}

	if(pad > 0){
		while(pad){
			ptr--;
			pad--;
			*ptr = ' ';
		}
	}

	cputs(ptr);
}

/**************** WRITE_ULONG ******************
 * Writes an unsigned long integer number.     *
 *                                             *
 *      Made by Haroldo de Oliveira Pinheiro   *
 *                                             *
 ***********************************************/
void write_ulong(number,pad)
ulong number;
int pad;
{
	register char *ptr;
	char buffer[11];

	if(pad > 10){
		pad = 10;
	}

	if(number){
		buffer[10] = 0;
		ptr = &(buffer[10]);

		while(number){
			ptr--;
			pad--;
			*ptr = '0' + (number % 10);
			number /= 10;
		}
	}else{
		ptr--;
		pad--;
		*ptr = '0';
	}

	if(pad > 0){
		while(pad){
			ptr--;
			pad--;
			*ptr = ' ';
		}
	}

	cputs(ptr);
}
