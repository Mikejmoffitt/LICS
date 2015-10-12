#include "metagrub.h"
#include "map.h"
#include "player.h"
#include "vramslots.h"

#define METAGRUB_DECEL FIX16(0.125)

static void en_anim_metagrub(en_metagrub *e);
static void en_proc_metagrub(en_metagrub *e);
static inline fix16 get_lunge_dx(u16 dir);

void en_init_metagrub(en_metagrub *e)
{
	e->head.hp = 1;
	e->head.width = 7;
	e->head.height = 8;
	e->move_cnt = 0;
	e->dx = FIX16(0.0);
	e->head.attr[1] = 0;
	e->head.x += 12;
	e->head.y += 7;
	e->head.anim_func = &en_anim_metagrub;
	e->head.proc_func = &en_proc_metagrub;
	e->head.cube_func = NULL;
}

static inline fix16 get_lunge_dx(u16 dir)
{
	fix16 ret = (dir == ENEMY_RIGHT) ? FIX16(1.875) : FIX16(-1.875);
	ret = fix16Add(FIX16(0.5),ret);
	u16 rando = GET_HVCOUNTER % 4;
	switch (rando)
	{
		case 0:
		default:
			return ret;
			break;

		case 1:
			ret = fix16Add(FIX16((dir == ENEMY_RIGHT)?(0.62):(-0.62)),ret);
			break;

		case 2:
			ret = fix16Add(FIX16((dir == ENEMY_RIGHT)?(0.84):(-0.84)),ret);
			break;

		case 3:
			ret = fix16Add(FIX16((dir == ENEMY_RIGHT)?(1.05):(-1.05)),ret);
			break;
	}
	return ret;
}

static void en_anim_metagrub(en_metagrub *e)
{
	if (e->dx != FZERO)
	{
		e->head.size[0] = SPRITE_SIZE(3,1);
		e->head.xoff[0] = -13;
		e->head.yoff[0] = -5;
		e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, (e->head.direction == ENEMY_RIGHT) ? 0 : 1, METAGRUB_VRAM_SLOT + 4);
	}
	else
	{
		e->head.size[0] = SPRITE_SIZE(2,2);
		e->head.xoff[0] = -9;
		e->head.yoff[0] = -8;
		e->head.attr[0] = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, (e->head.direction == ENEMY_RIGHT) ? 0 : 1, METAGRUB_VRAM_SLOT);
	}
}

static void en_proc_metagrub(en_metagrub *e)
{
	// Moving to the right
	if (e->dx > FIX16(0.125))
	{
		e->move_cnt = 20;
		e->dx = fix16Add(FIX16(-0.125), e->dx);
		e->head.direction = ENEMY_RIGHT;		
		if (map_collision(e->head.x + 11, e->head.y - 8))
		{
			e->dx = e->dx * -1;;
			e->head.direction = ENEMY_LEFT;
		}
	}
	// Moving to the left
	else if (e->dx < FIX16(-0.125))
	{
		e->move_cnt = 20;
		e->dx = fix16Add(FIX16(0.125), e->dx);
		e->head.direction = ENEMY_LEFT;
		
		if (map_collision(e->head.x - 13, e->head.y - 8))
		{
			e->dx = e->dx * -1;;
			e->head.direction = ENEMY_RIGHT;
		}
	}
	// Stopped
	else
	{
		// Clamp dx
		e->dx = FZERO;
		// Look towards player
		if (e->move_cnt == 20)
		{
			if (e->head.x < pl.px)
			{
				e->head.direction = ENEMY_RIGHT;
			}
			else
			{
				e->head.direction = ENEMY_LEFT;
			}
		}

		// Countdown until it's time to move again
		if (e->move_cnt > 0)
		{
			e->move_cnt--;
		}
		// Reached 0; do the lunge
		else if (e->move_cnt == 0)
		{
			e->dx = get_lunge_dx(e->head.direction); 
		}
	}
	e->head.x = e->head.x + fix16ToInt(e->dx);	
}
