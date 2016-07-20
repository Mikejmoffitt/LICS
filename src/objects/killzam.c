#include "killzam.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "cubes.h"
#include "state.h"
#include "player.h"
#include "projectiles.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void cube_func(void *v, cube *c);
static void vram_load(void);

// Fadein Solid Shoot/Fadeout Invisible
static u16 ksequence[4];
static u16 kanim_delay;
static fix16 khysteresis;
static fix16 kddy;

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(KILLZAM_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_killzam, vram_pos * 32, KILLZAM_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_killzam(en_killzam *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_func;

	e->head.hp = 3;
	e->head.x += 8;
	e->head.y += 24;
	e->y_orig = e->head.y;
	e->head.width = 7;
	e->head.height = 24;

	e->head.direction = ENEMY_RIGHT;

	e->head.size[1] = SPRITE_SIZE(2,3);
	e->head.xoff[1] = -8;
	e->head.yoff[1] = -24;
	e->head.attr[1] = NULL;

	e->head.size[0] = SPRITE_SIZE(1,1);
	e->head.yoff[0] = -12;
	e->head.xoff[0] = 64;
	e->head.attr[0] = NULL;

	e->dy = FIX16(0.0);
	e->v_dir = KILLZAM_DOWN;
	e->anim_cnt = 0;
	e->anim_frame = 0;
	e->timer = 0;

	ksequence[0] = system_ntsc ? 48 : 40; // Begin Flickering
	ksequence[1] = system_ntsc ? 96 : 80; // Go solid
	ksequence[2] = system_ntsc ? 144 : 120; // Begin Flickering Again
	ksequence[3] = system_ntsc ? 192 : 160; // Disappear
	kanim_delay = system_ntsc ? 8 : 6;
	khysteresis = system_ntsc ? FIX16(2.083) : FIX16(2.4);
	kddy = system_ntsc ? FIX16(0.2084) : FIX16(0.3);
}

// Reset the VRAM allocation position counter
void en_unload_killzam(void)
{
	vram_pos = 0;
}

static inline void v_movement(en_killzam *e)
{
	// Vertical movement
	if (e->v_dir == KILLZAM_DOWN)
	{
		e->dy = fix16Add(e->dy, kddy);
		if (e->dy > khysteresis)
		{
			e->v_dir = KILLZAM_UP;
		}
	}	
	else
	{
		e->dy = fix16Sub(e->dy, kddy);
		if (e->dy < -khysteresis)
		{
			e->v_dir = KILLZAM_DOWN;
		}
	}
	if (e->dy == FIX16(0.0) && e->v_dir == KILLZAM_DOWN)
	{
		e->head.y = e->y_orig;
	}
	e->head.y += fix16ToInt(fix16Add(e->dy, FIX16(0.5)));
}

static inline void set_pos(en_killzam *e)
{
	u16 rando = GET_HVCOUNTER % 512;
	if (rando > 300)
	{
		rando -= 300;
	}
	e->head.x = state.cam_x + 10 + rando;
	rando = GET_HVCOUNTER & 0xFF;
	if (rando > 170)
	{
		rando -= 170;
	}
	e->head.y = state.cam_y + 32 + rando;
	e->y_orig = e->head.y;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_killzam *e = (en_killzam *)v;
	v_movement(e);
	if (e->timer >= ksequence[3])
	{
		e->timer = 0;
	}
	else
	{
		e->timer++;
	}

	if (e->timer >= ksequence[2] && e->timer < ksequence[3])
	{
		e->head.harmful = ENEMY_HARM_NORMAL;
	}
	else
	{
		e->head.harmful = ENEMY_HARM_NONE;
	}

	// Repositioning trigger
	if (e->timer == 1)
	{
		set_pos(e);
	}
	// If overlapping the backdrop or player during positioning, reposition
	if (e->timer < ksequence[0])
	{
		if ((map_collision(e->head.x - e->head.width, e->head.y)) ||
			(map_collision(e->head.x - e->head.width, e->head.y)) ||
			(map_collision(e->head.x + e->head.width, e->head.y - e->head.height)) || 
			(map_collision(e->head.x + e->head.width, e->head.y - e->head.height)) ||
			(e->head.touching_player))
		{
			e->timer = 0;	
		}
	}
	else
	{
		// Look at player always
		if (e->head.x < pl.px)
		{
			e->head.direction = ENEMY_RIGHT;
		}
		else
		{
			e->head.direction = ENEMY_LEFT;
		}
	}
	
	// Shoot at player 
	if (e->timer == ksequence[2])
	{
		projectile_shoot_at(e->head.x, e->head.y - 8, pl.px, pl.py - 12);
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_killzam *e = (en_killzam *)v;
	if (e->timer >= ksequence[0])
	{
		if (e->anim_cnt >= kanim_delay)
		{
			e->anim_cnt = 0;
			if (e->anim_frame != 0)
			{
				e->anim_frame = 0;
			}
			else
			{
				e->anim_frame = 6;
			}
		}
		else
		{
			e->anim_cnt++;
		}
		// Flickering
		if ((e->timer < ksequence[1] && (system_osc >> 1) % 2 == 0) ||
			(e->timer >= ksequence[2] && (system_osc >> 1) % 2 == 0))
		{
			e->head.attr[1] = NULL;
		}
		else
		{
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + e->anim_frame);
		}

		// Show the projectile he's going to shoot
		if (e->timer >= ksequence[1] && e->timer < ksequence[2])
		{
			e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, 0, PROJECTILES_VRAM_SLOT + ((system_osc >> 2) % 2));
			e->head.xoff[0] = (e->head.direction == ENEMY_RIGHT) ? 0 : -8;
		}
		else
		{
			e->head.attr[0] = NULL;
		}
	}
	else
	{
		e->head.attr[0] = NULL;
		e->head.attr[1] = NULL;
	}
}

static void cube_func(void *v, cube *c)
{
	en_killzam *e = (en_killzam *)v;
	if (e->timer >= ksequence[1] && e->timer < ksequence[2])
	{
		enemy_cube_response((en_generic *)e, c);
	}
}
