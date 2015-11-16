#include "tossmuffin.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void vram_load(void);

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(TOSSMUFFIN_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_tossmuffin, vram_pos * 32, TOSSMUFFIN_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_tossmuffin(en_tossmuffin *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = NULL;

	e->head.hp = 2;
	e->head.x += 8;
	e->head.y += 24;
	e->head.width = 7;
	e->head.height = 18;

	e->head.direction = ENEMY_RIGHT;

	// Tossmuffin's head sprite
	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -24;
	e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos);

	// Little body sprite
	e->head.size[1] = SPRITE_SIZE(2,1);
	e->head.xoff[1] = -8;
	e->head.yoff[1] = -8;
	e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 8);
}

// Reset the VRAM allocation position counter
void en_unload_tossmuffin(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_tossmuffin *e = (en_tossmuffin *)v;
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_tossmuffin *e = (en_tossmuffin *)v;
}
