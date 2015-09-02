#ifndef SAVE_H
#define SAVE_H

#include <genesis.h>

#define SAVE_MAGIC 0x1337
#define SAVE_NG 0xDEAD
#define SAVE_MAP_W 26
#define SAVE_MAP_H 12

#define SAVE_NUM_ORBS 10
#define SAVE_DEFAULT_HP 5

#define SAVE_MAP 0x00
#define SAVE_CUBE_LIFT 0x02
#define SAVE_CUBE_KICK 0x04
#define SAVE_CUBE_PHANTOM 0x08

typedef struct save_file save_file;
struct save_file
{
	u16 magic_0;
	// Which CP orbs have been collected
	u16 orbs[SAVE_NUM_ORBS];
	// How many CP orbs are on lyle
	u16 orbs_holding;
	// How many CP orbs have been deposited
	u16 orbs_deposited;
	u16 magic_1;
	// HP orbs collected
	u16 hp_orbs[SAVE_NUM_ORBS];
	u16 max_hp;
	// abilities gained
	u16 have_lift;
	u16 have_jump;
	u16 have_kick;
	u16 have_phantom;
	u16 have_fast_phantom;
	u16 have_cheap_phantom;
	u16 have_cp_regen;
	u16 have_double_phantom;
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
