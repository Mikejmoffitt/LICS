#ifndef PLANT_H
#define PLANT_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_plant en_plant;
struct en_plant
{	
	// Header present in each drawable and processable enemy type
	en_header head;

	// Normal idle animation sequence variables
	u16 anim_frame;
	u16 anim_cnt;

	// Special sequence counters
	u16 timer;
	u16 cooldown_cnt;
};

// Initialize enemy at address e
void en_init_plant(en_plant *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_plant(void);

#endif
