#ifndef PROJECTILES_H
#define PROJECTILES_H

#define PROJECTILES_NUM 16

#define PROJECTILE_BALL 0
#define PROJECTILE_SPIKE 1
#define PROJECTILE_SPARK 2
#define PROJECTILE_DEATHBALL 3
#define PROJECTILE_BALL2 4

// Projectile "radius"
#define PROJECTILE_CHK 3

#include <genesis.h>

typedef struct projectile projectile;
struct projectile
{
	fix32 x;
	fix32 y;
	fix16 dx;
	fix16 dy;
	u16 active;
	u16 type;
};

void projectiles_dma_tiles(void);
void projectiles_init(void);
void projectiles_run(void);
void projectiles_draw(void);
void projectile_shoot(s16 x, s16 y, fix16 dx, fix16 dy, u16 type);
void projectile_shoot_at(s16 x, s16 y, s16 tx, s16 ty);

#endif
