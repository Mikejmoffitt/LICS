// Objects representing segues and exits for room

#ifndef SEGUE_H
#define SEGUE_H

#include <genesis.h>
#include "map.h"

typedef struct segue segue;
struct segue
{
	map_obj obj;
	u16 segue_num; // Num 0 is the "default" if none is specified
	u16 to_room; // Room ID to go to
	u16 to_segue; // segue to go to in specified room
};

#endif
