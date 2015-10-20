#ifndef ENEMY_H
#define ENEMY_H

#include <genesis.h>
#include "vramslots.h"
#include "enemy_types.h"
#include "cubes.h"
#include "gfx.h"
#include "pal.h"

// Size of array of enemy objects
#define ENEMIES_NUM 28

// How much generic data is allocated per enemy
#define ENEMY_DATA_SIZE 8

// Enemy / Object status
#define ENEMY_DISABLED 0
#define ENEMY_ONSCREEN 1
#define ENEMY_OFFSCREEN 2

// Direction constants
#define ENEMY_RIGHT 0
#define ENEMY_LEFT 1

// How long an enemy is frozen when struck with a cube
#define ENEMY_HURT_TIME 20

// Distance away from edge of screen to remain active
#define ENEMY_ACTIVE_DISTANCE 32

#ifndef FZERO
#define FZERO FIX16(0.0)
#endif

#define ENEMY_HARM_NONE 0
#define ENEMY_HARM_NORMAL 1
#define ENEMY_HARM_ALWAYS_BOUNCE_L 2
#define ENEMY_HARM_ALWAYS_BOUNCE_R 3
#define ENEMY_HARM_KILL 4

typedef struct en_header en_header;
struct en_header
{
	// Pointers to basic enemy functions
	void (*anim_func)(void *); 
	void (*proc_func)(void *);
	void (*cube_func)(void *, cube *);
	// Universal information all enemy objects share
	u16 active; // should it be drawn this frame?
	u16 type; // ID specifying which subclass of enemy it is
	u16 harmful; // How the player interacts
	u16 touching_player; // Signal for if enemy is overlapping the player
	u16 powerup_range; // The max type of powerup to get
	u16 direction; // Right/Left based on directions given above
	// Real-world position in pixels; center-bottom of enemy
	s16 x;
	s16 y;
	// Offset from origin for rendering ("hot spot")
	s16 xoff[2];
	s16 yoff[2];
	// Hitbox info
	s16 width; // Actually half-width for hitboxes
	s16 height;
	// Sprite engine register information
	// For use when drawing; should be pre-calculated during active area
	u16 attr[2];
	u16 size[2];
	// Health information
	u16 hurt_cnt; // When it reaches zero, hp--;
	u16 hp; // If hp == 0, enemy is destroyed
};

typedef struct en_generic en_generic;
struct en_generic
{
	en_header head;
	// Generic data space; more specific casted types will use these
	u16 data[ENEMY_DATA_SIZE];
};

// Graphics lists

static const u32 enemy_vram_src[] = 
{
	(u32)gfx_items, 
	(u32)gfx_en_metagrub,
	(u32)gfx_en_flip,
	(u32)gfx_en_boingo,
	(u32)gfx_en_gaxter,
	(u32)gfx_en_buggo,
	(u32)gfx_en_dancyflower,
	(u32)gfx_en_jraff,
	0
};

static const u16 enemy_vram_dest[] = 
{
	ITEMS_VRAM_SLOT * 32,
	METAGRUB_VRAM_SLOT * 32,
	FLIP_VRAM_SLOT * 32,
	BOINGO_VRAM_SLOT * 32,
	GAXTER_VRAM_SLOT * 32,
	BUGGO_VRAM_SLOT * 32,
	DANCYFLOWER_VRAM_SLOT * 32,
	JRAFF_VRAM_SLOT * 32,
	0
};

static const u16 enemy_vram_len[] =
{
	ITEMS_VRAM_LEN * 16,
	METAGRUB_VRAM_LEN * 16,
	FLIP_VRAM_LEN * 16,
	BOINGO_VRAM_LEN * 16,
	GAXTER_VRAM_LEN * 16,
	BUGGO_VRAM_LEN * 16,
	DANCYFLOWER_VRAM_LEN * 16,
	JRAFF_VRAM_LEN * 16,
	0
};

// Global list of enemies
extern en_generic enemies[ENEMIES_NUM];

// Copy enemy tile and palette data into VRAM/CRAM
void enemy_dma_tiles(void);

// Clear out enemy structures
void enemy_init(void);

// Runs logic for enemies (1 frame's worth)
void enemy_run(void);

// Renders all enemies as needed
void enemy_draw(void);

// The standard response to getting hit with a cube
void enemy_get_hurt(en_generic *e);

// Enemy e collides with cube c
void enemy_cube_response(en_generic *e, cube *c);
void enemy_cube_impact(en_generic *e, cube *c);

// Puts an enemy on the map. Returns a handle to the placed enemy if valid,
// and returns NULL if invalid or out of room.
en_generic *enemy_place(u16 x, u16 y, u16 type);

#endif
