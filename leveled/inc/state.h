#ifndef STATE_H
#define STATE_H


#define MODE_TILES 0
#define MODE_OBJECTS 1
#define MODE_OBJDATA 2

extern unsigned int scroll_x;
extern unsigned int scroll_y;
extern unsigned int scroll_max_x;
extern unsigned int scroll_max_y;

extern int mouse_x;
extern int mouse_y;

extern unsigned int cursor_x;
extern unsigned int cursor_y;

extern unsigned int active_window;

extern unsigned int selection;
extern unsigned int sel_size;

extern unsigned int edit_mode;

// Scrolling down the object listing
extern unsigned int obj_list_scroll;

// Which object, if any, is "active"
extern unsigned int obj_list_selected;

// What to be placed
extern unsigned int obj_place_type;
extern unsigned int obj_place_data;



#endif
