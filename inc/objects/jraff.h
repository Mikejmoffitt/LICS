#ifndef JRAFF_H
#define JRAFF_H

#include <genesis.h>
#include "enemy.h"

#define JRAFF_HEIGHT 64
#define JRAFF_WIDTH 7

#define JRAFF_ANIM_DELAY 10
#define JRAFF_ANIM_LEN 3

#define JRAFF_GRAVITY FIX16(0.2)

#define JRAFF_MOVE_DIV 1

typedef struct en_jraff en_jraff;
struct en_jraff
{
	en_header head;
	fix16 dy;
	u16 anim_cnt;
	u16 anim_frame;
	u16 move_cnt;
};

void en_init_jraff(en_jraff *e);
void en_unload_jraff(void);

#endif
