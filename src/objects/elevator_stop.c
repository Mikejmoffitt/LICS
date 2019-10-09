#include "elevator_stop.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "elevator.h"
#include "player.h"

// Nonzero means it's a left-side arrow
#define ELE_STOP_SIDE_MASK 0xFFF0
#define ELE_STOP_DOWN 0x00
#define ELE_STOP_UPDOWN 0x01
#define ELE_STOP_UP 0x02

#define STOP_SENSITIVITY 9

static void proc_func(void *v);
static void anim_func(void *v);
static void cube_func(void *v, cube *c);
static void vram_load(void);

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(ELEVATOR_STOP_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_elevator_stop, vram_pos * 32, ELEVATOR_STOP_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_elevator_stop(en_elevator_stop *e, u16 data)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_func;

	e->head.hp = 255;
	e->head.x += 16;
	e->head.y -= 1;
	e->head.width = 16;
	e->head.height = 8;

	if (data & ELE_STOP_SIDE_MASK)
	{
		e->head.direction = ENEMY_RIGHT;
	}
	else
	{
		e->head.direction = ENEMY_LEFT;
	}
	e->directions = data & 0xFF;

	e->head.harmful = ENEMY_HARM_NONE;

	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.size[1] = SPRITE_SIZE(2,2);

	// Upper arrow
	e->head.xoff[0] = (e->head.direction == ENEMY_RIGHT) ? 24-8 : -24-8;
	e->head.yoff[0] = -40;
	// Lower arrow
	e->head.xoff[1] = e->head.xoff[0];
	e->head.yoff[1] = -15;
}

// Reset the VRAM allocation position counter
void en_unload_elevator_stop(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_elevator_stop *e = (en_elevator_stop *)v;
	
	// Check that Lyle isn't going too fast
	if (pl.dx == FIX16(0.0))
	{
		// Check for overlap with the stop, with a narrow horizontal range
		if ((pl.px < e->head.x + STOP_SENSITIVITY &&
			pl.px > e->head.x - STOP_SENSITIVITY) &&
			e->head.touching_player &&
			!pl.control_disabled &&
			pl.grounded)
		{
			// Check for buttons
			if (buttons & BUTTON_DOWN && e->directions != ELE_STOP_UP)
			{
				trigger_elevator_move(ELEVATOR_MOVING_DOWN);
			}
			else if (buttons & BUTTON_UP && e->directions != ELE_STOP_DOWN)
			{
				trigger_elevator_move(ELEVATOR_MOVING_UP);
			}
		}
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_elevator_stop *e = (en_elevator_stop *)v;
	if (e->anim_cnt == 0)
	{
		e->anim_cnt = (system_ntsc ? 18 : 15);
		e->anim_frame = (e->anim_frame ? 0 : 1);
	}
	else
	{
		e->anim_cnt--;
	}

	if ((pl.px < e->head.x + STOP_SENSITIVITY &&
	    pl.px > e->head.x - STOP_SENSITIVITY) &&
	    e->head.touching_player)
	{
			
		// Flash the up arrow if it's not a down-only elevator, and we are at that
		// animation frame. Show a black arrow otherwise.
		if (e->anim_frame && e->directions != ELE_STOP_DOWN)
		{
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 4);
		}
		else
		{
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos);
		}

		// Flash the down arrow if it's not an up-only elevator, and we are at that
		// animation frame. Show a black arrow otherwise.
		if (e->anim_frame && e->directions != ELE_STOP_UP)
		{
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 1, 0, vram_pos + 4);
		}
		else
		{
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 1, 0, vram_pos);
		}
	}
	else
	{
		e->head.attr[0] = NULL;
		e->head.attr[1] = NULL;
	}
}

// Do nothing with a cube
static void cube_func(void *v, cube *c)
{
	(void)v;
	(void)c;
	return;
}
