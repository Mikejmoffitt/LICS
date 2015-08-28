#include <genesis.h>
#include "mpad.h"
#include "sprites.h"
#include "player.h"
#include "system.h"
#include "gfx.h"
#include "pal.h"
#include "map.h"
#include "tileset.h"
#include "mapdata.h"
#include "vramslots.h"

void player_test(void)
{
	player pl;
	map_load_tileset(0);

	map_file *mapf = &mapdata_testroom[0];

	u16 *gb = (&mapdata_testroom[0] + sizeof(map_file) - 2);
	VDP_doVRamDMA(gb,VDP_getAPlanAddress(),64);

	tileset_load_fg(TILESET_FG_OUTSIDE1);
	player_init(&pl);	
	pl.y = 64;
	pl.x = intToFix32(64);
	for (;;)
	{
		player_eval_grounded(&pl);
		player_input(&pl);
		player_cp(&pl);
		player_accel(&pl);
		player_jump(&pl);
		player_move(&pl);

		player_eval_grounded(&pl);
		player_calc_anim(&pl);
		
		player_draw(&pl);
		
		system_wait_v();
		
		player_dma(&pl);
		sprites_dma_simple();
	}
}

int main(void)
{
	system_init();

	player_test();
	return 0;	
}
