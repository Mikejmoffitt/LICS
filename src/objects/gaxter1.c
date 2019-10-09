#include "gaxter1.h"
#include "system.h"
#include "vramslots.h"
#include "player.h"

static void en_anim_gaxter1(void *v);
static void en_proc_gaxter1(void *v);

static fix16 accel;
static fix16 topspeed;

// Dynamic VRAM slot support
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(GAXTER_VRAM_LEN / 2);
		VDP_doVRamDMA((u32)gfx_en_gaxter, (vram_pos * 32), (GAXTER_VRAM_LEN / 2) * 16);
	}
}

void en_unload_gaxter1(void)
{
	vram_pos = 0;
}

void en_init_gaxter1(en_gaxter1 *e)
{
	vram_load();
	e->head.x += 8;
	e->head.y += 8;
	e->head.hp = GAXTER1_HP;
	e->head.width = GAXTER1_WIDTH;
	e->head.height = GAXTER1_HEIGHT;
	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.yoff[0] = -8;
	e->head.attr[1] = 0;
	e->head.harmful = ENEMY_HARM_NORMAL;

	e->head.anim_func = &en_anim_gaxter1;
	e->head.proc_func = &en_proc_gaxter1;
	e->head.cube_func = NULL; // standard cube reaction

	accel = system_ntsc ? FIX16(0.1429) : FIX16(0.206);
	topspeed = system_ntsc ? FIX16(2.25) : FIX16(2.7);
}

static void en_anim_gaxter1(void *v)
{
	u16 hflip, frame;
	en_gaxter1 *e = (en_gaxter1 *)v;

	if (e->anim_cnt == (system_ntsc ? 9 : 6))
	{
		e->anim_cnt = 0;
	}
	else
	{
		e->anim_cnt++;
	}

	hflip = (e->head.direction == ENEMY_RIGHT) ? 0 : 1;
	if (system_ntsc)
	{
		if (e->anim_cnt < 3)
		{
			frame = 0;
		}
		else if (e->anim_cnt < 6)
		{
			frame = 4;
		}
		else
		{
			frame = 8;
		}
	}
	else
	{
		if (e->anim_cnt < 2)
		{
			frame = 0;
		}
		else if (e->anim_cnt < 4)
		{
			frame = 4;
		}
		else
		{
			frame = 8;
		}
	}
	e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, hflip, vram_pos + frame);
}

static void en_proc_gaxter1(void *v)
{
	en_gaxter1 *e = (en_gaxter1 *)v;

	// Lazily aim towards player
	if (e->head.x < pl.px - GAXTER1_DEADZONE && e->dx < topspeed)
	{
		e->dx = fix16Add(e->dx, accel);
		e->head.direction = ENEMY_RIGHT;
	}
	else if (e->head.x > pl.px + GAXTER1_DEADZONE && e->dx > -topspeed)
	{
		e->dx = fix16Add(e->dx, -accel);
		e->head.direction = ENEMY_LEFT;
	}
	if (e->head.y < pl.py - GAXTER1_DEADZONE && e->dy < topspeed)
	{
		e->dy = fix16Add(e->dy, accel);
	}
	else if (e->head.y > pl.py + GAXTER1_DEADZONE && e->dy > -topspeed)
	{
		e->dy = fix16Add(e->dy, -accel);
	}

	// Movement due to acceleration
	e->head.x += fix16ToInt(e->dx + FIX16(0.5));
	e->head.y += fix16ToInt(e->dy + FIX16(0.5));
}
