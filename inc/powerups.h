#ifndef POWERUPS_H
#define POWERUPS_H

#include <genesis.h>

#define NUM_POWERUPS 8


#define POWERUP_S_HEIGHT 8
#define POWERUP_S_WIDTH 4

#define POWERUP_L_HEIGHT 14
#define POWERUP_L_WIDTH 7

#define POWERUP_NULL  0
#define POWERUP_HP    1
#define POWERUP_CP    2
#define POWERUP_HP_2X 3
#define POWERUP_CP_2X 4
#define POWERUP_CPORB 5
#define POWERUP_HPORB 6

typedef struct powerup powerup;
struct powerup
{
	u16 type;
	u16 num; // For CP / HP orb identification
	s16 x;
	s16 y;
	fix16 dy;
};

void powerup_init(void);
void powerup_dma_tiles(void);
void powerup_spawn(s16 x, s16 y, u16 type, u16 num);
void powerup_run(void);
void powerup_draw(void);


#endif
