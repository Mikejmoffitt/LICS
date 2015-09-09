#include "save.h"

save_file sram;

void save_write(void)
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
	if (sram.magic_0 != SAVE_MAGIC || sram.magic_1 != SAVE_MAGIC || sram.magic_2 != SAVE_MAGIC)
	{
		save_clear();
	}
}

void save_clear(void)
{
	u16 *sf = (u16 *)&sram;
	int i = (sizeof(save_file) / sizeof(u16));
	while (i--)
	{
		sf[i] = 0;
	}	
	sram.magic_0 = SAVE_MAGIC;
	sram.magic_1 = SAVE_MAGIC;
	sram.magic_2 = SAVE_MAGIC;

	sram.max_hp = SAVE_DEFAULT_HP;
	sram.have_phantom = 1;
	sram.have_fast_phantom = 1;
	sram.have_cheap_phantom = 1;
	sram.have_jump = 1;
	sram.have_lift = 1;

	sram.opt_interlace = SAVE_OPT_INTERLACE_NORMAL;
	sram.opt_ctrlscheme = SAVE_OPT_CTRL_NORMAL;
	sram.opt_mus = 0;
	sram.opt_sfx = 0;

	save_write();
}
