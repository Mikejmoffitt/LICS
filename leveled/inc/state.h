#ifndef STATE_H
#define STATE_H

#define SEL_SINGLE 0
#define SEL_FULL 1

#define WINDOW_MAP 0
#define WINDOW_VRAM 1
#define WINDOW_OBJ 2
#define WINDOW_META 3

// Drawing tiles with the brush
#define MODE_TILES 0

// Changing position and type of currently selected object
#define MODE_OBJECTS 1

extern unsigned int edit_mode;

extern unsigned int scroll_x;
extern unsigned int scroll_y;
extern unsigned int scroll_max_x;
extern unsigned int scroll_max_y;

extern int mouse_x;
extern int mouse_y;

extern unsigned int cursor_x;
extern unsigned int cursor_y;

extern unsigned int active_window;

extern unsigned int tile_sel;
extern unsigned int tile_snap;
extern unsigned int tile_src_size;
extern unsigned int tile_dest_size;

extern unsigned int tile_prio;
extern unsigned int tile_flip_v;
extern unsigned int tile_flip_h;


// Scrolling down the object listing
extern unsigned int obj_list_scroll;

// Which object, if any, is "active", 
extern unsigned int obj_list_sel;

// What to be placed
extern unsigned int obj_place_type;
extern unsigned int obj_place_data;

// For entering in hex data
extern int meta_cursor_pos;

void state_init(void);



#endif
