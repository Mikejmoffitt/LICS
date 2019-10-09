#ifndef VRAMSLOTS_H
#define VRAMSLOTS_H
/*

This is where VRAM slots are determined for everything in the game for some
minimal amount of static memory management.

VRAM_SLOT indeces are specified in tiles. Each tile is 32 bytes, but the VDP
routines disregard the LSB so an increment of 1 is moving across an entire
16-bit word. So, 64 bytes in would be address 32, not address 64. This is why
the length of a 256-tile DMA is only (256 * 16).

*/

// In-game VRAM slots

#define MAP_FG_VRAM_SLOT 0
#define MAP_FG_VRAM_LEN 256

#define BG_VRAM_SLOT (MAP_FG_VRAM_SLOT + MAP_FG_VRAM_LEN)
#define BG_VRAM_LEN 128

#define PAUSE_VRAM_SLOT (BG_VRAM_SLOT + BG_VRAM_LEN)
#define PAUSE_VRAM_LEN 256

#define PLAYER_VRAM_SLOT (PAUSE_VRAM_SLOT + PAUSE_VRAM_LEN)
#define PLAYER_VRAM_LEN 16

#define HUD_VRAM_SLOT (PLAYER_VRAM_SLOT + PLAYER_VRAM_LEN)
#define HUD_VRAM_LEN 32

#define CUBE_VRAM_SLOT (HUD_VRAM_SLOT + HUD_VRAM_LEN)
#define CUBE_VRAM_LEN 52

#define PARTICLES_VRAM_SLOT (CUBE_VRAM_SLOT + CUBE_VRAM_LEN)
#define PARTICLES_VRAM_LEN 81

#define POWERUP_VRAM_SLOT (PARTICLES_VRAM_SLOT + PARTICLES_VRAM_LEN)
#define POWERUP_VRAM_LEN 24

#define PROJECTILES_VRAM_SLOT (POWERUP_VRAM_SLOT + POWERUP_VRAM_LEN)
#define PROJECTILES_VRAM_LEN 29

#define ENEMY_VRAM_START (PROJECTILES_VRAM_SLOT + PROJECTILES_VRAM_LEN)

// Many level objects - length shown in number of tiles 
#define ITEMS_VRAM_LEN 48
#define METAGRUB_VRAM_LEN 7
#define FLIP_VRAM_LEN 12
#define BOINGO_VRAM_LEN 60
#define GAXTER_VRAM_LEN 24
#define BUGGO_VRAM_LEN 48
#define DANCYFLOWER_VRAM_LEN 30
#define JRAFF_VRAM_LEN 84
#define PILLA_VRAM_LEN 24
#define HEDGEDOG_VRAM_LEN 43
#define SHOOT_VRAM_LEN 12
#define LASER_VRAM_LEN 12
#define KILLZAM_VRAM_LEN 12
#define FLARGY_VRAM_LEN 36
#define PLANT_VRAM_LEN 65
#define TOSSMUFFIN_VRAM_LEN 27
#define TELEPORTER_VRAM_LEN 64
#define MAGIBEAR_VRAM_LEN 84
#define LAVA_VRAM_LEN 8
#define COW_VRAM_LEN 99
#define ELEVATOR_VRAM_LEN 64
#define ELEVATOR_STOP_VRAM_LEN 8
#define LAVAANIM_VRAM_LEN 8
#define SPOOKO_VRAM_LEN 6
#define WIP_VRAM_LEN 24
#define BOSS1_VRAM_LEN 200

#define MAP_FG_PALNUM 0
#define BG_PALNUM 1
#define OBJECTS_PALNUM 2
#define PLAYER_PALNUM 3
#define ENEMY_PALNUM OBJECTS_PALNUM
#define CUBE_PALNUM PLAYER_PALNUM
#define CUBE_ALT_PALNUM BG_PALNUM
#define HUD_PALNUM BG_PALNUM
#define PAUSE_PALNUM PLAYER_PALNUM
#define PARTICLES_PALNUM PLAYER_PALNUM
#define PARTICLES_ALT_PALNUM BG_PALNUM
#define POWERUP_CP_PALNUM PLAYER_PALNUM
#define POWERUP_HP_PALNUM OBJECTS_PALNUM

#endif
