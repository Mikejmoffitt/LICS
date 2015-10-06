#include "md_vdp.h"

static uint8_t reg_cache[VDP_NUM_REG];

void vdp_set_reg(uint8_t num, uint8_t val)
{
	volatile uint16_t *port = (uint16_t *)VDP_CONTROL_PORT;
	uint16_t write_v = (num << 8) | val;
	*port = write_v;
	
	reg_cache[num] = val;
}

void vdp_get_reg(uint8_t num)
{
	return reg_cache[num];
}
