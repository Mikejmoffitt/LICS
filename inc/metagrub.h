#ifndef METAGRUB_H
#define METAGRUB_H

#include <genesis.h>
#include "enemy.h"

typedef struct en_metagrub en_metagrub;
struct en_metagrub
{
	en_header head;
	u16 move_cnt; // When == 1, metagrub lurches forwards 
	fix16 dx;
};

// Clear out variables as needed
void en_init_metagrub(en_metagrub *e);

// Calculate tile attributes, sprite size, and x/y draw offsets
void en_anim_metagrub(en_metagrub *e);

// Process enemy logic for one frame
void en_proc_metagrub(en_metagrub *e);
#endif
