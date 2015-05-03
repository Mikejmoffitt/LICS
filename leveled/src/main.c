#include <stdio.h>
#include <allegro5/allegro.h>
#include "mdgfx.h"
#include "types.h"
#include "plane.h"
#include "display.h"

int main(int argc, char **argv)
{
	al_init();
	if (!display_init())
	{
		return 0;
	}
	if (argc < 3)
	{
		printf("Usage: leveled [tiledata] [paldata]\n");
		display_shutdown();
		return 0;
	}

	plane pl;
	plane_init(&pl);
	plane_load_tileset(&pl,argv[1],argv[2]);
	plane_create_data(&pl,2,2);

	plane_scroll_limits(&pl, &scroll_max_x, &scroll_max_y);

	for (;;)
	{
		al_set_target_bitmap(main_buffer);
		display_handle_scroll();
		plane_draw_vram(&pl, VRAM_DRAW_X, VRAM_DRAW_Y);
		plane_draw_map(&pl, PLANE_DRAW_X, PLANE_DRAW_Y);
		display_update();
	}
}
