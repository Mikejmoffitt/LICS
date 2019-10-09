#include "particles.h"
#include "state.h"
#include "sprites.h"
#include "system.h"
#include "vramslots.h"
#include "gfx.h"

static particle particles[PARTICLES_NUM];

void particles_dma_tiles(void)
{
	VDP_doVRamDMA((u32)gfx_particles,32 * PARTICLES_VRAM_SLOT, 16 * PARTICLES_VRAM_LEN);
	// Particles use Lyle's palette. No palette transfer here either.
}

void particles_init(void)
{
	u16 i = PARTICLES_NUM;
	while(i--)
	{
		particles[i].active = 0;
	}
}

void particles_run(void)
{
	u16 i = PARTICLES_NUM;
	while(i--)
	{
		if (particles[i].active)
		{
			particle *p = &particles[i];
			if (p->type != PARTICLE_TYPE_SPARKLE)
			{
				p->x += p->dx / 2;
				p->y += p->dy / 2;

				if (system_osc % 2)
				{
					p->x += p->dx & 1;
					p->y += p->dy & 1;
				}
			}
			p->anim_cnt++;
			p->active--;
		}
	}
}

void particles_draw(void)
{
	u16 i = PARTICLES_NUM;
	while(i--)
	{
		if (!particles[i].active)
		{
			continue;
		}
		particle *p = &particles[i];
		u16 gfx;
		u16 cent;
		u16 size;
		switch (p->type)
		{
			default:
			case PARTICLE_TYPE_SPARKLE:
				cent = 8;
				if (p->active > 16)
				{
					gfx = PARTICLES_VRAM_SLOT + 12;
				}
				else if (p->active > 12)
				{
					gfx = PARTICLES_VRAM_SLOT;
				}
				else if (p->active > 8)
				{
					gfx = PARTICLES_VRAM_SLOT + 4;
				}
				else if (p->active > 4)
				{
					gfx = PARTICLES_VRAM_SLOT + 8;
				}
				else
				{
					gfx = PARTICLES_VRAM_SLOT + 12;
				}
				size = SPRITE_SIZE(2,2);
				break;
			case PARTICLE_TYPE_FIZZLE:
				cent = 8;
				if (p->active > 12)
				{
					gfx = PARTICLES_VRAM_SLOT + 16;
				}
				else if (p->active > 8)
				{
					gfx = PARTICLES_VRAM_SLOT + 20;
				}
				else if (p->active > 4)
				{
					gfx = PARTICLES_VRAM_SLOT + 24;
				}
				else
				{
					gfx = PARTICLES_VRAM_SLOT + 28;
				}
				size = SPRITE_SIZE(2,2);
				break;

			case PARTICLE_TYPE_FIZZLERED:
				cent = 8;
				if (p->active > 12)
				{
					gfx = PARTICLES_VRAM_SLOT + 32;
				}
				else if (p->active > 8)
				{
					gfx = PARTICLES_VRAM_SLOT + 36;
				}
				else if (p->active > 4)
				{
					gfx = PARTICLES_VRAM_SLOT + 40;
				}
				else
				{
					gfx = PARTICLES_VRAM_SLOT + 44;
				}
				size = SPRITE_SIZE(2,2);
				break;

			case PARTICLE_TYPE_EXPLOSION:
				if ((p->active > 18) || (p->active < 16 && p->active > 12))
				{
					gfx = PARTICLES_VRAM_SLOT + 52;
					size = SPRITE_SIZE(3,3);
					cent = 12;
				}
				else if ((p->active > 15 && p->active < 19) || (p->active > 6 && p->active < 10))
				{
					gfx = PARTICLES_VRAM_SLOT + 48;
					size = SPRITE_SIZE(2,2);
					cent = 8;
				}
				else
				{
					gfx = PARTICLES_VRAM_SLOT + 61;
					size = SPRITE_SIZE(4,4);
					cent = 16;
				}
				break;
			case PARTICLE_TYPE_SAND:
				cent = 4;
				if (p->active >= 8)
				{
					gfx = PARTICLES_VRAM_SLOT + 77;
				}
				else if (p->active >= 6)
				{
					gfx = PARTICLES_VRAM_SLOT + 78;
				}
				else if (p->active >= 4)
				{
					gfx = PARTICLES_VRAM_SLOT + 77;
				}
				else if (p->active >= 2)
				{
					gfx = PARTICLES_VRAM_SLOT + 79;
				}
				else
				{
					gfx = PARTICLES_VRAM_SLOT + 80;
				}
				size = SPRITE_SIZE(1,1);
		}
		s16 tx = p->x - state.cam_x - cent;
		s16 ty = p->y - state.cam_y - cent;
		if (tx < -32 || tx > 336 || ty < -32 || ty > 256)
		{
			continue;
		}
		u16 palnum;
		if (p->type != PARTICLE_TYPE_SAND && p->type != PARTICLE_TYPE_FIZZLERED && p->type != PARTICLE_TYPE_EXPLOSION)
		{
			palnum = PARTICLES_ALT_PALNUM;
		}
		else
		{
			palnum = PARTICLES_PALNUM;
		}
		sprite_put(tx, ty, size, TILE_ATTR_FULL(palnum, 0, 0, 0, gfx));
	}
}

void particle_spawn(u16 x, u16 y, u16 type)
{
	u16 i = PARTICLES_NUM;
	while(i--)
	{
		if (!particles[i].active)
		{
			particles[i].x = x;
			particles[i].y = y;
			particles[i].type = type;
			switch (type)
			{
				case PARTICLE_TYPE_SPARKLE:
					particles[i].active = 20;
					break;
				case PARTICLE_TYPE_FIZZLE:
				case PARTICLE_TYPE_FIZZLERED:
					particles[i].active = 16;
					break;
				case PARTICLE_TYPE_EXPLOSION:
					particles[i].active = 21;
					break;
				case PARTICLE_TYPE_SAND:
					particles[i].active = 20;
			}
			particles[i].anim_cnt = 0;
			if (type == PARTICLE_TYPE_SPARKLE)
			{
				particles[i].x += (((system_osc) + GET_HVCOUNTER) % 16) - 8;
				particles[i].y += (GET_HVCOUNTER % 16) - 8;
			}
			else
			{
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

				if (type == PARTICLE_TYPE_SAND)
				{
					particles[i].dy -= 2;
				}
			}
			return;
		}

	}
}
