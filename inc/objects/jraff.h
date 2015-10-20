#ifndef JRAFF_H
#define JRAFF_H

#include <genesis.h>
#include "enemy.h"

#define JRAFF_HEIGHT 64
#define JRAFF_WIDTH 7

#define JRAFF_ANIM_DELAY 10
#define JRAFF_ANIM_LEN 3

typedef struct en_jraff en_jraff;
struct en_jraff
{
	en_header head;
	u16 anim_cnt;
	u16 anim_frame;
};

void en_init_jraff(en_jraff *e);
void en_unload_jraff(void);

#endif
