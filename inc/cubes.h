#ifndef CUBES_H
#define CUBES_H

#include <genesis.h>
#include "map.h"

#define CUBE_W 16
#define CUBE_H 16
#define CUBE_TOP -15
#define CUBE_BOTTOM 0
#define CUBE_LEFT -8
#define CUBE_RIGHT 7

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
// Lower nybble gives it an ID
#define CUBE_YELLOW_CPORB 0x40
#define CUBE_YELLOW_HPORB 0x80

#define CUBE_DESTROY_NORMAL 1
#define CUBE_DESTROY_EXPLOSION 2

#define CUBE_STATE_IDLE 0
#define CUBE_STATE_CARRY 1
#define CUBE_STATE_AIR 2
#define CUBE_STATE_KICKED 3

typedef struct cube cube;
struct cube
{
	u16 x;
	u16 y;
	s16 dx;
	fix16 dy;
	u16 state;
	u16 type;
	u16 item_spawn;
	u16 orb_num;
};

void cube_dma_tiles(void);
void cube_draw(u16 x, u16 y, u16 type);

#endif
