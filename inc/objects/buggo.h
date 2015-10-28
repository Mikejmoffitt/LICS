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

#define BUGGO_PLAYER_SENSE 80

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
void en_unload_buggo(void);

#endif
