#include "gaxter1.h"
#include "vramslots.h"
#include "player.h"

static void en_anim_gaxter1(void *v);
static void en_proc_gaxter1(void *v);

void en_init_gaxter1(en_gaxter1 *e)
{
	e->head.x += 8;
	e->head.y += 8;
	e->head.hp = GAXTER1_HP;
	e->head.width = GAXTER1_WIDTH;
	e->head.height = GAXTER1_HEIGHT;
	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -8;
	e->head.attr[1] = 0;
	e->head.harmful = ENEMY_HARM_NORMAL;

	e->head.anim_func = &en_anim_gaxter1;
	e->head.proc_func = &en_proc_gaxter1;
	e->head.cube_func = NULL; // standard cube reaction
}

static void en_anim_gaxter1(void *v)
{
	u16 hflip, frame;
	en_gaxter1 *e = (en_gaxter1 *)v;

	if (e->anim_cnt == GAXTER1_ANIM_T)
	{
		e->anim_cnt = 0;
	}
	else
	{
		e->anim_cnt++;
	}

	hflip = (e->head.direction == ENEMY_RIGHT) ? 0 : 1;

	if (e->anim_cnt < GAXTER1_ANIM_T / 3)
	{
		frame = 0;
	}
	else if (e->anim_cnt < (2 * GAXTER1_ANIM_T / 3))
	{
		frame = 4;
	}
	else
	{
		frame = 8;
	}
	e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, hflip, GAXTER_VRAM_SLOT + frame);
}

static void en_proc_gaxter1(void *v)
{
	en_gaxter1 *e = (en_gaxter1 *)v;

	// Lazily aim towards player
	if (e->head.x < pl.px - GAXTER1_DEADZONE && e->dx < GAXTER1_TOPSPEED)
	{
		e->dx = fix16Add(e->dx, GAXTER1_ACCEL);
		e->head.direction = ENEMY_RIGHT;
	}
	else if (e->head.x > pl.px + GAXTER1_DEADZONE && e->dx > -GAXTER1_TOPSPEED)
	{
		e->dx = fix16Add(e->dx, -GAXTER1_ACCEL);
		e->head.direction = ENEMY_LEFT;
	}
	if (e->head.y < pl.py - GAXTER1_DEADZONE && e->dy < GAXTER1_TOPSPEED)
	{
		e->dy = fix16Add(e->dy, GAXTER1_ACCEL);
	}
	else if (e->head.y > pl.py + GAXTER1_DEADZONE && e->dy > -GAXTER1_TOPSPEED)
	{
		e->dy = fix16Add(e->dy, -GAXTER1_ACCEL);
	}

	// Movement due to acceleration
	e->head.x += fix16ToInt(e->dx);
	e->head.y += fix16ToInt(e->dy);
}
