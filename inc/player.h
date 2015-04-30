#ifndef PLAYER_H
#define PLAYER_H

#include <genesis.h>

#define PLAYER_DX_MAX FIX16(1.7)
#define PLAYER_DX_MIN FIX16(-1.7)
#define PLAYER_DY_MAX FIX16(8.0)
#define PLAYER_X_ACCEL FIX16(0.15)
#define PLAYER_X_DECEL FIX16(0.15)
#define PLAYER_Y_ACCEL FIX16(0.23)
#define PLAYER_Y_ACCEL_WEAK FIX16(0.13)
#define PLAYER_JUMP_DY FIX16(-3.7)
#define PLAYER_JUMP_HOLD_STR FIX16(0.1)
#define PLAYER_STEP_UP 4

#define PLAYER_CUBE_FX 5 // If cube spawn count is above, make particles
#define PLAYER_CUBE_SPAWN 60 // Cube spawns at this point. 
#define PLAYER_CUBE_SPAWN_FAST 30

#define PLAYER_THROW_ANIM_LEN 8
#define PLAYER_KICK_ANIM_LEN 8 

#define PLAYER_LIFT_TIME 15

#define PLAYER_HURT_TIME 30
#define PLAYER_INVULN_TIME 80

#define PLAYER_MAX_CP 30 

typedef struct player player;
struct player
{
	// Lyle physics vars
	fix16 x, y, dx, dy;
	u16 holding_cube; // flag for if holding a cube
	u16 throw_cnt; // If non-zero, throwing anim plays
	u16 throwdown_cnt;
	u16 kick_cnt;
	u16 lift_cnt; // If non-zero, in the middle of lifting
	u16 cp_cnt; // Counts up to PLAYER_CUBE_SPAWN
	u16 hurt_cnt; // If non-zero, hurt anim, no controls
	u16 invuln_cnt; // If non-zero, lyle is flashing and invincible
	u16 hp;
	u16 cp;
};


#define LYLE_3x3_CUTOFF 0x14
// Put Lyle's sprite #n at addr dest
void player_dma(u16 num, u16 dest);




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
