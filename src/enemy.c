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
#include "pilla.h"
#include "hedgedog.h"
#include "shoot.h"
#include "laser.h"
#include "killzam.h"
#include "flargy.h"
#include "plant.h"
#include "tossmuffin.h"
#include "teleporter.h"
#include "magibear.h"
#include "lava.h"
#include "cow.h"
#include "elevator.h"
#include "elevator_stop.h"
#include "bounds.h"
#include "lavaanim.h"
#include "spooko.h"
#include "boss1.h"
#include "wip.h"
#include "bgscrolly.h"
#include "fakecube.h"

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

static u16 hurt_time;

static u8 powerup_spawn_chance;

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

	// Spawn powerups with a likelihood of 5/8
	switch (powerup_spawn_chance & 0x07)
	{
		case 0:
		case 2:
		case 3:
		case 5:
		case 6:
			powerup_spawn(e->head.x, e->head.y, 1 + (system_osc & (e->head.powerup_range)), 0);
		default:
			break;
	}

	powerup_spawn_chance++;
	playsound(SFX_ENEMY_EXPLODE);
}

void enemy_dma_tiles(void)
{
	// Enemy palette
	switch (state.current_room->sprite_palette)
	{
		default:
		case 0:
			VDP_doCRamDMA((u32)pal_enemy, ENEMY_PALNUM * 32, 16);
			break;
		case 1:
			VDP_doCRamDMA((u32)pal_enemy_cow, ENEMY_PALNUM * 32, 16);
			break;
		case 2:
			VDP_doCRamDMA((u32)pal_enemy_elevator, ENEMY_PALNUM * 32, 16);
			break;
	}
}

void enemy_init(void)
{
	u16 i = ENEMIES_NUM;
	u16 j;
	hurt_time = system_ntsc ? 24 : 20;
	en_unload_metagrub();
	en_unload_flip();
	en_unload_boingo();
	en_unload_item();
	en_unload_gaxter1();
	en_unload_gaxter2();
	en_unload_buggo();
	en_unload_dancyflower();
	en_unload_jraff();
	en_unload_pilla();
	en_unload_hedgedog();
	en_unload_shoot();
	en_unload_laser();
	en_unload_killzam();
	en_unload_flargy();
	en_unload_plant();
	en_unload_tossmuffin();
	en_unload_teleporter();
	en_unload_magibear();
	en_unload_lava();
	en_unload_cow();
	en_unload_elevator();
	en_unload_elevator_stop();
	en_unload_lavaanim();
	en_unload_spooko();
	en_unload_boss1();
	en_unload_wip();
	en_unload_bounds();
	en_unload_bgscrolly();
	en_unload_fakecube();
	enemy_vram_reset();

	powerup_spawn_chance = GET_HVCOUNTER;
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
		e->head.tall = 0;
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
			e->head.x > state.cam_x + STATE_SC_W + ENEMY_ACTIVE_DISTANCE ||
			e->head.y < state.cam_y - ENEMY_ACTIVE_DISTANCE * 2 ||
			e->head.y >= state.cam_y + STATE_SC_H + ENEMY_ACTIVE_DISTANCE)
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

		s16 randx, randy;
		randx = (GET_HVCOUNTER % 4) - 2;
		randy = ((GET_HVCOUNTER >> 1) % 4) - 2;

		if (e->head.hurt_cnt > 0)
		{
			ex = ex + randx;
			ey = ey + randy;
		}

		// Check bounds
		if (ex > -64 && ex < 384 && ey > -64 && ey < 256)
		{
			if (e->head.attr[0])
			{
				sprite_put(ex,ey, e->head.size[0], e->head.attr[0]);
			}
			if (e->head.tall)
			{
				while (ey > 0)
				{
					ey -= ((e->head.size[0] + 1) & 0x0003) << 3;
					sprite_put(ex,ey, e->head.size[0], e->head.attr[0]);
				}
			}
			else if (e->head.attr[1])
			{
				s16 ex = e->head.x + e->head.xoff[1] - state.cam_x;
				s16 ey = e->head.y + e->head.yoff[1] - state.cam_y;
				if (e->head.hurt_cnt > 0)
				{
					ex = ex + randx;
					ey = ey + randy;
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
		e->head.hurt_cnt = hurt_time;
		playsound(SFX_ENEMY_STRIKE);
	}
}

// The typical and default "bounce cube off enemy, damage enemy" reaction.
// Most enemies will use this.
void enemy_cube_response(en_generic *e, cube *c)
{
	if (e->head.hurt_cnt != 0)
	{
		return;
	}
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
	if (c->type == CUBE_GREEN)
	{
		c->state = CUBE_STATE_AIR;
		if (c->y < e->head.y)
		{
			c->dy = -1 * (c->dy >> 1);
		}
		else
		{
			c->dy = -cube_on_cube_dy;
			c->y -= 2;
		}
		if (c->dx == FZERO)
		{
			c->dx = (GET_HVCOUNTER % 2) ? 1 : -1;
		}
		else
		{
			if (c->x < e->head.x && c->dx > FIX16(0.0))
			{
				c->dx = c->dx * -1;
			}
			else if (c->x > e->head.x && c->dx < FIX16(0.0))
			{
				c->dx = c->dx * -1;
			}
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

u16 enemy_touching_enemy(en_generic *a, en_generic *b)
{
	return (a->head.x + a->head.width > b->head.x - b->head.width &&
	        a->head.x - a->head.width < b->head.x + b->head.width &&
	        a->head.y > b->head.y - b->head.height &&
	        a->head.y - a->head.height < b->head.y);
}

en_generic *enemy_place(u16 x, u16 y, u16 type, u16 data)
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
					en_init_boingo((en_boingo *)e, data);
					break;
				case ENEMY_ITEM:
					en_init_item((en_item *)e, data);
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
				case ENEMY_PILLA:
					en_init_pilla((en_pilla *)e, data);
					break;
				case ENEMY_HEDGEDOG:
					en_init_hedgedog((en_hedgedog *)e);
					break;
				case ENEMY_SHOOT:
					en_init_shoot((en_shoot *)e);
					break;
				case ENEMY_LASER:
					en_init_laser((en_laser *)e);
					break;
				case ENEMY_KILLZAM:
					en_init_killzam((en_killzam *)e);
					break;
				case ENEMY_FLARGY:
					en_init_flargy((en_flargy *)e);
					break;
				case ENEMY_PLANT:
					en_init_plant((en_plant *)e);
					break;
				case ENEMY_TOSSMUFFIN:
					en_init_tossmuffin((en_tossmuffin *)e);
					break;
				case ENEMY_TELEPORTER:
					en_init_teleporter((en_teleporter *)e, data);
					break;
				case ENEMY_MAGIBEAR:
					en_init_magibear((en_magibear *)e);
					break;
				case ENEMY_LAVA:
					en_init_lava((en_lava *)e);
					break;
				case ENEMY_COW:
					en_init_cow((en_cow *)e);
					break;

				case ENEMY_ELEVATOR:
					en_init_elevator((en_elevator *)e);
					break;
				case ENEMY_ELEVATOR_STOP:
					en_init_elevator_stop((en_elevator_stop *)e, data);
					break;

				case ENEMY_LAVAANIM:
					en_init_lavaanim((en_lavaanim *)e);
					break;

				case ENEMY_SPOOKO:
					en_init_spooko((en_spooko *)e, data);
					break;
				case ENEMY_BOSS1:
					en_init_boss1((en_boss1 *)e);
					break;

				case ENEMY_BOUNDS:
					en_init_bounds((en_bounds *)e);
					break;

				case ENEMY_WIP:
					en_init_wip((en_wip *)e);
					break;

				case ENEMY_BGSCROLLY:
					en_init_bgscrolly((en_bgscrolly *)e, data);
					break;

				case ENEMY_FAKECUBE:
					en_init_fakecube((en_fakecube *)e);
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
			if (pl.tele_in_cnt > 0 || pl.tele_out_cnt > 0)
			{
				continue;
			}
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
