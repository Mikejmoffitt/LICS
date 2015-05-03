/************************************************************************
 *                      FONT.C                                          *
 *                                                                      *
 *                         by Haroldo de Oliveira Pinheiro              *
 ************************************************************************/

#include "genesis.h"
#include "font.h"

void loadfont(font,basetile,chrcount,forecolor,backcolor)
uchar *font;
uint basetile;
uint chrcount;
uchar forecolor;
uchar backcolor;
{
	register ulong *pl;
	register ulong i;
	register uchar *pd;
	register ulong row;
	register uchar bits;
	register uint  j;

	forecolor &= 0x0F;
	backcolor &= 0x0F;

	i = basetile << 5;

	pl = (ulong *) GFXCNTL;
	*pl = GFX_WRITE_ADDR(i);

	pl = (ulong *) GFXDATA;

	pd = font;

	for(i = chrcount << 3; i; i--){
		row = 0;
		bits = (*pd++);

		for(j = 8; j; j--){
			row <<= 4;
			if(bits & 0x80){
				row |= forecolor;
			}else{
				row |= backcolor;
			}
			bits <<= 1;
		}

		*pl = row;
	}
}
