#ifndef MUSIC_H
#define MUSIC_H

#include <genesis.h>

#define SFX_JUMP 0
#define SFX_WALK1 1
#define SFX_WALK2 2

void music_init(void);
void music_play(u16 num);

#endif
