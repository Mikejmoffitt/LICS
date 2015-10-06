#include "md_dma.h"

void md_dma(uint8_t loc, uint32_t src, uint16_t dest, uint16_t len, int16_t step)
{
	volatile uint16_t *ctrl_w;
	volatile uint32_t *ctrl_l;

	u32 new_len;
	u32 bank_limit_b;
	u32 bank_limit_w;

	ctrl_w = (uint16_t *)VDP_CTRL_PORT;
	ctrl_l = (uint32_t *)VDP_CTRL_PORT;
	
	*ctrl_w = cont

}

void md_vram_fill(uint16_t dest, uint16_t len, uint8_t val, int16_t step)
{

}

void md_vram_copy(uint16_t src, uint16_t dest, uint16_t len)
{

}
