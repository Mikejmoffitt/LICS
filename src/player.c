#include "player.h"

void player_init(player *pl)
{
	pl->type = OBJ_PLAYER;
	pl->x = 0;
	pl->y = 0;
	pl->dx = FZERO;
	pl->dy = FZERO;
	pl->grounded = 0;
	pl->direction = PLAYER_RIGHT;
	pl->control_disabled = 0;
	
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
	pl->hp = PLAYER_START_HP;
	pl->cp = PLAYER_START_CP;

	pl->input = 0;
	pl->input_prev = 0;

}

void player_dma(u16 num, u16 dest)
{
	u16 offset = 0;
	dest = dest * 32;
	if (num < LYLE_3x3_CUTOFF)
	{
		offset = num * 6;
	}
	else
	{
		num -= 0x14;
		offset = 120 + (9 * num);
	}
	u16 size = (offset >= 120) ? (9 * 16) : (6 * 16);

	VDP_doVRamDMA(gfx_lyle + (32 *offset),dest,size);
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
	if (pl->dx > FZERO && !(pl->input & KEY_RIGHT))
	{
		pl->dx = fix16Sub(pl->dx,PLAYER_X_DECEL);	
		// Don't decel into the other direction
		if (fix16ToInt(pl->dx) < FZERO)
		{
			pl->dx = FZERO;
		}
	}
	else if (pl->dx < FZERO && !(pl->input & KEY_LEFT))
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
	// For now, a hack for testing
	if (fix32ToInt(pl->y) >= 150)
	{
		pl->grounded = 1;
		if (pl->dy > FZERO)
		{
			pl->dy = FZERO;
			pl->y = intToFix32(150);;
		}
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

void player_move(player *pl)
{
	// Do movement	
	pl->x = fix32Add(pl->x,fix16ToFix32(pl->dx));
	pl->y = fix32Add(pl->y,fix16ToFix32(pl->dy));

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
		if (pl->dx == FZERO) // Standing
		{
			pl->anim_frame = 0x00;
		}
		else // Walking cycle
		{
			switch ((pl->anim_cnt / 8)% 4)
			{
				case 0:
				case 2:
					pl->anim_frame = 0x02;
					break;
				case 1:
					pl->anim_frame = 0x03;
					break;
				case 3:
					pl->anim_frame = 0x01;
					break;
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
	if (pl->anim_frame < 0x10)
	{
		size = SPRITE_SIZE(2,3);
	}
	else if (pl->anim_frame < 0x14)
	{
		size = SPRITE_SIZE(3,2);
	}
	else
	{
		size = SPRITE_SIZE(3,3);
	}
	sprite_put(fix32ToInt(pl->x) - pl->cam_x, fix32ToInt(pl->y) - pl->cam_y, size, 
		TILE_ATTR(3,1,0,pl->direction) + PLAYER_VRAM_SLOT);
}
