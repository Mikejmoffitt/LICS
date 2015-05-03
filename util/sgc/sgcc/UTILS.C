#include "genesis.h"
#include "utils.h"

/* Rotina de n£meros aleat¢rios, baseada em RG_RAND.C (SNIPPETS) */
#define A 16807         /* multiplier */
#define M 2147483647L   /* 2**31 - 1 */
#define Q 127773L       /* m div a */
#define R 2836          /* m mod a */

ulong randomnum;

long nextlongrand()
{
      unsigned long lo, hi;

      randomnum++; /* adicionei por minha conta */

      lo = A * (long)(randomnum & 0xFFFF);
      hi = M * (long)((unsigned long)randomnum >> 16);
      lo += (hi & 0x7FFF) << 16;
      if (lo > M)
      {
            lo &= M;
            ++lo;
      }
      lo += hi >> 15;
      if (lo > M)
      {
            lo &= M;
            ++lo;
      }
      return (long)lo;
}

uint random(range)
uint range;
{
    randomnum = nextlongrand();
	return ((uint)randomnum % range);
}

extern uint extWIDTH;

char char2hex(ch)
char ch;
{
	return ((ch <= '9')?(ch-'0'):(ch-'A'+10));
}

uint get_str_char(str)
char **str;
{
	register uint ch;

	switch(**str){
		case '~':
			(*str)++;
			if(**str){
				ch = char2hex(**str);
				(*str)++;
				if(**str){
					ch = (ch << 4)| char2hex(**str);
					(*str)++;
				}
			}else{
				ch = '~';
			}
		break;
		default:
			ch = **str;
			(*str)++;
		break;
	}
	
	return ch;
}

uint strlen2(str)
char *str;
{
	register uint cnt;

	cnt = 0;
	while(get_str_char(&str)){
		cnt++;
	}
	return cnt;
}

void out_text(X,Y,pal,fieldAddr,str)
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
      chr = get_str_char(&str);
      if(chr == '\t'){
	     *pw = attr;
	     *pw = attr;
	     *pw = attr;
	     *pw = attr;
      }else{
	     *pw = chr | attr;
      }
   }
}

void out_uint(X,Y,pal,fieldAddr,pad,num)
uint X;
uint Y;
uint pal;
uint fieldAddr;
uint pad;
uint num;
{
	register uint i;
	register char str[6],*ptr;

	if(pad > 5){
		pad = 5;
	}

	for(i = 0; i != 6; i++){
		str[i] = ' ';
	}
	str[5] = 0;
	str[4] = '0';
	ptr = &str[5];

	do{
		ptr--;
		*ptr = '0' + (num % 10);
		if(pad)	pad--;
		num /= 10;
	}while(num);

	while(pad){
		ptr--;
		pad--;
	}

	out_text(X,Y,pal,fieldAddr,ptr);
}

void out_ulong(X,Y,pal,fieldAddr,pad,num)
uint X;
uint Y;
uint pal;
uint fieldAddr;
uint pad;
ulong num;
{
	register uint i;
	register char str[10],*ptr;

	if(pad > 9){
		pad = 9;
	}

	for(i = 0; i != 10; i++){
		str[i] = ' ';
	}
	str[9] = 0;
	str[8] = '0';
	ptr = &str[9];

	do{
		ptr--;
		*ptr = '0' + (num % 10);
		if(pad)	pad--;
		num /= 10;
	}while(num);

	while(pad){
		ptr--;
		pad--;
	}

	out_text(X,Y,pal,fieldAddr,ptr);
}
