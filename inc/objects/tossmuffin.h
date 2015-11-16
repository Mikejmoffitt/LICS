#ifndef TOSSMUFFIN_H
#define TOSSMUFFIN_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_tossmuffin en_tossmuffin;
struct en_tossmuffin
{	
	// Header present in each drawable and processable enemy type
	en_header head;
};

// Initialize enemy at address e
void en_init_tossmuffin(en_tossmuffin *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_tossmuffin(void);

#endif
