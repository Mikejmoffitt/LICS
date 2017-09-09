#ifndef MAP_H
#define MAP_H

/* Map.h
Support functions for rendering the map to plane A.
Also handles some object list functions.
*/

#include <genesis.h>

typedef enum mapset
{
	MAP_SET_OUTSIDE1,
	MAP_SET_OUTSIDE2,
	MAP_SET_INSIDE1,
	MAP_SET_SANDY1,
	MAP_SET_TELEPORTER,
	MAP_SET_OUTSIDE3,
	MAP_SET_PURPLEZONE,
	MAP_SET_ROOFTOP,
	MAP_SET_TECHNOZONE,

	MAP_SET_INVALID=255
} mapset_id;

#define MAP_SET_INVALID 255

#define MAP_NAME_SIZE 32
#define MAP_NUM_OBJS 128
#define MAP_NUM_ROOM_PTRS 16

// In the object list for the map, only the four main fields are defined

typedef struct map_list_obj map_list_obj;
struct map_list_obj
{
	u16 type; // Object ID
	u16 data; // Depends on object type. Best example is entrance IDs.
	u16 x; // X in real coordinates (pixel-precise)
	u16 y; // Y in real coordinates (pixel-precise)
};

typedef struct map_file map_file;
struct map_file
{
	// Room name (mostly for the editor)
	char name[MAP_NAME_SIZE];
	// Unique room identifier
	u8 music;
	u8 id;

	// Dimensions for the room in screens
	u8 ex1;
	u8 w;
	u8 ex2;
	u8 h;

	// Position in top-left of game-wide map
	u8 map_x;
	u8 map_y;

	// Graphics tileset and palette to display with (enum)
	u8 tileset;

	// Palette for enemies, objects
	u8 sprite_palette;

	// Which background to choose from (enum); palette implied
	u8 background;

	// Large array of map objects for the object list (null terminated)
	map_list_obj objects[MAP_NUM_OBJS];

	// Starting point of map data. In truth, this will be greater than one
	// in size; this struct is just being used to make the raw map data
	// easier to work with.
	u8 map_data[1];
};

// Clear map structures
void map_init(void);

// Which tilset is loaded?
u8 map_get_current_tileset(void);

// DMA a tileset into VRAM; prepare for palette
void map_load_tileset(u8 num);

// Return the map by its ID number
map_file *map_by_id(u8 num);

// Update the screen as needed based on movement differences
void map_draw_diffs(u16 moved);
void map_draw_full(u16 cam_x, u16 cam_y);
void map_draw_vertical(u16 cam_x, u16 cam_y, u16 bottom_side);
void map_draw_horizontal(u16 cam_x, u16 cam_y, u16 right_side);

// Commit DMA queue to VRAM
void map_dma(void);

// Show a map selection screen
void map_debug_chooser(void);

// Is this spot solid? (background)
u16 map_collision(u16 x, u16 y);

// Is this spot going to hurt me? (background)
u16 map_hurt(u16 x, u16 y);

#endif
