#ifndef MAP_H
#define MAP_H

/* Map.h
Support functions for rendering the map to plane A. 
Also handles some object list functions.
*/

#include <genesis.h>

#define MAP_OBJ_NULL 0
#define MAP_OBJ_ENTRANCE 1
#define MAP_OBJ_CUBE 2
#define MAP_OBJ_METAGRUB 3
#define MAP_OBJ_FLIP 4
#define MAP_OBJ_BOINGO 5
#define MAP_OBJ_ITEM 6
#define MAP_OBJ_GAXTER1 7
#define MAP_OBJ_GAXTER2 8
#define MAP_OBJ_BUGGO1 9
#define MAP_OBJ_BUGGO2 10
#define MAP_OBJ_DANCYFLOWER 11
#define MAP_OBJ_JRAFF 12
#define MAP_OBJ_PILLA 13
#define MAP_OBJ_HEDGEDOG 14
#define MAP_OBJ_SHOOT 15
#define MAP_OBJ_LASER 16
#define MAP_OBJ_KILLZAM 17
#define MAP_OBJ_FLARGY 18
#define MAP_OBJ_PLANT 19
#define MAP_OBJ_TOSSMUFFIN 20
#define MAP_OBJ_TELEPORTER 21
#define MAP_OBJ_MAGIBEAR 22
#define MAP_OBJ_LAVA 23
#define MAP_OBJ_COW 24
#define MAP_OBJ_CONTAINER 25
#define MAP_OBJ_HOOP 26
#define MAP_OBJ_FALSEBLOCK 27
#define MAP_OBJ_CP_PAD 28
#define MAP_OBJ_CP_METER 29
#define MAP_OBJ_DOG 30
#define MAP_OBJ_ELEVATOR 31
#define MAP_OBJ_ELEVATOR_STOP 32
#define MAP_OBJ_FISSINS 33
#define MAP_OBJ_BOSS1 34
#define MAP_OBJ_BOSS2 35
#define MAP_OBJ_BOSSF1 36
#define MAP_OBJ_BOSSF2 37
#define MAP_OBJ_EGG 38

#define MAP_SET_OUTSIDE1 0
#define MAP_SET_OUTSIDE2 1
#define MAP_SET_INSIDE1 2
#define MAP_SET_SANDY1 3
#define MAP_SET_TELEPORTER 4

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

// DMA a tileset into VRAM; prepare for palette
void map_load_tileset(u8 num);

// Return the map by its ID number
map_file *map_by_id(u8 num);

// Update the screen as needed based on movement differences
void map_draw_diffs(u16 moved);

// Commit DMA queue to VRAM
void map_dma(void);

// Is this spot solid? (background)
u16 map_collision(u16 x, u16 y);

// Is this spot going to hurt me? (background)
u16 map_hurt(u16 x, u16 y);

#endif
