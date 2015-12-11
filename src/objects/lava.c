#include "lava.h"
#include "gfx.h"
#include "vramslots.h"
#include "player.h"
#include "system.h"
#include "cubes.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void cube_func(void *v, cube *c);
static void vram_load(void);

static u16 kanim_speed;

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(LAVA_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_lava, vram_pos * 32, LAVA_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_lava(en_lava *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_func;

	e->head.hp = 1337;
	e->head.x += 8;
	e->head.y += 15;
	e->head.width = 4;
	e->head.height = 224;

	e->head.direction = ENEMY_RIGHT;
	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.tall = 1; 
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -15;

	// Start off without a reference to the cow
	e->cow = NULL;

	// Delay two frames before searching for the cow
	e->cow_search_counter = 2;

	kanim_speed = system_ntsc ? 6 : 5;
}

// Reset the VRAM allocation position counter
void en_unload_lava(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_lava *e = (en_lava *)v;

	// Make sure to always push the player away
	e->head.harmful = (e->head.x < pl.px ? ENEMY_HARM_ALWAYS_BOUNCE_R : ENEMY_HARM_ALWAYS_BOUNCE_L);

	if (!map_collision(e->head.x, e->head.y + 1))
	{
		// move down at 100px/second if not touching the ground
		if (system_ntsc)
		{
			e->head.y += (ntsc_counter != 0 && ntsc_counter != 3) ? 2 : 1;
		}
		else
		{
			e->head.y += 2;
		}
	}

	// Delayed cow-reference-search
	if (e->cow_search_counter > 0)
	{
		e->cow_search_counter--;
		if (e->cow_search_counter == 0)
		{
			u16 i = ENEMIES_NUM;
			while (i--)
			{
				// If this enemy is a cow, then we are set.
				en_generic *f = &enemies[i];
				if (f->head.type == ENEMY_COW)
				{
					e->cow = f;
					break;
				}
			}
		}
	}

	// If we have a cow reference, we may check collisions with it.
	if (e->cow && enemy_touching_enemy((en_generic *)e, e->cow))
	{
		// This funky alignment makes it so the lava only appears to move in 8px increments
		// so it doesn't look like a big strip of bacon scrolling upwards
		e->head.y = (7 + e->cow->head.y - e->cow->head.height) & (~0x0007);
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_lava *e = (en_lava *)v;
	if (e->anim_cnt == kanim_speed)
	{
		e->anim_cnt = 0;
		e->anim_frame = (e->anim_frame == 0) ? 1 : 0;
	}
	else
	{
		e->anim_cnt++;
	}

	e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + (e->anim_frame ? 4 : 0));
}

static void cube_func(void *v, cube *c)
{
	// Do nothing.
}
