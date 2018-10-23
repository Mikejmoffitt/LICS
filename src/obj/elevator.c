#include "obj/elevator.h"
#include "gfx.h"
#include "cubes.h"
#include "vramslots.h"
#include "system/system.h"
#include "player.h"
#include "system/music.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void cube_func(void *v, cube *c);
static void vram_load(void);

// TODO: NTSC Elevator is still 6/5 the PAL speed. Need to slot it down.
// TODO: Elevator sound effects / ambient, and backdrop

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(ELEVATOR_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_elevator, vram_pos * 32, ELEVATOR_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_elevator(en_elevator *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_func;

	e->head.hp = 1;
	e->head.x += 16;
	e->head.y += 48;
	e->head.width = 16;
	e->head.height = 64;

	e->anim_frame = 0;
	e->anim_cnt = 0;
	e->moving = ELEVATOR_MOVING_NONE;
	e->fresh_obj = 1;
	e->collision_timeout = 0;

	e->head.direction = ENEMY_RIGHT;
	e->head.harmful = ENEMY_HARM_NONE;

	e->head.size[0] = SPRITE_SIZE(4,4);
	e->head.size[1] = SPRITE_SIZE(4,4);
	e->head.xoff[0] = -16;
	e->head.xoff[1] = -16;
	e->head.yoff[0] = -48;
	e->head.yoff[1] = -16;
}

// Reset the VRAM allocation position counter
void en_unload_elevator(void)
{
	vram_pos = 0;
}

void trigger_elevator_move(u16 dir_move)
{
	u16 i = ENEMIES_NUM;
	while (i--)
	{
		en_generic *e = &enemies[i];
		if (e->head.type == ENEMY_ELEVATOR && e->head.active)
		{
			en_elevator *x = (en_elevator *)e;
			if (x->moving == ELEVATOR_MOVING_NONE)
			{
				x->collision_timeout = 32;
				x->moving = dir_move;
				playsound(SFX_ELEVATOR_MOVE);
			}
			return;
		}
	}
}

static void check_stop_coll(en_elevator *e)
{
	if (e->collision_timeout > 0)
	{
		e->collision_timeout--;
		return;
	}

	// Look for elevator stop objects
	u16 i = ENEMIES_NUM;
	while (i--)
	{
		en_generic *x = &enemies[i];
		if (x->head.type != ENEMY_ELEVATOR_STOP)
		{
			continue;
		}
		if ((x->head.y < (e->head.y + 2)) && ((x->head.y + 2) > e->head.y))
		{
			pl.y = FIX32(e->head.y - 1);
			pl.dy = FIX16(0.0);
			e->moving = ELEVATOR_MOVING_NONE;
			e->head.y = x->head.y+1;
			stopsound();
			return;
		}
	}
}

static void run_movement(en_elevator *e)
{
	// Move elevator, and latch player
	if (e->moving == ELEVATOR_MOVING_DOWN)
	{
		e->head.y += 2;
		pl.dy = FIX16(2.0);
	}
	else if (e->moving == ELEVATOR_MOVING_UP)
	{
		e->head.y -= 2;
		pl.dy = FIX16(-2.0);
	}

	if (e->moving == ELEVATOR_MOVING_NONE)
	{
		pl.ext_disable = 0;
	}
	else
	{
		pl.y = FIX32(e->head.y - 1);
		pl.ext_disable = 1;
		check_stop_coll(e);
	}
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_elevator *e = (en_elevator *)v;

	// Destroy any other elevators when touching player for the first time
	if (e->head.touching_player && e->fresh_obj)
	{
		u16 i = ENEMIES_NUM;
		while (i--)
		{
			en_generic *x = &enemies[i];
			if (x->head.type == ENEMY_ELEVATOR && !x->head.touching_player)
			{
				x->head.active = 0;
			}
		}
		e->fresh_obj = 0;
	}

	run_movement(e);
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_elevator *e = (en_elevator *)v;
	// Animation boilerplate
	if (e->anim_cnt == 0)
	{
		e->anim_cnt = (system_ntsc ? 12 : 10);
		e->anim_frame = (e->anim_frame ? 0 : 1);
	}
	else
	{
		e->anim_cnt--;
	}

	// Upper and lower halves, animated with offsets provided by anim_frame
	e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + (e->anim_frame ? 32 : 0));
	e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 16 + (e->anim_frame ? 32 : 0));
}

// Do nothing with a cube
static void cube_func(void *v, cube *c)
{
	(void)v;
	(void)c;
	return;
}
