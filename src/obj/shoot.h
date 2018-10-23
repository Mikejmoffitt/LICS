#ifndef SHOOT_H
#define SHOOT_H

#include <genesis.h>
#include "enemy.h"

#define SHOOT_DOWN 0
#define SHOOT_UP 1

#define SHOOT_DISTANCE 200
#define SHOOT_PROX 100

#define SHOOT_ANIM_LEN 20

#define SHOOT_WIDTH 11
#define SHOOT_HEIGHT 12

typedef struct en_shoot en_shoot;
struct en_shoot
{
	en_header head;
	fix16 dy; // Implemented as a schmitt trigger at +/0 3.33px/f
	// u16 h_cnt; // Movement only happens on transition to zero
	u16 h_rev_cnt;
	u16 v_dir;
	u16 anim_cnt;
	u16 anim_frame;
	u16 y_orig;
	u16 swoop_en;
};

void en_init_shoot(en_shoot *e);
void en_unload_shoot(void);

#endif
