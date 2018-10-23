#ifndef KILLZAM_H
#define KILLZAM_H

#include <genesis.h>
#include "enemy.h"

#define KILLZAM_UP 0
#define KILLZAM_DOWN 1

// Structure defining specific object type
typedef struct en_killzam en_killzam;
struct en_killzam
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	fix16 dy;
	u16 y_orig;
	u16 v_dir;
	u16 anim_cnt;
	u16 anim_frame;
	u16 timer;
};

// Initialize enemy at address e
void en_init_killzam(en_killzam *e);

// Deallocate VRAM associated with this type of enemy
void en_unload_killzam(void);

#endif
