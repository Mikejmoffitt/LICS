#include "bg.h"
#include "gfx.h"
#include "pal.h"
#include "vramslots.h"

void bg_load(u16 num)
{
	VDP_doCRamDMA((u32)pal_bg1, 32 * BG_PALNUM, 16);
}

void bg_scroll(u16 camx, u16 camy)
{

}
