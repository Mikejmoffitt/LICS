#ifndef MUSIC_H
#define MUSIC_H

#include <genesis.h>

#define SFX_JUMP 0
#define SFX_WALK1 1
#define SFX_WALK2 2
#define SFX_CUBETOSS 3
#define SFX_CUBEBOUNCE 4
#define SFX_FIZZLE 5
#define SFX_CUBESPAWN 6
#define SFX_ENEMY_EXPLODE 7
#define SFX_ENEMY_STRIKE 8
#define SFX_PAUSE 9
#define SFX_CUBE_EXPLODE 10
#define SFX_HURT 11
#define SFX_CUBELIFT 12
#define SFX_POWERUP_GET 13
#define SFX_BOGOLOGO 14
#define SFX_ENEMY_JUMP 15
#define SFX_ENEMY_SHOOT 16
#define SFX_ELEVATOR_MOVE 17

void music_init(void);
void music_play(u16 num);
void playsound(u16 num);
void stopsound(void);
void sfx_counters(void);

#endif
