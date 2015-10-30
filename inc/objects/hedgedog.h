#ifndef HEDGEDOG_H
#define HEDGEDOG_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_hedgedog en_hedgedog;
struct en_hedgedog
{	
	// Header present in each drawable and processable enemy type
	en_header head;
};

// Initialize enemy at address e
void en_init_hedgedog(en_hedgedog *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_hedgedog(void);

#endif
