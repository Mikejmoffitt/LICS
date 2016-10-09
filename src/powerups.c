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
#include "save.h"
#include "music.h"
#include "messagestrings.h"
#include "messagescreen.h"

static u16 kmax_dy;
static u16 kbounce_sub;
static u16 kgravity;
static u16 kceiling_dy;
static u16 kcreation_dy;

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
static inline void powerup_orb_collect(powerup *p);
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
		p->dy = kcreation_dy;
		p->num = 0;
	}

	kmax_dy = system_ntsc ? FIX16(5.0) : FIX16(6.0);
	kbounce_sub = system_ntsc ? FIX16(-1.667) : FIX16(-2.0);
	kgravity = system_ntsc ? FIX16(0.1667) : FIX16(0.2);
	kceiling_dy = system_ntsc ? FIX16(0.833) : FIX16(1.0);
	kcreation_dy = system_ntsc ? FIX16(-2.5) : FIX16(-3.0);
}

void powerup_dma_tiles(void)
{
	VDP_doVRamDMA((u32)gfx_powerups, POWERUP_VRAM_SLOT * 32, POWERUP_VRAM_LEN * 16);
}

void powerup_spawn(s16 x, s16 y, u16 type, u16 num)
{
	if (!sram.have_phantom)
	{
		if (type == POWERUP_CP || type == POWERUP_CP_2X)
		{
			return;
		}
	}
	u16 i = NUM_POWERUPS;
	while (i--)
	{
		powerup *p = &powerups[i];
		if (p->type == POWERUP_NULL)
		{
			p->type = type;
			p->x = x;
			p->y = y;
			p->dy = kcreation_dy;
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
			p->dy = kcreation_dy;
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
		switch (p->type)
		{
			default:
			case POWERUP_CPORB:
				attr = TILE_ATTR_FULL(3, 0, 0, 0, POWERUP_VRAM_SLOT + 8);
				break;
			case POWERUP_HPORB:
				attr = TILE_ATTR_FULL(1, 0, 0, 0, POWERUP_VRAM_SLOT);
				break;

			case POWERUP_CP:
				attr = TILE_ATTR_FULL(2, 0, 0, 0, POWERUP_VRAM_SLOT + 20);
				break;
			case POWERUP_CP_2X:
				attr = TILE_ATTR_FULL(2, 0, 0, 0, POWERUP_VRAM_SLOT + 22);
				break;
			case POWERUP_HP:
				attr = TILE_ATTR_FULL(1, 0, 0, 0, POWERUP_VRAM_SLOT + 16);
				break;
			case POWERUP_HP_2X:
				attr = TILE_ATTR_FULL(1, 0, 0, 0, POWERUP_VRAM_SLOT + 18);
				break;
		}

		if ((system_osc & 0x07) >= 4)
		{
			attr += (POWERUP_IS_ORB(p->type) ? 4 : 1);
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

	// Top collision with BG
	if (map_collision(p->x, p_top))
	{
		// Bounce the powerup downwards
		p->dy = kceiling_dy;
		p->y += 4;
	}
	// Bottom is colliding with the backdrop
	else if (map_collision(p->x, p->y))
	{
		if (p->y > FZERO)
		{
			// Halve dy
			p->dy = (p->dy >> 1) + (p->dy >> 3);
			p->dy = p->dy * -1;
			if (p->dy > FIX16(-1.2))
			{
				p->dy = FIX16(-1.2);
			}
		}
	}
}

static inline void powerup_orb_collect(powerup *p)
{
	if (p->type == POWERUP_CPORB)
	{
		// If we somehow already have this orb, don't increment the have count...
		if (!sram.cp_orbs_taken[p->num])
		{
			sram.cp_orbs_have++;
		}
		// Mark orb as taken
		sram.cp_orbs_taken[p->num] = 1;
		message_screen("got CP orb");
	}
	else if (p->type == POWERUP_HPORB)
	{
		// If we somehow already have this orb, don't increment the max health...
		if (!sram.hp_orbs_taken[p->num])
		{
			sram.max_hp++;
		}
		// Mark orb as taken
		sram.hp_orbs_taken[p->num] = 1;
		// Max out player's HP as well
		pl.hp = sram.max_hp;

		// Show message
		message_screen(string_item_health_increase);
	}
	// Write to SRAM / EEPROM the player's progress
	save_write();
}

static inline void powerup_player_collisions(powerup *p)
{
	if (pl.px + PLAYER_CHK_RIGHT > p->x - POWERUP_W(p->type) &&
	    pl.px + PLAYER_CHK_LEFT < p->x + POWERUP_W(p->type) &&
	    pl.py + PLAYER_CHK_TOP < p->y &&
	    pl.py + PLAYER_CHK_BOTTOM > p->y - POWERUP_H(p->type))
	{
		if (POWERUP_IS_ORB(p->type))
		{
			powerup_orb_collect(p);
		}
		else
		{
			switch (p->type)
			{
				case POWERUP_HP:
					pl.hp += 1;
					break;
				case POWERUP_HP_2X:
					pl.hp += 2;
					break;
				case POWERUP_CP:
					pl.cp += 4;
					break;
				case POWERUP_CP_2X:
					pl.cp += 8;
					break;
			}
			if (pl.hp > sram.max_hp)
			{
				pl.hp = sram.max_hp;
			}
			if (pl.cp > PLAYER_MAX_CP)
			{
				pl.cp = PLAYER_MAX_CP;
			}

			particle_spawn(p->x, p->y, PARTICLE_TYPE_SPARKLE);
			particle_spawn(p->x, p->y, PARTICLE_TYPE_SPARKLE);
			particle_spawn(p->x, p->y, PARTICLE_TYPE_SPARKLE);

		}
		playsound(SFX_POWERUP_GET);
		powerup_destroy(p);
	}
}

static inline void powerup_cube_collisions(powerup *p)
{
	u16 i = CUBES_NUM;
	while (i--)
	{
		cube *c = &cubes[i];
		if (c->state == CUBE_STATE_INACTIVE)
		{
			continue;
		}
		if (p->x > c->x + CUBE_LEFT &&
		    p->x < c->x + CUBE_RIGHT &&
		    p->y > c->y + CUBE_TOP &&
		    p->y < c->y + CUBE_BOTTOM)
		{
			if (p->dy > FZERO)
			{
				// Halve dy
				p->dy = (p->dy >> 1) + (p->dy >> 2);
				p->dy = p->dy * -1;
				if (p->dy > FIX16(-1.6))
				{
					p->dy = FIX16(-1.6);
				}
			}
			else
			{
				p->dy = FIX16(-3.0);
			}
		}
	}
}

static void powerup_move(powerup *p)
{
	p->dy = fix16Add(p->dy, kgravity);
	p->y = p->y + fix16ToInt(p->dy);
	powerup_bg_collisions(p);
	powerup_player_collisions(p);
	powerup_cube_collisions(p);
}
