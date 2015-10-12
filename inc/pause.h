#ifndef PAUSE_H
#define PAUSE_H

#include <genesis.h>

#define PAUSE_POWER_X 88
#define PAUSE_POWER_Y 176

#define PAUSE_MAP_X 8
#define PAUSE_MAP_Y 6

void pause_init(void);
void pause_dma_tiles(void);

// set palette, and draw the screen
void pause_setup(void);

void pause_exit(void);

void pause_screen_loop(void);

#endif
