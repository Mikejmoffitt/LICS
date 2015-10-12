// save.h
// routines for loading and writing to the save information stored in SRAM

#ifndef SAVE_H
#define SAVE_H

#include <genesis.h>
#include <sram.h>

#define SAVE_MAGIC 0x1337
#define SAVE_NG 0xDEAD
#define SAVE_MAP_W 25
#define SAVE_MAP_H 12

#define SAVE_NUM_ORBS 16
#define SAVE_DEFAULT_HP 5

#define SAVE_MAP 0x00
#define SAVE_CUBE_LIFT 0x02
#define SAVE_CUBE_KICK 0x04
#define SAVE_CUBE_PHANTOM 0x08

#define SAVE_OPT_CTRL_NORMAL 0
#define SAVE_OPT_CTRL_SWAP 1

#define SAVE_OPT_INTERLACE_NORMAL 0
#define SAVE_OPT_INTERLACE_ENABLED 1

typedef struct save_file save_file;
struct save_file
{
	u16 magic_0;
	// Which CP orbs have been collected
	u16 cp_orbs_taken[SAVE_NUM_ORBS];
	// How many CP orbs are on lyle
	u16 cp_orbs_have;
	// How many CP orbs have been deposited
	u16 cp_orbs_spent;
	u16 magic_1;
	// HP orbs collected
	u16 hp_orbs_taken[SAVE_NUM_ORBS];
	u16 max_hp;
	// abilities gained
	u16 have_map;
	u16 have_lift;
	u16 have_jump;
	u16 have_kick;
	u16 have_phantom;
	u16 have_fast_phantom;
	u16 have_cheap_phantom;
	u16 have_double_phantom;
	u16 have_orange;
	// Map reveal info
	u16 map[SAVE_MAP_H][SAVE_MAP_W];
	u16 magic_2;

	// Settings
	u16 opt_interlace;
	u16 opt_mus;
	u16 opt_sfx;
	u16 opt_ctrlscheme;
};

extern save_file sram;

// Commit save data to SRAM
void save_write(void);

// Load into save file. Will load a new save if the data is invalid.
void save_load(void);

// Populate save with new game information.
void save_clear(void);

#endif
