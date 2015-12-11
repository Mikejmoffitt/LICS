#include "cow.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"

#define COW_EATING 0
#define COW_FINISHED 1
#define COW_WALKING 2
#define COW_JUMPING 3
#define COW_ANGRY 4

static void proc_func(void *v);
static void anim_func(void *v);
static void vram_load(void);

static u16 keat_anim_speed;
static u16 kwalk_anim_speed;
static u16 kjump_anim_speed;

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
	e->head.cube_func = NULL;

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

	e->state = COW_WALKING;
	e->anim_frame = 0;
	e->anim_cnt = 0;

	keat_anim_speed = system_ntsc ? 14 : 12;
	kwalk_anim_speed = system_ntsc ? 12 : 10;
	kjump_anim_speed = system_ntsc ? 3 : 2;
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
	if (system_osc % 4 == 0)
	{
		e->head.x++;
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
	}

}
