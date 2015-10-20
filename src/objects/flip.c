#include "flip.h"
#include "vramslots.h"

static void en_anim_flip(void *v);
static void en_proc_flip(void *v);

// Dynamic VRAM slot support
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(FLIP_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_flip, vram_pos * 32, FLIP_VRAM_LEN * 16);
	}
}

void en_unload_flip(void)
{
	vram_pos = 0;
}

void en_init_flip(en_flip *e)
{
	vram_load();
	e->head.hp = 2;
	e->head.width = FLIP_WIDTH;
	e->head.height = FLIP_HEIGHT;
	e->head.direction = ENEMY_LEFT;

	e->head.size[0] = SPRITE_SIZE(3,2);
	e->head.xoff[0] = -11;
	e->head.yoff[0] = -12;
	e->head.attr[1] = 0;
	e->head.x += 12;
	e->head.y += 16;
	e->head.harmful = ENEMY_HARM_NORMAL;

	e->head.anim_func = &en_anim_flip;
	e->head.proc_func = &en_proc_flip;
	e->head.cube_func = NULL;

	e->dy = FIX16(0.0);
	e->y_orig = e->head.y;
	e->v_dir = FLIP_DOWN;
	e->h_cnt = 0;
	e->h_rev_cnt = 100;
}

static void en_anim_flip(void *v)
{
	en_flip *e = (en_flip *)v;
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
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, (e->head.direction == ENEMY_RIGHT) ? 0 : 1, vram_pos);
	}
	else
	{
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, (e->head.direction == ENEMY_RIGHT) ? 0 : 1, vram_pos + 6);
	}
}

static inline void bg_collision(en_flip *e)
{
	if (e->head.direction == ENEMY_RIGHT && 
	    map_collision(e->head.x + FLIP_WIDTH, e->head.y))
	{
		e->head.direction = ENEMY_LEFT;	
		e->head.x -= 4;
		e->h_rev_cnt = 100;
	}
	else if (e->head.direction == ENEMY_LEFT &&
	    map_collision(e->head.x - FLIP_WIDTH, e->head.y))
	{
		e->head.direction = ENEMY_RIGHT;
		e->head.x += 4;
		e->h_rev_cnt = 0;
	}
}

static inline void h_movement(en_flip *e)
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

		if (e->h_rev_cnt == FLIP_DISTANCE * 2)
		{
			e->h_rev_cnt = 0;
			e->head.direction = ENEMY_RIGHT;
		}
		else if (e->h_rev_cnt == FLIP_DISTANCE)
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

static inline void v_movement(en_flip *e)
{
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

static void en_proc_flip(void *v)
{
	en_flip *e = (en_flip *)v;
	h_movement(e);
	v_movement(e);

}
