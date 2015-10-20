#ifndef FLIP_H
#define FLIP_H

#include <genesis.h>
#include "enemy.h"

#define FLIP_DOWN 0
#define FLIP_UP 1

#define FLIP_DISTANCE 100

#define FLIP_DDY FIX16(0.2)
#define FLIP_DY_CUTOFF FIX16(2.4)

#define FLIP_ANIM_LEN 20

#define FLIP_H_CNT_MAX 2
#define FLIP_WIDTH 11
#define FLIP_HEIGHT 12

typedef struct en_flip en_flip;
struct en_flip
{
	en_header head;
	fix16 dy; // Implemented as a schmitt trigger at +/0 3.33px/f
	u16 h_cnt; // Movement only happens on transition to zero
	u16 h_rev_cnt;
	u16 v_dir;
	u16 anim_cnt;
	u16 y_orig;
};

void en_init_flip(en_flip *e);
void en_unload_flip(void);

#endif
