#ifndef MAP_H
#define MAP_H

/* Map.h
Support functions for rendering the map to plane A. 
Also handles some object list functions.
*/

#include <genesis.h>

#define MAP_NAME_SIZE 32
#define MAP_NUM_OBJS 128

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
	char name[MAP_NAME_SIZE];
	u16 room_num; // Unique room identifier
	u16 w; // Width of map in tiles
	u16 h; // Height of map in tiles
	u16 size; // Map size in 16-bit words, pre-calculated
	u8 map_x;
	u8 map_y;

	map_obj objects[MAP_NUM_OBJS];

	u8 tileset; 
	u8 palette;
	u8 background;
	u8 other;
	u16 map_data; // This will be abused based on the map dimensions. A real file will go longer than this.


};

#endif
