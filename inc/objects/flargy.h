#ifndef FLARGY_H
#define FLARGY_H

#include <genesis.h>
#include "enemy.h"

#define FLARGY_WALK_DISTANCE 19

// Structure defining specific object type
typedef struct en_flargy en_flargy;
struct en_flargy
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	u16 punch_cnt; // When non-zero, he is punching
	u16 h_cnt; // He will only walk when this is zero
	u16 walk_cnt;
	u16 anim_cnt;
	u16 anim_frame;
};

// Initialize enemy at address e
void en_init_flargy(en_flargy *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_flargy(void);

#endif
