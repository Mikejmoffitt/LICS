#ifndef COL_H
#define COL_H
#include <genesis.h>
#include "gfx.h"

#define COL_FONT_VRAM_OFFSET 0
#define COL_FONT_SEL 128

void col_init(void);
void col_puts(u16 x, u16 y, char *s);
void col_puts40(u16 x, u16 y, char *s);

#endif
