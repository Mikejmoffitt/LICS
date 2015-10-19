#ifndef BUGGO_H
#define BUGGO_H

#include <genesis.h>
#include "enemy.h"

#define BUGGO_T1 0
#define BUGGO_T2 1

#define BUGGO_WIDTH 7
#define BUGGO_HEIGHT 8

#define BUGGO_DISTANCE 100

#define BUGGO_ANIM_T 20

#define BUGGO_HP_1 2
#define BUGGO_HP_2 1

// TODO: Find real values
#define BUGGO_SPIKE_SPEED FIX16(6.5)
#define BUGGO_SPARK_SPEED FIX16(3.0)

#define BUGGO_PLAYER_SENSE 80

#define BUGGO_SHOT_FIRE 70
#define BUGGO_SHOT_TEST 40

#define BUGGO_SPARK_T 120
#define BUGGO_SPIN_T 60

// movement speed denominator
#define BUGGO_H_CNT_MAX 3

typedef struct en_buggo en_buggo;
struct en_buggo
{
	en_header head;
	u16 type;
	u16 h_cnt;
	u16 h_rev_cnt;
	u16 anim_cnt; 
	u16 shot_clock; // Countdown until next shot
	u16 spin_cnt; // For buggo1, this just counts 0-70 always
};

void en_init_buggo(en_buggo *e, u16 type);

#endif
