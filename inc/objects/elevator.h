#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <genesis.h>
#include "enemy.h"

#define ELEVATOR_MOVING_NONE 0
#define ELEVATOR_MOVING_DOWN 1
#define ELEVATOR_MOVING_UP 2

// Structure defining specific object type
typedef struct en_elevator en_elevator;
struct en_elevator
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	u16 anim_cnt;
	u16 anim_frame;
	u16 moving;
	u16 fresh_obj;
	u16 collision_timeout;
};

// Initialize enemy at address e
void en_init_elevator(en_elevator *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_elevator(void);

// Set the elevator(s) in motion
void trigger_elevator_move(u16 dir_move);

#endif
