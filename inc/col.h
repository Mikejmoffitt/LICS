// col.h
// 80-line and 40-line string printing for printing basic debug information

#ifndef COL_H
#define COL_H
#include <genesis.h>
#include "gfx.h"
#include "vramslots.h"
#define COL_SEL 128

void col_init(void);
void col_puthex(u16 x, u16 y, u32 val);
void col_puts(u16 x, u16 y, char *s);
void col_puts40(u16 x, u16 y, char *s);

#endif
