#include "save.h"

save_file sram;

static void save_commit(void)
{
	SRAM_enable();
	u16 *sf = (u16 *)&sram;
	int i = (sizeof(save_file) / sizeof(u16));
	while (i--)
	{
		SRAM_writeWord(i * 2, sf[i]);
	}
	SRAM_disable();
}

void save_write(void)
{
	sram.fresh_save = 0;
	save_commit();
}

void save_load(void)
{
	SRAM_enableRO();
	u16 *sf = (u16 *)&sram;
	int i = (sizeof(save_file) / sizeof(u16));
	while (i--)
	{
		sf[i] = SRAM_readWord(i * 2);
	}

	SRAM_disable();

	// Save reality check failed. Clear SRAM to start fresh.
	if (sram.magic_0 != SAVE_MAGIC || sram.magic_1 != SAVE_MAGIC || sram.magic_2 != SAVE_MAGIC)
	{
		save_clear();
	}
}

void save_clear(void)
{
	u16 *sf = (u16 *)&sram;
	int j;
	int i = (sizeof(save_file) / sizeof(u16));
	// Just-in-case full zero of SRAM
	while (i--)
	{
		sf[i] = 0;
	}

	// Set magic numbers for save validity check
	sram.magic_0 = SAVE_MAGIC;
	sram.magic_1 = SAVE_MAGIC;
	sram.magic_2 = SAVE_MAGIC;

	// Reset abilities
	sram.max_hp = SAVE_DEFAULT_HP;

	sram.have_map = 0;
	sram.have_lift = 0;
	sram.have_jump = 0;
	sram.have_phantom = 0;
	sram.have_fast_phantom = 0;
	sram.have_cheap_phantom = 0;
	sram.have_kick = 0;
	sram.have_orange = 0;
	sram.touched_cube = 0;
	sram.beat_boss1 = 0;
	sram.beat_boss2 = 0;
	sram.beat_boss3 = 0;

	sram.fresh_save = 1;

	// Clear map exploration
	for (j = 0; j < SAVE_MAP_H; j++)
	{
		for (i = 0; i < SAVE_MAP_W; i++)
		{
			sram.map[j][i] = 0;
		}
	}

	// Clear orb collection
	for (i = 0; i < SAVE_NUM_ORBS; i++)
	{
		sram.cp_orbs_taken[i] = 0;
		sram.hp_orbs_taken[i] = 0;
	}

	sram.cp_orbs_spent = 0;
	sram.cp_orbs_have = 0;

	// Reset options to defaults
	sram.opt_interlace = SAVE_OPT_INTERLACE_NORMAL;
	sram.opt_ctrlscheme = SAVE_OPT_CTRL_NORMAL;
	sram.opt_mus = 0;
	sram.opt_sfx = 0;

	save_commit();
}
