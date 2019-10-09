#ifndef LASER_H
#define LASER_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_laser en_laser;
struct en_laser
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	u16 timer;
};

// Initialize enemy at address e
void en_init_laser(en_laser *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_laser(void);

#endif
