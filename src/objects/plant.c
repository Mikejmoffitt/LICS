#include "plant.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "projectiles.h"
#include "player.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void vram_load(void);

static u16 ksequence[2]; // 0: Lean back 1: Shoot 2: Reset
static u16 kanim_speed;
static u16 kcooldown_time;

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(PLANT_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_plant, vram_pos * 32, PLANT_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_plant(en_plant *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = NULL;

	e->head.hp = 3;
	e->head.x += 8;
	e->head.y += 47;
	e->head.width = 9;
	e->head.height = 40;

	e->head.direction = ENEMY_RIGHT;

	// The head
	e->head.size[0] = SPRITE_SIZE(3,3);
	e->head.xoff[0] = -11;
	e->head.yoff[0] = -47;

	// The stalk
	e->head.size[1] = SPRITE_SIZE(2,4);
	e->head.xoff[1] = -8;
	e->head.yoff[1] = -31;

	ksequence[0] = system_ntsc ? 120 : 100;
	ksequence[1] = system_ntsc ? 156 : 130;
	kcooldown_time = system_ntsc ? 36 : 30;
	kanim_speed = system_ntsc ? 14 : 11;

	e->anim_cnt = 0;
	e->anim_frame = 0;
	e->cooldown_cnt = 0;
}

// Reset the VRAM allocation position counter
void en_unload_plant(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_plant *e = (en_plant *)v;
	e->timer++;
	if (e->timer >= ksequence[1])
	{
		e->cooldown_cnt = kcooldown_time;
		projectile_shoot_at(e->head.x + ((e->head.direction == ENEMY_RIGHT) ? 8 : -8), e->head.y - 32, pl.px, pl.py - 12);
		e->timer = 0;
	}
	if (e->cooldown_cnt > 0)
	{
		e->cooldown_cnt--;
	}
	e->head.direction = (e->head.x > pl.px) ? ENEMY_LEFT : ENEMY_RIGHT;
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_plant *e = (en_plant *)v;

	// Run normal animation sequence
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

	// Standing mode
	if (e->cooldown_cnt > 0)
	{
		s16 range_off;

		if (e->cooldown_cnt > kcooldown_time - 5)
		{
			range_off = 2;
		}
		else if (e->cooldown_cnt > kcooldown_time - 10)
		{
			range_off = -2;
		}
		else if (e->cooldown_cnt > kcooldown_time - 15)
		{
			range_off = 1;
		}
		else if (e->cooldown_cnt > kcooldown_time - 20)
		{
			range_off = -1;
		}
		else
		{
			range_off = 0;
		}
		e->head.size[0] = SPRITE_SIZE(2,4);
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 48);
		e->head.yoff[0] = -49;
		e->head.xoff[0] = (e->head.direction == ENEMY_RIGHT) ? 0 : -16;
		e->head.xoff[0]+= (e->head.direction == ENEMY_RIGHT) ? range_off : -range_off;

		e->head.size[1] = SPRITE_SIZE(2,4);
		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 16);
		e->head.xoff[1] = -8;
	}
	else if (e->timer < ksequence[0])
	{
		e->head.size[0] = SPRITE_SIZE(3,3);
		// On frames 0 and 3 the head is centered
		if (e->anim_frame == 0 || e->anim_frame == 3)
		{
			e->head.xoff[0] = (e->head.direction == ENEMY_RIGHT) ? -11 : -13;
		}
		// On frames 1 and 2 the head leans back one pixel
		else
		{
			e->head.xoff[0] = -12;
		}
		// The head bobs down one pixel on frames 2 and 3
		if (e->anim_frame >= 2)
		{
			e->head.yoff[0] = -46;
		}
		else
		{
			e->head.yoff[0] = -47;
		}

		// The head just shows a constant frame
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 56);

		// The body wiggles
		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + ((e->anim_frame % 2) ? 8 : 0));
		e->head.xoff[1] = -8;
		e->head.size[1] = SPRITE_SIZE(2,4);
	}
	else
	{
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + 56);
		e->head.yoff[0] = -46;
		if (e->head.direction == ENEMY_RIGHT)
		{
			e->head.xoff[0] = ((system_osc >> 2) % 2) ? -17 : -16;
		}
		else
		{
			e->head.xoff[0] = ((system_osc >> 2) % 2) ? -7 : -8;
		}

		e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, e->head.direction, vram_pos + (((system_osc >> 2) % 2) ? 24 : 36));
		e->head.xoff[1] = -12;
		e->head.size[1] = SPRITE_SIZE(3,4);
	}
}
