#ifndef WIP_H
#define WIP_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_wip en_wip;
struct en_wip
{	
	// Header present in each drawable and processable enemy type
	en_header head;
};

// Initialize enemy at address e
void en_init_wip(en_wip *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_wip(void);

#endif
