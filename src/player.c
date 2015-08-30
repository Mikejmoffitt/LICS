#include "player.h"

#include "gfx.h"
#include "pal.h"
#include "mpad.h"
#include "vramslots.h"
#include "sprites.h"
#include "objtypes.h"

#include "state.h"

void player_init(player *pl)
{
	pl->type = OBJ_PLAYER;
	pl->x = FZERO32;
	pl->y = FZERO32;
	pl->dx = FZERO;
	pl->dy = FZERO;
	pl->grounded = 0;
	pl->direction = PLAYER_RIGHT;
	
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
	pl->control_disabled = 0;

	pl->input = 0;
	pl->input_prev = 0;

	pl->hp = 0;
	pl->cp = 0;

	player_set_pal();
}

void player_dma(player *pl)
{
	u16 num = pl->anim_frame;
	u16 offset = 0;

	// Destination is specified in bytes
	u16 dest = PLAYER_VRAM_SLOT * 32;
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
	// Num tiles to transfer * word size = DMA size in words
	u16 size = (offset >= 120) ? (9 * 16) : (6 * 16);

	VDP_doVRamDMA(gfx_lyle + (32 *offset),dest,size);
}

void player_set_pal(void)
{
	VDP_doCRamDMA(pal_lyle, 32 * PLAYER_PALNUM, 16);
}

void player_input(player *pl)
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

void player_cp(player *pl)
{
	if (pl->input & KEY_B)
	{
		pl->cp_cnt++;
	}
	else
	{
		pl->cp_cnt = 0;
	}
}

void player_accel(player *pl)
{
	if (pl->control_disabled)
	{
		return;
	}

	// walking right and left
	if (pl->input & KEY_RIGHT)
	{
		pl->dx = fix16Add(pl->dx,PLAYER_X_ACCEL);
		pl->direction = 0;
	}
	else if (pl->input & KEY_LEFT)
	{
		pl->dx = fix16Sub(pl->dx,PLAYER_X_ACCEL);
		pl->direction = 1;
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

void player_eval_grounded(player *pl)
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

void player_jump(player *pl)
{
	if (pl->grounded)
	{
		if ((pl->input & KEY_C) && !(pl->input_prev & KEY_C))
		{
			pl->dy = PLAYER_JUMP_DY;	
		}
	}
}

static void player_walk_collision(player *pl)
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

static void player_vertical_collision(player *pl)
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
			for (int i = 0; i < 8; i++)
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
			py = 8 * (py / 8) + 3;
			pl->y = intToFix32(py);
			if (pl->dy < FIX16(-1.0))
			{
				pl->dy = FIX16(-1.0);
			}
		}
	}
}


void player_move(player *pl)
{
	// Do movement	
	pl->x = fix32Add(pl->x,fix16ToFix32(pl->dx));
	pl->y = fix32Add(pl->y,fix16ToFix32(pl->dy));

	player_vertical_collision(pl);
	player_walk_collision(pl);

	player_eval_grounded(pl);

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
	}
}

void player_calc_anim(player *pl)
{
	if (pl->grounded)
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
	
	if (pl->grounded)
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
	u16 size;
	u16 yoff;
	if (pl->anim_frame < 0x10)
	{
		size = SPRITE_SIZE(2,3);
		yoff = PLAYER_DRAW_TOP;
	}
	else if (pl->anim_frame < 0x14)
	{
		size = SPRITE_SIZE(3,2);
		yoff = PLAYER_DRAW_TOP + 8;
	}
	else
	{
		size = SPRITE_SIZE(3,3);
		yoff = PLAYER_DRAW_TOP;
	}
	sprite_put(fix32ToInt(pl->x) + PLAYER_DRAW_LEFT - state.cam_x, 
		fix32ToInt(pl->y) + yoff - state.cam_y, 
		size, 
		TILE_ATTR(PLAYER_PALNUM,1,0,pl->direction) + PLAYER_VRAM_SLOT);
}
