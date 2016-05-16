#include "teleporter.h"
#include "gfx.h"
#include "vramslots.h"
#include "system.h"
#include "save.h"
#include "particles.h"
#include "player.h"
#include "music.h"

static void proc_func(void *v);
static void anim_func(void *v);
static void cube_func(void *v, cube *c);
static void vram_load(void);

static u16 kactive_len; // Time until the tele is disabled after it's activated
static u16 kanim_len; // Player fade in/out animation for teleporters

// Dynamic VRAM slot allocation support code
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(TELEPORTER_VRAM_LEN);
		VDP_doVRamDMA((u32)gfx_en_teleporter, vram_pos * 32, TELEPORTER_VRAM_LEN * 16);
	}
}

// Initialization boilerplate
void en_init_teleporter(en_teleporter *e, u16 data)
{
	vram_load();
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	e->head.cube_func = &cube_func;

	e->head.hp = 255;
	e->head.x += 16;
	e->head.y += 15;
	e->head.width = 1; // Actual active area is very small in the center
	e->head.height = 1;

	e->head.harmful = ENEMY_HARM_NONE;

	e->head.direction = ENEMY_RIGHT;

	// The regular base
	e->head.size[0] = SPRITE_SIZE(4,2);
	e->head.xoff[0] = -16;
	e->head.yoff[0] = 1;
	e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 16);

	// The flashy blue part
	e->head.size[1] = SPRITE_SIZE(4,2);
	e->head.xoff[1] = -16;
	e->head.yoff[1] = -15;
	e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos);

	e->id = data & 0xFF;
	e->activator = (data & 0xFF00) >> 8; // Activators remain active always
	e->anim_cnt = 0;
	e->anim_frame = 0;
	e->active_cnt = 0;
	e->disabled = 0;

	kactive_len = system_ntsc ? 90 : 75;
	kanim_len = system_ntsc ? 150 : 125;

	// TEMPORARY DEBUG STUFF FOR UNTIL YOU CAN ACTUALLY FIND THESE
}

// Reset the VRAM allocation position counter
void en_unload_teleporter(void)
{
	vram_pos = 0;
}

// Single-frame physics and interaction handler
static void proc_func(void *v)
{
	en_teleporter *e = (en_teleporter *)v;

	// Disable teleporters that aren't activators and haven't been activated.
	if (!e->activator && !sram.tele_active[e->id])
	{
		e->disabled = 1;
	}

	if (e->active_cnt > 0)
	{
		e->active_cnt--;
		if (e->active_cnt == 0)
		{
			e->disabled = 1;
		}

		// Center player on teleport pad
		if (pl.px > e->head.x)
		{
			pl.x -= system_ntsc ? FIX32(0.25) : FIX32(0.30);
		}
		else if (pl.px < e->head.x)
		{
			pl.x += system_ntsc ? FIX32(0.25) : FIX32(0.30);
		}

		// Generate sparkles
		if (system_osc % 2)
		{
			particle_spawn(e->head.x - 8 + (GET_HVCOUNTER % 16), e->head.y - 32 + (GET_HVCOUNTER % 32), PARTICLE_TYPE_SPARKLE);
		}
	}
	
	// Player collides with teleporter active region.
	if (!e->disabled && e->active_cnt == 0 && e->head.touching_player && pl.tele_out_cnt == 0)
	{
		// Mark discovery of teleporter
		sram.tele_active[e->id] = 1;

		// Player is teleporting IN, so disable the teleporter once anim stops
		if (pl.tele_in_cnt > 1)
		{

			if (pl.tele_in_cnt == (system_ntsc ? 75 : 62))
			{
				playsound(SFX_BOGOLOGO);
			}
			// Generate sparkles
			if (system_osc % 2)
			{
				particle_spawn(e->head.x - 8 + (GET_HVCOUNTER % 16), e->head.y - 32 + (GET_HVCOUNTER % 32), PARTICLE_TYPE_SPARKLE);
			}
		}
		// Player has finished teleporting in
		else if (pl.tele_in_cnt == 1)
		{
			e->disabled = 1;
		}
		// Normal active player is ready to teleport
		else
		{
			pl.dx = FIX16(0.0);
			pl.dy = FIX16(0.0);
			pl.tele_out_cnt	= kanim_len;
			pl.tele_in_cnt	= kanim_len;
			e->active_cnt = kactive_len;
			playsound(SFX_BOGOLOGO);
		}
	}
}

// Single-frame animation and sprite placement handler
static void anim_func(void *v)
{
	en_teleporter *e = (en_teleporter *)v;

	if (e->disabled)
	{
		// Just show the base without the lit up logo
		e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 24);
		// No effects for a disabled teleporter.
		e->head.attr[1] = NULL;
	}
	else
	{
		// Run animation
		if (e->anim_cnt == 2)
		{
			if (e->anim_frame == 3)
			{
				e->anim_frame = 0;
			}
			else
			{
				e->anim_frame++;
			}
			e->anim_cnt = 0;
		}
		else
		{
			e->anim_cnt++;
		}
		
		if (e->anim_frame == 0)
		{
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos);
		}
		else if (e->anim_frame == 2)
		{
			e->head.attr[1] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 8);
		}
		else
		{
			e->head.attr[1] = NULL;
		}

		if (e->anim_frame >= 2)
		{
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 24);
		}
		else
		{
			e->head.attr[0] = TILE_ATTR_FULL(ENEMY_PALNUM, 0, 0, 0, vram_pos + 16);
		}
	}
}

static void cube_func(void *v, cube *c)
{
	(void)v;
	(void)c;
	return;
}
