#include "lavaanim.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "player.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void cube_func(void *v, cube *c);
static void vram_load(void);

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(LAVAANIM_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_lavaanim, vram_pos * 32, LAVAANIM_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_lavaanim(en_lavaanim *e)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_func;

	e->head.hp = 1;
	e->head.x += 8;
	e->head.y += 15;
	e->head.width = 7;
	e->head.height = 15;

	e->head.direction = ENEMY_RIGHT;

	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -15;

	e->anim_cnt = 0;
	e->anim_frame = 0;

	e->head.harmful = ENEMY_HARM_NONE;
}

// Reset the VRAM allocation position counter
void en_unload_lavaanim(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_lavaanim *e = (en_lavaanim *)v;
	// If not yet active, activate and destroy all others
	if (!e->head.active)
	{
		u16 i = ENEMIES_NUM;
		while(i--)
		{
			en_generic *q = &enemies[i];
			// Search for other lava tiles in the list and disable them
			if (q->head.type == e->head.type)
			{
				q->head.active = 0;
			}
		}
		// Mark this one as active
		e->head.active = 1;
	}
	// If active, DMA in lava animation tiles
	else
	{
		u32 source_pos = (e->anim_frame ? vram_pos : vram_pos + 4);
		u32 dest_pos = MAP_FG_VRAM_SLOT + 0xEE;

		// Copy tiles into BG area
		VDP_doVRamDMACopy((source_pos) * 32, (dest_pos) * 32, 32);
		VDP_waitDMACompletion();
		VDP_doVRamDMACopy((source_pos + 1) * 32, (dest_pos + 0x10) * 32, 32);
		VDP_waitDMACompletion();
		VDP_doVRamDMACopy((source_pos + 2) * 32, (dest_pos + 0x01) * 32, 32);
		VDP_waitDMACompletion();
		VDP_doVRamDMACopy((source_pos + 3) * 32, (dest_pos + 0x11) * 32, 32);
		VDP_waitDMACompletion();
	}

	// Position at player
	e->head.x = pl.px;
	e->head.y = pl.py;
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_lavaanim *e = (en_lavaanim *)v;

	if (e->anim_cnt >= (system_ntsc ? 12 : 10))
	{
		e->anim_cnt = 0;
		e->anim_frame = (e->anim_frame ? 0 : 1);
	}
	else
	{
		e->anim_cnt++;
	}

	e->head.attr[0] = 0;
	e->head.attr[1] = 0;
}

// Don't get hurt by cubes.
static void cube_func(void *v, cube *c)
{
	return;
}
