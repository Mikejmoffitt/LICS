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
	u16 destructable;
	u16 item_spawn;
	u16 orb_num;
};

void cube_dma_tiles(void);

#endif
