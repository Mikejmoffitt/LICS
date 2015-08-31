#ifndef STATE_H
#define STATE_H

#include <genesis.h>
#include "map.h"

// Screen scrolling transition boundary
#define STATE_SC_W 320
#define STATE_SC_H 240
#define STATE_SC_SEAMX 160
#define STATE_SC_SEAMY 112

#define STATE_PLANE_W 64
#define STATE_PLANE_H 32

#define STATE_TRANSITION_MARGIN 6

#ifndef FZERO
#define FZERO FIX16(0.0)
#endif

// Some global game state

typedef struct gamestate gamestate;
struct gamestate
{
	map_file *current_room;
	u8 *current_map;
	u16 cam_x, cam_y;
	u16 current_music;
	u16 next_id;
	u16 next_entrance;
	u8 current_id;
};

extern gamestate state;

// Load a room by its ID number
void state_load_room(u8 roomnum);

// Update camera X based on player position.
void state_update_scroll(u16 px, u16 py);

// Check if player has reached a screen edge, load new room if needed
u16 state_watch_transitions(u16 px, u16 py, fix16 dx, fix16 dy);

#endif
