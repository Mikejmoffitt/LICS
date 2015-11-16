#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>

#define LYLE_3x3_CUTOFF 0x14

#define FZERO FIX16(0.0)
#define FZERO32 FIX32(0.0) 
#define PLAYER_STEP_UP 3

#define PLAYER_ACTION_THROW 2
#define PLAYER_ACTION_LIFT 2
#define PLAYER_ACTION_KICK 3

#define PLAYER_MAX_CP 30

// TODO: Find the real numbers for this. These are made up.
#define PLAYER_CP_SPAWN_PRICE 4
#define PLAYER_CP_SPAWN_CHEAP 2

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

#define PLAYER_DRAW_LEFT -8
#define PLAYER_DRAW_TOP -23

// Hold the player's constants, loaded and calculated at init
typedef struct player_k player_k;
struct player_k
{
	fix16 dx_max;
	fix16 dy_max;
	fix16 x_accel;
	fix16 y_accel;
	fix16 y_accel_weak;
	fix16 jump_dy;
	fix16 ceiling_dy;
	fix16 hurt_dx;
	u16 throw_anim_len;
	u16 kick_anim_len;
	u16 cubejump_anim_len;
	u16 lift_time;
	u16 hurt_time;
	u16 hurt_timeout;
	u16 invuln_time;
	u16 cp_restore_period;
	u16 cp_restore_period_fast;
	u16 cp_spawn_fast;
	u16 cp_spawn_slow;
	u16 cube_fx;
	u16 animspeed;
};

typedef struct player player;
struct player
{
	// Lyle physics vars
	fix32 x, y;
	// Storage of 'real pixel' position
	u16 px, py;
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
	u16 tele_out_cnt; // Counts down to a room transition.
	u16 tele_in_cnt; // Counts down to player being able to move again post transition.
	                 // Won't count down if there is a tele_out in progress.
	u16 cubejump_disable;
	
	u16 holding_cube; // flag for if holding a cube
	u16 invuln_cnt; // If non-zero, lyle is flashing and invincible
	u16 cp_cnt; // Counts up to PLAYER_CUBE_SPAWN

	// Player status
	u16 hp;
	u16 cp;
};

extern player pl;

// Init function for Lyle's variables
void player_init(void);

// For use in-game - don't reset health, cp, etc
void player_init_soft(void);

// Prepare to possibly copy to VRAM
void player_dma_setup(void);

// Copy graphics into VRAM slot
void player_dma(void);

// Do a jump
void player_do_jump(void);

// Run player's logic for one frame
void player_run(void);

// Test for box collision with the player
u16 player_collision(s16 x1, s16 x2, s16 y1, s16 y2);

// Update entry in sprite table cache
void player_draw(void);

// The bouncing effect associated with taking damage
void player_get_bounced(void);

// Damage the player
void player_get_hurt(void);

// Set coordinates; updates internal pixel-pos ones too
void player_set_xy_fix32(fix32 x, fix32 y);
void player_set_xy_px(s16 x, s16 y);

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
