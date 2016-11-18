#include "boss1.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "save.h"

// BOSS 1 STATES
// =============
//
// APPROACHING
// Walks in from the top-left, roars, drops down into the pit
//
// ATTACKING
// Waits a bit, then opens the mouth, spwaning a projectile, 2-3 times
// --> CHARGING
//
// CHARGING
// Shows charge animation, delay, then runs towards player
// --> WALLHIT
//
// WALLHIT
// Triggers cube shower
// Shows collided animation, delay, then turns to player and resumes.
// --> ATTACKING
//
// EXPLODING
// Lots of explosions spawned.
// --> INACTIVE

static void proc_func(void *v);
static void anim_func(void *v);
static void vram_load(void);

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(BOSS1_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_boss1, vram_pos * 32, BOSS1_VRAM_LEN * 16);
	}
}

// For when the boss is invulnerable
static void cube_invuln(void *v, cube *c)
{
	en_boss1 *e = (en_boss1 *)v;
	cube_destroy(c);
}

// Initialization boilerplate
void en_init_boss1(en_boss1 *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_invuln;

	e->head.hp = 5;
	e->head.x += 24;
	e->head.y += 16;
	e->head.width = 24;
	e->head.height = 16;

	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(3,4);
	e->head.size[1] = SPRITE_SIZE(3,4);
	e->head.xoff[0] = -24;
	e->head.yoff[0] = -16;
	e->head.xoff[1] = 0;
	e->head.yoff[1] = -16;
}

// Reset the VRAM allocation position counter
void en_unload_boss1(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_boss1 *e = (en_boss1 *)v;
	if (sram.beat_boss1)
	{
		e->head.active = 0;
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_boss1 *e = (en_boss1 *)v;
}
