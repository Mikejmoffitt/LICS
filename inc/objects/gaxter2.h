#ifndef GAXTER2_H
#define GAXTER2_H

#include <genesis.h>
#include "enemy.h"

#define GAXTER2_DOWN 0
#define GAXTER2_UP 1

#define GAXTER2_DISTANCE 50

#define GAXTER2_DDY FIX16(0.2)
#define GAXTER2_DY_CUTOFF FIX16(2.4)

#define GAXTER2_ANIM_LEN 20

#define GAXTER2_HP 3
#define GAXTER2_H_CNT_MAX 2
#define GAXTER2_WIDTH 6
#define GAXTER2_HEIGHT 8

#define GAXTER2_ANIM_T 9

#define GAXTER2_SHOT_FLICKER_T 70
#define GAXTER2_SHOT_FIRE_T 100

#define GAXTER2_SHOT_SPEED FIX16(2.2)

typedef struct en_gaxter2 en_gaxter2;
struct en_gaxter2
{
	en_header head;
	fix16 dy; // Implemented as a schmitt trigger at +/0 3.33px/f
	u16 h_cnt; // Movement only happens on transition to zero
	u16 h_rev_cnt;
	u16 v_dir;
	u16 anim_cnt;
	u16 y_orig;
	u16 shot_clock; // Countdown to firing 
};

void en_init_gaxter2(en_gaxter2 *e);


#endif
