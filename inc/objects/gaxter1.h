#ifndef GAXTER1_H
#define GAXTER1_H

#include <genesis.h>
#include "enemy.h"

#define GAXTER1_ACCEL FIX16(0.1429)
#define GAXTER1_TOPSPEED FIX16(2.8571)
#define GAXTER1_DEADZONE 20
#define GAXTER1_HP 2
#define GAXTER1_WIDTH 6
#define GAXTER1_HEIGHT 10

#define GAXTER1_ANIM_T 9

typedef struct en_gaxter1 en_gaxter1;
struct en_gaxter1
{
	en_header head;
	fix16 dx;
	fix16 dy;
	u16 anim_cnt;
};

void en_init_gaxter1(en_gaxter1 *e);

#endif
