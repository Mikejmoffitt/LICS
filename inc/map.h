#ifndef MAP_H
#define MAP_H

/* Map.h
Support functions for rendering the map to plane A. 
Also handles some object list functions.
*/

#include <genesis.h>

#define MAP_NAME_SIZE 32
#define MAP_NUM_OBJS 128

#define MAP_SET_OUTSIDE1 0
#define MAP_SET_OUTSIDE2 1
#define MAP_SET_INSIDE1 2

#define MAP_OBJ_NULL 0
#define MAP_OBJ_SPAWN 1
#define MAP_OBJ_CUBE 2

// In the object list for the map, only the four main fields are defined
// The remaining twelve are used by the objects in-game, unpacked into the
// actual object list.

#define MAP_OBJ_LIST_SIZE (4 * 2);
// Each map object besides lyle is 32 bytes, or 16 words
#define MAP_OBJ_SIZE (2 * 16)


typedef struct map_obj map_obj;
struct map_obj
{
	u16 type; // Object ID 
	u16 data; // Depends on object type. Best example is entrance IDs.
	u16 x; // X in real coordinates (pixel-precise)
	u16 y; // Y in real coordinates (pixel-precise)
	u16 padding[MAP_OBJ_SIZE - (4 * 2)]; // Fill in space for 16 words
};

// Used in the map data structures - similar, but without game variables
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

// DMA a tileset and its associated palette into VRAM and CRAM
void map_load_tileset(u8 num);

// Return the map by its ID number
map_file *map_by_id(u8 num);

// Update columns on left or right sides of the screen
void map_draw_horizontal(u16 cam_x, u16 cam_y, u16 right_side);

// Update rows on top and bottom of the screen
void map_draw_vertical(u16 cam_x, u16 cam_y, u16 bottom_side);

// Draw the full screen for the current map at these camera coords
void map_draw_full(u16 cam_x, u16 cam_y);

// Commit DMA queue to VRAM
void map_dma(void);

// What is at this spot on the map (background)?
u16 map_collision(u16 x, u16 y);

#endif
