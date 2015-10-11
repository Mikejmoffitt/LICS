#include "flip.h"
#include "vramslots.h"

#include "gfx.h"

void en_init_flip(en_flip *e)
{
	e->head.hp = 2;
	e->head.width = 11;
	e->head.height = 12;
	e->dy = FIX16(0.0);
	e->head.direction = ENEMY_LEFT;
	e->v_dir = FLIP_DOWN;
	e->h_cnt = 0;
	e->h_rev_cnt = 100;
	e->head.size[0] = SPRITE_SIZE(3,2);
	e->head.xoff[0] = -11;
	e->head.yoff[0] = -12;
	e->head.attr[1] = 0;
	e->head.x += 12;
	e->head.y += 16;
	e->y_orig = e->head.y;
}

void en_anim_flip(en_flip *e)
{
	// Animation counter
	if (e->anim_cnt == FLIP_ANIM_LEN)
	{
		e->anim_cnt = 0;
	}
	else
	{
		e->anim_cnt++;
	}

	if (e->anim_cnt > FLIP_ANIM_LEN / 2)
	{	
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, (e->head.direction == ENEMY_RIGHT) ? 0 : 1, FLIP_VRAM_SLOT);
	}
	else
	{
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, (e->head.direction == ENEMY_RIGHT) ? 0 : 1, FLIP_VRAM_SLOT + 6);
	}
}


void en_proc_flip(en_flip *e)
{
	// Horizontal Movement
	if (e->h_cnt == FLIP_H_CNT_MAX)
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

		if (e->h_rev_cnt == 200)
		{
			e->h_rev_cnt = 0;
			e->head.direction = ENEMY_RIGHT;
		}
		else if (e->h_rev_cnt == 100)
		{
			e->head.direction = ENEMY_LEFT;
		}
		
		e->h_rev_cnt++;
	}
	else
	{
		e->h_cnt++;
	}
	// Vertical movement
	if (e->v_dir == FLIP_DOWN)
	{
		e->dy = fix16Add(e->dy, FLIP_DDY);
		if (e->dy > FLIP_DY_CUTOFF)
		{
			e->v_dir = FLIP_UP;
		}
	}	
	else
	{
		e->dy = fix16Sub(e->dy, FLIP_DDY);
		if (e->dy < FLIP_DY_CUTOFF * -1)
		{
			e->v_dir = FLIP_DOWN;
		}
	}
	if (e->dy == FIX16(0.0) && e->v_dir == FLIP_DOWN)
	{
		e->head.y = e->y_orig;
	}
	e->head.y += fix16ToInt(fix16Add(e->dy, FIX16(0.5)));
}
