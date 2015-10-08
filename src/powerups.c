#include "powerups.h"
#include "player.h"
#include "cubes.h"
#include "gfx.h"
#include "vramslots.h"
#include "map.h"
#include "particles.h"
#include "state.h"
#include "system.h"
#include "sprites.h"

#define POWERUP_IS_ORB(t) (t == POWERUP_CPORB || t == POWERUP_HPORB)
#define POWERUP_W(t) (POWERUP_IS_ORB(t) ? POWERUP_L_WIDTH : POWERUP_S_WIDTH)
#define POWERUP_H(t) (POWERUP_IS_ORB(t) ? POWERUP_L_HEIGHT : POWERUP_S_HEIGHT)

#ifndef FZERO
#define FZERO FIX16(0.0)
#endif

// Array of all active powerups
static powerup powerups[NUM_POWERUPS];

static void powerup_destroy(powerup *p);
static inline void powerup_bg_collisions(powerup *p);
static inline void powerup_player_collisions(powerup *p);
static inline void powerup_cube_collisions(powerup *p);
static void powerup_move(powerup *p);

void powerup_init(void)
{
	u16 i = NUM_POWERUPS;
	while (i--)
	{
		powerup *p = &powerups[i];
		p->type = POWERUP_NULL;
		p->x = -32;
		p->y = -32;
		p->dy = POWERUP_CREATION_DY; 
		p->num = 0;
	}
}

void powerup_dma_tiles(void)
{
	VDP_doVRamDMA((u32)gfx_powerups, POWERUP_VRAM_SLOT * 32, POWERUP_VRAM_LEN * 16);
}

void powerup_spawn(s16 x, s16 y, u16 type, u16 num)
{
	u16 i = NUM_POWERUPS;
	while (i--)
	{
		powerup *p = &powerups[i];
		if (p->type == POWERUP_NULL)
		{
			p->type = type;
			p->x = x;
			p->y = y;
			p->dy = POWERUP_CREATION_DY; 
			p->num = num;
			return;
		}
	}

	// Couldn't find one; go through and find the first non-orb one
	i = NUM_POWERUPS;
	while (i--)
	{
		powerup *p = &powerups[i];
		if (p->type != POWERUP_CPORB && p->type != POWERUP_HPORB)
		{
			p->type = type;
			p->x = x;
			p->y = y;
			p->dy = POWERUP_CREATION_DY; 
			p->num = num;
			return;
		}
	}
}

void powerup_run(void)
{
	u16 i = NUM_POWERUPS;
	while (i--)
	{
		powerup *p = &powerups[i];
		if (p->type == POWERUP_NULL)
		{
			continue;
		}
		powerup_move(p);
	}
}

void powerup_draw(void)
{
	u16 i = NUM_POWERUPS;
	while (i--)
	{
		u16 sx;
		u16 sy;
		u16 attr;
		powerup *p = &powerups[i];
		if (p->type == POWERUP_NULL)
		{
			continue;
		}
		sx = p->x - state.cam_x - (POWERUP_W(p->type));
		sy = p->y - state.cam_y - (POWERUP_H(p->type));
		// Check if sprite is out of bounds
		if (!(sx > -16 && sx < 320 && sy > -16 && sy < 240))
		{
			continue;
		}
		// Base video address
		attr = TILE_ATTR_FULL(1, 0, 0, 0, POWERUP_VRAM_SLOT);
		switch (p->type)
		{
			case POWERUP_CPORB:
				// CP Orb is 8 tiles later than the HP orb
				attr += 8;
				attr += TILE_ATTR(2, 0, 0, 0);
			case POWERUP_HPORB:
				// Make the orb flash
				attr += ((system_osc >> 1) % 4 >= 2) ? 4 : 0;
				break;
			case POWERUP_CP:
			case POWERUP_CP_2X:
				attr += TILE_ATTR(2, 0, 0, 0);
			case POWERUP_HP:
			case POWERUP_HP_2X:
				attr += 16;
				attr += (p->type - POWERUP_HP) << 1;
				attr += ((system_osc >> 1) % 4 >= 2) ? 1 : 0;
				break;
		}

		sprite_put(sx, sy, POWERUP_IS_ORB(p->type) ? SPRITE_SIZE(2,2) : SPRITE_SIZE(1,1), attr);
	}
}

// -------- Private static functions -------
static void powerup_destroy(powerup *p)
{
	p->type = POWERUP_NULL;
}

static inline void powerup_bg_collisions(powerup *p)
{
	s16 p_top = p->y - POWERUP_H(p->type);


	// Bottom is colliding with the backdrop
	if (map_collision(p->x, p->y) && p->dy >= FZERO)
	{
		// Halve dy
		p->dy = (p->dy >> 1) + (p->dy >> 3);
		p->dy = p->dy * -1;
		if (p->dy > FIX16(-1.2))
		{
			p->dy = FIX16(-1.2);
		}
	}

	// Top of powerup is hitting BG
	if (map_collision(p->x, p_top))
	{
		// Check that area just above is also solid
		if (map_collision(p->x, p_top - 8))
		{
			// Bounce the powerup downwards
			p->dy = POWERUP_CEILING_DY;
		}
	}
}

static inline void powerup_player_collisions(powerup *p)
{

}

static inline void powerup_cube_collisions(powerup *p)
{

}

static void powerup_move(powerup *p)
{
	p->dy = fix16Add(p->dy, POWERUP_GRAVITY);
	p->y = p->y + fix16ToInt(p->dy);
	powerup_bg_collisions(p);
	powerup_player_collisions(p);
	powerup_cube_collisions(p);
}
