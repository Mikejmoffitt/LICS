#include "hedgedog.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "map.h"
#include "player.h"
#include "projectiles.h"
#include "music.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void vram_load(void);

static fix16 kgravity;
static fix16 kjump_str_base; // Base Y vector for a jump
static u16 kjump_t; // Delay until time to jump
static u16 kanim_speed; // Air animation delay (multiplied by 3 for ground)
static fix16 kshot_speed[3];

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(HEDGEDOG_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_hedgedog, vram_pos * 32, HEDGEDOG_VRAM_LEN * 16);
	}
}

static inline fix16 get_jump_str(void)
{
	return kjump_str_base + (FIX16(-1.0) >> (GET_HVCOUNTER & 0x03));
}

// Initialization boilerplate
void en_init_hedgedog(en_hedgedog *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = NULL;

	e->head.hp = 2;
	e->head.x += 12;
	e->head.y += 15;
	e->head.width = 10;
	e->head.height = 12;

	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(3,2);
	e->head.xoff[0] = -12;
	e->head.yoff[0] = -16;

	kgravity = system_ntsc ? FIX16(0.167) : FIX16(0.2);
	kjump_str_base = system_ntsc ? FIX16(-5.0) : FIX16(-6.0);
	kjump_t = system_ntsc ? 72 : 60;
	kshot_speed[0] = system_ntsc ? FIX16(1.45) : FIX16(1.740);
	kshot_speed[1] = system_ntsc ? FIX16(2.30) : FIX16(2.76);
	kshot_speed[2] = system_ntsc ? FIX16(3.0) : FIX16(3.6);
	kanim_speed = system_ntsc ? 3 : 2;

	e->dy = FIX16(0.0);
	e->walk_direction = ENEMY_LEFT;
	e->jump_cnt = 0;
	e->did_shoot = 0;
	e->state = HEDGEDOG_GROUNDED;
	e->h_delay = 0;
	e->anim_frame = 0;
	e->anim_cnt = 0;
}

static inline void y_movement(en_hedgedog *e)
{
	e->head.y += fix16ToInt(e->dy);
	// Landed on ground?
	if (map_collision(e->head.x, e->head.y))
	{
		// Set grounded state, lock to grid
		e->state = HEDGEDOG_GROUNDED;
		e->head.y = e->head.y & 0xFFF8;
		e->anim_cnt = 0;
		e->anim_frame = 0;
	}
	// Normal gravity process
	else
	{
		e->dy = fix16Add(kgravity, e->dy);
	}
}

static inline void do_shots(en_hedgedog *e)
{
	// Just passed apex of jump, time to shoot
	if (e->dy >= FIX16(0.0) && !e->did_shoot)
	{
		e->did_shoot = 1;
		if (e->head.direction == ENEMY_RIGHT)
		{
			projectile_shoot(e->head.x, e->head.y - 4, kshot_speed[0], kshot_speed[2], PROJECTILE_BALL2); 
			projectile_shoot(e->head.x, e->head.y - 4, kshot_speed[1], kshot_speed[1], PROJECTILE_BALL2); 
			projectile_shoot(e->head.x, e->head.y - 4, kshot_speed[2], kshot_speed[0], PROJECTILE_BALL2); 
		}
		else
		{
			projectile_shoot(e->head.x, e->head.y - 4, -kshot_speed[0], kshot_speed[2], PROJECTILE_BALL2); 
			projectile_shoot(e->head.x, e->head.y - 4, -kshot_speed[1], kshot_speed[1], PROJECTILE_BALL2); 
			projectile_shoot(e->head.x, e->head.y - 4, -kshot_speed[2], kshot_speed[0], PROJECTILE_BALL2); 
		}
		playsound(SFX_ENEMY_SHOOT);
	}
}

// Reset the VRAM allocation position counter
void en_unload_hedgedog(void)
{
	vram_pos = 0;
}

static inline void x_movement(en_hedgedog *e)
{
	if (e->h_delay == HEDGEDOG_HDELAY_MAX)
	{
		e->h_delay = 0;
	}
	else
	{
		e->h_delay++;
		return;
	}
	if (e->walk_direction == ENEMY_RIGHT)
	{
		e->head.x++;
	}
	else
	{
		e->head.x--;
	}
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_hedgedog *e = (en_hedgedog *)v;

	if (e->head.x < pl.px)
	{
		e->head.direction = ENEMY_RIGHT;
	}
	else
	{
		e->head.direction = ENEMY_LEFT;
	}

	if (e->state == HEDGEDOG_GROUNDED)
	{
		x_movement(e);
		if (e->jump_cnt < kjump_t)
		{
			e->jump_cnt++;
		}
		else
		{
			e->state = HEDGEDOG_JUMPING;
			e->dy = get_jump_str();
			e->walk_direction = (e->walk_direction == ENEMY_RIGHT) ? ENEMY_LEFT : ENEMY_RIGHT;
			e->jump_cnt = 0;
			e->did_shoot = 0;
			e->anim_cnt = 0;
			e->anim_frame = 0;
			playsound(SFX_ENEMY_JUMP);
		}
		e->head.width = 12;
	}
	else
	{
		y_movement(e);
		do_shots(e);
		e->head.width = 8;
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_hedgedog *e = (en_hedgedog *)v;

	if (e->state == HEDGEDOG_GROUNDED)
	{
		if (e->anim_cnt >= ((kanim_speed << 1) + kanim_speed))
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
		e->head.size[0] = SPRITE_SIZE(3,3);
		e->head.yoff[0] = -24;
		e->head.xoff[0] = -12;
		switch (e->anim_frame)
		{
			default:
				e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 16);
				break;
			case 1:
				e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 25);
				break;
			case 3:
				e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 34);
				break;
		}
	}
	else
	{		
		if (e->anim_cnt >= (kanim_speed))
		{
			e->anim_cnt = 0;
			if (e->anim_frame >= 3)
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
		e->head.size[0] = SPRITE_SIZE(2,2);
		e->head.yoff[0] = -16;
		e->head.xoff[0] = -8;
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + (e->anim_frame << 2));
	}

}
