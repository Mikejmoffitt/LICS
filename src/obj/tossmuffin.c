#include "obj/tossmuffin.h"
#include "gfx.h"
#include "vramslots.h"
#include "system/system.h"
#include "map.h"
#include "player.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void vram_load(void);

static u16 klift_len;
static u16 kanim_speed;

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(TOSSMUFFIN_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_tossmuffin, vram_pos * 32, TOSSMUFFIN_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_tossmuffin(en_tossmuffin *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = NULL;

	e->head.hp = 2;
	e->head.x += 8;
	e->head.y += 24;
	e->head.width = 7;
	e->head.height = 18;

	e->head.direction = ENEMY_RIGHT;

	// Tossmuffin's head sprite
	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -24;
	e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos);

	// Little body sprite
	e->head.size[1] = SPRITE_SIZE(2,1);
	e->head.xoff[1] = -8;
	e->head.yoff[1] = -8;
	e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 8);

	e->holding_cube = NULL;
	e->anim_frame = 0;
	e->anim_cnt = 0;
	e->lift_cnt = 0;
	e->toss_cnt = 0;
	e->walk_delay = 0;
	e->saw_player = 0;

	klift_len = system_ntsc ? 18 : 15;
	kanim_speed = system_ntsc ? 12 : 10;
}

// Reset the VRAM allocation position counter
void en_unload_tossmuffin(void)
{
	vram_pos = 0;
}

static inline void toss_cube(en_tossmuffin *e)
{
	// Toss the cube; remove handle to it. It's free now~
	e->holding_cube->dx = (e->head.direction == ENEMY_RIGHT) ? 3 : -3;
	e->holding_cube->dy = system_ntsc ? FIX16(-3.333) : FIX16(-4.0);
	e->holding_cube->state = CUBE_STATE_AIR;
	e->holding_cube->direction = e->head.direction;
	e->holding_cube = NULL;
}

static inline void scan_cubes(en_tossmuffin *e)
{
	u16 i = CUBES_NUM;
	while (i--)
	{
		cube *c = &cubes[i];
		if (c->state == CUBE_STATE_IDLE)
		{
			// Touched a cube; nab it
			if (e->head.x - e->head.width <= c->x + CUBE_RIGHT &&
				e->head.x + e->head.width >= c->x + CUBE_LEFT &&
				e->head.y - e->head.height <= c->y + CUBE_BOTTOM &&
				e->head.y >= c->y + CUBE_TOP)
			{
				e->holding_cube = c;
				e->lift_cnt = klift_len;
				e->toss_cnt = klift_len;

				// Turn towards the player with the cube lifted
				e->head.direction = (e->head.x < pl.px) ? ENEMY_RIGHT : ENEMY_LEFT;
				break;
			}
		}
	}
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_tossmuffin *e = (en_tossmuffin *)v;

	if (!e->saw_player)
	{
		e->head.direction = (e->head.x < pl.px) ? ENEMY_RIGHT : ENEMY_LEFT;
		e->saw_player = 1;
	}

	if (e->holding_cube && e->lift_cnt > 0)
	{
		// Hold the cube in position above the head.
		e->holding_cube->y = e->head.y -22;
		e->holding_cube->x = e->head.x;

		e->lift_cnt--;
		if (e->lift_cnt == 0)
		{
			toss_cube(e);
		}
	}
	else if (e->toss_cnt > 0)
	{
		e->toss_cnt--;
	}
	// On the prowl for cubes
	else if (!e->holding_cube)
	{
		e->toss_cnt = 0;
		e->holding_cube = 0;
		if (e->walk_delay > 0)
		{
			e->walk_delay--;
		}
		else
		{
			e->head.x += (e->head.direction == ENEMY_RIGHT) ? 1 : -1;
			// Simple BG collision for the sides
			if (e->head.direction == ENEMY_RIGHT &&
			    map_collision(e->head.x + e->head.width, e->head.y - 12))
			{
				e->head.direction = ENEMY_LEFT;
			}
			else if (e->head.direction == ENEMY_LEFT &&
			   map_collision(e->head.x - e->head.width, e->head.y - 12))
			{
				e->head.direction = ENEMY_RIGHT;
			}
			e->walk_delay = 2;
		}
		scan_cubes(e);	// Normal walking about
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_tossmuffin *e = (en_tossmuffin *)v;

	// Animation sequence
	if (e->anim_cnt == kanim_speed)
	{
		if (e->anim_frame == 3)
		{
			e->anim_frame = 0;
		}
		else
		{
			e->anim_frame++;
		}
		e->anim_cnt = 0;
	}
	else
	{
		e->anim_cnt++;
	}

	// Frame calculation
	if (e->lift_cnt > 0)
	{
		e->head.yoff[0] = -23;
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 4);

		e->head.size[1] = SPRITE_SIZE(2,1);
		e->head.yoff[1] = -8;
		e->head.xoff[1] = -8;
		// Masking of system_osc makes his legs wiggle every 2 frames
		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction,
			vram_pos + 14 + ((system_osc >> 1) % 2 ? 2 : 0));
	}
	else if (e->toss_cnt > 0)
	{
		e->head.yoff[0] = -24;
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos);
		e->head.size[1] = SPRITE_SIZE(3,1);
		e->head.xoff[1] = (e->head.direction == ENEMY_RIGHT ? -8 : -16);
		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 18);
		e->head.yoff[1] = -8;
	}
	else
	{
		e->head.yoff[0] = (e->anim_frame % 2) ? -24: -25;
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos);
		e->head.xoff[0] = -8;
		e->head.size[1] = SPRITE_SIZE(2,1);
		e->head.xoff[1] = -8;

		if (e->anim_frame == 0)
		{
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 10);
			e->head.yoff[1] = -9;
		}
		else if (e->anim_frame == 2)
		{
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 12);
			e->head.yoff[1] = -9;
		}
		else
		{
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 8);
			e->head.yoff[1] = -8;
		}
	}
}
