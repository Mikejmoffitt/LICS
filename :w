#include "metagrub.h"
#include "map.h"

#include <genesis.h>

#define METAGRUB_DECEL FIX16(0.125)

static inline fix16 get_lunge_dx(u16 dir)
{
	fix16 ret = (dir == ENEMY_RIGHT) ? FIX16(1.875) : FIX16(-1.875);
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
			ret = fix16Add(FIX16((dir == ENEMY_RIGHT)?(0.94):(-0.94)),ret);
			break;

		case 3:
			ret = fix16Add(FIX16((dir == ENEMY_RIGHT)?(1.25):(-1.25)),ret);
			break;
	}
	return ret;
}


void en_anim_metagrub(en_metagrub *e)
{
	if (e->dx != FZERO)
	{
		e->head.size = SPRITE_SIZE(3,1);
		e->head.xoff = -13;
		e->head.yoff = -7;
		e->head.attr = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, (e->head.direction == ENEMY_RIGHT) ? 0 : 1, METAGRUB_VRAM_SLOT + 4);
	}
	else
	{
		e->head.size = SPRITE_SIZE(2,2);
		e->head.xoff = -9;
		e->head.yoff = -15;
		e->head.attr = TILE_ATTR_FULL(PLAYER_PALNUM, 0, 0, (e->head.direction == ENEMY_RIGHT) ? 0 : 1, METAGRUB_VRAM_SLOT);
	}
}

void en_proc_metagrub(en_metagrub *e)
{
	if (e->dx > FIX16(0.125))
	{
		e->move_cnt = 20;
		e->dx = fix16Add(FIX16(-0.125), e->dx);
		e->head.direction = ENEMY_RIGHT;		
		if (map_collision(e->head.x + 11, e->head.y))
		{
			e->dx = fix16Mul(e->dx, FIX16(-1.0));
			e->head.direction = ENEMY_LEFT;
		}
	}
	else if (e->dx < FIX16(-0.125))
	{
		e->move_cnt = 20;
		e->dx = fix16Add(FIX16(0.125), e->dx);
		e->head.direction = ENEMY_LEFT;
		
		if (map_collision(e->head.x - 13, e->head.y))
		{
			e->dx = fix16Mul(e->dx, FIX16(-1.0));
			e->head.direction = ENEMY_RIGHT;
		}
	}
	else
	{
		e->dx = FZERO;
		if (e->move_cnt > 0)
		{
			e->move_cnt--;
		}
		else if (e->move_cnt == 0)
		{
			e->dx = get_lunge_dx(e->head.direction); 
		}
	}

	e->head.x = e->head.x + fix16ToInt(e->dx);	

	if (e->dx < FZERO)
	{

	}
}
