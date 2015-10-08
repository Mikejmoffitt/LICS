// cubes.h
// The objects Lyle can spawn or pick up to attack enemies with
#ifndef CUBES_H
#define CUBES_H

#include <genesis.h>
#include "map.h"

#define CUBES_NUM 32

#define CUBE_W 16
#define CUBE_H 16
#define CUBE_TOP -15
#define CUBE_BOTTOM 0
#define CUBE_LEFT -8
#define CUBE_RIGHT 7

#define CUBE_KICK_DX 3

#define CUBE_BOUNCE_COUNT_INIT 1

/* Cube type descriptions */
#define CUBE_BLUE 0x0100
#define CUBE_PHANTOM 0x0200
#define CUBE_GREEN 0x0300
#define CUBE_RED 0x0400
#define CUBE_YELLOW 0x0800
#define CUBE_ORANGE 0x1000
#define CUBE_SPAWNER 0x2000
// Yellow cube's lower bytes become a bitfield describing contents
/*

Example values:

0x800 - HP up
0x801 - 2XHP up
0x820 - CP up
0x821 - 2XCP up
0x840 - CP ORB #0
0x884 - HP Orb #4

*/


#define CUBE_YELLOW_HP 0x01
#define CUBE_YELLOW_HP_2X 0x02
#define CUBE_YELLOW_CP 0x03
#define CUBE_YELLOW_CP_2X 0x04
// Lower nybble gives it an ID (which CP orb?)
#define CUBE_YELLOW_CPORB 0x40
#define CUBE_YELLOW_HPORB 0x80

#define CUBE_STATE_INACTIVE 0
#define CUBE_STATE_IDLE 1 
#define CUBE_STATE_AIR 2
#define CUBE_STATE_KICKED 3

#define CUBE_STATE_FIZZLE 4
#define CUBE_STATE_FIZZLERED 5
#define CUBE_STATE_EXPLODE 6
#define CUBE_FIZZLE_TIME 7

// -------------- Cube physics constants ------------------------
// Basics
#define CUBE_GRAVITY FIX16(0.2)

// Green cube specifics
#define CUBE_ON_CUBE_DY	FIX16(-3)
#define CUBE_ON_CUBE_DX 1
#define CUBE_MAX_GROUND_PUSH 8
#define CUBE_COL_T 8

#define CUBE_BOUNCE_COEF FIX16(0.35)
#define CUBE_BOUNCE_CUTOFF FIX16(-1.25)
#define CUBE_TINYBOUNCE_HACK FIX16(-0.6)
#define CUBE_CEILING_DY FIX16(3.0)
typedef struct cube cube;
struct cube
{
	u16 state;
	s16 x;
	s16 y;
	s16 dx;
	fix16 dy;
	s16 bounce_count;
	u16 cube_col_timeout;
	u16 type;
};

extern cube cubes[CUBES_NUM];

void cube_dma_tiles(void);
void cubes_init(void);
void cube_delete(cube *c);
void cubes_run(void);
void cubes_draw(void);
void cube_draw_single(u16 x, u16 y, u16 type);
void cube_spawn(u16 x, u16 y, u16 type, u16 state, s16 dx, fix16 dy);

#endif
