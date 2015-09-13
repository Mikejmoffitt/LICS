// mpad.h
// Megadrive gamepad reading routine

#ifndef MPAD_H
#define MPAD_H
#include <genesis.h>

#define PLAYER_1 0
#define PLAYER_2 1
#define PLAYER_3 2

#define KEY_UP 0x01
#define KEY_DOWN 0x02
#define KEY_LEFT 0x04
#define KEY_RIGHT 0x08
#define KEY_A 0x10
#define KEY_B 0x20
#define KEY_C 0x40
#define KEY_START 0x80

#define VDP_GAMEPAD 0xA10003

// Returns a bitfield - SCBARLDU (EX: bit0, LSB, is 1 when "up" is held);
u8 pad_read(u16 pl);

#endif

