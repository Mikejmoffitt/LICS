#ifndef BOINGO_H
#define BOINGO_H

#include <genesis.h>
#include "enemy.h"

#define BOINGO_JUMP_TIME 20
#define BOINGO_GRAVITY FIX16(0.2)

#define BOINGO_STANDING 0
#define BOINGO_JUMPING 1

#define BOINGO_ANIM_SPEED_STAND 9
#define BOINGO_ANIM_SPEED_JUMP 2

#define BOINGO_AIR_W 7
#define BOINGO_AIR_H 23

#define BOINGO_GND_W 9
#define BOINGO_GND_H 15

typedef struct en_boingo en_boingo;
struct en_boingo
{
	en_header head;
	u16 jump_cnt; // Countdown to jump
	u16 anim_cnt;
	u16 anim_frame;
	u16 state;
	fix16 dy;
};

void en_init_boingo(en_boingo *e);

#endif
