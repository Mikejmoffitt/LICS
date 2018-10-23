#ifndef MAGIBEAR_H
#define MAGIBEAR_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_magibear en_magibear;
struct en_magibear
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	u16 move_cnt;
	u16 shot_cnt;
	u16 mouth_cnt;
	u16 anim_cnt;
	u16 anim_frame;
};

// Initialize enemy at address e
void en_init_magibear(en_magibear *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_magibear(void);

#endif
