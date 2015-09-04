#ifndef PARTICLES_H
#define PARTICLES_H

#include <genesis.h>

#define PARTICLE_LIFE_DEFAULT 10
#define PARTICLE_LIFE_FIZZLE 11
#define PARTICLE_LIFE_SPARKLE 14
#define PARTICLE_LIFE_EXPLOSION 18

#define PARTICLE_TYPE_SPARKLE 0
#define PARTICLE_TYPE_FIZZLE 1
#define PARTICLE_TYPE_FIZZLERED 2
#define PARTICLE_TYPE_EXPLOSION 3

#define PARTICLES_NUM 16

typedef struct particle particle;
struct particle
{
	u16 x, y;
	s16 dx, dy;
	u16 active; // Countdown for object life
	u16 anim_cnt;
	u16 type;
};

// DMA particle graphics (they share lyle's palette)
void particles_dma_tiles(void);

// Clear out particles array
void particles_init(void);

// Go through particles, move them about, destroy them, etc.
void particles_run(void);

// Plot sprites for particles
void particles_draw(void);

// Spawn a particle at real-world location
void particle_spawn(u16 x, u16 y, u16 type);

#endif
