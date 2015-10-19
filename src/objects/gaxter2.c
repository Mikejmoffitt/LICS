#include "gaxter2.h"
#include "vramslots.h"
#include "projectiles.h"
#include "gfx.h"
#include "system.h"
#include "player.h"

static void en_anim_gaxter2(void *v);
static void en_proc_gaxter2(void *v);

void en_init_gaxter2(en_gaxter2 *e)
{
	e->head.hp = GAXTER2_HP;
	e->head.width = GAXTER2_WIDTH;
	e->head.height = GAXTER2_HEIGHT;
	e->head.direction = ENEMY_LEFT;

	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -8;
	e->head.size[1] = SPRITE_SIZE(1,1);
	e->head.yoff[1] = 4;
	e->head.attr[1] = 0;
	e->head.x += 8;
	e->head.y += 8;
	e->head.harmful = ENEMY_HARM_NORMAL;

	e->head.anim_func = &en_anim_gaxter2;
	e->head.proc_func = &en_proc_gaxter2;
	e->head.cube_func = NULL;

	e->dy = FIX16(0.0);
	e->y_orig = e->head.y;
	e->v_dir = GAXTER2_DOWN;
	e->h_cnt = 0;
	e->h_rev_cnt = GAXTER2_DISTANCE;
	e->shot_clock = 0;
}

static void en_anim_gaxter2(void *v)
{
	u16 hflip, frame;
	en_gaxter2 *e = (en_gaxter2 *)v;

	if (e->anim_cnt == GAXTER2_ANIM_T)
	{
		e->anim_cnt = 0;
	}
	else
	{
		e->anim_cnt++;
	}

	hflip = (e->head.x < pl.px) ? 0 : 1;

	if (e->anim_cnt < GAXTER2_ANIM_T / 3)
	{
		frame = 12;
	}
	else if (e->anim_cnt < (2 * GAXTER2_ANIM_T / 3))
	{
		frame = 16;
	}
	else
	{
		frame = 20;
	}
	e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, hflip, GAXTER_VRAM_SLOT + frame);

	// Use sprite #2 to draw
	if (e->shot_clock >= GAXTER2_SHOT_FLICKER_T && system_osc % 2 == 0)
	{
		e->head.attr[1] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, 0, PROJECTILES_VRAM_SLOT + ((system_osc >> 2) % 2));
		e->head.xoff[1] = (hflip) ? (-GAXTER2_WIDTH - 4) : (GAXTER2_WIDTH - 4);
	}
	else
	{
		e->head.attr[1] = 0;
	}
}

static inline void shot_proc(en_gaxter2 *e)
{
	if (e->shot_clock == GAXTER2_SHOT_FIRE_T)
	{
		e->shot_clock = 0;
		if (e->head.x < pl.px)
		{
			projectile_shoot(e->head.x + GAXTER2_WIDTH, e->head.y + 6, 
			                 GAXTER2_SHOT_SPEED, GAXTER2_SHOT_SPEED, 
			                 PROJECTILE_BALL);
		}
		else
		{
			projectile_shoot(e->head.x - GAXTER2_WIDTH, e->head.y + 6, 
			                 -GAXTER2_SHOT_SPEED, GAXTER2_SHOT_SPEED, 
			                 PROJECTILE_BALL);
		}
	}
	else
	{
		e->shot_clock++;
	}
}

static inline void bg_collision(en_gaxter2 *e)
{
	if (e->head.direction == ENEMY_RIGHT && 
	    map_collision(e->head.x + GAXTER2_WIDTH, e->head.y))
	{
		e->head.direction = ENEMY_LEFT;	
		e->head.x -= 4;
		e->h_rev_cnt = 100;
	}
	else if (e->head.direction == ENEMY_LEFT &&
	    map_collision(e->head.x - GAXTER2_WIDTH, e->head.y))
	{
		e->head.direction = ENEMY_RIGHT;
		e->head.x += 4;
		e->h_rev_cnt = 0;
	}
}

static inline void h_movement(en_gaxter2 *e)
{
	// Horizontal Movement
	if (e->h_cnt == GAXTER2_H_CNT_MAX)
	{
		if (e->head.direction == ENEMY_RIGHT)
		{
			e->head.x++;
		}
		else
		{
			e->head.x--;
		}
		e->h_cnt = 0;	

		if (e->h_rev_cnt == GAXTER2_DISTANCE * 2)
		{
			e->h_rev_cnt = 0;
			e->head.direction = ENEMY_RIGHT;
		}
		else if (e->h_rev_cnt == GAXTER2_DISTANCE)
		{
			e->head.direction = ENEMY_LEFT;
		}
		
		e->h_rev_cnt++;
	}
	else
	{
		e->h_cnt++;
	}
	bg_collision(e);
}

static inline void v_movement(en_gaxter2 *e)
{
	// Vertical movement
	if (e->v_dir == GAXTER2_DOWN)
	{
		e->dy = fix16Add(e->dy, GAXTER2_DDY);
		if (e->dy > GAXTER2_DY_CUTOFF)
		{
			e->v_dir = GAXTER2_UP;
		}
	}	
	else
	{
		e->dy = fix16Sub(e->dy, GAXTER2_DDY);
		if (e->dy < GAXTER2_DY_CUTOFF * -1)
		{
			e->v_dir = GAXTER2_DOWN;
		}
	}
	if (e->dy == FIX16(0.0) && e->v_dir == GAXTER2_DOWN)
	{
		e->head.y = e->y_orig;
	}
	e->head.y += fix16ToInt(fix16Add(e->dy, FIX16(0.5)));
}

static void en_proc_gaxter2(void *v)
{
	en_gaxter2 *e = (en_gaxter2 *)v;
	h_movement(e);
	v_movement(e);
	shot_proc(e);
}
