#ifndef TILESET_H
#define TILESET_H

#define TILESET_FG_OUTSIDE1 0
#define TILESET_FG_OUTSIDE2 1
#define TILESET_FG_INSIDE1 3

#define TILESET_FG_NUMTILES 256
#define TILESET_FG_DMA_SIZE (TILESET_FG_NUMTILES * 16);
#define TILESET_FG_PAL_NUM 0
#define TILESET_FG_VRAM_SLOT 0

#define TILESET_BG_NUMTILES 256
#define TILESET_BG_DMA_SIZE (TILESET_BG_NUMTILES * 16);
#define TILESET_BG_PAL_NUM 1
#define TILESET_BG_VRAM_SLOT 256

#include <genesis.h>
#include "gfx.h"
#include "pal.h"

void tileset_load_fg(register set);

#endif
