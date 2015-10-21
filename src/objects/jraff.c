#include "jraff.h"
#include "gfx.h"
#include "vramslots.h"
#include "map.h"

static void proc_func(void *v);
static void anim_func(void *v);

// Dynamic VRAM slot support
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(JRAFF_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_jraff, vram_pos * 32, JRAFF_VRAM_LEN * 16);
	}
}

void en_unload_jraff(void)
{
	vram_pos = 0;
}

void en_init_jraff(en_jraff *e)
{
	vram_load();
	e->head.hp = 2;
	e->head.x += 12;
	e->head.y += 63;
	e->head.width = JRAFF_WIDTH;
	e->head.height = JRAFF_HEIGHT;

	e->head.harmful = ENEMY_HARM_NORMAL;
	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(3,4);
	e->head.size[1] = SPRITE_SIZE(3,4);
	e->head.xoff[0] = -12;
	e->head.xoff[1] = -12;
	e->head.yoff[0] = -63;
	e->head.yoff[1] = -31;

	e->head.cube_func = NULL;
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;

	e->anim_cnt = 0;
	e->anim_frame = 0;
}

static void h_movement(en_jraff *e)
{
	s16 test_x;
	if (e->move_cnt == JRAFF_MOVE_DIV)
	{
		e->move_cnt = 0;
	}
	else
	{
		e->move_cnt++;
		return;
	}
	if (e->head.direction == ENEMY_RIGHT)
	{
		test_x = e->head.x + e->head.width;
	}
	else
	{
		test_x = e->head.x - e->head.width;
	}
	if (map_collision(test_x, e->head.y - 1))
	{
		e->head.direction = (e->head.direction == ENEMY_RIGHT) ? ENEMY_LEFT : ENEMY_RIGHT;
	}
	if (e->head.direction == ENEMY_RIGHT)
	{
		e->head.x++;
	}
	else
	{
		e->head.x--;
	}
}

static void v_movement(en_jraff *e)
{
	e->head.y += fix16ToInt(e->dy);
	if (!map_collision(e->head.x, e->head.y))
	{
		e->dy = fix16Add(e->dy, JRAFF_GRAVITY);
	}
	if (e->dy > FIX16(0.0) && map_collision(e->head.x, e->head.y))
	{
		e->dy = FIX16(0.0);	
		e->head.y = (e->head.y) & 0xFFF8;
	}
}

static void proc_func(void *v)
{
	en_jraff *e = (en_jraff *)v;	

	h_movement(e);
	v_movement(e);
}

static void anim_func(void *v)
{
	en_jraff *e = (en_jraff *)v;
	// Animation Counter
	if (e->anim_cnt == JRAFF_ANIM_DELAY)
	{
		e->anim_cnt = 0;
		if (e->anim_frame == JRAFF_ANIM_LEN)
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
		e->anim_cnt++;
	}

	// Bottom half [1]
	if (e->anim_frame == 1)
	{
		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos);
	}
	else if (e->anim_frame == 3)
	{
		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 12);
	}
	else
	{
		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 24);
	}
	
	// Top half [0]
	e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 36 + (12 * e->anim_frame));
}
