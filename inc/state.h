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

#define STATE_TRANSITION_MARGIN 7

#define STATE_SCROLL_DMA 2
#define STATE_SCROLL_SINGLE 1

#define STATE_MOVED_NONE 0x00
#define STATE_MOVED_X 0x01
#define STATE_MOVED_Y 0x02
#define STATE_MOVED_BOTH (STATE_MOVED_X | STATE_MOVED_Y)

#define STATE_SCROLL_INVALID 32767

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
	u16 vs_en;
	u16 hs_en;
	u16 xscroll_cmd;
	u16 yscroll_cmd;
	u16 xscroll_vals[STATE_PLANE_H];
	u16 yscroll_vals[STATE_PLANE_W / 2];
	u8 current_id;
	u8 fresh_room;
};

extern gamestate state;

// Load a room by its ID number
void state_load_room(u8 roomnum);

// Update camera X based on player position. Returns a bit field
// describing in which ways the scrolling has changed.
// Possible returns:
// 	STATE_MOVED_NONE
//	STATE_MOVED_X
//	STATE_MOVED_Y
//	STATE_MOVED_BOTH
u16 state_update_scroll(u16 px, u16 py);

// Commit scroll table updates
void state_dma_scroll(void);

// Check if player has reached a screen edge, load new room if needed
u16 state_watch_transitions(u16 px, u16 py, fix16 dx, fix16 dy);

#endif
