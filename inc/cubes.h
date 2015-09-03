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

/* Cube type descriptions */
#define CUBE_BLUE 0x100
#define CUBE_PHANTOM 0x200
#define CUBE_GREEN 0x300
#define CUBE_RED 0x400
#define CUBE_YELLOW 0x800
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


#define CUBE_YELLOW_DOUBLE 0x10
#define CUBE_YELLOW_CP 0x20
// Lower nybble gives it an ID (which CP orb?)
#define CUBE_YELLOW_CPORB 0x40
#define CUBE_YELLOW_HPORB 0x80

#define CUBE_STATE_INACTIVE 0
#define CUBE_STATE_IDLE 1 
#define CUBE_STATE_CARRY 2
#define CUBE_STATE_AIR 3
#define CUBE_STATE_KICKED 4

typedef struct cube cube;
struct cube
{
	u16 state;
	u16 x;
	u16 y;
	s16 dx;
	fix16 dy;
	u16 type;
};

void cube_dma_tiles(void);
void cubes_init(void);
void cubes_run(void);
void cubes_draw(void);
void cube_draw_single(u16 x, u16 y, u16 type);

#endif
