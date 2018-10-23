#ifndef TELEPORTER_H
#define TELEPORTER_H

#include <genesis.h>
#include "enemy.h"

// Structure defining specific object type
typedef struct en_teleporter en_teleporter;
struct en_teleporter
{	
	// Header present in each drawable and processable enemy type
	en_header head;
	u16 id;
	u16 activator;
	u16 anim_cnt;
	u16 anim_frame;
	u16 active_cnt; // On transition from 1 --> 0, teleporter is disabled
	u16 disabled;
};

// Initialize enemy at address e
void en_init_teleporter(en_teleporter *e, u16 data);

// Deallocate VRAM associated with this type of enemy
void en_unload_teleporter(void);

#endif
