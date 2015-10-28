#include "items.h"
#include "save.h"
#include "gfx.h"
#include "vramslots.h"
#include "cubes.h"
#include "system.h"

static u16 kanim_len;
static u16 kfloat_len;

// Dynamic VRAM slot support
static u16 vram_pos;
static void vram_load(void)
{
	if (vram_pos == 0)
	{
		vram_pos = enemy_vram_alloc(ITEMS_VRAM_LEN / 2);
		VDP_doVRamDMA((u32)gfx_items, (vram_pos * 32), ITEMS_VRAM_LEN * 16);
	}
}

void en_unload_item(void)
{
	vram_pos = 0;
}

// Non-null but empty cube function to not allow it to get hit at all
static void cube_func(void *v, cube *c)
{
	return;
}

static void proc_func(void *v)
{
	en_item *e = (en_item *)v;

// TODO: Go to popup screen to show message for collection

	switch (e->item_type)
	{
		default:
		case ITEM_MAP:
			if (e->head.touching_player)
			{
				sram.have_map = 1;
				save_write();
			}
			if (sram.have_map)
			{
				e->head.active = ENEMY_DISABLED;
			}
			break;
		case ITEM_LIFT:
			if (e->head.touching_player)
			{
				sram.have_lift = 1;
				save_write();
			}
			if (sram.have_lift)
			{
				e->head.active = ENEMY_DISABLED;
			}
			break;
		case ITEM_JUMP:
			if (e->head.touching_player)
			{
				sram.have_jump = 1;
				save_write();
			}
			if (sram.have_jump)
			{
				e->head.active = ENEMY_DISABLED;
			}
			break;
		case ITEM_PHANTOM:
			if (e->head.touching_player)
			{
				sram.have_phantom = 1;
				save_write();
			}
			if (sram.have_phantom)
			{
				e->head.active = ENEMY_DISABLED;
			}
			break;
		case ITEM_KICK:
			if (e->head.touching_player)
			{
				sram.have_kick = 1;
				save_write();
			}
			if (sram.have_kick)
			{
				e->head.active = ENEMY_DISABLED;
			}
			break;
		case ITEM_ORANGE:
			if (e->head.touching_player)
			{
				sram.have_orange = 1;
				save_write();
			}
			if (sram.have_orange)
			{
				e->head.active = ENEMY_DISABLED;
			}
			break;
			
	}
}

static void anim_func(void *v)
{
	en_item *e = (en_item *)v;
	// Animation counters
	e->anim_cnt++;
	if (e->anim_cnt >= kanim_len)
	{
		e->anim_cnt = 0;
	}
	e->float_cnt++;
	if (e->float_cnt >= kfloat_len)
	{
		e->float_cnt = 0;
	}

	// Make the item float up and down a little
	if (e->float_cnt < (kfloat_len / 4))
	{	
		e->head.yoff[0] = -17;
	}
	else if (e->float_cnt > (kfloat_len / 2) && e->float_cnt < (3 * kfloat_len / 4))
	{
		e->head.yoff[0] = -15;
	}
	else
	{
		e->head.yoff[0] = -16;
	}

	// Select tiles
	e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, 0,
	  vram_pos + (e->item_type * 8) + ((e->anim_cnt > kanim_len / 2) ? 4 : 0));
}

void en_init_item(en_item *e, u16 type)
{
	vram_load();
	e->head.hp = 0xFF;
	e->head.width = 6;
	e->head.height = 10;
	e->head.y += 13; // Offset a little from the ground
	e->head.x += 8;
	e->head.attr[1] = 0; // no second sprite
	e->head.size[0] = SPRITE_SIZE(2,2);
	e->head.xoff[0] = -8;
	e->head.harmful = ENEMY_HARM_NONE;

	e->head.cube_func = &cube_func;
	e->head.proc_func = &proc_func;
	e->head.anim_func = &anim_func;
	
	e->anim_cnt = 0;
	e->float_cnt = 0;
	kanim_len = system_ntsc ? 32 : 38;
	kfloat_len = system_ntsc ? 18 : 22;
	e->item_type = type;
}
