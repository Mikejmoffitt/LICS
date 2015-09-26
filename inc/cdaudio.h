// Sega CD Mode 1 support code for SGDK
// Michael Moffitt (https://github.com/mikejmoffitt)
// Adapted from Chilly Willy's Mode 1 CD Player

#ifndef CDAUDIO_H
#define CDAUDIO_H

#define SEGACD_REG_CPU 0xA12000
#define SEGACD_REG_MEM 0xA12002
#define SEGACD_REG_HINT 0xA12006
#define SEGACD_REG_STOPWATCH 0xA1200C
#define SEGACD_REG_COMM 0xA1200E

#define SEGACD_TRACK_W 0xA12010
#define SEGACD_PMODE_W 0xA12012

#define SEGACD_PROGRAM_ADDR 0x420000
#define SEGACD_PROGRAM_OFF 0x426000
#define SEGACD_PROGRAM_LEN 0x20000

#define SEGACD_CPU_IEN_MASK 0x8000

#include <genesis.h>

extern volatile uint8_t *segacd_bios_addr;

int32_t cdaudio_init(void);
void cdaudio_play_once(uint8_t trk);
void cdaudio_play_loop(uint8_t trk);
void cdaudio_stop(void);
void cdaudio_pause(void);
int32_t cdaudio_is_active(void);

#endif
