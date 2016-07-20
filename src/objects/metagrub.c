#include "metagrub.h"
#include "map.h"
#include "player.h"
#include "vramslots.h"
#include "system.h"

static void en_anim_metagrub(void *v);
static void en_proc_metagrub(void *v);
static inline fix16 get_lunge_dx(u16 dir);

static fix16 lunge_strength;
static fix16 decel;
static u16 lunge_time;

// Dynamic VRAM slot support
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(METAGRUB_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_metagrub, (vram_pos * 32), METAGRUB_VRAM_LEN * 16);
	}
}

void en_unload_metagrub(void)
{
	vram_pos = 0;
}

void en_init_metagrub(en_metagrub *e)
{
	vram_load();
	e->head.hp = 1;
	e->head.width = 7;
	e->head.height = 8;
	e->head.attr[1] = 0;
	e->head.x += 12;
	e->head.y += 7;
	e->head.harmful = ENEMY_HARM_NORMAL;

	e->head.anim_func = &en_anim_metagrub;
	e->head.proc_func = &en_proc_metagrub;
	e->head.cube_func = NULL;
	e->dx = FIX16(0.0);
	e->move_cnt = 0;

	// Region constants
	lunge_time = system_ntsc ? 24 : 20;
	decel = system_ntsc ? FIX16(0.096) : FIX16(0.13824);
	lunge_strength = system_ntsc ? FIX16(1.417) : FIX16(1.7004);
}

static fix16 lunge_str_ntsc[] = 
{
	FIX16(0.0),
	FIX16(0.52),
	FIX16(0.70),
	FIX16(0.875)
};

static fix16 lunge_str_pal[] = 
{
	FIX16(0.0),
	FIX16(0.624),
	FIX16(0.84),
	FIX16(1.05)
};

static inline fix16 get_lunge_dx(u16 dir)
{
	fix16 ret = lunge_strength;
	u16 rando = GET_HVCOUNTER % 4;
	if (system_ntsc)
	{
		ret = fix16Add(lunge_str_ntsc[rando], ret);
	}
	else
	{
		ret = fix16Add(lunge_str_pal[rando], ret);
	}
	if (dir == ENEMY_LEFT)
	{
		ret = -ret;
	}
	return ret;
}

static void en_anim_metagrub(void *v)
{
	en_metagrub *e= (en_metagrub *)v;
	if (e->dx != FZERO)
	{
		e->head.size[0] = SPRITE_SIZE(3,1);
		e->head.xoff[0] = -13;
		e->head.yoff[0] = -5;
		e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, (e->head.direction == ENEMY_RIGHT) ? 0 : 1, vram_pos + 4);
	}
	else
	{
		e->head.size[0] = SPRITE_SIZE(2,2);
		e->head.xoff[0] = -9;
		e->head.yoff[0] = -8;
		e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, (e->head.direction == ENEMY_RIGHT) ? 0 : 1, vram_pos);
	}
}

static void en_proc_metagrub(void *v)
{
	en_metagrub *e= (en_metagrub *)v;
	// Moving to the right
	if (e->dx > decel)
	{
		e->move_cnt = lunge_time;
		e->dx = fix16Add(-decel, e->dx);
		e->head.direction = ENEMY_RIGHT;		
		if (map_collision(e->head.x + 11, e->head.y - 8))
		{
			e->dx = e->dx * -1;;
			e->head.direction = ENEMY_LEFT;
		}
	}
	// Moving to the left
	else if (e->dx < -decel)
	{
		e->move_cnt = lunge_time;
		e->dx = fix16Add(decel, e->dx);
		e->head.direction = ENEMY_LEFT;
		
		if (map_collision(e->head.x - 13, e->head.y - 8))
		{
			e->dx = e->dx * -1;;
			e->head.direction = ENEMY_RIGHT;
		}
	}
	// Stopped
	else
	{
		// Clamp dx
		e->dx = FZERO;
		// Look towards player
		if (e->move_cnt == lunge_time)
		{
			if (e->head.x < pl.px)
			{
				e->head.direction = ENEMY_RIGHT;
			}
			else
			{
				e->head.direction = ENEMY_LEFT;
			}
		}

		// Countdown until it's time to move again
		if (e->move_cnt > 0)
		{
			e->move_cnt--;
		}
		// Reached 0; do the lunge
		else if (e->move_cnt == 0)
		{
			e->dx = get_lunge_dx(e->head.direction); 
		}
	}
	e->head.x = e->head.x + fix16ToInt(e->dx);	
}
