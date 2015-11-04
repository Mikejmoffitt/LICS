#include "flargy.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "cubes.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void cube_func(void *v, cube *c);
static void vram_load(void);

static u16 kh_cnt_max;
static u16 kanim_delay;
static u16 kpunch_time;
static fix16 kpunch_dy;

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(FLARGY_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_flargy, vram_pos * 32, FLARGY_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_flargy(en_flargy *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_func;

	e->head.hp = 2;
	e->head.x += 8;
	e->head.y += 31;
	e->head.width = 7;
	e->head.height = 28;

	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(2, 4);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -31;

	e->head.attr[1] = NULL;
	e->head.yoff[1] = -19;
	e->head.size[1] = SPRITE_SIZE(2, 2);

	e->anim_cnt = 0;
	e->anim_frame = 0;
	e->h_cnt = 0;
	e->punch_cnt = 0;
	e->walk_cnt = 0;

	kpunch_time = system_ntsc ? 24 : 20;
	kanim_delay = system_ntsc ? 10 : 8;
	kh_cnt_max = system_ntsc ? 2 : 1;
	kpunch_dy = system_ntsc ? FIX16(-1.0) : FIX16(-1.2);
}

// Reset the VRAM allocation position counter
void en_unload_flargy(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_flargy *e = (en_flargy *)v;
	if (e->punch_cnt == 0)
	{
		if (e->h_cnt == 0)
		{
			// Reverse if we've walked enough distance
			if (e->walk_cnt == 0)
			{
				e->head.direction = (e->head.direction == ENEMY_RIGHT) ? ENEMY_LEFT : ENEMY_RIGHT;
				e->walk_cnt = FLARGY_WALK_DISTANCE;
			}
			else
			{
				e->walk_cnt--;
				e->head.x += (e->head.direction == ENEMY_RIGHT) ? 1 : -1;
			}
			e->h_cnt = kh_cnt_max;
		}
		else
		{
			e->h_cnt--;
		}
	}
	else
	{
		e->punch_cnt--;
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_flargy *e = (en_flargy *)v;

	// Animate our guy
	if (e->anim_cnt == 0)
	{
		e->anim_cnt = kanim_delay;
		if (e->anim_frame == 3)
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
		e->anim_cnt--;
	}

	// Is he punching?
	if (e->punch_cnt > 0)
	{
		// Arm outstretched animation
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 24);

		// Get the big fist in there
		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 32);

		// Offset relative to the left side of sprite 0
		s16 xoff = 10;

		// This lets the fist wobble back and forth a bit.
		if (e->punch_cnt >= kpunch_time - 2)
		{
			// Right at the start of the animation it is extra outstretched
			xoff += 2;
		}
		else if ((e->punch_cnt >= kpunch_time - 12) && (system_osc >> 2) % 2)
		{
			// Bob in and out a tiny bit after the first heave
			xoff++;
		}
		e->head.xoff[1] = (e->head.direction == ENEMY_RIGHT) ? xoff : -xoff;
		e->head.xoff[1] -= 8;
	}
	else
	{	
		// No fist here
		e->head.attr[1] = NULL;

		// Calculate VRAM offset to show the correct animation frame
		u16 calc_pos;
		if (e->anim_frame == 0)
		{
			calc_pos = vram_pos;
		}
		else if (e->anim_frame == 2)
		{
			calc_pos = vram_pos + 16;
		}
		else
		{
			calc_pos = vram_pos + 8;
		}
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, calc_pos);
	}
}

static void cube_func(void *v, cube *c)
{
	en_flargy *e = (en_flargy *)v;
	if (e->punch_cnt != 0)
	{	
		return;
	}
	else if (c->state == CUBE_STATE_AIR || c->state == CUBE_STATE_KICKED)
	{
		if (e->head.direction == ENEMY_RIGHT && 
		    c->x > e->head.x &&
		    c->direction != e->head.direction)
		{
			c->dx = 4;
			c->dy = kpunch_dy;
			e->punch_cnt = kpunch_time;
			c->state = CUBE_STATE_AIR;
		}
		else if (e->head.direction == ENEMY_LEFT && 
		    c->x > e->head.x &&
		    c->direction != e->head.direction)
		{
			c->dx = -4;
			c->dy = kpunch_dy;
			e->punch_cnt = kpunch_time;
			c->state = CUBE_STATE_AIR;
		}
		else
		{
			enemy_cube_response((en_generic *)v, c);
		}
	}
}
