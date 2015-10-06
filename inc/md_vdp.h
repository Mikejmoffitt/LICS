#ifndef MD_VDP_H
#define MD_VDP_H

#define VDP_DATA_PORT 0xC00000
#define VDP_HV_COUNTER 0xC00008
#define VDP_CTRL_PORT 0xC00004
#define VDP_PSG 0xC00011
#define VDP_DEBUG_REG 0xC0001C

#include <stdint.h>

#define VDP_NUM_REG 0x13

// Directly set a VDP register
void vdp_set_reg(uint8_t num, uint8_t val);
// Read back the value last written to the VDP register
void vdp_get_reg(uint8_t num);

#endif
