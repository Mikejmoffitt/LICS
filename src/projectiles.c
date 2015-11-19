#include "projectiles.h"
#include "state.h"
#include "sprites.h"
#include "system.h"
#include "vramslots.h"
#include "map.h"
#include "gfx.h"
#include "player.h"
#include "particles.h"

static projectile projectiles[PROJECTILES_NUM];

void projectiles_dma_tiles(void)
{
	VDP_doVRamDMA((u32)gfx_projectiles, 32 * PROJECTILES_VRAM_SLOT, 16 * PROJECTILES_VRAM_LEN);
}

void projectiles_init(void)
{
	u16 i = PROJECTILES_NUM;
	while(i--)
	{
		projectiles[i].active = 0;
	}
}

// Destroy the particle, or in the case of the ball, bounce upwards first
static inline void handle_collision(projectile *p)
{
	s32 px = fix32ToInt(p->x);
	s32 py = fix32ToInt(p->y);
	if (map_collision(px, py))
	{
		if (p->type == PROJECTILE_BALL &&
			p->dy > FIX16(0.0))
		{
			p->dy = p->dy * -1;
		}	
		else
		{
			p->active = 0;
		}
	}
}

// Hurt player and destroy the particle on collision
static inline void check_for_player(projectile *p)
{
	s32 px = fix32ToInt(p->x);
	s32 py = fix32ToInt(p->y);
	if (pl.invuln_cnt == 0 &&
	    player_collision(px - PROJECTILE_CHK, 
   	                     px + PROJECTILE_CHK, 
	                     py - PROJECTILE_CHK, 
	                     py + PROJECTILE_CHK))
	{
		player_get_hurt();
		p->active = 0;
	}
}

// Everything but the spark places a sparkle as it moves
static inline void put_particles(projectile *p)
{
	if (p->type != PROJECTILE_SPARK && (system_osc % 8 == 0))
	{
		particle_spawn(fix32ToInt(p->x), fix32ToInt(p->y), PARTICLE_TYPE_SPARKLE);
	}
}

// Move the particle for one tick
static inline void movement(projectile *p)
{
	s32 px;
	s32 py;
	if (p->type != PROJECTILE_SPIKE)
	{
		p->x = fix32Add(p->dx << 4, p->x);
	}
	if (p->type != PROJECTILE_SPARK)
	{
		p->y = fix32Add(p->dy << 4, p->y);
	}

	if (p->type == PROJECTILE_DEATHORB)
	{
		fix16 kcmp = system_ntsc ? FIX16(3.0) : FIX16(3.28);
		fix16 kaccel = system_ntsc ? FIX16(0.21) : FIX16(0.28);
		if (p->v_dir == PROJECTILE_UP)
		{
			p->dy -= kaccel;
			if (p->dy <= -kcmp)
			{
				p->v_dir = PROJECTILE_DOWN;
			}
		}
		else
		{
			p->dy += kaccel;
			if (p->dy >= kcmp)
			{
				p->v_dir = PROJECTILE_UP;
			}
		}
	}

	// Check for violation of room boundaries
	px = fix32ToInt(p->x);
	py = fix32ToInt(p->y);
	if (px < 0 || px > state.bound_x ||
		py < 0 || py > state.bound_y)
	{
		p->active = 0;
	}
}

static inline void handle_life(projectile *p)
{
	if (p->type == PROJECTILE_DEATHORB && p->active > 0)
	{
		if (p->active >= (system_ntsc ? 181 : 151))
		{
			p->active = 0;
		}
		else
		{
			p->active++;
		}
	}
}

void projectiles_run(void)
{
	u16 i = PROJECTILES_NUM;
	while(i--)
	{
		if (projectiles[i].active)
		{
			projectile *p = &projectiles[i];

			movement(p);
			put_particles(p);
			check_for_player(p);
			handle_collision(p);
			handle_life(p);
		}
	}
}

void projectiles_draw(void)
{
	u16 i = PROJECTILES_NUM;
	while(i--)
	{
		if (projectiles[i].active)
		{
			projectile *p = &projectiles[i];

			// Death orb should flash near the end of its life
			if (p->type == PROJECTILE_DEATHORB && 
			    p->active > (system_ntsc ? 109 : 91) &&
			    system_osc % 4 >= 2)
			{
				continue;
			}

			s16 tx = fix32ToInt(p->x) - state.cam_x - 4;
			s16 ty = fix32ToInt(p->y) - state.cam_y - 4;
			u16 size;

			// Projectiles too far off screen are deactivated
			if (tx < -64 || tx > 384 || ty < -64 || ty > 304)
			{
				p->active = 0;
				continue;
			}
			
			u16 attr;
			if (p->type == PROJECTILE_SPIKE)
			{
				attr = TILE_ATTR_FULL(HUD_PALNUM, 0, 0, 0, PROJECTILES_VRAM_SLOT + 2);
				size = SPRITE_SIZE(1,1);
			}
			else if (p->type == PROJECTILE_SPARK)
			{
				attr = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, 0, PROJECTILES_VRAM_SLOT + 3 + ((system_osc >> 2) % 2));
				size = SPRITE_SIZE(1,1);
				ty -= 3;
			}
			else if (p->type == PROJECTILE_DEATHORB)
			{
				u16 fr_off = ((system_osc >> 2) % 2);
				tx -= 4;
				ty -= 4;
				size = SPRITE_SIZE(2,2);
				attr = TILE_ATTR_FULL(fr_off ? PLAYER_PALNUM : ENEMY_PALNUM, 0, 0, 0, PROJECTILES_VRAM_SLOT + (fr_off ? 5 : 9));
			}
			else
			{
				attr = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, 0, PROJECTILES_VRAM_SLOT + ((system_osc >> 2) % 2));
				size = SPRITE_SIZE(1,1);
			}

			sprite_put(tx, ty, size, attr);
		}
	}
}

void projectile_shoot(s16 x, s16 y, fix16 dx, fix16 dy, u16 type)
{
	u16 i = PROJECTILES_NUM;
	while (i--)
	{
		if (!projectiles[i].active)
		{
			projectiles[i].x = FIX32(x);
			projectiles[i].y = FIX32(y);
			projectiles[i].dx = dx;
			projectiles[i].dy = dy;
			projectiles[i].type = type;
			projectiles[i].v_dir = PROJECTILE_UP;	
			projectiles[i].active = 1;
			return;
		}
	}
}

void projectile_shoot_at(s16 x, s16 y, s16 tx, s16 ty)
{
	// Get rise-over-run coefficient
// 	fix16 ratio = fix16Div(FIX16(ty - y), FIX16(tx - x));
	s16 dx, dy;
	dx = FIX16(tx - x) >> 4;
	dy = FIX16(ty - y) >> 4;

	// Reductions for distance, to pretend this isn't a bad function
	if (tx - x > 64 || x - tx > 64 || ty - y > 48 || y - ty > 48)
	{	
		dx = dx >> 1;
		dy = dy >> 1;
	}
	if (tx - x > 140 || x - tx > 140 || ty - y > 96 || y - ty > 96)
	{	
		dx = dx >> 1;
		dy = dy >> 1;
	}
	if (tx - x > 256 || x - tx > 256 || ty - y > 128 || y - ty > 128)
	{	
		dx = dx >> 1;
		dy = dy >> 1;
	}
	projectile_shoot(x, y, dx, dy, PROJECTILE_BALL2); 
}
