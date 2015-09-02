#include "particles.h"
#include "state.h"
#include "sprites.h"
#include "system.h"

static particle particles[PARTICLES_NUM];

void particles_dma_tiles(void)
{

}

void particles_init(void)
{
	for (u16 i = 0; i < PARTICLES_NUM; i++)
	{
		particles[i].active = 0; 
	}
}

void particles_run(void)
{
	for (u16 i = 0; i < PARTICLES_NUM; i++)
	{
		if (particles[i].active)
		{
			particle *p = &particles[i];
			p->x += p->dx;
			p->y += p->dy;
			p->anim_cnt++;
			p->active--;
		}
	}
}

void particles_draw(void)
{
	for (u16 i = 0; i < PARTICLES_NUM; i++)
	{
		if (!particles[i].active)
		{
			continue;
		}
		particle *p = &particles[i];
		// TODO: Rip out test that just places tile 4 with actual graphics
		u16 cent = (p->type == PARTICLE_TYPE_EXPLOSION) ? 12 : 4;
		sprite_put(p->x - state.cam_x - cent, p->y - state.cam_y - cent, 0, 4 + p->anim_cnt);
	}
}

void particle_spawn(u16 x, u16 y, u16 type)
{
	for (u16 i = 0; i < PARTICLES_NUM; i++)
	{	
		if (!particles[i].active)
		{
			particles[i].x = x;
			particles[i].y = y;
			particles[i].type = type;
			particles[i].active = PARTICLE_LIFE_DEFAULT;
			particles[i].anim_cnt = 0;
			// Bad temporary random
			particles[i].dy = (system_osc / 2) % 2 ? -2 : 2;
			particles[i].dx = system_osc  % 2 ? -2 : 2;
			return;
		}
		
	}
}
