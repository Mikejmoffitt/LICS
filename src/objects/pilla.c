#include "pilla.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "map.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void vram_load(void);

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(PILLA_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_pilla, vram_pos * 32, PILLA_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_pilla(en_pilla *e, u16 head)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = NULL;

	e->head.hp = 1;
	e->head.x += 8;
	e->head.y += 16;
	e->head.width = 7;
	e->head.height = 15;

	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -16;

	e->dy = FIX16(0.0);
	e->is_head = head;
}

// Reset the VRAM allocation position counter
void en_unload_pilla(void)
{
	vram_pos = 0;
}

static inline void y_movement(en_pilla *e)
{
	e->head.y += fix16ToInt(e->dy);
	if (map_collision(e->head.x, e->head.y))
	{
		e->dy = FIX16(0.0);
		e->head.y = e->head.y & 0xFFF8;
	}
	else
	{
		e->dy = fix16Add(e->dy, system_ntsc ? FIX16(0.1667) : FIX16(0.2));
	}
}

static inline void x_movement(en_pilla *e)
{
	if ((!system_ntsc) || (ntsc_counter != 0))
	{
		e->head.x += (e->head.direction == ENEMY_RIGHT) ? 1 : -1;
	}
	if (e->head.direction == ENEMY_RIGHT && 
	    map_collision(e->head.x + e->head.width, e->head.y - 4))
	{
		e->head.direction = ENEMY_LEFT;
		e->head.x -= 4;
	}
	else if (e->head.direction == ENEMY_LEFT && 
	    map_collision(e->head.x - e->head.width, e->head.y - 4))
	{
		e->head.direction = ENEMY_RIGHT;
		e->head.x += 4;
	}
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_pilla *e = (en_pilla *)v;
	x_movement(e);
	y_movement(e);
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_pilla *e = (en_pilla *)v;
	u16 frame = 0;

	if (e->anim_cnt == 0)
	{
		if (e->anim_frame == 0)
		{
			e->anim_frame = 3;
		}
		else
		{
			e->anim_frame--;
		}
		e->anim_cnt = system_ntsc ? 6 : 5;
	}
	else
	{
		e->anim_cnt--;
	}

	if (!e->is_head)
	{
		frame += 12;
	}

	if (e->anim_frame == 0)
	{
		frame += 4;
	}
	else if (e->anim_frame == 2)
	{
		frame += 8;
	}


	e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + frame);
}
