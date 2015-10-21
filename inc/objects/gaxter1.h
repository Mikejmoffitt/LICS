#ifndef GAXTER1_H
#define GAXTER1_H

#include <genesis.h>
#include "enemy.h"

#define GAXTER1_DEADZONE 20
#define GAXTER1_HP 2
#define GAXTER1_WIDTH 6
#define GAXTER1_HEIGHT 10

typedef struct en_gaxter1 en_gaxter1;
struct en_gaxter1
{
	en_header head;
	fix16 dx;
	fix16 dy;
	u16 anim_cnt;
};

void en_init_gaxter1(en_gaxter1 *e);
void en_unload_gaxter1(void);

#endif
