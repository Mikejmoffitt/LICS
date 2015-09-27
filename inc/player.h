#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>

#define LYLE_3x3_CUTOFF 0x14

#define PLAYER_DX_MAX FIX16(1.7)
#define PLAYER_DX_MIN FIX16(-1.7)
#define PLAYER_DY_MAX FIX16(8.0)
#define PLAYER_X_ACCEL FIX16(0.15)
#define PLAYER_X_DECEL FIX16(0.15)

#define PLAYER_Y_ACCEL FIX16(0.276)
#define PLAYER_Y_ACCEL_WEAK FIX16(0.156)
#define PLAYER_JUMP_DY FIX16(-3.7 + 0.12)
//#define PLAYER_JUMP_DY FIX16(-3.7)
#define PLAYER_JUMP_HOLD_STR FIX16(0.1)
#define PLAYER_STEP_UP 3
#define FZERO FIX16(0.0)
#define FZERO32 FIX32(0.0)
#define PLAYER_CEILING_VECY FIX16(-0.5)

#define PLAYER_ACTION_THROW 2
#define PLAYER_ACTION_LIFT 2
#define PLAYER_ACTION_KICK 3

#define PLAYER_THROW_ANIM_LEN 8
#define PLAYER_KICK_ANIM_LEN 8
#define PLAYER_CUBEJUMP_ANIM_LEN 20

#define PLAYER_LIFT_TIME 15

#define PLAYER_HURT_TIME 30
#define PLAYER_INVULN_TIME 80

#define PLAYER_MAX_CP 30
#define PLAYER_CP_RESTORE_PERIOD 250
#define PLAYER_CP_RESTORE_PERIOD_FAST 250

#define PLAYER_CP_SPAWN_SLOW 60
#define PLAYER_CP_SPAWN_FAST 30

// TODO: Find the real numbers for this. These are made up.
#define PLAYER_CP_SPAWN_PRICE 4
#define PLAYER_CP_SPAWN_CHEAP 2
#define PLAYER_CUBE_SPAWN 60 // Cube spawns at this point. 
#define PLAYER_CUBE_SPAWN_FAST 30
#define PLAYER_CUBE_FX 5 // If cube spawn count is above, make particles

#define PLAYER_RIGHT 0
#define PLAYER_LEFT 1


#define PLAYER_START_HP 5
#define PLAYER_START_CP 5

// Pixel dimensions for player
#define PLAYER_HEIGHT 20
#define PLAYER_WIDTH 9

// 0,0 is the bottom-center of the player.
#define PLAYER_CHK_LEFT -5
#define PLAYER_CHK_RIGHT 4
#define PLAYER_CHK_TOP -18
#define PLAYER_CHK_BOTTOM 0
#define PLAYER_CHK_FOOT (PLAYER_CHK_BOTTOM - PLAYER_STEP_UP) 
#define PLAYER_CHK_MID (PLAYER_CHK_FOOT - 8)

#define PLAYER_ANIMSPEED 6

#define PLAYER_DRAW_LEFT -8
#define PLAYER_DRAW_TOP -23

typedef struct player player;
struct player
{
	// Lyle physics vars
	fix32 x, y;
	void *on_cube;
	fix16 dx, dy;
	u16 grounded;
	u16 direction;
	u16 control_disabled;

	// Animation vars
	u16 anim_cnt; // Counts always, used for modulus for animations
	u16 anim_frame; // Which animation frame to DMA

	// Action-in-progress counters
	u16 throwdown_cnt; // If non-zero, air throwing-down anim plays
	u16 throw_cnt; // If non-zero, throwing anim plays
	u16 kick_cnt; // If non-zero, kick anim plays
	u16 lift_cnt; // If non-zero, in the middle of lifting
	u16 hurt_cnt; // If non-zero, hurt anim, no controls
	u16 action_cnt; // Generic action delay countdown
	u16 cubejump_disable;

	u16 holding_cube; // flag for if holding a cube
	u16 invuln_cnt; // If non-zero, lyle is flashing and invincible
	u16 cp_cnt; // Counts up to PLAYER_CUBE_SPAWN

	// Player status
	u16 hp;
	u16 cp;

	// Hold last gamepad state
	u16 input;
	u16 input_prev;
};

// Init function for Lyle's variables
void player_init(player *pl);

// For use in-game - don't reset health, cp, etc
void player_init_soft(player *pl);

// Prepare to possibly copy to VRAM
void player_dma_setup(player *pl);

// Copy graphics into VRAM slot
void player_dma(player *pl);

// Run player's logic for one frame
void player_run(player *pl);

// Update entry in sprite table cache
void player_draw(player *pl);




#endif




/* Lyle sprite table:

00 - stand
01 - walk1
02 - walk2
03 - walk3
04 - jump
05 - lift
06 - hurt
07 - jumpthrow
08 - stand_l
09 - walk1_l
0A - walk2_l
0B - walk3_l
0C - jump_l
0D - dazed
0E - look up
0F - die1
10 - die2
11 - splat1
12 - splat2
13 - getup / splat3
14 - crush1
15 - crush2
16 - throw
17 - kick
*/
