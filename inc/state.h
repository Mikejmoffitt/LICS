#ifndef STATE_H
#define STATE_H

#include <genesis.h>
#include "map.h"
// Player screen centering offset
#define STATE_SC_OFFX 5
#define STATE_SC_OFFY 8

// Screen scrolling transition boundary
#define STATE_SC_W 320
#define STATE_SC_H 224
#define STATE_SC_SEAMX 160
#define STATE_SC_SEAMY 112

// Some global game state

typedef struct gamestate gamestate;
struct gamestate
{
	map_file *current_room;
	u8 *current_map;
	u16 cam_x, cam_y;
};

extern gamestate state;

void state_load_room(u8 roomnum);

void state_update_scroll(u16 px, u16 py);

#endif
