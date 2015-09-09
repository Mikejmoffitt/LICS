#include "player.h"

#include "gfx.h"
#include "pal.h"
#include "mpad.h"
#include "vramslots.h"
#include "sprites.h"

#include "state.h"
#include "save.h"
#include "system.h"
#include "cubes.h"
#include "particles.h"

static u32 lyle_dma_src;
static u16 lyle_dma_dest;
static u16 lyle_dma_len;

static u16 ctype;

static u16 cp_restore_cnt;

static void player_set_pal(void)
{
	VDP_doCRamDMA((u32)pal_lyle, 32 * PLAYER_PALNUM, 16);
}

void player_init(player *pl)
{
	pl->x = FZERO32;
	pl->y = FZERO32;
	pl->direction = PLAYER_RIGHT;
	
	pl->hp = 5;
	pl->cp = 30;
	player_init_soft(pl);
}

void player_init_soft(player *pl)
{
	pl->dx = FZERO;
	pl->dy = FZERO;
	pl->grounded = 0;
	
	pl->anim_cnt = 0;
	pl->anim_frame = 0;
	pl->holding_cube = 0;
	pl->throw_cnt = 0;
	pl->throwdown_cnt = 0;
	pl->kick_cnt = 0;
	pl->lift_cnt = 0;
	pl->cp_cnt = 0;
	pl->hurt_cnt = 0;
	pl->invuln_cnt = 0;
	pl->action_cnt = 0;
	pl->control_disabled = 0;
	pl->input = 0;
	pl->input_prev = 0;
	pl->cubejump_disable = 0;
	pl->on_cube = NULL;
	player_set_pal();
}

void player_dma_setup(player *pl)
{
	u16 num = pl->anim_frame;
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

void player_dma(player *pl)
{
	VDP_doVRamDMA((u32)lyle_dma_src,lyle_dma_dest,lyle_dma_len);
}

static void player_input(player *pl)
{
	if (!pl->control_disabled)
	{
		pl->input_prev = pl->input;
		pl->input = pad_read(0);
	}
	else
	{
		pl->input = 0;
		pl->input_prev = 0;
	}
}

static void player_cp(player *pl)
{
	// We don't have this power, don't bother
	if (!sram.have_phantom)
	{
		pl->cp_cnt = 0;
		return;
	}
	cp_restore_cnt++;
	// Manage periodic restoration of CP
	if (cp_restore_cnt >= PLAYER_CP_RESTORE_PERIOD)
	{
		cp_restore_cnt = 0;
		if (pl->cp != PLAYER_MAX_CP)
		{
			pl->cp++;
		}
	}
	// In the middle of doing something that voids this ability
	if (pl->lift_cnt || pl->hurt_cnt || pl->action_cnt)
	{
		return;
	}
	
	u16 cube_price = (sram.have_cheap_phantom ? PLAYER_CP_SPAWN_CHEAP : PLAYER_CP_SPAWN_PRICE);
	// Spawning of the cube; are we not holding one, and can afford one?
	if (!pl->holding_cube && pl->cp >= cube_price)
	{
		if (pl->input & KEY_B)
		{
			pl->cp_cnt++;
		}
		else
		{
			pl->cp_cnt = 0;
		}
		u16 cube_spawn_period = (sram.have_fast_phantom ? PLAYER_CP_SPAWN_FAST : PLAYER_CP_SPAWN_SLOW);
		if (pl->cp_cnt >= cube_spawn_period)
		{
			switch (ctype)
			{
				default:
					ctype = CUBE_PHANTOM;
					break;
				case CUBE_PHANTOM:
					ctype = CUBE_BLUE;
					break;
				case CUBE_BLUE:
					ctype = CUBE_RED;
					break;
				case CUBE_RED:
					ctype = CUBE_YELLOW;
					break;
				case CUBE_YELLOW:
					ctype = CUBE_GREEN;
					break;
				case CUBE_GREEN:
					ctype = CUBE_PHANTOM;
					break;
			}
			pl->holding_cube = ctype;
			pl->cp_cnt = 0;
			pl->cp -= cube_price;
		}
	}
	// Sparkling effect when cube is starting to form
	if (pl->cp_cnt > PLAYER_CUBE_FX && system_osc % 2)
	{
		particle_spawn(fix32ToInt(pl->x), fix32ToInt(pl->y) - 32, PARTICLE_TYPE_SPARKLE);
	}
}

static void player_accel(player *pl)
{
	if (pl->control_disabled)
	{
		return;
	}
	if (pl->lift_cnt)
	{
		return;
	}
	// walking right and left
	if (pl->input & KEY_RIGHT)
	{
		pl->dx = fix16Add(pl->dx,PLAYER_X_ACCEL);
		pl->direction = PLAYER_RIGHT;
	}
	else if (pl->input & KEY_LEFT)
	{
		pl->dx = fix16Sub(pl->dx,PLAYER_X_ACCEL);
		pl->direction = PLAYER_LEFT;
	}

	// deceleration
	if (pl->dx > FZERO && !(pl->input & (KEY_RIGHT | KEY_LEFT)))
	{
		pl->dx = fix16Sub(pl->dx,PLAYER_X_DECEL);	
		// Don't decel into the other direction
		if (fix16ToInt(pl->dx) < FZERO)
		{
			pl->dx = FZERO;
		}
	}
	else if (pl->dx < FZERO && !(pl->input & (KEY_RIGHT | KEY_LEFT)))
	{
		pl->dx = fix16Add(pl->dx,PLAYER_X_ACCEL);				
		// Don't decel into the other direction
		if (pl->dx > FZERO)
		{
			pl->dx = FZERO;
		}
	}

	// If dy/dx is almost zero, make it zero
	if (pl->dx > FIX16(-0.1) && pl->dx < FIX16(0.1) && !(pl->input & (KEY_RIGHT | KEY_LEFT)))
	{
		pl->dx = FZERO;
	}

	// Limit top speed
	if (pl->dx > PLAYER_DX_MAX)
	{
		pl->dx = PLAYER_DX_MAX;
	}
	else if (pl->dx < PLAYER_DX_MIN)
	{
		pl->dx = PLAYER_DX_MIN;
	}
}

static void player_eval_grounded(player *pl)
{
	if (pl->dy < FZERO)
	{
		pl->grounded = 0;
		return;
	}
	u16 px = fix32ToInt(pl->x);
	u16 py = fix32ToInt(pl->y);
	// "Is the tile one pixel below me solid?"
	if ((map_collision(px + PLAYER_CHK_RIGHT - 1, py + PLAYER_CHK_BOTTOM + 1)) || 
		(map_collision(px + PLAYER_CHK_LEFT + 1, py + PLAYER_CHK_BOTTOM + 1)))
	{
		pl->grounded = 1;
	}
	else
	{
		pl->grounded = 0;
	}
}

static void player_jump(player *pl)
{
	// Very small timeout to prevent lyle from cube-jumping so the occasional
	// normal jump doesn't accidentally become a cube jump off a ledge
	if (pl->grounded || pl->on_cube)
	{
		pl->cubejump_disable = 2;
	}

	if (pl->lift_cnt)
	{
		return;
	}

	// C key pressed, negative edge (1 -> 0)
	if ((pl->input & KEY_C) && !(pl->input_prev & KEY_C))
	{
		// Normal jump off the ground
		if (pl->grounded || pl->on_cube)
		{
			goto do_jump;
		}
		// Cube jumping, if we have the ability
		else if (pl->holding_cube && sram.have_jump && !pl->cubejump_disable)
		{
			pl->throwdown_cnt = PLAYER_CUBEJUMP_ANIM_LEN;

			// If the wall behind the player is solid, align the cube's X to it
			// so it doesn't fizzle immediately on throw. Not sure exactly what
			// made this happen in the original but this is mimicing what I see
			// when I play the game.
			u16 cx = fix32ToInt(pl->x);
			u16 px = fix32ToInt(pl->x);
			u16 back_x = (pl->direction == PLAYER_LEFT) ? 
				(px + PLAYER_CHK_RIGHT + 4) : 
				(px + PLAYER_CHK_LEFT - 4);

			// Align the cube
			if (map_collision(back_x, fix32ToInt(pl->y) + PLAYER_CHK_BOTTOM))
			{
				cx = ((cx + 4) / 8) * 8;
			}
		
			// Generate a cube to throw
			cube_spawn(cx,
				fix32ToInt(pl->y) - 12,
				pl->holding_cube,
				CUBE_STATE_AIR,
				0, FIX16(4));

			pl->holding_cube = 0;
			// Generate cube of right type, throw it down
			goto do_jump;
		}
	}

	return;
do_jump:
	pl->dy = PLAYER_JUMP_DY;
	// Play SFX
	return;
}

static void player_toss_cubes(player *pl)
{
	if (pl->holding_cube && (pl->input & KEY_B) && (!(pl->input_prev & KEY_B)))
	{
		s16 cdx;
		fix16 cdy;
		// Holding down; do a short toss
		if (pl->input & (KEY_DOWN))
		{
			cdx = (pl->direction == PLAYER_RIGHT) ? 1 : -1;
			cdy = FIX16(-2.0);
		}
		// Holding up; toss straight up
		else if (pl->input & KEY_UP)
		{
			cdx = 0;
			cdy = FIX16(-5.0);
		}
		// Throw with direction right
		else if (pl->input & KEY_RIGHT && pl->direction == PLAYER_RIGHT)
		{
			cdx = 4;
			cdy = FIX16(-1.0);
		}
		// Left
		else if (pl->input & KEY_LEFT && pl->direction == PLAYER_LEFT)
		{
			cdx = -4;
			cdy = FIX16(-1.0);
		}
		else
		{
			cdx = (pl->direction == PLAYER_RIGHT) ? 2 : -2;
			cdy = FIX16(-2.0);
		}
		
		// Generate a cube to throw
		cube_spawn(fix32ToInt(pl->x),
			fix32ToInt(pl->y) - 23,
			pl->holding_cube,
			CUBE_STATE_AIR,
			cdx, cdy);

		// Player response
		pl->holding_cube = 0;
		pl->action_cnt = PLAYER_ACTION_THROW;
		pl->throw_cnt = PLAYER_THROW_ANIM_LEN;
	}
}

static void player_kick_cubes(player *pl)
{
	if (!sram.have_kick)
	{
		return;
	}
}

static void player_lift_cubes(player *pl)
{
	if (!sram.have_lift)
	{
		return;
	}	// In the middle of doing something that voids this ability
	if (pl->hurt_cnt || pl->action_cnt)
	{
		return;
	}
	if (pl->on_cube && pl->lift_cnt == 0 && pl->input & KEY_B && !(pl->input_prev & KEY_B))
	{	
		pl->lift_cnt = PLAYER_LIFT_TIME + 1;
		pl->dx = FZERO;
	}
	if (pl->lift_cnt == 1 && pl->on_cube)
	{
		cube *c = (cube *)pl->on_cube;
		pl->holding_cube = c->type;
		cube_delete(c);

		// Re-implement the MMF version bug where you can jump while lifting
		if (pl->input & KEY_C)
		{
			pl->dy = PLAYER_JUMP_DY;
		}
	}
}

static void player_special_counters(player *pl)
{
	if (pl->throwdown_cnt)
	{
		pl->throwdown_cnt--;
	}
	if (pl->throw_cnt)
	{
		pl->throw_cnt--;
	}
	if (pl->lift_cnt)
	{
		pl->lift_cnt--;
	}
	if (pl->hurt_cnt)
	{
		pl->hurt_cnt--;
	}
	if (pl->invuln_cnt)
	{
		pl->invuln_cnt--;
	}
	if (pl->action_cnt)
	{
		pl->action_cnt--;
	}
	if (pl->cubejump_disable)
	{
		pl->cubejump_disable--;
	}
}

static void player_bg_horizontal_collision(player *pl)
{
	u16 py = fix32ToInt(pl->y);
	u16 px = fix32ToInt(pl->x);

	// Horizontal collision
	if (pl->dx > FZERO)
	{
		if ((map_collision(px + PLAYER_CHK_RIGHT, py + PLAYER_CHK_TOP)) ||
			(map_collision(px + PLAYER_CHK_RIGHT, py + PLAYER_CHK_FOOT)) ||
			(map_collision(px + PLAYER_CHK_RIGHT, py + PLAYER_CHK_MID))) 
		{
			px = 8 * (px / 8);
			px += 3;
			pl->x = intToFix32(px);
			pl->dx = FZERO;
		}
	}
	else if (pl->dx < FZERO)
	{
		if ((map_collision(px + PLAYER_CHK_LEFT, py + PLAYER_CHK_TOP)) ||
			(map_collision(px + PLAYER_CHK_LEFT, py + PLAYER_CHK_FOOT) ) ||
			(map_collision(px + PLAYER_CHK_LEFT, py + PLAYER_CHK_MID) )) 
		{
			px = 8 * (px / 8);
			px += 5;
			pl->x = intToFix32(px);
			pl->dx = FZERO;
		}
	}
}

static void player_bg_vertical_collision(player *pl)
{
	u16 py = fix32ToInt(pl->y);
	u16 px = fix32ToInt(pl->x);
	px -= fix16ToInt(pl->dx);
	// Vertical collision
	// "Am I now stuck with my feet in the ground?"
	if (pl->dy >= FZERO)
	{
		if ((map_collision(px + PLAYER_CHK_RIGHT - 1, py + PLAYER_CHK_BOTTOM)) ||
			(map_collision(px + PLAYER_CHK_LEFT + 1, py + PLAYER_CHK_BOTTOM)))
		{
			// Snap to nearest 8px boundary
			py = 8 * (py / 8) - 1;
			pl->y = intToFix32(py);
			pl->dy = FZERO;
			int i = 8;
			while (i--)
			{
				// Are we still stuck? Move up 8px since the snap didn't quite work right.
				if ((map_collision(px + PLAYER_CHK_RIGHT - 1, py + PLAYER_CHK_BOTTOM)) ||
					(map_collision(px + PLAYER_CHK_LEFT + 1, py + PLAYER_CHK_BOTTOM)))
				{
					pl->y = fix32Sub(pl->y,intToFix32(1));
				}
				else
				{
					break;
				}
			}
			player_eval_grounded(pl);
			// Somehow we aren't grounded now - move down one tile.
			if (!pl->grounded)
			{
				pl->y = fix32Add(pl->y,intToFix32(8));
				player_eval_grounded(pl);
			}
		}
	}
	else if (pl->dy < FZERO)
	{
		// "Am I now stuck with my head in the ceiling?"
		if ((map_collision(px + PLAYER_CHK_RIGHT - 1, py + PLAYER_CHK_TOP - 1)) ||
			(map_collision(px + PLAYER_CHK_LEFT + 1, py + PLAYER_CHK_TOP - 1)))
		{
			// Snap to nearest 8px boundary, with head room accounted for
			py = 8 * ((py + 4) / 8) + 3;
			pl->y = intToFix32(py);
			if (pl->dy < PLAYER_CEILING_VECY)
			{
				pl->dy = PLAYER_CEILING_VECY;
			}

		}
	}
}

// Interactions with stationary cubes

static void player_cube_horizontal_collision(player *pl, cube *c)
{
	if (c->state == CUBE_STATE_AIR)
	{
		return;
	}
	u16 py = fix32ToInt(pl->y);
	u16 px = fix32ToInt(pl->x);

	// check we are within appropriate Y bounds
	if (c->y + CUBE_TOP <= py + PLAYER_CHK_BOTTOM - 1 && 
		c->y + CUBE_BOTTOM >= py + PLAYER_CHK_TOP + 1)
	{
		// Horizontal collision
		if (pl->dx > FZERO)
		{
			if (c->x + CUBE_LEFT >= px) 
			{
				px = (c->x + CUBE_LEFT) - PLAYER_CHK_RIGHT - 1;
				pl->x = intToFix32(px);
				pl->dx = FZERO;
			}
		}
		else if (pl->dx < FZERO)
		{
			if (c->x + CUBE_RIGHT <= px) 
			{
				px = (c->x + CUBE_RIGHT) - PLAYER_CHK_LEFT + 1;
				pl->x = intToFix32(px);
				pl->dx = FZERO;
			}
		}
	}
}

static void player_cube_vertical_collision(player *pl, cube *c)
{
	u16 py = fix32ToInt(pl->y);
	u16 px = fix32ToInt(pl->x);
	px -= fix16ToInt(pl->dx);

	// appropriate X bounds
	if (c->x + CUBE_LEFT <= px + PLAYER_CHK_RIGHT && 
		c->x + CUBE_RIGHT >= px + PLAYER_CHK_LEFT)
	{
		// Vertical collision
		// "Am I now stuck with my feet in a cube?"
		if (pl->dy > FZERO)
		{
			if (py + PLAYER_CHK_BOTTOM >= c->y + CUBE_TOP && py + PLAYER_CHK_TOP < c->y + CUBE_BOTTOM)
			{
				// Snap to the cube
				py = c->y + CUBE_TOP - PLAYER_CHK_BOTTOM - 1;
				pl->y = intToFix32(py);
				pl->dy = FZERO;
			}
		}
		else if (pl->dy < FZERO)
		{
			// "Am I now stuck with my head in the cube?"
			if (py + PLAYER_CHK_TOP< c->y + CUBE_BOTTOM)
			{
				py = c->y + CUBE_BOTTOM - PLAYER_CHK_TOP + 1;
				pl->y = intToFix32(py);
				if (pl->dy < PLAYER_CEILING_VECY)
				{
					pl->dy = PLAYER_CEILING_VECY;
				}
			}
		}
	}
}

static void player_cube_eval_standing(player *pl, cube *c)
{
	u16 py = fix32ToInt(pl->y);
	u16 px = fix32ToInt(pl->x);
	if (c->x + CUBE_LEFT <= px + PLAYER_CHK_RIGHT && 
		c->x + CUBE_RIGHT >= px + PLAYER_CHK_LEFT && 
		py + PLAYER_CHK_BOTTOM + 1>= c->y + CUBE_TOP)
	{
		// Already standing on a cube? Determine which cube is closer and
		// use that cube as the "standing-on cube" reference for lifting.
		if (pl->on_cube)
		{
			// Get X distance difference for new and old cubes
			cube *oc = (cube *)(pl->on_cube);
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
			if (new_diff_x > orig_diff_x)
			{
				pl->on_cube = (void *)c;
			}
			// Otherwise, the previous remains.
		}
		else
		{
			pl->on_cube = (void *)c;
		}
	}
}

static void player_cube_collision(player *pl)
{
	u16 px = (u16)fix32ToInt(pl->x);
	u16 py = (u16)fix32ToInt(pl->y);
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
		else if (c->state == CUBE_STATE_IDLE && 
			c->x + CUBE_LEFT <= px + PLAYER_CHK_RIGHT + 1 && 
			c->x + CUBE_RIGHT >= px + PLAYER_CHK_LEFT - 1&& 
			c->y + CUBE_TOP <= py + PLAYER_CHK_BOTTOM + 1 && 
			c->y + CUBE_BOTTOM >= py + PLAYER_CHK_TOP - 1)
		{
			player_cube_vertical_collision(pl, c);
			player_cube_horizontal_collision(pl, c);
			player_cube_eval_standing(pl, c);
		}
	}
}

static void player_move(player *pl)
{
	// Do movement	
	pl->x = fix32Add(pl->x,fix16ToFix32(pl->dx));
	pl->y = fix32Add(pl->y,fix16ToFix32(pl->dy));

	pl->on_cube = NULL;
	player_bg_vertical_collision(pl);
	player_bg_horizontal_collision(pl);
	player_eval_grounded(pl);
	player_cube_collision(pl);

	// In the air, gravity is affected by the player holding jump or not
	if (!pl->grounded)
	{
		// The jump holding only affects gravity on the way up, though
		if ((pl->input & KEY_C) && pl->dy < FZERO)
		{
			pl->dy = fix16Add(pl->dy,PLAYER_Y_ACCEL_WEAK);
		}
		else
		{
			pl->dy = fix16Add(pl->dy,PLAYER_Y_ACCEL);
		}
		if (pl->dy > PLAYER_DY_MAX)
		{
			pl->dy = PLAYER_DY_MAX;
		}
	}
}

static void player_calc_anim(player *pl)
{
	if (pl->invuln_cnt && (system_osc % 8 > 3))
	{
		return;
	}
	if (pl->grounded || pl->on_cube)
	{
		pl->anim_cnt++;
		if (pl->anim_cnt == PLAYER_ANIMSPEED * 4)
		{
			pl->anim_cnt = 0;
		}
	}
	else
	{
		pl->anim_cnt = 0;
	}
	if (pl->throw_cnt > 0)
	{
		pl->anim_frame = 0x16;
		return;
	}
	else if (pl->throwdown_cnt > 0)
	{
		pl->anim_frame = 0x07;
		return;
	}
	else if (pl->kick_cnt > 0)
	{
		pl->anim_frame = 0x17;
		return;
	}
	else if (pl->lift_cnt > 0)
	{
		pl->anim_frame = 0x05;
		return;
	}
	else if (pl->hurt_cnt > 0)
	{
		pl->anim_frame = 0x06;	
		return;
	}
	
	if (pl->grounded || pl->on_cube)
	{
		if (!(pl->input & (KEY_LEFT | KEY_RIGHT))) // Standing
		{
			pl->anim_frame = 0x00;
		}
		else // Walking cycle
		{
			if (pl->anim_cnt < PLAYER_ANIMSPEED)
			{
				pl->anim_frame = 0x02;
			}
			else if (pl->anim_cnt < (PLAYER_ANIMSPEED * 2))
			{
				pl->anim_frame = 0x03;
			}
			else if (pl->anim_cnt < (PLAYER_ANIMSPEED * 3))
			{
				pl->anim_frame = 0x02;
			}
			else
			{
				pl->anim_frame = 0x01;
			}
		}
	}
	else
	{
		pl->anim_frame = 0x04;
	}
	if ((pl->cp_cnt > PLAYER_CUBE_FX) || pl->holding_cube)
	{
		// Offset to arms-up version
		pl->anim_frame += 0x08;
	}
}

void player_draw(player *pl)
{
	if (pl->invuln_cnt && (system_osc % 8 > 3))
	{
		return;
	}
	u16 size;
	s16 yoff;
	s16 xoff = (pl->lift_cnt ? (pl->lift_cnt / 2) % 2 : 0);
	if (pl->anim_frame < 0x10)
	{
		size = SPRITE_SIZE(2,3);
		yoff = PLAYER_DRAW_TOP;
	}
	else if (pl->anim_frame < 0x14)
	{
		size = SPRITE_SIZE(3,2);
		yoff = PLAYER_DRAW_TOP + 8;

		xoff = -4;
	}
	else
	{
		size = SPRITE_SIZE(3,3);
		yoff = PLAYER_DRAW_TOP;
		xoff = (pl->direction) ? -8 : 0;
	}
	sprite_put(fix32ToInt(pl->x) + xoff + PLAYER_DRAW_LEFT - state.cam_x, 
		fix32ToInt(pl->y) + yoff - state.cam_y, 
		size, 
		TILE_ATTR(PLAYER_PALNUM,1,0,pl->direction) + PLAYER_VRAM_SLOT);

	// Draw a cube he is holding
	if (pl->holding_cube)
	{
		cube_draw_single(fix32ToInt(pl->x) + PLAYER_DRAW_LEFT, fix32ToInt(pl->y) + PLAYER_DRAW_TOP - 15, pl->holding_cube);
	}
}

void player_run(player *pl)
{
	player_input(pl);
	player_accel(pl);
	player_kick_cubes(pl);
	player_toss_cubes(pl);
	player_lift_cubes(pl);
	player_jump(pl);
	player_move(pl);
	player_cp(pl);
	player_eval_grounded(pl);
	player_calc_anim(pl);
	player_dma_setup(pl);
	player_special_counters(pl);
}


