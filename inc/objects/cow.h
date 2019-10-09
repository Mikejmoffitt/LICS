#ifndef COW_H
#define COW_H

#include <genesis.h>
#include "enemy.h"

#define COW_EATING 0
#define COW_FINISHED 1
#define COW_WALKING 2
#define COW_JUMPING 3
#define COW_ANGRY 4
#define COW_PREPARING 5

// Structure defining specific object type
typedef struct en_cow en_cow;
struct en_cow
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	fix16 dy;
	u16 state;
	u16 anger_count;
	u16 anim_frame;
	u16 anim_cnt;
	u16 walk_cnt; // For getting that 5px in 12 frame timing for NTSC
	u16 shot_cnt; // For angry mode
};

// Initialize enemy at address e
void en_init_cow(en_cow *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_cow(void);

#endif
