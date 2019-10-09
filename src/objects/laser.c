#include "laser.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "cubes.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void cube_func(void *v, cube *c);
static void vram_load(void);

static u16 ksequence[4]; // Start flickering, start harming, stop harming, stop flickering

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(LASER_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_laser, vram_pos * 32, LASER_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_laser(en_laser *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_func;

	e->head.hp = 1;
	e->head.x += 8;
	e->head.y += 15;
	e->head.width = 4;
	e->head.height = 16;
	e->head.harmful = ENEMY_HARM_NONE;

	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -15;

	e->timer = 0;

	// 30 45 95 110
	ksequence[0] = system_ntsc ? 36 : 30;
	ksequence[1] = system_ntsc ? 54 : 45;
	ksequence[2] = system_ntsc ? 114 : 95;
	ksequence[3] = system_ntsc ? 132 : 110;
}

// Reset the VRAM allocation position counter
void en_unload_laser(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_laser *e = (en_laser *)v;
	e->timer++;
	if (e->timer >= ksequence[1] && e->timer < ksequence[2])
	{
		e->head.harmful = ENEMY_HARM_NORMAL;
	}
	else
	{
		e->head.harmful = ENEMY_HARM_NONE;
		if (e->timer >= ksequence[3])
		{
			e->timer = 0;
		}
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_laser *e = (en_laser *)v;
	if (e->timer < ksequence[0])
	{
		e->head.attr[0] = 0;
	}
	else if (e->timer < ksequence[1] || e->timer >= ksequence[2])
	{
		e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, (system_osc >> 2) % 2, vram_pos + 4 + (((system_osc >> 3) % 2) << 2));
	}
	else
	{
		e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, (system_osc >> 2) % 2, vram_pos);
	}
}

static void cube_func(void *v, cube *c)
{
	return;
}
