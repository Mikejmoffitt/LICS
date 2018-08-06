#ifndef BOSS1_H
#define BOSS1_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_boss1 en_boss1;
struct en_boss1
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	fix16 dy;
	u16 anim_seq;
	u16 anim_counter;
	u16 phase_timer;
};

// Initialize enemy at address e
void en_init_boss1(en_boss1 *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_boss1(void);

#endif
