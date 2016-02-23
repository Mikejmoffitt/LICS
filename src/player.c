#include "player.h"
#include "sfx.h"
#include "gfx.h"
#include "pal.h"
#include "vramslots.h"
#include "sprites.h"
#include "music.h"
#include "state.h"
#include "save.h"
#include "system.h"
#include "cubes.h"
#include "particles.h"

static u32 lyle_dma_src;
static u16 lyle_dma_dest;
static u16 lyle_dma_len;
static u16 cp_restore_cnt;

static void player_set_pal(void);
static void player_eval_control_en();
static void player_cp(void);
static void player_accel(void);
static void player_eval_grounded(void);
static void player_jump(void);
static void player_toss_cubes(void);
static void player_kick_cube(cube *c);
static void player_lift_cubes(void);
static void player_special_counters(void);
static void player_bg_horizontal_collision(void);
static void player_bg_vertical_collision(void);
static void player_cube_horizontal_collision(cube *c);
static void player_cube_vertical_collision(cube *c);
static void player_cube_eval_standing(cube *c);
static void player_cube_collision(void);
static void player_move(void);
static void player_calc_anim(void);
static void player_entrance_coll(void);

player pl;

// Constants manager, to make transition from NTSC --> PAL a lot easier
static player_k plk;
static void player_init_constants(void)
{
	plk.dx_max = system_ntsc ? FIX16(1.54) : FIX16(1.8);
	plk.dy_max = system_ntsc ? FIX16(6.67) : FIX16(8.0);
	plk.x_accel = system_ntsc ? FIX16(0.125) : FIX16(0.17);
	plk.y_accel = system_ntsc ? FIX16(0.21) : FIX16(0.276);
	plk.y_accel_weak = system_ntsc ? FIX16(0.10) : FIX16(0.156);
	plk.jump_dy = system_ntsc ? FIX16(-2.94) : FIX16(-3.58);
	plk.ceiling_dy = system_ntsc ? FIX16(-0.42) : FIX16(-0.5);
	plk.hurt_dx = system_ntsc ? FIX16(-1.92) : FIX16(-2.3);

	plk.throw_anim_len = system_ntsc ? 10 : 8;
	plk.kick_anim_len = system_ntsc ? 10 : 8;
	plk.cubejump_anim_len = system_ntsc ? 24 : 20;
	plk.lift_time = system_ntsc ? 18 : 15;
	plk.cp_restore_period = system_ntsc ? 300 : 250;
	plk.cp_restore_period_fast = system_ntsc ? 150 : 125;
	plk.cp_spawn_fast = system_ntsc ? 36 : 30;
	plk.cp_spawn_slow = system_ntsc ? 72 : 60;

	plk.hurt_time = system_ntsc ? 35 : 29;
	plk.hurt_timeout = system_ntsc ? 24 : 20;
	plk.invuln_time = system_ntsc ? 95 : 79;
	plk.cube_fx = system_ntsc ? 6 : 5;
	plk.animspeed = system_ntsc ? 6 : 5;

	plk.tele_anim = system_ntsc ? 75 : 62;

	// Cool cheat codes
	if (buttons & BUTTON_Y)
	{
		plk.dx_max = system_ntsc ? FIX16(5.0) : FIX16(6.0);
		plk.x_accel = system_ntsc ? FIX16(0.5) : FIX16(0.6);
		plk.jump_dy = system_ntsc ? FIX16(-5.0) : FIX16(-6.0);
		plk.animspeed = 3;

		plk.throw_anim_len = system_ntsc ? 6 : 5;
		plk.kick_anim_len = system_ntsc ? 6 : 5;
		plk.cubejump_anim_len = system_ntsc ? 6 : 5;
		plk.lift_time = system_ntsc ? 6 : 5;
		plk.cp_restore_period = system_ntsc ? 30 : 25;
		plk.cp_restore_period_fast = system_ntsc ? 15 : 12;
		plk.cp_spawn_fast = system_ntsc ? 6 : 5;
		plk.cp_spawn_slow = system_ntsc ? 12 : 10;
	}
}

static void player_set_pal(void)
{
	VDP_doCRamDMA((u32)pal_lyle, 32 * PLAYER_PALNUM, 16);
}

static inline void player_eval_control_en(void)
{
	if (pl.hurt_cnt > 0 || pl.hp == 0 || pl.tele_in_cnt > 0 || pl.tele_out_cnt > 0)
	{
		pl.control_disabled = 1;
	}
	else
	{
		pl.control_disabled = 0;
	}
}

void player_init(void)
{
	player_init_constants();
	pl.dx = FZERO;
	pl.dy = FZERO;
	pl.x = FZERO32;
	pl.y = FZERO32;
	pl.px = 0;
	pl.py = 0;
	pl.direction = PLAYER_RIGHT;
	
	pl.hp = 5;
	pl.cp = 30;
	pl.tele_in_cnt = 0;
	pl.tele_out_cnt = 0;
	player_init_soft();
}

void player_init_soft(void)
{
	pl.grounded = 0;
	
	pl.anim_cnt = 0;
	pl.anim_frame = 0;
	pl.holding_cube = 0;
	pl.throw_cnt = 0;
	pl.throwdown_cnt = 0;
	pl.kick_cnt = 0;
	pl.lift_cnt = 0;
	pl.cp_cnt = 0;
	pl.hurt_cnt = 0;
	pl.invuln_cnt = 0;
	pl.action_cnt = 0;
	pl.control_disabled = 0;
	pl.cubejump_disable = 0;
	pl.on_cube = NULL;
	player_set_pal();
}

void player_dma_setup(void)
{
	u16 num = pl.anim_frame;
	u16 offset;

	// Most sprites are just six tiles - 3x2 or 2x3
	if (num < LYLE_3x3_CUTOFF)
	{
		offset = num * 6;
	}
	// All 3x3 sprites are stored later in Lyle's graphics set
	else
	{
		num -= 0x14;
		offset = 120 + (9 * num);
	}
	
	lyle_dma_len = (offset >= 120) ? (9 * 16) : (6 * 16);
	lyle_dma_src = (u32)(gfx_lyle + (32 * offset));
	lyle_dma_dest = PLAYER_VRAM_SLOT * 32;

}

void player_dma(void)
{
	VDP_doVRamDMA((u32)lyle_dma_src,lyle_dma_dest,lyle_dma_len);
}

static void player_cp(void)
{
	// We don't have this power, don't bother
	if (!sram.have_phantom)
	{
		pl.cp_cnt = 0;
		return;
	}
	cp_restore_cnt++;
	// Manage periodic restoration of CP
	if (cp_restore_cnt >= plk.cp_restore_period)
	{
		cp_restore_cnt = 0;
		if (pl.cp != PLAYER_MAX_CP)
		{
			pl.cp++;
		}
	}
	// In the middle of doing something that voids this ability
	if (pl.lift_cnt || pl.hurt_cnt || pl.action_cnt || pl.control_disabled)
	{
		return;
	}
	
	u16 cube_price = (sram.have_cheap_phantom ? PLAYER_CP_SPAWN_CHEAP : PLAYER_CP_SPAWN_PRICE);
	// Spawning of the cube; are we not holding one, and can afford one?
	if (!pl.holding_cube && pl.cp >= cube_price)
	{
		if (buttons & BUTTON_B)
		{
			pl.cp_cnt++;
			if (pl.cp_cnt == plk.cube_fx + 1)
			{
				playsound(SFX_CUBESPAWN);
			}
			else if (pl.cp_cnt == plk.cp_spawn_fast + 1)
			{
				playsound(SFX_CUBESPAWN);
			}
		}
		else
		{
			if (pl.cp_cnt > plk.cube_fx)
			{
				stopsound();
			}
			pl.cp_cnt = 0;
		}
		u16 cube_spawn_period = (sram.have_fast_phantom ? plk.cp_spawn_fast : plk.cp_spawn_slow);
		if (pl.cp_cnt >= cube_spawn_period)
		{
			pl.holding_cube = CUBE_PHANTOM;
			pl.cp_cnt = 0;
			pl.cp -= cube_price;
		}
	}
	// Sparkling effect when cube is starting to form
	if (pl.cp_cnt > plk.cube_fx && system_osc % 2)
	{
		particle_spawn(pl.px, pl.py - 32, PARTICLE_TYPE_SPARKLE);

	}
}

static inline void player_walking_sound(void)
{
	if (pl.anim_cnt == (plk.animspeed * 1) - 1)
	{
		playsound(SFX_WALK1);
	}
	else if (pl.anim_cnt == (plk.animspeed * 3) - 1)
	{
		playsound(SFX_WALK2);
	}
}

static inline void player_hurt_decel(void)
{
	if (pl.dx > FZERO)
	{
		pl.dx = fix16Sub(pl.dx,plk.x_accel >> 1);	
		// Don't accel into the other direction
		if (fix16ToInt(pl.dx) < FZERO)
		{
			pl.dx = FZERO;
		}
	}
	else
	{
		pl.dx = fix16Add(pl.dx,plk.x_accel >> 1);				
		// Don't accel into the other direction
		if (pl.dx > FZERO)
		{
			pl.dx = FZERO;
		}
	}
}

static inline void player_accel(void)
{
	if (pl.control_disabled || pl.lift_cnt)
	{
		player_hurt_decel();
		return;
	}

	// decel
	if (!(buttons & (BUTTON_RIGHT | BUTTON_LEFT)))
	{
		if (pl.dx > FZERO)
		{
			pl.dx = fix16Sub(pl.dx,plk.x_accel);	
			// Don't accel into the other direction
			if (fix16ToInt(pl.dx) < FZERO)
			{
				pl.dx = FZERO;
			}
		}
		else
		{
			pl.dx = fix16Add(pl.dx,plk.x_accel);				
			// Don't accel into the other direction
			if (pl.dx > FZERO)
			{
				pl.dx = FZERO;
			}
		}
	}

	// walking right and left
	if (buttons & BUTTON_RIGHT)
	{
		pl.dx = fix16Add(pl.dx,plk.x_accel);
		pl.direction = PLAYER_RIGHT;
		player_walking_sound();
	}
	else if (buttons & BUTTON_LEFT)
	{
		pl.dx = fix16Sub(pl.dx,plk.x_accel);
		pl.direction = PLAYER_LEFT;
		player_walking_sound();
	}
	// If dy/dx is almost zero, make it zero
	if (pl.dx > FIX16(-0.1) && pl.dx < FIX16(0.1) && !(buttons & (BUTTON_RIGHT | BUTTON_LEFT)))
	{
		pl.dx = FZERO;
	}

	// Limit top speed
	if (pl.dx > plk.dx_max)
	{
		pl.dx = plk.dx_max;
	}
	else if (pl.dx < -plk.dx_max)
	{
		pl.dx = -plk.dx_max;
	}
}

static void player_eval_grounded(void)
{
	if (pl.dy < FZERO)
	{
		pl.grounded = 0;
		return;
	}
	u16 px = fix32ToInt(pl.x);
	u16 py = fix32ToInt(pl.y);
	// "Is the tile one pixel below me solid?"
	if ((map_collision(px + PLAYER_CHK_RIGHT - 1, py + PLAYER_CHK_BOTTOM + 1)) || 
		(map_collision(px + PLAYER_CHK_LEFT + 1, py + PLAYER_CHK_BOTTOM + 1)))
	{
		pl.grounded = 1;
	}
	else
	{
		pl.grounded = 0;
	}
}

void player_do_jump(void)
{
	pl.dy = plk.jump_dy;
}

static void player_jump(void)
{
	// Very small timeout to prevent lyle from cube-jumping so the occasional
	// normal jump doesn't accidentally become a cube jump off a ledge
	if (pl.grounded || pl.on_cube)
	{
		pl.cubejump_disable = 2;
	}

	if (pl.lift_cnt || pl.control_disabled)
	{
		return;
	}

	// C key pressed, negative edge (1 -> 0)
	if ((buttons & BUTTON_C) && !(buttons_prev & BUTTON_C))
	{
		// Normal jump off the ground
		if (pl.grounded || pl.on_cube)
		{
			playsound(SFX_JUMP);
			goto do_jump;
		}
		// Cube jumping, if we have the ability
		else if (pl.holding_cube && sram.have_jump && !pl.cubejump_disable)
		{
			pl.throwdown_cnt = plk.cubejump_anim_len;

			// If the wall behind the player is solid, align the cube's X to it
			// so it doesn't fizzle immediately on throw. Not sure exactly what
			// made this happen in the original but this is mimicing what I see
			// when I play the game.
			u16 cx = fix32ToInt(pl.x);
			u16 cy = fix32ToInt(pl.y) - 12;
			u16 px = fix32ToInt(pl.x);
			u16 back_x = (pl.direction == PLAYER_LEFT) ? 
				(px + PLAYER_CHK_RIGHT + 4) : 
				(px + PLAYER_CHK_LEFT - 4);

			// Align the cube
			if (map_collision(back_x, fix32ToInt(pl.y) + PLAYER_CHK_BOTTOM))
			{
				cx = ((cx + 4) / 8) * 8;
			}
			
			// Generate a cube to throw
			cube_spawn(cx,
				cy,
				pl.holding_cube,
				CUBE_STATE_AIR,
				0, system_ntsc ? FIX16(3.33) : FIX16(4));

			pl.holding_cube = 0;
			// Generate cube of right type, throw it down
			playsound(SFX_CUBETOSS);
			goto do_jump;
		}
	}

	return;
do_jump:
	pl.dy = plk.jump_dy;
	// Play SFX
	return;
}

static void player_toss_cubes(void)
{
	if (pl.control_disabled)
	{
		return;
	}
	if (pl.holding_cube && (buttons & BUTTON_B) && (!(buttons_prev & BUTTON_B)))
	{
		s16 cdx;
		fix16 cdy;
		// Holding down; do a short toss
		if (buttons & (BUTTON_DOWN) && pl.grounded)
		{
			cdx = (pl.direction == PLAYER_RIGHT) ? 1 : -1;
			cdy = system_ntsc ? FIX16(-1.77) : FIX16(-2.0);
		}
		// Holding up; toss straight up
		else if (buttons & BUTTON_UP)
		{
			cdx = 0;
			cdy = system_ntsc ? FIX16(-4.2) : FIX16(-5.0);
		}
		// Throw with direction right
		else if (buttons & BUTTON_RIGHT && pl.direction == PLAYER_RIGHT)
		{
			cdx = 4;
			cdy = system_ntsc ? FIX16(-0.84) : FIX16(-1.0);
		}
		// Left
		else if (buttons & BUTTON_LEFT && pl.direction == PLAYER_LEFT)
		{
			cdx = -4;
			cdy = system_ntsc ? FIX16(-0.84) : FIX16(-1.0);
		}
		else
		{
			cdx = (pl.direction == PLAYER_RIGHT) ? 2 : -2;
			cdy = system_ntsc ? FIX16(-1.77) : FIX16(-2.0);
		}

		u16 cy = fix32ToInt(pl.y) - 23;
		u16 cx = fix32ToInt(pl.x);

		// Check if the new cube would be in a wall, and push it down if 
		// necessary by at most two tiles
		if (map_collision(cx,cy + CUBE_TOP))
		{
			cy += 8;
		}
		if (map_collision(cx,cy + CUBE_TOP))
		{
			cy += 8;
		}
		
		// Generate a cube to throw
		cube_spawn(cx, cy,
			pl.holding_cube,
			CUBE_STATE_AIR,
			cdx, cdy);

		playsound(SFX_CUBETOSS);

		// Player response
		pl.holding_cube = 0;
		pl.action_cnt = PLAYER_ACTION_THROW;
		pl.throw_cnt = plk.throw_anim_len;
	}
}

static void player_kick_cube(cube *c)
{
	if (c->state != CUBE_STATE_IDLE || !(pl.grounded || pl.on_cube) || 
		pl.action_cnt || !sram.have_kick)
	{
		return;
	}
	u16 py = fix32ToInt(pl.y);
	u16 px = fix32ToInt(pl.x);
	// check we are within appropriate Y bounds
	if ((buttons & BUTTON_B) && !(buttons_prev & BUTTON_B) &&
		c->y + CUBE_TOP <= py + PLAYER_CHK_BOTTOM - 1 && 
		c->y + CUBE_BOTTOM >= py + PLAYER_CHK_TOP + 8)
	{
		// Just touching the left side of it
		if (px == (c->x + CUBE_LEFT) - PLAYER_CHK_RIGHT - 1 &&
			pl.direction == PLAYER_RIGHT)
		{
			pl.kick_cnt = plk.kick_anim_len;
			c->state = CUBE_STATE_KICKED;
			c->dx = CUBE_KICK_DX;
			pl.action_cnt = PLAYER_ACTION_LIFT;
			playsound(SFX_CUBEBOUNCE);
			c->direction = pl.direction;
		}
		else if (px == (c->x + CUBE_RIGHT) - PLAYER_CHK_LEFT + 1 &&
			pl.direction == PLAYER_LEFT)
		{
			pl.kick_cnt = plk.kick_anim_len;
			c->state = CUBE_STATE_KICKED;
			c->dx = (CUBE_KICK_DX * -1);
			pl.action_cnt = PLAYER_ACTION_LIFT;
			playsound(SFX_CUBEBOUNCE);
			c->direction = pl.direction;
		}
	}
}

static void player_lift_cubes(void)
{
	if (!sram.have_lift)
	{
		return;
	}	// In the middle of doing something that voids this ability
	if (pl.hurt_cnt || pl.action_cnt || pl.control_disabled)
	{
		return;
	}
	if (pl.on_cube && pl.lift_cnt == 0 && buttons & BUTTON_B && !(buttons_prev & BUTTON_B))
	{	
		pl.lift_cnt = plk.lift_time + 1;
		pl.action_cnt = PLAYER_ACTION_LIFT;
		pl.dx = FZERO;
	}
	if (pl.lift_cnt == 1 && pl.on_cube)
	{
		cube *c = (cube *)pl.on_cube;
		pl.holding_cube = c->type;
		cube_delete(c);

		// Re-implement the MMF version bug where you can jump while lifting
		if (buttons & BUTTON_C)
		{
			pl.dy = plk.jump_dy;
		}
		pl.action_cnt = PLAYER_ACTION_LIFT;
	}
}

static void player_special_counters(void)
{
	if (pl.throwdown_cnt)
	{
		pl.throwdown_cnt--;
	}
	if (pl.throw_cnt)
	{
		pl.throw_cnt--;
	}
	if (pl.kick_cnt)
	{
		pl.kick_cnt--;
	}
	if (pl.lift_cnt)
	{
		pl.lift_cnt--;
	}
	if (pl.hurt_cnt)
	{
		pl.hurt_cnt--;
	}
	if (pl.invuln_cnt)
	{
		pl.invuln_cnt--;
	}
	if (pl.action_cnt)
	{
		pl.action_cnt--;
	}
	if (pl.cubejump_disable)
	{
		pl.cubejump_disable--;
	}
}

static void player_bg_horizontal_collision(void)
{
	u16 py = fix32ToInt(pl.y);
	u16 px = fix32ToInt(pl.x);

	// Horizontal collision
	if (pl.dx > FZERO)
	{
		if ((map_collision(px + PLAYER_CHK_RIGHT, py + PLAYER_CHK_TOP)) ||
			(map_collision(px + PLAYER_CHK_RIGHT, py + PLAYER_CHK_FOOT)) ||
			(map_collision(px + PLAYER_CHK_RIGHT, py + PLAYER_CHK_MID))) 
		{
			px = 8 * (px / 8);
			px += 3;
			pl.x = intToFix32(px);
			pl.dx = FZERO;
		}
	}
	else if (pl.dx < FZERO)
	{
		if ((map_collision(px + PLAYER_CHK_LEFT, py + PLAYER_CHK_TOP)) ||
			(map_collision(px + PLAYER_CHK_LEFT, py + PLAYER_CHK_FOOT) ) ||
			(map_collision(px + PLAYER_CHK_LEFT, py + PLAYER_CHK_MID) )) 
		{
			px = 8 * (px / 8);
			px += 5;
			pl.x = intToFix32(px);
			pl.dx = FZERO;
		}
	}
}

static void player_bg_vertical_collision(void)
{
	u16 py = fix32ToInt(pl.y);
	u16 px = fix32ToInt(pl.x);
	px -= fix16ToInt(pl.dx);
	// Vertical collision
	// "Am I now stuck with my feet in the ground?"
	if (pl.dy > FZERO)
	{
		if ((map_collision(px + PLAYER_CHK_RIGHT - 1, py + PLAYER_CHK_BOTTOM)) ||
			(map_collision(px + PLAYER_CHK_LEFT + 1, py + PLAYER_CHK_BOTTOM)))
		{
			// Snap to nearest 8px boundary
			py = 8 * (py / 8) - 1;
			pl.y = intToFix32(py);
			pl.dy = FZERO;
			int i = 8;
			while (i--)
			{
				// Are we still stuck? Move up 8px since the snap didn't quite work right.
				if ((map_collision(px + PLAYER_CHK_RIGHT - 1, py + PLAYER_CHK_BOTTOM)) ||
					(map_collision(px + PLAYER_CHK_LEFT + 1, py + PLAYER_CHK_BOTTOM)))
				{
					pl.y = fix32Sub(pl.y,intToFix32(1));
				}
				else
				{
					break;
				}
			}
			player_eval_grounded();
			// Somehow we aren't grounded now - move down one tile.
			if (!pl.grounded)
			{
				pl.y = fix32Add(pl.y,intToFix32(8));
				player_eval_grounded();
			}
		}
	}
	else if (pl.dy < FZERO)
	{
		// "Am I now stuck with my head in the ceiling?"
		if ((map_collision(px + PLAYER_CHK_RIGHT - 1, py + PLAYER_CHK_TOP - 1)) ||
			(map_collision(px + PLAYER_CHK_LEFT + 1, py + PLAYER_CHK_TOP - 1)))
		{
			// Snap to nearest 8px boundary, with head room accounted for
			py = 8 * ((py + 4) / 8) + 3;
			pl.y = intToFix32(py);
			if (pl.dy < plk.ceiling_dy)
			{
				pl.dy = plk.ceiling_dy;
			}

		}
	}
}

// Interactions with stationary cubes

static void player_cube_horizontal_collision(cube *c)
{
	if (c->state == CUBE_STATE_AIR || c->type == CUBE_SPAWNER)
	{
		return;
	}
	u16 py = fix32ToInt(pl.y);
	u16 px = fix32ToInt(pl.x);

	// check we are within appropriate Y bounds
	if (c->y + CUBE_TOP <= py + PLAYER_CHK_BOTTOM - 1 && 
		c->y + CUBE_BOTTOM >= py + PLAYER_CHK_TOP + 1)
	{
		// Horizontal collision
		if (pl.dx > FZERO)
		{
			if (c->x + CUBE_LEFT >= px) 
			{
				px = (c->x + CUBE_LEFT) - PLAYER_CHK_RIGHT - 1;
				pl.x = intToFix32(px);
				pl.dx = FZERO;
			}
		}
		else if (pl.dx < FZERO)
		{
			if (c->x + CUBE_RIGHT <= px) 
			{
				px = (c->x + CUBE_RIGHT) - PLAYER_CHK_LEFT + 1;
				pl.x = intToFix32(px);
				pl.dx = FZERO;
			}
		}
	}
}

static void player_cube_vertical_collision(cube *c)
{
	if (c->type == CUBE_SPAWNER)
	{
		return;
	}
	u16 py = fix32ToInt(pl.y);
	u16 px = fix32ToInt(pl.x);
	px -= fix16ToInt(pl.dx);

	// appropriate X bounds
	if (c->x + CUBE_LEFT <= px + PLAYER_CHK_RIGHT - 1 && 
		c->x + CUBE_RIGHT >= px + PLAYER_CHK_LEFT + 1)
	{
		// Vertical collision
		// "Am I now stuck with my feet in a cube?"
		if (pl.dy > FZERO)
		{
			if (py + PLAYER_CHK_BOTTOM >= c->y + CUBE_TOP && py + PLAYER_CHK_TOP < c->y + CUBE_BOTTOM)
			{
				// Snap to the cube
				py = c->y + CUBE_TOP - PLAYER_CHK_BOTTOM - 1;
				pl.y = intToFix32(py);
				pl.dy = FZERO;
			}
		}
		else if (pl.dy < FZERO)
		{
			// "Am I now stuck with my head in the cube?"
			if (py + PLAYER_CHK_TOP< c->y + CUBE_BOTTOM && py + PLAYER_CHK_BOTTOM > c->y + CUBE_BOTTOM)
			{
				py = c->y + CUBE_BOTTOM - PLAYER_CHK_TOP + 1;
				pl.y = intToFix32(py);
				if (pl.dy < plk.ceiling_dy)
				{
					pl.dy = plk.ceiling_dy;
				}
			}
		}
	}
}

static void player_cube_eval_standing(cube *c)
{
	u16 py = fix32ToInt(pl.y);
	u16 px = fix32ToInt(pl.x);
	if (c->x + CUBE_LEFT <= px + PLAYER_CHK_RIGHT - 1 && 
		c->x + CUBE_RIGHT >= px + PLAYER_CHK_LEFT + 1 && 
		py + PLAYER_CHK_BOTTOM + 1>= c->y + CUBE_TOP && 
		py + PLAYER_CHK_TOP < c->y + CUBE_TOP &&
		c->type != CUBE_SPAWNER)
	{
		// Already standing on a cube? Determine which cube is closer and
		// use that cube as the "standing-on cube" reference for lifting.
		if (pl.on_cube)
		{
			// Get X distance difference for new and old cubes
			cube *oc = (cube *)(pl.on_cube);
			s16 orig_diff_x = px - oc->x;
			s16 new_diff_x = px - c->x;
			// Make absolute values
			if (orig_diff_x < 0)
			{
				orig_diff_x = orig_diff_x * -1;
			}
			if (new_diff_x < 0)
			{
				new_diff_x = new_diff_x * -1;
			}
			// Our new cube is cloesr. Use it.
			if (new_diff_x < orig_diff_x)
			{
				pl.on_cube = (void *)c;
			}
			// Otherwise, the previous remains.
		}
		else
		{
			pl.on_cube = (void *)c;
		}
	}
}

static inline void player_cube_collision(void)
{
	u16 px = (u16)fix32ToInt(pl.x);
	u16 py = (u16)fix32ToInt(pl.y);
	int i = CUBES_NUM;
	while (i--)
	{
		cube *c = &cubes[i];
		// Don't bother with collision checks if it's an inactive cube
		if (c->state == CUBE_STATE_INACTIVE)
		{
			continue;
		}
		// Cube overlaps player boundaries, it's a collision
		else if (
			c->x + CUBE_LEFT <= px + PLAYER_CHK_RIGHT + 1 && 
			c->x + CUBE_RIGHT >= px + PLAYER_CHK_LEFT - 1 && 
			c->y + CUBE_TOP <= py + PLAYER_CHK_BOTTOM + 1 && 
			c->y + CUBE_BOTTOM >= py + PLAYER_CHK_TOP - 1 )
		{
			if (c->type == CUBE_SPAWNER)
			{
				cube_restrict_spawn(c);
			}
			else if (c->state == CUBE_STATE_IDLE)
			{
				player_cube_vertical_collision(c);
				player_cube_horizontal_collision(c);
				player_cube_eval_standing(c);
				player_kick_cube(c);
			}
			else if (c->state == CUBE_STATE_AIR && pl.throw_cnt == 0 && pl.kick_cnt == 0 && pl.throwdown_cnt == 0)
			{
				if (pl.hurt_cnt < plk.hurt_time - plk.hurt_timeout)
				{
					player_get_bounced();
					player_get_hurt();
				}

			}
		}

		// Check for cube bouncing on the fake cube player is holding
		if (pl.holding_cube)
		{
			if (c->state == CUBE_STATE_AIR &&
				c->x + CUBE_LEFT <= px + PLAYER_CHK_RIGHT + 1 && 
				c->x + CUBE_RIGHT >= px + PLAYER_CHK_LEFT - 1 && 
				c->y + CUBE_TOP <= py + PLAYER_CHK_BOTTOM - 24 && 
				c->y + CUBE_BOTTOM >= py + PLAYER_CHK_TOP - 15 )
			{
				if (c->dx == FZERO)
				{
					c->dx = GET_HVCOUNTER % 2 ? 1 : -1;
				}
				c->dy = cube_on_cube_dy;
				playsound(SFX_CUBEBOUNCE);
			}
		}
	}
}

static inline void player_move(void)
{
	// Do movement	
	pl.x = fix32Add(pl.x,fix16ToFix32(pl.dx));
	pl.y = fix32Add(pl.y,fix16ToFix32(pl.dy));

	pl.on_cube = NULL;
	player_bg_vertical_collision();
	player_bg_horizontal_collision();
	player_eval_grounded();
	player_cube_collision();

	// In the air, gravity is affected by the player holding jump or not
	if (!pl.grounded)
	{
		// The jump holding only affects gravity on the way up, though
		if (((buttons & BUTTON_C) && !pl.control_disabled) && pl.dy < FZERO)
		{
			pl.dy = fix16Add(pl.dy,plk.y_accel_weak);
		}
		else
		{
			pl.dy = fix16Add(pl.dy,plk.y_accel);
		}
		if (pl.dy > plk.dy_max)
		{
			pl.dy = plk.dy_max;
		}
	}

	pl.px = fix32ToInt(pl.x);
	pl.py = fix32ToInt(pl.y);
}

static inline void player_calc_anim(void)
{
	if (pl.grounded || pl.on_cube)
	{
		pl.anim_cnt++;
		if (pl.anim_cnt == plk.animspeed * 4)
		{
			pl.anim_cnt = 0;
		}
	}
	else
	{
		pl.anim_cnt = 0;
	}
	if (pl.invuln_cnt && (system_osc % 2))
	{
		return;
	}
	if (pl.hp == 0)
	{
		pl.anim_frame = 0x0F;
	}
	else
	{
		if (pl.tele_out_cnt > 0 || pl.tele_in_cnt > 0)
		{
			pl.anim_frame = 0x00;
			return;
		}
		if (pl.throw_cnt > 0)
		{
			pl.anim_frame = 0x16;
			return;
		}
		else if (pl.throwdown_cnt > 0)
		{
			pl.anim_frame = 0x07;
			return;
		}
		else if (pl.kick_cnt > 0)
		{
			pl.anim_frame = 0x17;
			return;
		}
		else if (pl.lift_cnt > 0)
		{
			pl.anim_frame = 0x05;
			return;
		}
		else if (pl.hurt_cnt > 0)
		{
			pl.anim_frame = 0x06;	
			return;
		}
		
		if (pl.grounded || pl.on_cube)
		{
			if (!(buttons & (BUTTON_LEFT | BUTTON_RIGHT))) // Standing
			{
				pl.anim_frame = 0x00;
			}
			else // Walking cycle
			{
				if (pl.anim_cnt < plk.animspeed)
				{
					pl.anim_frame = 0x02;
				}
				else if (pl.anim_cnt < (plk.animspeed * 2))
				{
					pl.anim_frame = 0x03;
				}
				else if (pl.anim_cnt < (plk.animspeed * 3))
				{
					pl.anim_frame = 0x02;
				}
				else
				{
					pl.anim_frame = 0x01;
				}
			}
		}
		else
		{
			pl.anim_frame = 0x04;
		}
		if ((pl.cp_cnt > plk.cube_fx) || pl.holding_cube)
		{
			// Offset to arms-up version
			pl.anim_frame += 0x08;
		}
	}
}

u16 player_collision(s16 x1, s16 x2, s16 y1, s16 y2)
{
	if (x1 > pl.px + PLAYER_CHK_RIGHT + 1 ||
	x2 < pl.px + PLAYER_CHK_LEFT - 1 ||
	y1 > pl.py + PLAYER_CHK_BOTTOM + 1 ||
	y2 < pl.py + PLAYER_CHK_TOP - 1 )
	{
		return 0;
	}
	return 1;
}

void player_draw(void)
{
	// Draw a cube he is holding
	if (pl.holding_cube)
	{
		cube_draw_single(fix32ToInt(pl.x) + PLAYER_DRAW_LEFT, fix32ToInt(pl.y) + PLAYER_DRAW_TOP - 15, pl.holding_cube);
	}
	if (pl.invuln_cnt && (system_osc % 2))
	{
		return;
	}

	// Teleporter in flashing
	if (pl.tele_out_cnt > 0)
	{
		if (pl.tele_out_cnt >= plk.tele_anim && (system_osc >> 2) % 2)
		{
			return;
		}
		else if (pl.tele_out_cnt < plk.tele_anim)
		{
			return;
		}
	}
	else if (pl.tele_in_cnt > 0)
	{
		if (pl.tele_in_cnt <= plk.tele_anim && (system_osc >> 2) % 2)
		{
			return;
		}
		else if (pl.tele_in_cnt > plk.tele_anim)
		{
			return;
		}
	}
	u16 size;
	s16 yoff;
	s16 xoff = (pl.lift_cnt ? (pl.lift_cnt / 2) % 2 : 0);
	if (pl.anim_frame < 0x10)
	{
		size = SPRITE_SIZE(2,3);
		yoff = PLAYER_DRAW_TOP;
	}
	else if (pl.anim_frame < 0x14)
	{
		size = SPRITE_SIZE(3,2);
		yoff = PLAYER_DRAW_TOP + 8;

		xoff = -4;
	}
	else
	{
		size = SPRITE_SIZE(3,3);
		yoff = PLAYER_DRAW_TOP;
		xoff = (pl.direction) ? -8 : 0;
	}
	sprite_put(fix32ToInt(pl.x) + xoff + PLAYER_DRAW_LEFT - state.cam_x, 
		fix32ToInt(pl.y) + yoff - state.cam_y, 
		size, 
		TILE_ATTR(PLAYER_PALNUM,0,0,pl.direction) + PLAYER_VRAM_SLOT);

}

static inline void player_entrance_coll(void)
{
	u16 i = STATE_NUM_ENTRANCES;
	while (i--)
	{
		if (i == 0)
		{
			continue;
		}
		entrance *e = &(state.entrances[i]);
		if (player_collision(e->x + ENTRANCE_CHK_LEFT, e->x + ENTRANCE_CHK_RIGHT, e->y + ENTRANCE_CHK_TOP, e->y + ENTRANCE_CHK_BOTTOM))
		{
			// Don't bother room lookup if this is marked already
			if (state.next_id != e->to_roomid)
			{
				// Check if room exists, and if not, set map pointer to zero
				if (map_by_id(e->to_roomid) == NULL)
				{
					state.next_id = 0;
				}
				state.next_entrance = e->to_num;
				state.next_id = e->to_roomid;
			}
			if (debug_en)
			{
				if (state.next_entrance == 0)
				{
					sprite_put(pl.px - 8 - state.cam_x, pl.py - 40 - state.cam_y, SPRITE_SIZE(1,1), TILE_ATTR_FULL(3, 0, 0, 0, 0x54E));
					sprite_put(pl.px - state.cam_x, pl.py - 40 - state.cam_y, SPRITE_SIZE(1,1), TILE_ATTR_FULL(3, 0, 0, 0, 0x547));
				}
				else
				{
					sprite_put(pl.px - 8 - state.cam_x, pl.py - 40 - state.cam_y, SPRITE_SIZE(1,1), TILE_ATTR_FULL(1, 0, 0, 0, 0x54F));
					sprite_put(pl.px - state.cam_x, pl.py - 40 - state.cam_y, SPRITE_SIZE(1,1), TILE_ATTR_FULL(1, 0, 0, 0, 0x54B));
					
				}
			}
		}
	}
	// Indicate that the next room pointer set is no good
}

static inline void player_chk_spikes(void)
{
	if (pl.invuln_cnt == 0 && map_hurt(pl.px,pl.py + PLAYER_CHK_BOTTOM))
	{
		player_get_hurt();
	}
}

static void player_teleport_seq(void)
{
	if (pl.tele_out_cnt > 0)
	{
		if (pl.holding_cube)
		{
			cube *c = cube_spawn(pl.px, pl.py - 32, pl.holding_cube, CUBE_STATE_AIR, FIX16(0.0), FIX16(0.0));
			cube_destroy(c);
			pl.holding_cube = NULL;
		}
		pl.tele_out_cnt--;
		// Transition to zero. Trigger a room transition.
		if (pl.tele_out_cnt == 0)
		{
			state.teleflag = 1;
		}
	}
	else if (pl.tele_in_cnt > 0)
	{
		pl.tele_in_cnt--;
	}
}

void player_run(void)
{
	player_eval_control_en();
	player_teleport_seq();
	player_accel();
	player_toss_cubes();
	player_lift_cubes();
	player_jump();
	player_move();
	player_chk_spikes();
	player_cp();
	player_eval_grounded();
	player_calc_anim();
	player_entrance_coll();
	player_dma_setup();
	player_special_counters();
}

void player_get_bounced(void)
{
	pl.dy = plk.jump_dy;
	if (pl.direction == PLAYER_RIGHT)
	{
		pl.dx = plk.hurt_dx;
	}
	else
	{
		pl.dx = -plk.hurt_dx;
	}
}

void player_get_hurt(void)
{
	if (pl.invuln_cnt == 0)
	{
		player_get_bounced();
		pl.hurt_cnt = plk.hurt_time;
		pl.invuln_cnt = plk.invuln_time;
		pl.cp_cnt = 0;

		if (pl.hp > 0)
		{
			pl.hp--;
		}

		// A cube being held gets dropped
		if (pl.holding_cube)
		{
			u16 cx = fix32ToInt(pl.x);
			u16 cy = fix32ToInt(pl.y) - 12;
			s16 cdx = (pl.direction == PLAYER_RIGHT) ? 1 : -1;
			fix16 cdy = system_ntsc ? FIX16(-2.5) : FIX16(-3.0);
			cube_spawn(cx, cy,
				pl.holding_cube,
				CUBE_STATE_AIR,
				cdx, cdy);
			pl.holding_cube = 0;
		}
	}
}

void player_set_xy_fix32(fix32 x, fix32 y)
{
	pl.x = x;
	pl.y = y;
	pl.px = fix32ToInt(x);
	pl.py = fix32ToInt(y);
}

void player_set_xy_px(s16 x, s16 y)
{
	pl.x = FIX32(x);
	pl.y = FIX32(y);
	pl.px = x;
	pl.py = y;
}
