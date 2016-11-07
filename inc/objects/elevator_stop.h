#ifndef ELEVATOR_STOP_H
#define ELEVATOR_STOP_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_elevator_stop en_elevator_stop;
struct en_elevator_stop
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	u16 side;
	u16 directions;
	u16 anim_frame;
	u16 anim_cnt;
};

// Initialize enemy at address e
void en_init_elevator_stop(en_elevator_stop *e, u16 data);

// Deallocate VRAM associated with this type of enemy
void en_unload_elevator_stop(void);

#endif
