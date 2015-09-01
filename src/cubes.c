#include "cubes.h"
#include "vramslots.h"
#include "gfx.h"

void cube_dma_tiles(void)
{
	VDP_doVRamDMA(gfx_cubes, CUBE_VRAM_SLOT * 32 ,CUBE_VRAM_LEN * 16);
	// Cubes use Lyle's palette. No palette transfer here.
}

