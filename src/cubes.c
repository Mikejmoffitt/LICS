#include "cubes.h"
#include "vramslots.h"
#include "gfx.h"
#include "sprites.h"
#include "state.h"
#include "system.h"
#include "player.h"
#include "map.h"
#include "particles.h"

static cube cubes[CUBES_NUM];

void cube_dma_tiles(void)
{
	VDP_doVRamDMA(gfx_cubes, CUBE_VRAM_SLOT * 32 ,CUBE_VRAM_LEN * 16);
	// Cubes use Lyle's palette. No palette transfer here.
}

void cubes_init(void)
{
	for (register i = 0; i < CUBES_NUM; i++)
	{
		cube *c = &cubes[i];
		c->state = CUBE_STATE_INACTIVE;
	}
}

static void cube_move(cube *c)
{
	if (c->state == CUBE_STATE_AIR)
	{	
		c->y += fix16ToInt(c->dy);
		c->dy = fix16Add(c->dy, CUBE_GRAVITY);
	}
	c->x += c->dx;

}

void cubes_run(player *pl)
{
	for (register i = 0; i < CUBES_NUM; i++)
	{
		cube *c = &cubes[i];
		if (c->state == CUBE_STATE_INACTIVE)
		{
			continue;
		}
		else if (c->state >= CUBE_STATE_FIZZLE)
		{
			if (c->dy > 0)
			{
				c->dy--;
			}
			else
			{
				c->state = CUBE_STATE_INACTIVE;	
			}
			continue;
		}
		else if (c->state != CUBE_STATE_IDLE)
		{
			cube_move(c);
		}

		// Basic collision with Plane A
		if (c->state != CUBE_STATE_FIZZLE)
		{
			if (map_collision(c->x + CUBE_LEFT, c->y + CUBE_BOTTOM) || 
				map_collision(c->x + CUBE_RIGHT, c->y + CUBE_BOTTOM) || 
				map_collision(c->x + CUBE_LEFT, c->y + CUBE_TOP) || 
				map_collision(c->x + CUBE_RIGHT, c->y + CUBE_TOP)) 
			{
				if (c->type != CUBE_GREEN)
				{
					c->dx = 0;
					c->dy = 7;
					if (c->type == CUBE_RED)
					{
						c->state = CUBE_STATE_EXPLODE;
					}
					else
					{
						c->state = CUBE_STATE_FIZZLE;
					}
				}
			}
		}
	}
}

void cubes_draw(void)
{
	for (register i = 0; i < CUBES_NUM; i++)
	{
		cube *c = &cubes[i];
		if (c->state == CUBE_STATE_INACTIVE)
		{
			continue;
		}
		else if (c->state == CUBE_STATE_FIZZLE)
		{
			particle_spawn(c->x, c->y, PARTICLE_TYPE_FIZZLE);
			continue;
		}
		else if (c->state == CUBE_STATE_FIZZLERED)
		{
			particle_spawn(c->x, c->y, PARTICLE_TYPE_FIZZLERED);
			continue;
		}
		else if (c->state == CUBE_STATE_EXPLODE)
		{
			particle_spawn(c->x, c->y, PARTICLE_TYPE_EXPLOSION);
			continue;
		}
		s16 cx = c->x - state.cam_x + CUBE_LEFT;
		s16 cy = c->y - state.cam_y + CUBE_TOP;
		if (cx <= -16 || cx >= 320 || cy <= -16 || cy >= 240)
		{
			continue;
		}
		cube_draw_single(c->x + CUBE_LEFT, c->y + CUBE_TOP, c->type);
	}
}

void cube_draw_single(u16 x, u16 y, u16 type)
{
	u16 frame = TILE_ATTR_FULL(CUBE_PALNUM, 1, 0, 0, CUBE_VRAM_SLOT);
	switch (type & 0xF00)
	{
		case CUBE_PHANTOM:
			frame += 16 + (4 * ((system_osc / 4) % 4));
			break;
		case CUBE_YELLOW:
			frame += 4;
			break;
		case CUBE_RED:
			frame += 8;
			break;
		case CUBE_GREEN:
			frame += 12;
			break;
	}

	sprite_put(x - state.cam_x, y - state.cam_y, SPRITE_SIZE(2,2), frame);
}

void cube_spawn(u16 x, u16 y, u16 type, u16 state, s16 dx, fix16 dy)
{
	for (register i = 0; i < CUBES_NUM; i++)
	{
		cube *c = &cubes[i];
		if (c->state == CUBE_STATE_INACTIVE)
		{
			c->state = state;
			c->x = x;
			c->y = y;
			c->dx = dx;
			c->dy = dy;
			c->type = type;
			return;
		}
	}
}
