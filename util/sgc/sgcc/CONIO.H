#ifndef _INC_CONIO_

extern uint extWIDTH;

typedef struct{
	unsigned char winleft;
	unsigned char wintop;
	unsigned char winright;
	unsigned char winbottom;
	unsigned char attribute;
	unsigned char normattr;
	unsigned char currmode;
	unsigned char screenheight;
	unsigned char screenwidth;
	unsigned char curx;
	unsigned char cury;
} text_info_rec;

extern text_info_rec text_info;

/* included */
void    clrscr();
void    gotoxy(posx, posy);
uint    wherex();
uint    wherey();
uint    putch(_c);

/* included by Haroldo */
int		cputs(str);
void	window(left, top, right, bottom);

/* Extra functions (by Haroldo) */
void	init_text_mode();
void	out_text_XY(X,Y,pal,fieldAddr,str);
void	write_uint(number,pad);
void	write_ulong(number,pad);

/* to include */
/*
int     cprintf(const char *_format, ...) __attribute__((format(printf,1,2)));
void    delline(void);
int     getch(void);
int     getche(void);
void    textbackground(int _color);
void    textcolor(int _color);
*/


/* no necessary or impossible to define */
/*
enum text_modes { LASTMODE=-1, BW40=0, C40, BW80, C80, MONO=7, C4350=64 };

enum COLORS {
	BLACK,          
	BLUE,
	GREEN,
	CYAN,
	RED,
	MAGENTA,
	BROWN,
	LIGHTGRAY,
	DARKGRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTCYAN,
	LIGHTRED,
	LIGHTMAGENTA,
	YELLOW,
	WHITE
};

extern int directvideo; 
extern int _wscroll;

#define BLINK   0x80

#define _NOCURSOR      0
#define _SOLIDCURSOR   1
#define _NORMALCURSOR  2

void    blinkvideo(void);
char *  cgets(char *_str); 
void    clreol(void);
int     cscanf(const char *_format, ...) __attribute__((format(scanf,1,2)));
int     gettext(int _left, int _top, int _right, int _bottom, void *_destin);
void    gettextinfo(struct text_info *_r);
void    highvideo(void);
void    insline(void);
void    intensevideo(void);
void    lowvideo(void);
int     movetext(int _left, int _top, int _right, int _bottom, int _destleft, int _desttop);
void    normvideo(void);
int     puttext(int _left, int _top, int _right, int _bottom, void *_source);
void    _set_screen_lines(int _nlines);
void    _setcursortype(int _type);
void    textmode(int _mode);
void    textattr(int _attr);
int     ungetch(int); 
*/

#define _INC_CONIO_

#endif
