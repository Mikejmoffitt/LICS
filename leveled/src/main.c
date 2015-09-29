#include <stdio.h>
#include <allegro5/allegro.h>
#include <string.h>
#include "mdgfx.h"
#include "plane.h"
#include "display.h"
#include "state.h"

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("Usage: leveled mapfile\n");
		return 0;
	}
	else
	{
		map_fname = argv[1];
	}
	map_load();
	printf("[main] Done with map loading, making display\n");
	al_init();
	if (!display_init())
	{
		return 0;
	}
	state_init();

// Only once the map has been loaded or created can we begin to use plane

	plane_init();
	plane_load_fg();
	plane_scroll_limits(&scroll_max_x, &scroll_max_y);

	while(!quit)
	{
		al_set_target_bitmap(main_buffer);
		plane_draw_vram(VRAM_DRAW_X, VRAM_DRAW_Y);
		plane_draw_map(PLANE_DRAW_X, PLANE_DRAW_Y);
		plane_draw_object_list(OBJ_DRAW_X, OBJ_DRAW_Y);
		plane_draw_meta(META_DRAW_X, META_DRAW_Y);
		plane_handle_mouse();
		plane_handle_io();
		display_update();
		display_handle_queue();
	}
}
