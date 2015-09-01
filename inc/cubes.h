#ifndef CUBES_H
#define CUBES_H

#include <genesis.h>
#include "map.h"

// Used in the "data" field
#define CUBE_BLUE 0
#define CUBE_GREEN 1
#define CUBE_RED 2
#define CUBE_YELLOW_HP1 3
#define CUBE_YELLOW_HP2 4
#define CUBE_YELLOW_CP1 5
#define CUBE_YELLOW_CP2 6
#define CUBE_YELLOW_ORB 7

typedef struct cube cube;
struct cube
{
	// Properties every loaded sprite has
	map_obj obj;
	u8 active;
	s8 dx, dy;
};

void cube_dma_tiles(void);

#endif
