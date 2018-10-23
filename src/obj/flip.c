#include "obj/flip.h"
#include "vramslots.h"
#include "system/system.h"

static void en_anim_flip(void *v);
static void en_proc_flip(void *v);

static fix16 ddy;
static fix16 dy_cutoff;
static u16 anim_len;

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
	e->anim_cnt = 0;
	e->anim_frame = 0;
	e->pal_adjust = 0;

	ddy = system_ntsc ? FIX16(0.2) : FIX16(0.288);
	dy_cutoff = system_ntsc ? FIX16(2.4) : FIX16(2.88);
	anim_len = system_ntsc ? 10 : 8;
}

static void en_anim_flip(void *v)
{
	en_flip *e = (en_flip *)v;
	// Animation counter
	if (e->anim_cnt == anim_len)
	{
		e->anim_cnt = 0;
		if (e->anim_frame == 6)
		{
			e->anim_frame = 0;
		}
		else
		{
			e->anim_frame = 6;
		}
	}
	else
	{
		e->anim_cnt++;
	}
	e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, (e->head.direction == ENEMY_RIGHT) ? 0 : 1, vram_pos + e->anim_frame);

}

static void bg_collision(en_flip *e)
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

static inline void h_movement_kernel(en_flip *e)
{

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
}

static inline void h_movement(en_flip *e)
{
	// Horizontal Movement
	if (!system_ntsc)
	{
		e->pal_adjust++;
		if (e->pal_adjust == 4)
		{
			e->pal_adjust = 0;
			h_movement_kernel(e);
			bg_collision(e);
		}
	}
	h_movement_kernel(e);
	bg_collision(e);
}

static inline void v_movement(en_flip *e)
{
	// Vertical movement
	if (e->v_dir == FLIP_DOWN)
	{
		e->dy = fix16Add(e->dy, ddy);
		if (e->dy > dy_cutoff)
		{
			e->v_dir = FLIP_UP;
		}
	}
	else
	{
		e->dy = fix16Sub(e->dy, ddy);
		if (e->dy < -dy_cutoff)
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
