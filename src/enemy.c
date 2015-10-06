#include "enemy.h"
#include "enemy_types.h"
#include "metagrub.h"

#include "particles.h"

#include "state.h"
#include "sprites.h"

static en_generic enemies[ENEMIES_NUM];

static inline void enemy_explode(en_generic *e)
{
	e->head.active = 0;
	particle_spawn(e->head.x, e->head.y, PARTICLE_TYPE_FIZZLE);
	particle_spawn(e->head.x, e->head.y, PARTICLE_TYPE_FIZZLE);
}

void enemy_dma_tiles(void)
{
	u16 i = 0;

	// Enemy palette
	VDP_doCRamDMA((u32)pal_enemy, ENEMY_PALNUM * 32, 16);

	// Enemy graphics
	while (enemy_vram_len[i] != 0)
	{
		VDP_doVRamDMA(enemy_vram_src[i], enemy_vram_dest[i], enemy_vram_len[i]);
		i++;
	}
}

void enemy_init(void)
{
	u16 i = ENEMIES_NUM;
	u16 j;
	while (i--)
	{
		en_generic *e = &enemies[i];
		e->head.active = 0;
		e->head.type = ENEMY_NULL;
		e->head.direction = ENEMY_RIGHT;
		e->head.x = -32;
		e->head.y = -32;
		e->head.xoff = 0;
		e->head.yoff = 0;
		e->head.attr = 0;
		e->head.size = SPRITE_SIZE(1,1);
		e->head.hurt_cnt = 0;
		e->head.hp = 1;

		j = ENEMY_DATA_SIZE;
		while (j--)
		{
			e->data[j] = 0;
		}
	}
}

void enemy_run(player *pl)
{
	u16 i = ENEMIES_NUM;
	while (i--)
	{
		en_generic *e = &enemies[i];
		if (e->head.type == ENEMY_NULL)
		{
			return;
		}
		if (e->head.active == 0)
		{
			continue;
		}
		// Process enemy hurt counter
		if (e->head.hurt_cnt != 0)
		{
			e->head.hurt_cnt--;
			if (e->head.hurt_cnt == 0)
			{
				if (e->head.hp != 0)
				{
					e->head.hp--;
				}
				else
				{
					enemy_explode(e);
				}
			}
			continue;
			// Enemies processing damage don't run their processes
		}
		// Enemy is valid. Handle it based on type.
		switch (e->head.type)
		{
			default:
				continue;
			case ENEMY_METAGRUB:
				en_proc_metagrub((en_metagrub *)e);
				en_anim_metagrub((en_metagrub *)e);
				break;
		}
	}
}

void enemy_draw(void)
{
	u16 i = ENEMIES_NUM;
	while (i--)
	{	
		en_generic *e = &enemies[i];
		if (e->head.type == ENEMY_NULL)
		{
			return;
		}
		if (e->head.active == 0)
		{
			continue;
		}
		// Handle enemies with special drawing routines
		switch (e->head.type)
		{
			default:
				break;
		}

		// Calculate screen position
		s16 ex = e->head.x + e->head.xoff - state.cam_x;
		s16 ey = e->head.y + e->head.yoff - state.cam_y;

		// Check bounds
		if (ex > -32 && ex < 320 && ey > -32 && ey < 240)
		{
			sprite_put(ex,ey, e->head.size, e->head.attr);
		}
	}
}

en_generic *enemy_place(u16 x, u16 y, u16 type)
{
	u16 i = ENEMIES_NUM;
	while (i--)
	{
		en_generic *e = &enemies[i];
		if (e->head.type == ENEMY_NULL)
		{
			e->head.x = x;
			e->head.y = y;
			e->head.type = type;

			e->head.active = 1;

			switch (type)
			{
				default:
					e->head.hp = 1;
					e->head.xoff = 0;
					e->head.yoff = 0;
					e->head.hurt_cnt = 0;
					break;
			}
			return &enemies[0];
		}
	}
	return NULL;
}
