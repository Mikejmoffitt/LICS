#include "cow.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "enemy.h"
#include "cubes.h"
#include "projectiles.h"
#include "player.h"

#define COW_EATING 0
#define COW_FINISHED 1
#define COW_WALKING 2
#define COW_JUMPING 3
#define COW_ANGRY 4
#define COW_PREPARING 5

#define COW_ANGER_NUM_HITS 10

static void proc_func(void *v);
static void anim_func(void *v);
static void cube_func(void *v, cube *c);
static void vram_load(void);

static u16 keat_anim_speed;
static u16 kwalk_anim_speed;
static u16 kjump_anim_speed;
static u16 kprep_time;
static u16 kshot_time;
static fix16 kjump_str;
static fix16 kgravity;

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(COW_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_cow, vram_pos * 32, COW_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_cow(en_cow *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_func;

	e->head.hp = 99;
	e->head.x += 20;
	e->head.y += 24;
	e->head.width = 14;
	e->head.height = 21;

	e->head.direction = ENEMY_RIGHT;

	e->head.harmful = ENEMY_HARM_NONE;

	// The caboose
	e->head.size[0] = SPRITE_SIZE(2,3);
	e->head.xoff[0] = -20;
	e->head.yoff[0] = -24;

	// The head
	e->head.size[1] = SPRITE_SIZE(3,3);
	e->head.xoff[1] = -4;
	e->head.yoff[1] = -24;

	e->state = COW_EATING;
	e->anim_frame = 0;
	e->anim_cnt = 0;
	e->anger_count = 0;
	e->shot_cnt = 0;
	e->walk_cnt = 0;

	keat_anim_speed = system_ntsc ? 14 : 12;
	kwalk_anim_speed = system_ntsc ? 12 : 10;
	kjump_anim_speed = system_ntsc ? 3 : 2;
	kprep_time = system_ntsc ? 48 : 40;
	kshot_time = system_ntsc ? 12 : 10;
	kgravity = system_ntsc ? FIX16(0.104166666667) : FIX16(0.15);
	kjump_str = system_ntsc ? FIX16(4.16666667) : FIX16(5.0);
}

// Reset the VRAM allocation position counter
void en_unload_cow(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_cow *e = (en_cow *)v;

	if (e->state == COW_WALKING)
	{
		u16 i;
		if (system_ntsc)
		{
			if (ntsc_counter == 0)
			{
				e->walk_cnt = (e->walk_cnt) ? 0 : 1;
			}
			if (ntsc_counter % 2 == e->walk_cnt)
			{
				goto do_walk;
			}
		}
		else if (system_osc % 2 == 0)
		{
			goto do_walk;
		}
		goto skip_walk;
do_walk:
		e->head.x += (e->head.direction == ENEMY_RIGHT) ? 1 : -1;

		// Collision with a wall; flip cow
		if (map_collision(e->head.x + (e->head.direction == ENEMY_RIGHT ? e->head.width : -e->head.width), e->head.y - 12))
		{
			e->head.direction = (e->head.direction == ENEMY_RIGHT) ? ENEMY_LEFT : ENEMY_RIGHT;
		}
skip_walk:
		// Check for collision with boundary
		i = ENEMIES_NUM;
		while (i--)
		{
			en_generic *b = &enemies[i];
			if (b->head.type == ENEMY_BOUNDS)
			{
				if (enemy_touching_enemy(b, (en_generic *)e))
				{
					e->state = COW_FINISHED;
				}
			}
		}
	}
	else if (e->state == COW_FINISHED)
	{
		if (e->anger_count >= COW_ANGER_NUM_HITS)
		{
			e->state = COW_ANGRY;
		}
	}
	else if (e->state == COW_JUMPING)
	{
		// basic gravity
		e->head.y += fix16ToInt(e->dy);
		e->dy += kgravity;

		// Land on ground, go back to walking
		if (e->dy > FIX16(0.0) && map_collision(e->head.x, e->head.y))
		{
			e->state = COW_WALKING;
		}
	}
	else if (e->state == COW_ANGRY)
	{
		e->head.direction = (e->head.x > pl.px) ? ENEMY_LEFT : ENEMY_RIGHT;
		if (e->shot_cnt == 0)
		{
			e->shot_cnt = kshot_time;
			projectile_shoot_at(e->head.x, e->head.y - 10, pl.px, pl.py - 10);
		}
		else
		{
			e->shot_cnt--;
		}
	}
	else if (e->state == COW_PREPARING)
	{
		if (e->anger_count > 0)
		{
			e->anger_count--;
			if (e->anger_count == 0)
			{
				e->state = COW_WALKING;
			}
		}
	}

}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_cow *e = (en_cow *)v;

	if (e->state != COW_ANGRY && e->anim_cnt == 0)
	{
		u16 len_cmp;
		switch (e->state)
		{
			default:
			case COW_EATING:
			case COW_FINISHED:
				e->anim_cnt = keat_anim_speed;
				len_cmp = 1;
				break;
			case COW_WALKING:
				e->anim_cnt = kwalk_anim_speed;
				len_cmp = 3;
				break;
			case COW_JUMPING:
				e->anim_cnt = kjump_anim_speed;
				len_cmp = 3;
				break;
			case COW_PREPARING:
				e->anim_cnt = 0;
				e->anim_frame = 0;
				len_cmp = 1;
				break;
		}
		if (e->anim_frame >= len_cmp)
		{
			e->anim_frame = 0;
		}
		else
		{
			e->anim_frame++;
		}

	}
	else
	{
		e->anim_cnt--;
	}

	// Calculate sprite offsets
	if (e->state == COW_JUMPING && e->anim_frame != 1 && e->anim_frame != 3)
	{
		// Prevent bouncing during jumping animation
		e->head.yoff[0] = -25;
		e->head.yoff[1] = -25;
	}
	else
	{
		e->head.yoff[0] = -24;
		e->head.yoff[1] = -24;
	}

	e->head.xoff[0] = (e->head.direction == ENEMY_RIGHT) ? -20 : 4;
	e->head.xoff[1] = (e->head.direction == ENEMY_RIGHT) ? -4 : -20;

	// Calculate display frame
	switch (e->state)
	{
		default:
		case COW_EATING:
		case COW_FINISHED:
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos);
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + (e->anim_frame == 0 ? 18 : 27));
			break;
		case COW_WALKING:
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos);
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 45);
			if (e->anim_frame == 1)
			{
				e->head.attr[0] += 6;
				e->head.attr[1] += 9;
			}
			else if (e->anim_frame == 3)
			{
				e->head.attr[0] += 12;
				e->head.attr[1] += 18;
			}
			break;
		case COW_JUMPING:
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos);
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 72);

			if (e->anim_frame == 1)
			{
				e->head.attr[0] += 6;
				e->head.attr[1] += 9;
			}
			else if (e->anim_frame == 3)
			{
				e->head.attr[0] += 12;
				e->head.attr[1] += 18;
			}
			break;
		case COW_ANGRY:
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos);
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 36);
			break;
		case COW_PREPARING:
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos);
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 45);
			break;

	}
}

static void cube_func(void *v, cube *c)
{
	en_cow *e = (en_cow *)v;
	if (e->state != COW_JUMPING)
	{
		if (e->head.hurt_cnt == 0)
		{
			// You can't kill the cow!
			e->head.hp = 99;
			if (e->state == COW_FINISHED)
			{
				e->anger_count++;
			}
			else if (e->state == COW_WALKING)
			{
				e->dy = -kjump_str;
				e->state = COW_JUMPING;
			}
			else if (e->state == COW_EATING)
			{
				e->anger_count = kprep_time;
				e->state = COW_PREPARING;
			}
		}

		enemy_cube_response((en_generic *)e, c);
	}

}
