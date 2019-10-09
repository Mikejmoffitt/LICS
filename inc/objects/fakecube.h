#ifndef FAKECUBE_H
#define FAKECUBE_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_fakecube en_fakecube;
struct en_fakecube
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	u16 hide_counter;
};

// Initialize enemy at address e
void en_init_fakecube(en_fakecube *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_fakecube(void);

#endif
