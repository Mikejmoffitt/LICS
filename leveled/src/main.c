#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <string.h>
#include "mdgfx.h"
#include "types.h"
#include "plane.h"
#include "display.h"

const char *get_file(char *path, char *title, char *patterns)
{
	ALLEGRO_FILECHOOSER *dialogue = al_create_native_file_dialog(
		path, title, patterns, ALLEGRO_FILECHOOSER_FILE_MUST_EXIST);
	al_show_native_file_dialog(display,dialogue);
	if (al_get_native_file_dialog_count(dialogue) == 0)
	{
		printf("Nothing selected...\n");
		return NULL;
	}

	char *ret_str = (char *)malloc(sizeof(char) * strlen(al_get_native_file_dialog_path(dialogue,0)) + 1);
	strncpy(ret_str,al_get_native_file_dialog_path(dialogue,0),255);
	al_destroy_native_file_dialog(dialogue);
	return ret_str;
}

int main(int argc, char **argv)
{
	al_init();
	if (!display_init())
	{
		return 0;
	}

	const char *tf_name;
	const char *pf_name;

	if (argc < 3)
	{
		printf("Usage: leveled [tiledata] [paldata]\nGoing interactive.\n");
		tf_name = get_file(".","Open tile data","*.bin;*.*");
		pf_name = get_file(".","Open palette data","*.pal;*.bin;*.*");
	}
	else
	{
		tf_name = argv[1];
		pf_name = argv[2];
	}

	if (tf_name == NULL || pf_name == NULL)
	{
		return 0;
	}
	printf("Using %s and %s\n",tf_name,pf_name);
	plane pl;
	plane_init(&pl);
	printf("Loading tileset...\n");
	plane_load_tileset(&pl,tf_name,pf_name);
	//free(pf_name);
	//free(tf_name);
	plane_create_data(&pl,2,2);

	plane_scroll_limits(&pl, &scroll_max_x, &scroll_max_y);

	while(!quit)
	{
		al_set_target_bitmap(main_buffer);
		display_handle_scroll();
		plane_draw_vram(&pl, VRAM_DRAW_X, VRAM_DRAW_Y);
		plane_draw_map(&pl, PLANE_DRAW_X, PLANE_DRAW_Y);
		plane_handle_mouse(&pl);
		display_update();
		display_handle_queue();
	}
}
