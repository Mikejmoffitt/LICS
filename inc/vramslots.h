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

#define BG_VRAM_SLOT (MAP_FG_VRAM_SLOT + MAP_FG_VRAM_LEN)
#define BG_VRAM_LEN 128

#define PAUSE_VRAM_SLOT (BG_VRAM_SLOT + BG_VRAM_LEN)
#define PAUSE_VRAM_LEN 256

#define PLAYER_VRAM_SLOT (PAUSE_VRAM_SLOT + PAUSE_VRAM_LEN)
#define PLAYER_VRAM_LEN 16

#define HUD_VRAM_SLOT (PLAYER_VRAM_SLOT + PLAYER_VRAM_LEN)
#define HUD_VRAM_LEN 32

#define CUBE_VRAM_SLOT (HUD_VRAM_SLOT + HUD_VRAM_LEN)
#define CUBE_VRAM_LEN 52

#define PARTICLES_VRAM_SLOT (CUBE_VRAM_SLOT + CUBE_VRAM_LEN)
#define PARTICLES_VRAM_LEN 77

#define POWERUP_VRAM_SLOT (PARTICLES_VRAM_SLOT + PARTICLES_VRAM_LEN)
#define POWERUP_VRAM_LEN 24

#define ENEMY_START (POWERUP_VRAM_SLOT + POWERUP_VRAM_LEN)

// Many enemies!
#define METAGRUB_VRAM_SLOT ENEMY_START
#define METAGRUB_VRAM_LEN 9
#define FLIP_VRAM_SLOT (METAGRUB_VRAM_SLOT + METAGRUB_VRAM_LEN)
#define FLIP_VRAM_LEN 12
#define BOINGO_VRAM_SLOT (FLIP_VRAM_SLOT + FLIP_VRAM_LEN)
#define BOINGO_VRAM_LEN 24

#define MAP_FG_PALNUM 0
#define BG_PALNUM 1
#define OBJECTS_PALNUM 2
#define ENEMY_PALNUM OBJECTS_PALNUM
#define PLAYER_PALNUM 3
#define CUBE_PALNUM PLAYER_PALNUM
#define CUBE_ALT_PALNUM BG_PALNUM
#define HUD_PALNUM BG_PALNUM
#define PAUSE_PALNUM PLAYER_PALNUM
#define PARTICLES_PALNUM PLAYER_PALNUM
#define PARTICLES_ALT_PALNUM BG_PALNUM
#define POWERUP_CP_PALNUM PLAYER_PALNUM
#define POWERUP_HP_PALNUM OBJECTS_PALNUM

#endif
