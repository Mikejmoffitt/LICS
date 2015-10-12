#ifndef PAUSE_H
#define PAUSE_H

#include <genesis.h>

void pause_init(void);
void pause_dma_tiles(void);

// set palette, and draw the screen
void pause_setup(void);

void pause_exit(void);

void pause_screen_loop(void);

#endif
