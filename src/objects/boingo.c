#include "boingo.h"
#include "vramslots.h"
#include "player.h"
#include "map.h"
#include "system.h"
#include "music.h"
#include "particles.h"

static void cube_func(void *v, cube *c);
static void en_proc_boingo(void *v);
static void en_anim_boingo(void *v);
static void bg_collisions(en_boingo *e);
static void do_jump(en_boingo *e);

// Local type-wide constants
static fix16 jump_str;
static fix16 gravity;
static u16 jump_time;
static u16 anim_speed;

// Dynamic VRAM slot support
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(BOINGO_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_boingo, vram_pos * 32, BOINGO_VRAM_LEN * 16);
	}
}

void en_unload_boingo(void)
{
	vram_pos = 0;
}

void en_init_boingo(en_boingo *e, u16 type)
{
	vram_load();
	e->head.width = 9;
	e->head.height = 15;
	e->head.direction = ENEMY_RIGHT;
	e->head.size[0] = SPRITE_SIZE(3,2);
	e->head.attr[1] = 0;
	e->head.xoff[0] = -12;
	e->head.x += 12;
	e->head.y += 15;
	e->head.harmful = ENEMY_HARM_NORMAL;

	e->dy = FIX16(0.0);
	e->state = BOINGO_STANDING;
	e->jump_cnt = 0;
	e->anim_cnt = 0;
	e->type = type;
	e->head.hp = type == BOINGO_TYPE_CUBE ? 2 : 1;

	e->head.proc_func = &en_proc_boingo;
	e->head.anim_func = &en_anim_boingo;
	e->head.cube_func = cube_func;

	// Establish constants for NTSC/PAL
	jump_str = (system_ntsc) ? FIX16(-0.83) : FIX16(-0.9959);
	gravity = system_ntsc ? FIX16(0.167) : FIX16(0.24048);
	jump_time = (system_ntsc) ? 24 : 20;
	anim_speed = system_ntsc ? 11 : 9;
}

static void en_anim_boingo(void *v)
{
	en_boingo *e = (en_boingo *)v;
	e->anim_cnt++;

	// Run animation
	if (e->anim_cnt >= ((e->state == BOINGO_STANDING) ? anim_speed : BOINGO_ANIM_SPEED_JUMP))
	{

		e->anim_cnt = 0;
		e->anim_frame++;
		if (e->anim_frame >= 2)
		{
			e->anim_frame = 0;
		}
	}

	if (e->state == BOINGO_STANDING)
	{
		if (e->type != BOINGO_TYPE_CUBE)
		{
			e->head.width = BOINGO_GND_W;
			e->head.height = BOINGO_GND_H;
			e->head.xoff[0] = -12;
			e->head.yoff[0] = -14;
			e->head.size[0] = SPRITE_SIZE(3,2);
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos);
			e->head.attr[1] = NULL;
			if (e->anim_frame)
			{
				e->head.attr[0] += 6;
			}
			if (e->type == BOINGO_TYPE_ANGRY)
			{
				e->head.attr[0] += 32;
			}
		}
		else
		{
			e->head.width = BOINGO_CUBE_GND_W;
			e->head.height = BOINGO_GND_H;

			// The cube
			e->head.xoff[0] = -8;
			e->head.yoff[0] = -19 + e->anim_frame;
			e->head.size[0] = SPRITE_SIZE(2,2);
			e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, 0, vram_pos + 56);

			// The legs
			e->head.xoff[1] = -8;
			e->head.yoff[1] = -3;
			e->head.size[1] = SPRITE_SIZE(2,1);
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 24 + (e->anim_frame ? 2 : 0));
		}
	}
	else
	{
		if (e->type != BOINGO_TYPE_CUBE)
		{
			e->head.width = BOINGO_AIR_W;
			e->head.height = BOINGO_AIR_H;
			e->head.xoff[0] = -8;
			e->head.yoff[0] = -19;
			e->head.size[0] = SPRITE_SIZE(2,3);

			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 12);
			e->head.attr[1] = NULL;

			if (e->anim_frame)
			{
				e->head.attr[0] += 6;
			}
			if (e->type == BOINGO_TYPE_ANGRY)
			{
				e->head.attr[0] += 32;
			}
		}
		else
		{
			e->head.width = BOINGO_AIR_W;
			e->head.height = BOINGO_AIR_H;

			// The cube
			e->head.xoff[0] = -8;
			e->head.yoff[0] = -19;
			e->head.size[0] = SPRITE_SIZE(2,2);
			e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, 0, vram_pos + 56);

			// The legs
			e->head.xoff[1] = -8;
			e->head.yoff[1] = -3;
			e->head.size[1] = SPRITE_SIZE(2,1);
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 28 + (e->anim_frame ? 2 : 0));
		}
	}

}

static void bg_collisions(en_boingo *e)
{
	// Collision with ceiling
	if (e->dy < FIX16(0.0) && map_collision(e->head.x, e->head.y - BOINGO_AIR_H))
	{
		e->dy = FIX16(2.0);
		e->head.y += 5;
	}
	// With ground
	else if (e->dy > FIX16(0.0) && map_collision(e->head.x, e->head.y))
	{
		e->dy = FIX16(0.0);
		// Lock to 8x8 grid
		e->head.y = (e->head.y) & 0xFFF8;
		e->state = BOINGO_STANDING;
		return;
	}

	// Left side wall
	if (e->head.direction == ENEMY_LEFT &&
	    map_collision(e->head.x - BOINGO_AIR_W, e->head.y - (BOINGO_AIR_H / 2)))
	{
		e->head.direction = ENEMY_RIGHT;
		e->head.x += 4;
	}
	else if (e->head.direction == ENEMY_RIGHT &&
	    map_collision(e->head.x + BOINGO_AIR_W, e->head.y - (BOINGO_AIR_H / 2)))
	{
		e->head.direction = ENEMY_LEFT;
		e->head.x -= 4;
	}
}

static const fix16 str_table_pal[] =
{
	FIX16(0.00),
	FIX16(-0.6),
	FIX16(-1.2),
	FIX16(-1.8),
	FIX16(-2.4),
	FIX16(-3.0),
	FIX16(-3.6),
	FIX16(-4.2)
};

static const fix16 str_table_ntsc[] =
{
	FIX16(0.00),
	FIX16(-0.5),
	FIX16(-1.0),
	FIX16(-1.5),
	FIX16(-2.0),
	FIX16(-2.5),
	FIX16(-3.0),
	FIX16(-3.5)
};

static const fix16 str_table_angry_pal[] =
{
	FIX16(0.00),
	FIX16(-0.6),
	FIX16(-1.2),
	FIX16(-1.8),
	FIX16(-2.4),
	FIX16(-3.0),
	FIX16(-3.6),
	FIX16(-4.2)
};

static const fix16 str_table_angry_ntsc[] =
{
	FIX16(0.00),
	FIX16(-0.5),
	FIX16(-1.0),
	FIX16(-1.5),
	FIX16(-2.0),
	FIX16(-2.5),
	FIX16(-3.0),
	FIX16(-3.5)
};

static void do_jump(en_boingo *e)
{
	if (pl.px < e->head.x)
	{
		e->head.direction = ENEMY_LEFT;
	}
	else
	{
		e->head.direction = ENEMY_RIGHT;
	}
	// Base jump strength
	e->dy = jump_str;

	// Additional jump strength from random generator
	if (e->type != BOINGO_TYPE_ANGRY)
	{
		if (system_ntsc)
		{
			e->dy += str_table_ntsc[GET_HVCOUNTER & 0x07];
		}
		else
		{
			e->dy += str_table_pal[GET_HVCOUNTER & 0x07];
		}
	}
	e->state = BOINGO_JUMPING;
	e->jump_cnt = 0;
}

static void en_proc_boingo(void *v)
{
	en_boingo *e = (en_boingo *)v;

	if (e->type == BOINGO_TYPE_TO_NORMAL)
	{
		particle_spawn(e->head.x, e->head.y - 5, PARTICLE_TYPE_FIZZLE);
		particle_spawn(e->head.x + 3, e->head.y, PARTICLE_TYPE_FIZZLE);
		particle_spawn(e->head.x - 1, e->head.y + 3, PARTICLE_TYPE_FIZZLE);
		particle_spawn(e->head.x - 3, e->head.y - 2, PARTICLE_TYPE_FIZZLE);
		playsound(SFX_ENEMY_EXPLODE);
		e->type = BOINGO_TYPE_NORMAL;
	}
	// Standing state
	if (e->state == BOINGO_STANDING)
	{
		if (e->jump_cnt >= jump_time)
		{
			do_jump(e);
		}
		else
		{
			e->jump_cnt++;
		}
	}
	else if (e->state == BOINGO_JUMPING)
	{
		e->head.y += fix16ToInt(e->dy);
		e->dy = fix16Add(e->dy, gravity);
		e->head.x += (e->head.direction == ENEMY_RIGHT) ? 1 : -1;
		bg_collisions(e);
	}
}

static void cube_func(void *v, cube *c)
{
	en_boingo *e = (en_boingo *)v;

	if (e->type == BOINGO_TYPE_CUBE)
	{
		e->type = BOINGO_TYPE_TO_NORMAL;
	}

	enemy_cube_response((en_generic *)e, c);
}
