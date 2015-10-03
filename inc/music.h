#ifndef MUSIC_H
#define MUSIC_H

#include <genesis.h>

#define SFX_JUMP 0
#define SFX_WALK1 1
#define SFX_WALK2 2
#define SFX_CUBETOSS 3
#define SFX_CUBEBOUNCE 4
#define SFX_FIZZLE 5

void music_init(void);
void music_play(u16 num);
void playsound(u16 num);
void sfx_counters(void);

#endif
