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

extern volatile unsigned char *segacd_bios_addr;

int cdaudio_init(void);
void cdaudio_play_once(unsigned char trk);
void cdaudio_play_loop(unsigned char trk);
void cdaudio_stop(void);
void cdaudio_pause(void);
int cdaudio_is_active(void);

#endif
