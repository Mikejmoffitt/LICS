#include "items.h"
#include "save.h"
#include "gfx.h"
#include "vramslots.h"
#include "cubes.h"

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
	if (e->anim_cnt >= ITEM_ANIM_LEN)
	{
		e->anim_cnt = 0;
	}
	e->float_cnt++;
	if (e->float_cnt >= ITEM_FLOAT_LEN)
	{
		e->float_cnt = 0;
	}

	// Make the item float up and down a little
	if (e->float_cnt < (ITEM_FLOAT_LEN / 4))
	{	
		e->head.yoff[0] = -17;
	}
	else if (e->float_cnt > (ITEM_FLOAT_LEN / 2) && e->float_cnt < (3 * ITEM_FLOAT_LEN / 4))
	{
		e->head.yoff[0] = -15;
	}
	else
	{
		e->head.yoff[0] = -16;
	}

	// Select tiles
	e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, 0,
	  ITEMS_VRAM_SLOT + (e->item_type * 8) + ((e->anim_cnt > ITEM_ANIM_LEN / 2) ? 4 : 0));
}

void en_init_item(en_item *e)
{
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
}
