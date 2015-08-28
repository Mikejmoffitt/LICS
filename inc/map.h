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

typedef struct map_obj map_obj;
struct map_obj
{
	u16 x; // X in real coordinates (pixel-precise)
	u16 y; // Y in real coordinates (pixel-precise)
	u16 type; // Object ID 
	u16 data; // Depends on object type. Best example is entrance IDs.
};


typedef struct map_file map_file;
struct map_file
{	
	// Room name (mostly for the editor)
	char name[MAP_NAME_SIZE];
	// Unique room identifier
	u16 id; 
	// Dimensions for the room in screens
	u16 w; 
	u16 h;

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
	map_obj objects[MAP_NUM_OBJS];

	// Starting point of map data. In truth, this will be greater than one
	// in size; this struct is just being used to make the raw map data 
	// easier to work with.
	u16 map_data[1]; 
};

void map_load_tileset(register num);
void map_load_palette(register num);

#endif
