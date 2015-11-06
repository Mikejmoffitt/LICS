#include "state.h"

unsigned int edit_mode;

unsigned int scroll_x;
unsigned int scroll_y;
unsigned int scroll_max_x;
unsigned int scroll_max_y;

int mouse_x;
int mouse_y;

unsigned int cursor_x;
unsigned int cursor_y;

unsigned int active_window;

unsigned int tile_sel;
unsigned int tile_src_size;
unsigned int tile_dest_size;
unsigned int tile_prio;
unsigned int tile_flip_v;
unsigned int tile_flip_h;

// Scrolling down the object listing
unsigned int obj_list_scroll;

// Which object, if any, is "active", 
unsigned int obj_list_sel;

// What to be placed
unsigned int obj_place_type;
unsigned int obj_place_data;

int meta_cursor_pos;

void state_init(void)
{
	scroll_x = 0;
	scroll_y = 0;
	scroll_max_x = 40;
	scroll_max_y = 28;
	cursor_x = 0;
	cursor_y = 0;
	mouse_x = 0;
	mouse_y = 0;
	tile_src_size = SEL_FULL;
	tile_dest_size = SEL_SINGLE;
	tile_prio = 0;
	tile_flip_v = 0;
	tile_flip_h = 0;
	tile_sel = 0;
	obj_list_scroll = 0;
	obj_list_sel = 0;
	active_window = 0;
	meta_cursor_pos = -1;

}
