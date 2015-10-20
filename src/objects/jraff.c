#include "jraff.h"
#include "gfx.h"
#include "vramslots.h"

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
	e->head.y += 64;
	e->head.width = JRAFF_WIDTH;
	e->head.height = JRAFF_HEIGHT;

	e->head.harmful = ENEMY_HARM_NORMAL;
	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(3,4);
	e->head.size[1] = SPRITE_SIZE(3,4);
	e->head.xoff[0] = -12;
	e->head.xoff[1] = -12;
	e->head.yoff[0] = -64;
	e->head.yoff[1] = -32;

	e->head.cube_func = NULL;
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;

	e->anim_cnt = 0;
	e->anim_frame = 0;
}

static void proc_func(void *v)
{
	return;
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
