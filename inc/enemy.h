#ifndef ENEMY_H
#define ENEMY_H

#include <genesis.h>
#include "player.h"
#include "vramslots.h"
#include "enemy_types.h"
#include "gfx.h"
#include "pal.h"

#define ENEMIES_NUM 24
#define ENEMY_DATA_SIZE 12

#define ENEMY_RIGHT 0
#define ENEMY_LEFT 1

#ifndef FZERO
#define FZERO FIX16(0.0)
#endif

typedef struct en_header en_header;
struct en_header
{
	// Universal information all enemy objects share
	u16 active;
	u16 type;
	u16 direction;
	// Real-world position
	s16 x;
	s16 y;
	s16 xoff;
	s16 yoff;
	// For use when drawing; should be pre-calculated during active area
	u16 attr;
	u16 size;
	// Health information
	u16 hurt_cnt; // When it reaches zero, hp--;
	u16 hp; // If hp == 0, enemy is destroyed
};

typedef struct en_flip en_flip;
struct en_flip
{
	en_header head;
	u16 move_timer; // Always incrementing; used for sinFix16 lookup
	u16 flip_cnt; // When == 1, direction switches
};

typedef struct en_boingo en_boingo;
struct en_boingo
{
	en_header head;
	u16 move_cnt; // When == 1, boingo jumps at random-ish height in direction
	fix16 dy;
};


typedef struct en_generic en_generic;
struct en_generic
{
	en_header head;
	// Generic data space; more specific casted types will use these
	u16 data[ENEMY_DATA_SIZE];
};


static const u16 enemy_palnums[] = 
{
	0,
	PLAYER_PALNUM,   // 01 Metagrub
	OBJECTS_PALNUM,  // 02 Flip
	PLAYER_PALNUM,   // 03 Boingo
	0
};

static const u32 enemy_vram_src[] = 
{
	(u32)gfx_en_metagrub,
	(u32)gfx_en_flip,
	(u32)gfx_en_boingo,
	0
};

static const u16 enemy_vram_dest[] = 
{
	METAGRUB_VRAM_SLOT * 32,
	FLIP_VRAM_SLOT * 32,
	BOINGO_VRAM_SLOT * 32,
	0
};

static const u16 enemy_vram_len[] =
{
	METAGRUB_VRAM_LEN * 16,
	FLIP_VRAM_LEN * 16,
	BOINGO_VRAM_LEN * 16,
	0
};

// Copy enemy tile and palette data into VRAM/CRAM
void enemy_dma_tiles(void);

// Clear out enemy structures
void enemy_init(void);

// Runs logic for enemies (1 frame's worth)
void enemy_run(player *pl);

// Renders all enemies as needed
void enemy_draw(void);

// Puts an enemy on the map. Returns a handle to the placed enemy if valid,
// and returns NULL if invalid or out of room.
en_generic *enemy_place(u16 x, u16 y, u16 type);

#endif
