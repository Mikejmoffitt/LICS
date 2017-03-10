#include "wip.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "player.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void vram_load(void);

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(WIP_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_wip, vram_pos * 32, WIP_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_wip(en_wip *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = NULL;
	e->head.harmful = ENEMY_HARM_NONE;

	e->head.hp = 0xFF;
	e->head.x += 24;
	e->head.y += 32;
	e->head.width = 24;
	e->head.height = 32;

	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(3,4);
	e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, 0, vram_pos);
	e->head.xoff[0] = -24;
	e->head.yoff[0] = -32;
	e->head.size[1] = SPRITE_SIZE(3,4);
	e->head.attr[1] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, 0, vram_pos + 12);
	e->head.xoff[1] = 0;
	e->head.yoff[1] = -32;
}

// Reset the VRAM allocation position counter
void en_unload_wip(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_wip *e = (en_wip *)v;
	e->head.hp = 0xFF;
	if (e->head.x >= 128)
	{
		e->head.harmful = ENEMY_HARM_ALWAYS_BOUNCE_L;
	}
	else
	{
		e->head.harmful = ENEMY_HARM_ALWAYS_BOUNCE_R;
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_wip *e = (en_wip *)v;
	(void)e;
}
