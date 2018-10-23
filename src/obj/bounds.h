#ifndef BOUNDS_H
#define BOUNDS_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_bounds en_bounds;
struct en_bounds
{	
	// Header present in each drawable and processable enemy type
	en_header head;
};

// Initialize enemy at address e
void en_init_bounds(en_bounds *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_bounds(void);

#endif
