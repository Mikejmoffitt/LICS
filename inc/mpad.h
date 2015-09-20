// mpad.h
// Megadrive gamepad reading routine

#ifndef MPAD_H
#define MPAD_H

#define PLAYER_1 0
#define PLAYER_2 1
#define PLAYER_3 2

#define KEY_UP 0x0001
#define KEY_DOWN 0x0002
#define KEY_LEFT 0x0004
#define KEY_RIGHT 0x0008
#define KEY_A 0x0040
#define KEY_B 0x0010
#define KEY_C 0x0020
#define KEY_START 0x0080
#define KEY_X 0x0400
#define KEY_Y 0x0200
#define KEY_Z 0x0100
#define KEY_MODE 0x0800
#define MPAD_6BUTTON 0x1000

#define VDP_GAMEPAD 0xA10003

// Returns
// (0 0 0 1 M X Y Z S A C B R L D U) or (0 0 0 0 0 0 0 0 S A C B R L D U)
// More or less just Chilly Willy's code
// I retyped it so I can know what's going on
extern unsigned short pad_read(unsigned short p);

#endif

