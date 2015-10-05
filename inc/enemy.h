#ifndef ENEMY_H
#define ENEMY_H

#include <genesis.h>
#include "vramslots.h"
#include "pal.h"
#include "gfx.h"

#define ENEMY_NULL 0
#define ENEMY_METAGRUB 1
#define ENEMY_FLIP 2
#define ENEMY_BOINGO 3

typedef struct en_header en_header;
{
	// Universal information all enemy objects share
	u16 active;
	u16 type;
	// Real-world position
	s16 x;
	s16 y;
	// For use when drawing; should be pre-calculated during active area
	u16 attr;
	u16 size;
	// Health information
	u16 hurt_cnt; // When == 1, hp--;
	u16 hp; // If hp == 0, enemy is destroyed
};

typedef struct en_generic en_generic;
struct en_generic
{
	en_header head;
	// Generic data space; more specific casted types will use these
	u16 data[16];
};

typedef struct en_metagrub en_metagrub;
struct en_metagrub
{
	en_header head;
	u16 

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

void enemy_init(void);

#endif
