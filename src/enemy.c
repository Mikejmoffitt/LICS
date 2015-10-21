#include "enemy.h"
#include "enemy_types.h"

#include "metagrub.h"
#include "flip.h"
#include "boingo.h"
#include "items.h"
#include "sprites.h"
#include "gaxter1.h"
#include "gaxter2.h"
#include "buggo.h"
#include "dancyflower.h"
#include "jraff.h"

#include "state.h"
#include "particles.h"

#include "player.h"
#include "music.h"
#include "powerups.h"
#include "system.h"
#include "cubes.h"
#include "save.h"
#include "music.h"

en_generic enemies[ENEMIES_NUM];
u16 vram_slot;

static void enemy_explode(en_generic *e);
static void enemy_player_scan();

// ---------------------------------------------------------------------

static void enemy_explode(en_generic *e)
{
	e->head.active = ENEMY_DISABLED;
	particle_spawn(e->head.x, e->head.y, PARTICLE_TYPE_FIZZLERED);
	particle_spawn(e->head.x + e->head.width, e->head.y, PARTICLE_TYPE_FIZZLERED);
	particle_spawn(e->head.x - e->head.width, e->head.y, PARTICLE_TYPE_FIZZLERED);
	particle_spawn(e->head.x, e->head.y - e->head.height, PARTICLE_TYPE_FIZZLERED);
	particle_spawn(e->head.x + e->head.width, e->head.y - e->head.height, PARTICLE_TYPE_FIZZLERED);
	particle_spawn(e->head.x - e->head.width, e->head.y - e->head.height, PARTICLE_TYPE_FIZZLERED);
	if (GET_HVCOUNTER % 2)
	{
		powerup_spawn(e->head.x, e->head.y, 1 + (system_osc & (e->head.powerup_range)), 0);
	}
	playsound(SFX_ENEMY_EXPLODE);
}

void enemy_dma_tiles(void)
{
	// Enemy palette
	VDP_doCRamDMA((u32)pal_enemy, ENEMY_PALNUM * 32, 16);
}

void enemy_init(void)
{
	u16 i = ENEMIES_NUM;
	u16 j;
	en_unload_metagrub();
	en_unload_flip();
	en_unload_boingo();
	en_unload_item();
	en_unload_gaxter1();
	en_unload_gaxter2();
	en_unload_buggo();
	en_unload_dancyflower();
	en_unload_jraff();
	enemy_vram_reset();
	while (i--)
	{
		en_generic *e = &enemies[i];
		e->head.anim_func = NULL;
		e->head.proc_func = NULL;
		e->head.cube_func = NULL;
		e->head.type = ENEMY_NULL;
		e->head.direction = ENEMY_RIGHT;
		e->head.active = ENEMY_DISABLED;
		e->head.x = -32;
		e->head.y = -32;
		e->head.xoff[0] = 0;
		e->head.yoff[0] = 0;
		e->head.attr[0] = 0;
		e->head.size[0] = SPRITE_SIZE(1,1);
		e->head.attr[1] = 0;
		e->head.hurt_cnt = 0;
		e->head.hp = 1;
		e->head.width = 4;
		e->head.height = 4;
		e->head.powerup_range = 1;

		j = ENEMY_DATA_SIZE;
		while (j--)
		{
			e->data[j] = 0;
		}
	}
}

void enemy_run(void)
{
	u16 i = ENEMIES_NUM;
	while (i--)
	{
		en_generic *e = &enemies[i];
		// Enemies are added in order, only once, so a null enemy type means
		// this is the end of the list and we can stop processing it.
		if (e->head.type == ENEMY_NULL)
		{
			break;
		}
		if (e->head.active == ENEMY_DISABLED)
		{
			continue;
		}
		// Enemy must be within 64px of the screen to be "alive"
		else if (e->head.x < state.cam_x - ENEMY_ACTIVE_DISTANCE ||
			e->head.x > state.cam_x + STATE_SC_W  + ENEMY_ACTIVE_DISTANCE ||
			e->head.y < state.cam_y - ENEMY_ACTIVE_DISTANCE ||
			e->head.y > state.cam_y + STATE_SC_H + (ENEMY_ACTIVE_DISTANCE * 2))
		{
			e->head.active = ENEMY_OFFSCREEN;
			continue;
		}
		e->head.active = ENEMY_ONSCREEN;
		// Process enemy hurt counter
		if (e->head.hurt_cnt != 0)
		{
			e->head.hurt_cnt--;
			if (e->head.hurt_cnt == 0)
			{
				if (e->head.hp > 1)
				{
					e->head.hp--;
				}
				else
				{
					enemy_explode(e);
				}
			}
			// Enemies processing damage don't run their processes
			continue;
		}
		// Enemy is valid. Run its processes if needed.
		if (e->head.proc_func)
		{
			e->head.proc_func((void *)e);
		}
		if (e->head.anim_func)
		{
			e->head.anim_func((void *)e);
		}
	}
	enemy_player_scan();
}

void enemy_draw(void)
{
	u16 i = ENEMIES_NUM;
	while (i--)
	{	
		en_generic *e = &enemies[i];
		// Enemies are added in order, only once, so a null enemy type means
		// this is the end of the list and we can stop processing it.
		if (e->head.type == ENEMY_NULL)
		{
			return;
		}
		// Not to be drawn
		else if (e->head.active != ENEMY_ONSCREEN)
		{
			continue;
		}
		// Flashing for hurt counter
		else if (e->head.hurt_cnt % 4 > 1)
		{
			continue;
		}

		// Calculate screen position
		s16 ex = e->head.x + e->head.xoff[0] - state.cam_x;
		s16 ey = e->head.y + e->head.yoff[0] - state.cam_y;

		if (e->head.hurt_cnt > 0)
		{
			ex = ex - 2 + GET_HVCOUNTER % 4;
			ey = ey - 2 + (GET_HVCOUNTER >> 1) % 4;
		}

		// Check bounds
		if (ex > -32 && ex < 320 && ey > -32 && ey < 240)
		{
			sprite_put(ex,ey, e->head.size[0], e->head.attr[0]);
			if (e->head.attr[1])
			{
				s16 ex = e->head.x + e->head.xoff[1] - state.cam_x;
				s16 ey = e->head.y + e->head.yoff[1] - state.cam_y;
				if (e->head.hurt_cnt > 0)
				{
					ex = ex - 2 + GET_HVCOUNTER % 4;
					ey = ey - 2 + (GET_HVCOUNTER >> 1) % 4;
				}
				sprite_put(ex,ey, e->head.size[1], e->head.attr[1]);
			}
		}
	}
}

void enemy_get_hurt(en_generic *e)
{
	if (e->head.hurt_cnt == 0)
	{
		e->head.hurt_cnt = ENEMY_HURT_TIME;
		playsound(SFX_ENEMY_STRIKE);
	}
}

// The typical and default "bounce cube off enemy, damage enemy" reaction.
// Most enemies will use this. 
void enemy_cube_response(en_generic *e, cube *c)
{
	if (c->type == CUBE_PHANTOM && sram.have_double_phantom)
	{
		// Let the enemy get hurt extra
		if (e->head.hp > 1)
		{
			e->head.hp--;
		}
	}
	if (c->type == CUBE_RED)
	{	
		if (e->head.hp > 1)
		{
			e->head.hp--;
		}
		// Let the enemy get hurt extra
		if (e->head.hp > 1)
		{
			e->head.hp--;
		}

	}
	if (e->head.hurt_cnt != 0)
	{
		return;	
	}
	if (c->type == CUBE_GREEN)
	{
		c->state = CUBE_STATE_AIR;
		if (c->y < e->head.y)
		{
			c->dy = -1 * (c->dy >> 1);
		}
		else
		{
			c->dy = -CUBE_ON_CUBE_DY;
		}
		if (c->dx == FZERO)
		{
			c->dx = (GET_HVCOUNTER % 2) ? 1 : -1;
		}
		else
		{
			c->dx = c->dx * -1;
			cube_clamp_dx(c);
		}
	}
	else if (c->state != CUBE_STATE_EXPLODE && c->state != CUBE_STATE_FIZZLE)
	{
		cube_destroy(c);	
	}
	enemy_get_hurt(e);
}

// Called by cubes.c when a collision against an enemy is detected
void enemy_cube_impact(en_generic *e, cube *c)
{
	// If a cube impact handler is registered, run it
	if (e->head.cube_func)
	{
		e->head.cube_func((void *)e, c);
	}
	// This is the default impact response
	else
	{
		enemy_cube_response(e, c);
	}
}

en_generic *enemy_place(u16 x, u16 y, u16 type)
{
	u16 i = ENEMIES_NUM;
	if (type == ENEMY_NULL)
	{
		return NULL;
	}
	while (i--)
	{
		en_generic *e = &enemies[i];
		if (e->head.type == ENEMY_NULL)
		{
			e->head.x = x;
			e->head.y = y;
			e->head.type = type;
			e->head.hurt_cnt = 0;
			e->head.touching_player = 0;
			e->head.harmful = ENEMY_HARM_NORMAL;
			e->head.anim_func = NULL;
			e->head.proc_func = NULL;
			e->head.cube_func = NULL;
			e->head.active = ENEMY_OFFSCREEN;
			
			switch (type)
			{
				default:
					e->head.active = ENEMY_DISABLED;
					return NULL;
					break;
				case ENEMY_METAGRUB:
					en_init_metagrub((en_metagrub *)e);
					break;
				case ENEMY_FLIP:
					en_init_flip((en_flip *)e);
					break;
				case ENEMY_BOINGO:
					en_init_boingo((en_boingo *)e);
					break;
				case ENEMY_ITEM:
					en_init_item((en_item *)e);
					break;
				case ENEMY_GAXTER1:
					en_init_gaxter1((en_gaxter1 *)e);
					break;
				case ENEMY_GAXTER2:
					en_init_gaxter2((en_gaxter2 *)e);
					break;
				case ENEMY_BUGGO1:
					en_init_buggo((en_buggo *)e, BUGGO_T1);
					break;
				case ENEMY_BUGGO2:
					en_init_buggo((en_buggo *)e, BUGGO_T2);
					break;
				case ENEMY_DANCYFLOWER:
					en_init_dancyflower((en_dancyflower *)e);
					break;
				case ENEMY_JRAFF:
					en_init_jraff((en_jraff *)e);
					break;
			}
			return &enemies[i];
		}
	}
	return NULL;
}

static void enemy_player_scan(void)
{
	u16 i = ENEMIES_NUM;
	while (i--)
	{
		en_generic *e = &enemies[i];
		if (e->head.type == ENEMY_NULL)
		{
			return;
		}
		else if (e->head.active != ENEMY_ONSCREEN)
		{
			continue;
		}
		if (e->head.x - e->head.width <= pl.px + PLAYER_CHK_RIGHT &&
			e->head.x + e->head.width >= pl.px + PLAYER_CHK_LEFT &&
			e->head.y - e->head.height <= pl.py + PLAYER_CHK_BOTTOM &&
			e->head.y >= pl.py + PLAYER_CHK_TOP)
		{
			e->head.touching_player = 1;
			if (e->head.harmful == ENEMY_HARM_NORMAL)
			{
				player_get_hurt();
			}
			else if (e->head.harmful == ENEMY_HARM_ALWAYS_BOUNCE_L)
			{
				// Bounce the player always away from the enemy, since this
				// type is typically used as a blockade.
				player_get_hurt();
				player_get_bounced();
				if (pl.dx > FIX16(0.0))
				{
					pl.dx = -pl.dx;
				}
				pl.direction = PLAYER_RIGHT;
			}
			else if (e->head.harmful == ENEMY_HARM_ALWAYS_BOUNCE_R)
			{
				// Bounce the player always away from the enemy, since this
				// type is typically used as a blockade.
				player_get_hurt();
				player_get_bounced();
				if (pl.dx < FIX16(0.0))
				{
					pl.dx = -pl.dx;
				}
				pl.direction = PLAYER_LEFT;
			}
			else if (e->head.harmful == ENEMY_HARM_KILL)
			{
				// Set up conditions to ensure the player will die
				pl.hp = 0;
				pl.invuln_cnt = 0;
				player_get_hurt();
			}
		}
		else
		{
			e->head.touching_player = 0;
		}
	}
}


u16 enemy_vram_alloc(u16 len)
{
	u16 ret = vram_slot;
	vram_slot += len;
	return ret;
}

void enemy_vram_reset(void)
{
	vram_slot = ENEMY_VRAM_START;
}
