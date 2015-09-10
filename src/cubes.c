#include "cubes.h"
#include "vramslots.h"
#include "gfx.h"
#include "sprites.h"
#include "state.h"
#include "system.h"
#include "player.h"
#include "map.h"
#include "particles.h"

cube cubes[CUBES_NUM];

void cube_dma_tiles(void)
{
	VDP_doVRamDMA((u32)gfx_cubes, CUBE_VRAM_SLOT * 32,CUBE_VRAM_LEN * 16);
	// Cubes use Lyle's palette. No palette transfer here.
}

void cubes_init(void)
{
	int i = CUBES_NUM;
	while (i--)
	{
		cube *c = &cubes[i];
		c->state = CUBE_STATE_INACTIVE;
	}
}

static void cube_destroy(cube *c)
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

void cube_delete(cube *c)
{
	c->type == NULL;
	c->state = CUBE_STATE_INACTIVE;
}

static void cube_move(cube *c)
{
	// Y movement only applies to an airborn cube
	if (c->state == CUBE_STATE_AIR)
	{	
		c->y += fix16ToInt(c->dy);
		c->dy = fix16Add(c->dy, CUBE_GRAVITY);
	}
	c->x += c->dx;
	if (c->state == CUBE_STATE_KICKED)
	{
		// in free air, change to an air cube and fall straight down
		if (!map_collision(c->x + CUBE_LEFT, c->y + CUBE_BOTTOM + 1) &&
			!map_collision(c->x + CUBE_RIGHT, c->y + CUBE_BOTTOM + 1))
		{
			c->dx = FZERO;
			c->state = CUBE_STATE_AIR;
			c->dy = FZERO;
			// lock to grid
			c->x = (c->x / 8) * 8;
		}
	}
	// Check for cube out of bounds
	if (c->x + CUBE_LEFT > state.current_room->w * STATE_SC_W || 
		c->x + CUBE_RIGHT < 0 ||
		c->y + CUBE_TOP > state.current_room->h * STATE_SC_H || 
		c->y + CUBE_BOTTOM < 0)
	{
		c->state = CUBE_STATE_INACTIVE;
		c->x = -32;
		c->y = -32;
	}
}

static void cube_on_cube_collisions(cube *c)
{
	int i = CUBES_NUM;
	while (i--)
	{
		cube *d = &cubes[i];
		if (d->state == CUBE_STATE_INACTIVE || d->state == CUBE_STATE_FIZZLE || d == c)
		{
			continue;
		}
		if (c->x + CUBE_LEFT <= d->x + CUBE_RIGHT && 
			c->x + CUBE_RIGHT >= d->x + CUBE_LEFT && 
			c->y + CUBE_TOP <= d->y + CUBE_BOTTOM &&
			c->y + CUBE_BOTTOM >= d->y + CUBE_TOP)
		{
			if (c->type != CUBE_GREEN && c->state != CUBE_STATE_IDLE)
			{
				cube_destroy(c);
			}
			else if (c->dy != FZERO)
			{
				c->dy = FZERO;
			}
			if (d->type != CUBE_GREEN && d->state != CUBE_STATE_IDLE)
			{
				cube_destroy(d);
			}
			else if (d->dy != FZERO)
			{
				d->dy = FZERO;
			}
		}
	}
}

void cubes_run(player *pl)
{
	int i = CUBES_NUM;
	while (i--)
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
			// Collison is processed a frame late intentionally to mimic the
			// original game's behavior. 
			if (c->state != CUBE_STATE_FIZZLE)
			{
				if (map_collision(c->x + CUBE_LEFT, c->y + CUBE_BOTTOM) || 
					map_collision(c->x + CUBE_RIGHT, c->y + CUBE_BOTTOM) || 
					map_collision(c->x + CUBE_LEFT, c->y + CUBE_TOP) || 
					map_collision(c->x + CUBE_RIGHT, c->y + CUBE_TOP)) 
				{
					if (c->type != CUBE_GREEN)
					{
						cube_destroy(c);
					}
				}
			}
			cube_move(c);
			if (c->state == CUBE_STATE_AIR ||
				c->state == CUBE_STATE_KICKED)
			{
				cube_on_cube_collisions(c);
			}
		}
	}
}

void cubes_draw(void)
{
	int i = CUBES_NUM;
	while (i--)
	{
		cube *c = &cubes[i];
		if (c->state == CUBE_STATE_INACTIVE)
		{
			continue;
		}
		else if (c->state == CUBE_STATE_FIZZLE)
		{
			particle_spawn(c->x, c->y - 8, PARTICLE_TYPE_FIZZLE);
			continue;
		}
		else if (c->state == CUBE_STATE_FIZZLERED)
		{
			particle_spawn(c->x, c->y - 8, PARTICLE_TYPE_FIZZLERED);
			continue;
		}
		else if (c->state == CUBE_STATE_EXPLODE)
		{
			particle_spawn(c->x, c->y - 8, PARTICLE_TYPE_EXPLOSION);
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
	int i = CUBES_NUM;
	while (i--)
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
			break;
		}
	}
}
