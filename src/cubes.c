#include "cubes.h"
#include "vramslots.h"
#include "gfx.h"
#include "sprites.h"
#include "state.h"
#include "system.h"

void cube_dma_tiles(void)
{
	VDP_doVRamDMA(gfx_cubes, CUBE_VRAM_SLOT * 32 ,CUBE_VRAM_LEN * 16);
	// Cubes use Lyle's palette. No palette transfer here.
}

void cube_draw(u16 x, u16 y, u16 type)
{
	u16 frame = TILE_ATTR_FULL(CUBE_PALNUM, 1, 0, 0, CUBE_VRAM_SLOT);
	switch (type & 0xF00)
	{
		case CUBE_PHANTOM:
			frame += 16 + (4 * ((system_osc / 4) % 4));
			break;
		case CUBE_YELLOW:
			frame += 4;
			break;
		case CUBE_RED:
			frame += 8;
			break;
		case CUBE_GREEN:
			frame += 12;
			break;
	}

	sprite_put(x - state.cam_x, y - state.cam_y, SPRITE_SIZE(2,2), frame);
}
