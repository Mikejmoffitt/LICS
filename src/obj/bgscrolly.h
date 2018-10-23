#ifndef BGSCROLLY_H
#define BGSCROLLY_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_bgscrolly en_bgscrolly;
struct en_bgscrolly
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	u16 initialized;
	u16 action;
	u16 enabled;
};

// Initialize enemy at address e
void en_init_bgscrolly(en_bgscrolly *e, u16 data);

// Deallocate VRAM associated with this type of enemy
void en_unload_bgscrolly(void);

#endif
