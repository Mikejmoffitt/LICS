// Sega CD Mode 1 support code for SGDK
// Michael Moffitt (https://github.com/mikejmoffitt)
// Adapted from Chilly Willy's Mode 1 CD Player

#ifndef CDAUDIO_H
#define CDAUDIO_H

int cdaudio_init(void);
void cdaudio_play_once(unsigned char trk);
void cdaudio_play_loop(unsigned char trk);
void cdaudio_stop(void);
void cdaudio_pause(void);
int cdaudio_is_active(void);

#endif
