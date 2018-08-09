#include "boss1.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "save.h"
#include "cubes.h"

// BOSS 1 STATES
// =============
//
// APPROACHING
// Walks in from the top-left, roars, drops down into the pit
// --> CHARGING
//
// ATTACKING
// Waits a bit, then opens the mouth, spwaning a projectile, 2-3 times
// --> CHARGING
//
// CHARGING
// Standing anim delay, running animation delay, then runs towards player
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
//
// At no point is the boss invulnerable. The player should not have phantom
// cubes at this point. Nothing blocks the player from jumping over the boss
// using a dropped cube, and going to retrieve the phantom cube power. This
// oversight exists in the original game, and is preserved here for people who
// may want to speedrun the game.

static void proc_func(void *v);
static void anim_func(void *v);
static void vram_load(void);
static void set_animation(en_boss1 *e, uint16_t seq);

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	// Don't bother loading the graphics if we are going to dispose
	// of the boss immediately.
	if (vram_pos == 0 && !sram.beat_boss1)
	{
		vram_pos = enemy_vram_alloc(BOSS1_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_boss1, vram_pos * 32, BOSS1_VRAM_LEN * 16);
	}
}

// For when the boss is invulnerable
static void cube_invuln(void *v, cube *c)
{
	en_boss1 *e = (en_boss1 *)v;
	if (c->type != CUBE_GREENBLUE && c->state != CUBE_STATE_EXPLODE && c->state != CUBE_STATE_FIZZLE)
	{
		cube_destroy(c);
	}
}

// Initialization boilerplate
void en_init_boss1(en_boss1 *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_invuln;

	e->head.hp = 5;
	e->head.x = -28;
	e->head.y = 96;
	e->head.width = 20;
	e->head.height = 24;

	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(3,4);
	e->head.size[1] = SPRITE_SIZE(3,4);
	e->head.xoff[0] = -24;
	e->head.yoff[0] = -32;
	e->head.xoff[1] = 0;
	e->head.yoff[1] = -32;
	e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos);
	e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 12);

	e->dy = FIX16(0.0);
	e->phase_counter = 0;
	e->pending_shots = 0;
	e->anim_seq = 0;
}

// Reset the VRAM allocation position counter
void en_unload_boss1(void)
{
	vram_pos = 0;
}

#define BOSS1_ANIM_STOPPED 0
#define BOSS1_ANIM_WALKING 1
#define BOSS1_ANIM_RUNNING 2
#define BOSS1_ANIM_ROARING 3
#define BOSS1_ANIM_HITWALL 4
#define BOSS1_ANIM_TURNING 5
#define BOSS1_ANIM_DESCENDING 6

#define BOSS1_APPROACH_START (system_ntsc ? 240 : 200)
#define BOSS1_APPROACH_STOP (system_ntsc ? 282 : 235)
#define BOSS1_ROAR_START (system_ntsc ? 312 : 260)
#define BOSS1_ROAR_STOP (system_ntsc ? 360 : 300)
#define BOSS1_DESCEND_START (system_ntsc ? 396 : 330)
#define BOSS1_DESCEND_STOP (system_ntsc ? 480 : 400)

// Descend stops when he hits the ground
// States

static void proc_battle(void *v)
{
	en_boss1 *e = (en_boss1 *)v;
	if (e->head.hp > 0)
	{
		// Run logic on only 5/6 of the frames in NTSC mode for speed adjust
		if ((system_ntsc && ntsc_counter != 1) || !system_ntsc)
		{
			e->phase_counter++;
		}
		else if (e->phase_counter < 230)
		{
			return;
		}
	}
	// This is a nearly direct port of the original MMF logic. Just full
	// disclosure - this isn't really how I'd do it.
	// TODO: Refactor to use a simple state machine.
	if (e->phase_counter == -20)
	{
		// TODO screen shake enable
		// TODO playsound(crash)
		set_animation(e, BOSS1_ANIM_HITWALL);
	}
	else if (e->phase_counter == 5)
	{
		set_animation(e, BOSS1_ANIM_STOPPED);
	}
	else if (e->phase_counter == 18 && e->pending_shots == 15)
	{
		set_animation(e, BOSS1_ANIM_TURNING);
		e->head.direction = (e->head.direction == ENEMY_RIGHT) ?
		                    ENEMY_LEFT : ENEMY_RIGHT;
	}
	else if (e->phase_counter == 19 && e->pending_shots > 0)
	{
		e->phase_counter--;
		e->pending_shots--;
		// TODO enable cube falling
	}
	else if (e->phase_counter == 20)
	{
		// TODO stop screen shake
	}
	else if (e->phase_counter == 25)
	{
		set_animation(e, BOSS1_ANIM_STOPPED);
	}
	else if (e->phase_counter == 50)
	{
		// TODO set pending shots to random(5)
		e->pending_shots = 3;
	}
	else if (e->phase_counter == 70) // <-- shots remaining, returns here
	{
		set_animation(e, BOSS1_ANIM_STOPPED);
	}
	else if (e->phase_counter == 100 && e->pending_shots > 0)
	{
		set_animation(e, BOSS1_ANIM_ROARING);
	}
	else if (e->phase_counter == 120 && e->pending_shots > 0)
	{
		// TODO Fire bouncy shot at player; expand bullets type?
		e->pending_shots--;
		e->phase_counter = 51;
	}
	else if (e->phase_counter == 200)
	{
		set_animation(e, BOSS1_ANIM_WALKING);
	}
	else if (e->phase_counter == 229)
	{
		set_animation(e, BOSS1_ANIM_RUNNING);
	}
	else if (e->phase_counter >= 230)
	{
		s16 px_per_frame = 3;
		if ((system_ntsc && (ntsc_counter % 2)))
		{
			px_per_frame = 2;
		}

		if (e->head.direction == ENEMY_RIGHT)
		{
			e->head.x += px_per_frame;
			if (map_collision(e->head.x + e->head.width,
				              e->head.y - (e->head.height / 2)))
			{
				e->phase_counter = -21;
				e->pending_shots = 15; // Per MMF, re-used as a turn anim timer
			}
		}
		else
		{
			e->head.x -= px_per_frame;
			if (map_collision(e->head.x - e->head.width,
				              e->head.y - (e->head.height / 2)))
			{
				e->phase_counter = -21;
				e->pending_shots = 15; // Per MMF, re-used as a turn anim timer
			}
		}
	}
}

static void set_animation(en_boss1 *e, u16 seq)
{
	if (seq != e->anim_seq)
	{
		e->anim_seq = seq;
		e->anim_counter = 0;
	}
}

static void proc_approach(void *v)
{
	en_boss1 *e = (en_boss1 *)v;
	e->phase_counter++;
	if (e->phase_counter >= BOSS1_APPROACH_START && e->phase_counter < BOSS1_APPROACH_STOP)
	{
		set_animation(e, BOSS1_ANIM_RUNNING);
		if ((system_ntsc && ntsc_counter != 1) || !system_ntsc)
		{
			e->head.x += 1;
		}
		// Debug color
	}
	else if (e->phase_counter >= BOSS1_ROAR_START && e->phase_counter < BOSS1_ROAR_STOP)
	{
		set_animation(e, BOSS1_ANIM_ROARING);
		if (e->phase_counter == BOSS1_ROAR_START)
		{
			// playsound(sfx_boss1_roar);
		}
	}
	else if (e->phase_counter >= BOSS1_ROAR_STOP && e->phase_counter < BOSS1_DESCEND_START)
	{
		set_animation(e, BOSS1_ANIM_STOPPED);
	}
	else if (e->phase_counter >= BOSS1_DESCEND_START)
	{
		set_animation(e, BOSS1_ANIM_DESCENDING);
		// Jump off the ledge
		if (e->phase_counter == BOSS1_DESCEND_START)
		{
			e->dy = system_ntsc ? FIX16(-1.666) : FIX16(-2.000);
		}

		// Walk forwards
		if ((system_ntsc && ntsc_counter != 1) || !system_ntsc)
		{
			e->head.x += 1;
		}

		// Fall to the ground
		e->dy = fix16Add(e->dy, system_ntsc ? FIX16(0.13888) : FIX16(0.20));
		e->head.y += fix16ToInt(e->dy);
		if (e->head.y >= 208)
		{
			e->dy = FIX16(0.0);
			e->head.y = 208;
			e->head.proc_func = proc_battle;
			set_animation(e, BOSS1_ANIM_STOPPED);
			e->phase_counter = 0;
			e->head.cube_func = NULL; // No longer invulnerable
			// playsound(sfx_crash);
			return;
		}
	}
	else
	{
		set_animation(e, BOSS1_ANIM_STOPPED);
	}
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_boss1 *e = (en_boss1 *)v;
	if (sram.beat_boss1)
	{
		e->head.active = 0;
	}
	// Kick off the boss battle
	else if (e->head.proc_func == proc_func)
	{
		e->phase_counter = 0;
		e->head.proc_func = proc_approach;
	}
}

static void anim_func(void *v)
{
	u16 frame_num = 0;
	en_boss1 *e = (en_boss1 *)v;
	e->head.size[0] = SPRITE_SIZE(3,4);
	e->head.size[1] = SPRITE_SIZE(3,4);
	e->head.xoff[0] = -24;
	e->head.yoff[0] = -32;
	e->head.xoff[1] = 0;
	e->head.yoff[1] = -32;

	e->anim_counter++;

	switch (e->anim_seq)
	{
		default:
			break;
		case BOSS1_ANIM_STOPPED:
			frame_num = 0;
			break;
		case BOSS1_ANIM_RUNNING:
			if (e->anim_counter >= 9)
			{
				e->anim_counter = 0;
			}
			frame_num = 3 + (e->anim_counter / 3);
			if (frame_num < 5)
			{
				e->head.yoff[0] -= (frame_num - 3);
				e->head.yoff[1] -= (frame_num - 3);
			}
			break;
		case BOSS1_ANIM_WALKING:
			if (e->anim_counter >= 12)
			{
				e->anim_counter = 0;
			}
			if (e->anim_counter < 3)
			{
				frame_num = 1;
			}
			else if (e->anim_counter >= 6 && e->anim_counter < 9)
			{
				frame_num = 2;
			}
			if (frame_num != 0)
			{
				e->head.yoff[0] -= 1;
				e->head.yoff[1] -= 1;
			}
			break;
		case BOSS1_ANIM_HITWALL:
			e->head.xoff[0] += ((e->anim_counter % 4 < 2) ? 1 : -1);
			e->head.xoff[1] += ((e->anim_counter % 4 < 2) ? 1 : -1);
			frame_num = 6;
			break;
		case BOSS1_ANIM_ROARING:
			e->head.xoff[0] += ((e->anim_counter % 4 < 2) ? 1 : -1);
			e->head.xoff[1] += ((e->anim_counter % 4 < 2) ? 1 : -1);
			frame_num = 7;
			break;
		case BOSS1_ANIM_DESCENDING:
			frame_num = 4;
			break;
		case BOSS1_ANIM_TURNING:
			frame_num = 8;
			e->head.xoff[0] = -16;
			e->head.xoff[1] = 0;
			e->head.size[0] = SPRITE_SIZE(2,4);
			e->head.size[1] = SPRITE_SIZE(2,4);
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0,
			                                 vram_pos + (24 * frame_num));
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 1,
			                                 vram_pos + (24 * frame_num));
			return;
	}
	if (e->head.direction == ENEMY_LEFT)
	{
		s16 st;
		st = e->head.xoff[0];
		e->head.xoff[0] = e->head.xoff[1];
		e->head.xoff[1] = st;
		st = e->head.yoff[0];
		e->head.yoff[0] = e->head.yoff[1];
		e->head.yoff[1] = st;
	}
	e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + (24 * frame_num));
	e->head.attr[1] = e->head.attr[0] + 12;
}

