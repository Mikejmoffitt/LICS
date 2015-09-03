#ifndef VRAMSLOTS_H
#define VRAMSLOTS_H
/*

This is where VRAM slots are determined for everything in the game for some
minimal amount of static memory management.

VRAM_SLOT indeces are specified in tiles. Each tile is 32 bytes, but the VDP
routines disregard the LSB so an increment of 1 is moving across an entire
16-bit word. So, 64 bytes in would be address 32, not address 64. This is why
the length of a 256-tile DMA is only (256 * 16).

*/

#define MAP_FG_VRAM_SLOT 0
#define MAP_FG_VRAM_LEN 256

#define MAP_BG_VRAM_SLOT (MAP_FG_VRAM_SLOT + MAP_FG_VRAM_LEN)
#define MAP_BG_VRAM_LEN 512

#define PLAYER_VRAM_SLOT (MAP_BG_VRAM_SLOT + MAP_BG_VRAM_LEN)
#define PLAYER_VRAM_LEN 9

#define HUD_VRAM_SLOT (PLAYER_VRAM_SLOT + PLAYER_VRAM_LEN)
#define HUD_VRAM_LEN 32

#define CUBE_VRAM_SLOT (HUD_VRAM_SLOT + HUD_VRAM_LEN)
#define CUBE_VRAM_LEN 64

#define PARTICLES_VRAM_SLOT (CUBE_VRAM_SLOT + CUBE_VRAM_LEN)
#define PARTICLES_VRAM_LEN 77

#define COL_VRAM_SLOT MAP_BG_VRAM_SLOT + 256
#define COL_VRAM_LEN 256


#define MAP_FG_PALNUM 0
#define MAP_BG_PALNUM 1
#define OBJECTS_PALNUM 2
#define PLAYER_PALNUM 3
#define CUBE_PALNUM PLAYER_PALNUM
#define HUD_PALNUM PLAYER_PALNUM
#define PARTICLES_PALNUM PLAYER_PALNUM

#endif
