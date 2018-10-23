#include "obj/spooko.h"
#include "gfx.h"
#include "vramslots.h"
#include "system/system.h"
#include "cubes.h"

static void cube_func(void *v, cube *c);
static void vram_load(void);

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(SPOOKO_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_spooko, vram_pos * 32, SPOOKO_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_spooko(en_spooko *e, u16 size)
{
	vram_load();
	e->head.cube_func = &cube_func;

	e->head.hp = 1;
	e->head.x += 8;
	e->head.y += 15;
	e->head.width = 7;
	e->head.height = 15;

	e->head.direction = ENEMY_RIGHT;
	e->head.harmful = ENEMY_HARM_NONE;

	e->head.size[0] = size ? SPRITE_SIZE(2, 1) : SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = size ? -7 : -15;
	e->head.attr[0] = TILE_ATTR_FULL(2, 0, 0, 0, size ? vram_pos + 4 : vram_pos);
	e->size = size;
}

// Reset the VRAM allocation position counter
void en_unload_spooko(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void cube_func(void *v, cube *c)
{
	(void)v;
	(void)c;
	return;
}
