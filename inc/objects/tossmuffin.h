#ifndef TOSSMUFFIN_H
#define TOSSMUFFIN_H

#include <genesis.h>
#include "enemy.h"
#include "cubes.h"

// Structure defining specific object type
typedef struct en_tossmuffin en_tossmuffin;
struct en_tossmuffin
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	cube *holding_cube;
	u16 anim_frame;
	u16 anim_cnt;
	u16 lift_cnt;
	u16 toss_cnt;
	u16 walk_delay;
	u16 saw_player;
};

// Initialize enemy at address e
void en_init_tossmuffin(en_tossmuffin *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_tossmuffin(void);

#endif
