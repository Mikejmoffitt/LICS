#include "particles.h"
#include "state.h"
#include "sprites.h"
#include "system.h"
#include "vramslots.h"

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
			p->x += p->dx / 2;
			p->y += p->dy / 2;
			if (system_osc % 2)
			{
				p->x += p->dx & 1;
				p->y += p->dy & 1;
			}
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
		s16 tx = p->x - state.cam_x - cent;
		s16 ty = p->y - state.cam_y - cent;
		if (tx < -16 || tx > 320 || ty < -16 || ty > 240)
		{
			continue;
		}
		sprite_put(tx, ty, 0, TILE_ATTR_FULL(PARTICLES_PALNUM, 1, 0, 0, PLAYER_VRAM_SLOT + p->anim_cnt));
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
			particles[i].dy = (((system_osc) + GET_HVCOUNTER) % 4) - 2;
			particles[i].dx = (GET_HVCOUNTER % 4) - 2;
			// Don't let a vector element be zero
			if (particles[i].dy >= 0)
			{
				particles[i].dy++;
			}
			if (particles[i].dx >= 0)
			{
				particles[i].dx++;
			}
			return;
		}
		
	}
}
