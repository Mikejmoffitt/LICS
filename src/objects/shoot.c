#include "shoot.h"
#include "vramslots.h"
#include "system.h"
#include "player.h"

// TODO: Verify shoot's swoop distance.

static void en_anim_shoot(void *v);
static void en_proc_shoot(void *v);

static fix16 ddy;
static fix16 dy_cutoff;
static fix16 dy_cutoff_big;
static u16 anim_len;

// Dynamic VRAM slot support
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(SHOOT_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_shoot, vram_pos * 32, SHOOT_VRAM_LEN * 16);
	}
}

void en_unload_shoot(void)
{
	vram_pos = 0;
}

void en_init_shoot(en_shoot *e)
{
	vram_load();
	e->head.hp = 2;
	e->head.width = SHOOT_WIDTH;
	e->head.height = SHOOT_HEIGHT;
	e->head.direction = ENEMY_LEFT;

	e->head.size[0] = SPRITE_SIZE(3,2);
	e->head.xoff[0] = -11;
	e->head.yoff[0] = -12;
	e->head.attr[1] = 0;
	e->head.x += 12;
	e->head.y += 16;
	e->head.harmful = ENEMY_HARM_NORMAL;

	e->head.anim_func = &en_anim_shoot;
	e->head.proc_func = &en_proc_shoot;
	e->head.cube_func = NULL;

	e->dy = FIX16(0.0);
	e->y_orig = e->head.y;
	e->v_dir = SHOOT_DOWN;
	e->h_rev_cnt = 200;
	e->anim_cnt = 0;
	e->anim_frame = 0;
	e->swoop_en = 0;

	ddy = system_ntsc ? FIX16(0.2) : FIX16(0.288);
	dy_cutoff = system_ntsc ? FIX16(2.2) : FIX16(2.64);
	dy_cutoff_big = system_ntsc ? FIX16(4.18) : FIX16(5.016);
	anim_len = system_ntsc ? 10 : 8;
}

static void en_anim_shoot(void *v)
{
	en_shoot *e = (en_shoot *)v;
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

static inline void bg_collision(en_shoot *e)
{
	// TODO: Verify this early return is okay.
	// I don't think shoot ever is supposed to hit the background, at least not during a swoop.
	(void)e;
	return;
	if (e->head.direction == ENEMY_RIGHT &&
	    map_collision(e->head.x + SHOOT_WIDTH, e->head.y))
	{
		e->head.direction = ENEMY_LEFT;
		e->head.x -= 4;
		e->h_rev_cnt = 100;
	}
	else if (e->head.direction == ENEMY_LEFT &&
	    map_collision(e->head.x - SHOOT_WIDTH, e->head.y))
	{
		e->head.direction = ENEMY_RIGHT;
		e->head.x += 4;
		e->h_rev_cnt = 0;
	}
}

static inline void h_movement(en_shoot *e)
{
	if (system_ntsc && ntsc_counter == 0)
	{
		return;
	}
	if (e->head.direction == ENEMY_RIGHT)
	{
		e->head.x++;
	}
	else
	{
		e->head.x--;
	}

	if (e->h_rev_cnt == SHOOT_DISTANCE * 2)
	{
		e->h_rev_cnt = 0;
		e->head.direction = ENEMY_RIGHT;
	}
	else if (e->h_rev_cnt == SHOOT_DISTANCE)
	{
		e->head.direction = ENEMY_LEFT;
	}

	e->h_rev_cnt++;
	bg_collision(e);
}

static inline void v_movement(en_shoot *e)
{
	// Vertical movement
	if (e->v_dir == SHOOT_DOWN)
	{
		e->dy = fix16Add(e->dy, ddy);
		if (e->swoop_en)
		{
			if (e->dy > dy_cutoff_big)
			{
				e->v_dir = SHOOT_UP;
			}
		}
		else
		{
			if (e->dy > dy_cutoff)
			{
				e->v_dir = SHOOT_UP;
			}
		}
	}
	else
	{
		e->dy = fix16Sub(e->dy, ddy);
		if (e->swoop_en)
		{
			if (e->dy < -dy_cutoff_big)
			{
				e->v_dir = SHOOT_DOWN;
				e->swoop_en = 0;
			}
		}
		else
		{
			if (e->dy < -dy_cutoff)
			{
				e->v_dir = SHOOT_DOWN;
			}
		}
	}
	if (e->dy == FIX16(0.0) && e->v_dir == SHOOT_DOWN)
	{
		e->head.y = e->y_orig;
		// Check player prox for swoop enable
		if (pl.px < e->head.x + SHOOT_PROX && pl.px > e->head.x - SHOOT_PROX)
		{
			e->swoop_en = 1;
		}
	}
	e->head.y += fix16ToInt(fix16Add(e->dy, FIX16(0.5)));
}

static void en_proc_shoot(void *v)
{
	en_shoot *e = (en_shoot *)v;
	h_movement(e);
	v_movement(e);

}
