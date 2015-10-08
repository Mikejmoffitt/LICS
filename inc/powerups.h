#ifndef POWERUPS_H
#define POWERUPS_H

#define POWERUP_MAX_DY FIX16(6.0)
#define POWERUP_BOUNCE_SUB FIX16(-2.0)
#define POWERUP_GRAVITY FIX16(0.2)
#define POWERUP_CEILING_DY FIX16(1.0)
#define POWERUP_CREATION_DY FIX16(-3.0)

#define POWERUP_S_HEIGHT 8
#define POWERUP_S_WIDTH 4

#define POWERUP_L_HEIGHT 14
#define POWERUP_L_WIDTH 7

#define POWERUP_NULL  0
#define POWERUP_HP    1 
#define POWERUP_HP_2X 2 
#define POWERUP_CP    3
#define POWERUP_CP_2X 4
#define POWERUP_CPORB 5
#define POWERUP_HPORB 6

#include <genesis.h>

#define NUM_POWERUPS 8

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