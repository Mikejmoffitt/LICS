#include "magibear.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "player.h"
#include "map.h"
#include "projectiles.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void vram_load(void);

static u16 kmouth_time;
static u16 kshot_time;
static u16 kanim_speed;
static u16 kshot_speed;

// Movement is every 5th frame in PAL land; every 6th in NTSC land

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(MAGIBEAR_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_magibear, vram_pos * 32, MAGIBEAR_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_magibear(en_magibear *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = NULL;

	e->head.hp = 3;
	e->head.x += 20;
	e->head.y += 31;
	e->head.width = 20;
	e->head.height = 27;

	e->head.direction = ENEMY_RIGHT;

	// The butt
	e->head.size[0] = SPRITE_SIZE(2,4);
	e->head.xoff[0] = -20;
	e->head.yoff[0] = -31;

	// The front
	e->head.size[1] = SPRITE_SIZE(3,4);
	e->head.xoff[1] = -4;
	e->head.yoff[1] = -31;

	kshot_speed = system_ntsc ? FIX16(0.75) : FIX16(0.90);
	kshot_time = system_ntsc ? 120 : 100;
	kmouth_time = system_ntsc ? 24 : 20;
	kanim_speed = system_ntsc ? 10 : 8;

	e->move_cnt = 0;
	e->shot_cnt = kshot_time;
	e->mouth_cnt = 0;
	e->anim_cnt = 0;
	e->anim_frame = 0;
}

// Reset the VRAM allocation position counter
void en_unload_magibear(void)
{
	vram_pos = 0;
}

static inline void face_player(en_magibear *e)
{
	// Offsets are for hysteresis
	if (e->head.direction == ENEMY_LEFT &&
	    e->head.x < pl.px - 10)
	{
		e->head.direction = ENEMY_RIGHT;
	}
	else if (e->head.direction == ENEMY_RIGHT &&
	    e->head.x > pl.px + 10)
	{
		e->head.direction = ENEMY_LEFT;
	}
}

static inline void h_movement(en_magibear *e)
{
	// Dip out every 6th frame for NTSC adjustment
	if (system_ntsc && ntsc_counter == 1)
	{
		return;
	}
	// Push out of backdrops
	if (e->head.direction == ENEMY_LEFT &&
	    map_collision(e->head.x - e->head.width, e->head.y - 12))
	{
		e->head.x++;
	}
	else if (e->head.direction == ENEMY_RIGHT &&
	    map_collision(e->head.x + e->head.width, e->head.y - 12))
	{
		e->head.x--;
	}

	// Movement delay counter
	if (e->move_cnt > 0)
	{
		e->move_cnt--;
	}
	else
	{
		e->move_cnt = 4;
		e->head.x += (e->head.direction == ENEMY_RIGHT) ? 1 : -1;
	}
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_magibear *e = (en_magibear *)v;

	if (e->mouth_cnt > 0)
	{
		e->mouth_cnt--;
		if (e->mouth_cnt == 0)
		{
			e->shot_cnt = kshot_time - (system_ntsc % 32) - (system_ntsc % 16);
		}
	}
	else
	{
		if (e->shot_cnt > 0)
		{
			// Standard behavior
			face_player(e);
			h_movement(e);
			e->shot_cnt--;
		}
		else
		{
			// Fire a shot
			e->mouth_cnt = kmouth_time;
			if (e->head.direction == ENEMY_RIGHT)
			{
				projectile_shoot(e->head.x + 14, e->head.y - 12, kshot_speed, FIX16(0.0), PROJECTILE_DEATHORB);
			}
			else
			{
				projectile_shoot(e->head.x - 14, e->head.y - 12, -kshot_speed, FIX16(0.0), PROJECTILE_DEATHORB);
			}
		}
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_magibear *e = (en_magibear *)v;

	// Run animation sequence
	if (e->anim_cnt == kanim_speed)
	{
		e->anim_cnt = 0;
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
		e->anim_cnt++;
	}

	// Set placement based on direction
	if (e->head.direction == ENEMY_RIGHT)
	{
		e->head.xoff[0] = -20;
		e->head.xoff[1] = -4;
	}
	else
	{
		e->head.xoff[0] = 4;
		e->head.xoff[1] = -20;
	}

	// Set attribute information
	if (e->mouth_cnt > 0)
	{
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos);
		if (system_osc % 8 >= 4)
		{
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 60);
		}
		else
		{
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 72);
		}
	}
	else if (e->anim_frame == 1)
	{
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 8);
		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 36);
	}
	else if (e->anim_frame == 3)
	{
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 16);
		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 48);
	}
	else
	{
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos);
		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 24);
	}
}
