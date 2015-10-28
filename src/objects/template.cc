#include "template.h"
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
		vram_pos = enemy_vram_alloc(template_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_template, vram_pos * 32, template_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_template(en_template *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = NULL;

	e->head.hp = 1;
	e->head.x += 8;
	e->head.y += 15;
	e->head.width = 7;
	e->head.height = 15;

	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -15;
}

// Reset the VRAM allocation position counter
void en_unload_template(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_template *e = (en_template *)v;
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_template *e = (en_template *)v;
}
