#ifndef MAP_H
#define MAP_H

/* Map.h
Support functions for rendering the map to plane A. 
Also handles some object list functions.
*/

#include <stdint.h>

#define MAP_NAME_SIZE 32
#define MAP_NUM_OBJS 128

typedef struct map_obj map_obj;
struct map_obj
{
	uint16_t x; // X in real coordinates (pixel-precise)
	uint16_t y; // Y in real coordinates (pixel-precise)
	uint16_t type; // Object ID 
	uint16_t data; // Depends on object type. Best example is entrance IDs.
};


typedef struct map_file map_file;
struct map_file
{	
	// Room name (mostly for the editor)
	char name[MAP_NAME_SIZE];
	// Unique room identifier
	uint16_t id; 
	// Dimensions for the room in screens
	uint16_t w; 
	uint16_t h;

	// Position in top-left of game-wide map
	uint8_t map_x;
	uint8_t map_y;

	// Graphics tileset and palette to display with (enum)
	uint8_t tileset; 
	uint8_t tile_palette;

	// Palette for enemies, objects
	uint8_t sprite_palette;

	// Which background to choose from (enum); palette implied
	uint8_t background;

	// Large array of map objects for the object list (null terminated)
	map_obj objects[MAP_NUM_OBJS];

	// Starting point of map data. In truth, this will be greater than one
	// in size; this struct is just being used to make the raw map data 
	// easier to work with.
	uint16_t map_data[1]; 
};

#endif
