#ifndef MAP_H
#define MAP_H

#include <allegro5/allegro.h>
#include <stdlib.h>
#include <stdio.h>
#include "plane.h"

#define MAP_SET_OUTSIDE1 0
#define MAP_SET_OUTSIDE2 1
#define MAP_SET_INSIDE1 2
#define MAP_SET_SANDY1 3
#define MAP_SET_TELEPORTER 4

#define MAP_NAME_SIZE 32
#define MAP_NUM_OBJS 128

#define MAP_WIDTH 40
#define MAP_HEIGHT 30
#define MAP_NAME_LEN 32
#define MAP_MAX_WIDTH 24
#define MAP_MAX_HEIGHT 24

#define OBJ_NULL 0
#define OBJ_ROOMPTR 1
#define OBJ_CUBE 2

// ROOM POINTER NOTES
// Lowest nybble is the room pointer ID  
// Seocnd-lowest nybble is the room pointer ID to point to
// High byte is the room to point to

/* -------------- Object Types --------------

0x0000 NULL
0x0001 EXIT
0x0002 CUBE
0x0003 

*/

typedef struct map_obj map_obj;
struct map_obj
{
	uint16_t type;
	uint16_t data;
	uint16_t x;
	uint16_t y;
};

// Abstraction of map file header
typedef struct map_file map_file;
struct map_file
{
	char name[MAP_NAME_SIZE];

	// Room identifier, unique to this one
	uint8_t music;
	uint8_t id;

	// Dimensions in 320x240 segments
	uint8_t ex1;
	uint8_t w; // w * 40 columns
	uint8_t ex2;
	uint8_t h; // h * 32 rows

	// Position for the top-left of this room on the game map
	uint8_t map_x;
	uint8_t map_y;

	// Which graphics/palette set to show
	uint8_t tileset;

	// Palette set for objects (pal 3)
	uint8_t sprite_palette;

	// Background choice (enum)
	uint8_t background;

	// Big object list
	map_obj objects[MAP_NUM_OBJS];
};

extern const char *map_fname;
extern uint16_t *map_data;
extern map_file map_header;

void map_data_interview(void);
void map_new(void);
void map_save(void);
void map_load(void);

#endif
