// Provides a few BG scroll options
#include "obj/bgscrolly.h"
#include "system/system.h"
#include "player.h"
#include "state.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void cube_func(void *v, cube *c);
static void vram_load(void);

// Dynamic VRAM slot allocation support code
static void vram_load(void)
{
	return;
}

// Initialization boilerplate
void en_init_bgscrolly(en_bgscrolly *e, u16 data)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_func;

	e->head.hp = 1;
	e->head.y = 256;
	e->head.width = 1;
	e->head.height = 1;

	e->head.size[0] = SPRITE_SIZE(1,1);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -15;
	e->action = data;
	e->enabled = 0;

	e->head.harmful = ENEMY_HARM_NONE;
}

// Reset the VRAM allocation position counter
void en_unload_bgscrolly(void)
{
	return;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_bgscrolly *e = (en_bgscrolly *)v;
	// If not yet active, activate and destroy all others that do X scroll
	if (!e->enabled)
	{
		u16 i = ENEMIES_NUM;
		while(i--)
		{
			en_generic *q = &enemies[i];
			// Search for other scrollers in the list and disable them
			if (q->head.type == e->head.type)
			{
				q->head.active = 0;
			}
		}
		// Mark this one as active
		e->enabled = 1;
		e->head.active = 1;
	}
	else
	{
		state.bg_cam_x_offset += 2;
	}

	// Position at player
	e->head.x = pl.px;
	e->head.y = pl.py;
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	(void)v;
	return;
}

// Don't get hurt by cubes.
static void cube_func(void *v, cube *c)
{
	return;
}
