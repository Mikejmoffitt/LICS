#ifndef TITLESCREEN_H
#define TITLESCREEN_H

#include "vramslots.h"

#define BOGOLOGO_VRAM_SLOT ENEMY_VRAM_START
#define BOGOLOGO_VRAM_LEN (16 * 6)
#define TITLELOGO_VRAM_SLOT (BOGOLOGO_VRAM_SLOT + BOGOLOGO_VRAM_LEN)
#define TITLELOGO_VRAM_LEN (14 * 9)

void title_play_intro(void);

#endif
