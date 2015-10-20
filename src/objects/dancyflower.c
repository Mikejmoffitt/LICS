#include "dancyflower.h"
#include "cubes.h"

static void anim_func(void *v);
static void proc_func(void *v);
static void cube_func(void *v, cube *c);

static u16 has_been_destroyed;

void en_init_dancyflower(en_dancyflower *e)
{
	e->head.width = DANCYFLOWER_WIDTH;
	e->head.height = DANCYFLOWER_HEIGHT;
	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(2,4);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -32;
	e->head.size[1] = SPRITE_SIZE(3,2);
	e->head.xoff[1] = -12;
	e->head.yoff[1] = -48; 
	e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, DANCYFLOWER_VRAM_SLOT);
	e->head.x += 12;
	e->head.y += 48;
	e->head.harmful = ENEMY_HARM_ALWAYS_BOUNCE_L;
	e->head.hp = 2;
	e->anim_cnt = 0;
	e->anim_frame = 0;

	e->head.anim_func = &anim_func;
	e->head.proc_func = &proc_func;
	e->head.cube_func = &cube_func;


	// Only one of these per game
	if (has_been_destroyed)
	{
		e->head.type = ENEMY_DISABLED;
	}
}

void anim_func(void *v)
{
	en_dancyflower *e = (en_dancyflower *)v;
	if (e->anim_cnt == DANCYFLOWER_ANIM_DELAY)
	{
		e->anim_cnt = 0;
		if (e->anim_frame == DANCYFLOWER_ANIM_LEN)
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

	// Bob head up and down
	e->head.yoff[1] = -48 + (e->anim_frame % 2);

	// Body dances left and right
	if (e->anim_frame == 1)
	{
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, DANCYFLOWER_VRAM_SLOT + 14);
	}
	else if (e->anim_frame == 3)
	{
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, DANCYFLOWER_VRAM_SLOT + 22);
	}
	else
	{
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, DANCYFLOWER_VRAM_SLOT + 6);
	}
	
}

void proc_func(void *v)
{
	return;
}

void cube_func(void *v, cube *c)
{
	en_dancyflower *e = (en_dancyflower *)v;
	if (e->head.hp <= 1)
	{
		has_been_destroyed = 1;
	}
	enemy_cube_response((en_generic *)v, c);
}
