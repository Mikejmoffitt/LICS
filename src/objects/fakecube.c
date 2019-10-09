#include "fakecube.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "cubes.h"
#include "player.h"
#include "save.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void cube_func(void *v, cube *c);

void en_init_fakecube(en_fakecube *e)
{
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_func;

	e->head.hp = 99;
	e->head.x += 8;
	e->head.y += 15;
	e->head.width = 8;
	e->head.height = 16;
	e->head.harmful = ENEMY_HARM_NONE;
	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -15;
	e->head.attr[0] = TILE_ATTR_FULL(BG_PALNUM, 0, 0, 0, CUBE_VRAM_SLOT);
}

// Reset the VRAM allocation position counter
void en_unload_fakecube(void)
{
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_fakecube *e = (en_fakecube *)v;
	// If the player has phantom power, convert all fakecubes into actual
	// idle cubes to protect a potential ceiling exit
	// TODO: Remove impossible logic after finished testing the boss
	if (1 == 0 && sram.have_phantom)
	{
		cube_spawn(e->head.x, e->head.y, CUBE_BLUE,
		           CUBE_STATE_IDLE, FIX16(0.0), FIX16(0.0));
		e->head.active = 0;
		return;
	}

	
	if (e->hide_counter > 0)
	{
		e->hide_counter--;
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_fakecube *e = (en_fakecube *)v;
	u16 reappear_val = system_ntsc ? 60 : 50;
	e->head.attr[0] = NULL;
	if (e->hide_counter > reappear_val)
	{
		e->head.attr[0] = NULL;
	}
	else if (((e->hide_counter > 0) && (((system_osc >> 2) % 2)))
	         || e->hide_counter == 0)
	{
		e->head.attr[0] = TILE_ATTR_FULL(BG_PALNUM, 0, 0, 0, CUBE_VRAM_SLOT);
	}
	else
	{
		e->head.attr[0] = NULL;
	}
}

static void cube_func(void *v, cube *c)
{
	en_fakecube *e = (en_fakecube *)v;
	u16 reappear_val = system_ntsc ? 60 : 50;
	if ((c->x > e->head.x - 2) && (c->x < e->head.x + 2))
	{
		if (e->hide_counter < reappear_val)
		{
			e->hide_counter = system_ntsc ? (240) : (200);
		}
		else if (c->dy <= FIX16(0.0))
		{
//			c->state = CUBE_STATE_INACTIVE;
		}
	}
}
