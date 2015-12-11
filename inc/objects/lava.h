#ifndef LAVA_H
#define LAVA_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_lava en_lava;
struct en_lava
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	en_generic *cow;
	u16 cow_search_counter;
	u16 anim_cnt;
	u16 anim_frame;
};

// Initialize enemy at address e
void en_init_lava(en_lava *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_lava(void);

#endif
