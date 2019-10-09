#ifndef SPOOKO_H
#define SPOOKO_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_spooko en_spooko;
struct en_spooko
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	u16 size;
};

// Initialize enemy at address e
void en_init_spooko(en_spooko *e, u16 size);

// Deallocate VRAM associated with this type of enemy
void en_unload_spooko(void);

#endif
