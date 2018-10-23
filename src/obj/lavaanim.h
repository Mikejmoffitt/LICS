#ifndef LAVAANIM_H
#define LAVAANIM_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_lavaanim en_lavaanim;
struct en_lavaanim
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	u16 anim_cnt;
	u16 anim_frame;
};

// Initialize enemy at address e
void en_init_lavaanim(en_lavaanim *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_lavaanim(void);

#endif
