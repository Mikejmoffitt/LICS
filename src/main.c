#include <genesis.h>
#include "mpad.h"
#include "sprites.h"
#include "player.h"
#include "system.h"
#include "gfx.h"
#include "pal.h"
#include "map.h"
#include "mapdata.h"
#include "vramslots.h"
#include "state.h"

void player_test(void)
{
	player pl;
	state_load_room(1);
	for (int i = 0; i < 28; i++)
	{

		VDP_doVRamDMA((state.current_map + (80 * 5) * (i + 2)),VDP_getAPlanAddress() + 128 * i,40);
	}

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

		state_update_scroll(fix32ToInt(pl.x),fix32ToInt(pl.y));
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
