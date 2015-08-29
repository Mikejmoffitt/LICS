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

	for (int i = 0; i < 32; i++)
	{

		VDP_doVRamDMA((mapf->map_data + 160 * (i + 4)),VDP_getAPlanAddress() + 128 * i,40);
	}


	int i = 0;

	//VDP_doVRamDMA(data,VDP_getAPlanAddress() / 2,5120);

	tileset_load_fg(TILESET_FG_OUTSIDE2);
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
