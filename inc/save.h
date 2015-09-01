#ifndef SAVE_H
#define SAVE_H

#include <genesis.h>

#define SAVE_MAGIC 0x1337
#define SAVE_NG 0xDEAD
#define SAVE_MAP_W 26
#define SAVE_MAP_H 12

typedef struct save_file save_file;
struct save_file
{
	u16 magic_0;
	// Which orbs have been collected
	u16 orbs[16];
	u16 magic_1;
	// Bitfield for abilities gained, if needed (I think orbs decide this)
	u16 abilities;
	// Map reveal info
	u16 map[SAVE_MAP_H][SAVE_MAP_W];
	u16 magic_2;
};

extern save_file sram;

// Commit save data to SRAM
void save_write(void);

// Load into save file. Will load a new save if the data is invalid.
void save_load(void);

// Populate save with new game information.
void save_clear(void);

#endif
