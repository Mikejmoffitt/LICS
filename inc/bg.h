// bg.h
// Functions for supporting paralax backdrops

#ifndef BG_H
#define BG_H
#include <genesis.h>

void bg_load(u16 num);
void bg_scroll_x(u16 amt);
void bg_scroll_y(u16 amt);
void bg_dma_scroll(void);

#endif
