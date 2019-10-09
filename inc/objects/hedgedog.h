#ifndef HEDGEDOG_H
#define HEDGEDOG_H

#include <genesis.h>
#include "enemy.h"

#define HEDGEDOG_GROUNDED 0
#define HEDGEDOG_JUMPING 1

#define HEDGEDOG_HDELAY_MAX 2

// Structure defining specific object type
typedef struct en_hedgedog en_hedgedog;
struct en_hedgedog
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	fix16 dy;
	u16 state;
	u16 jump_cnt;
	u16 anim_cnt;
	u16 anim_frame;
	u16 did_shoot;
	u16 walk_direction;
	u16 h_delay;
};

// Initialize enemy at address e
void en_init_hedgedog(en_hedgedog *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_hedgedog(void);

#endif
