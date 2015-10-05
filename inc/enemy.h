#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_NULL 0
#define ENEMY_METAGRUB 1
#define ENEMY_FLIP 2
#define ENEMY_BOINGO 3

static const u16 enemy_palnums[] = 
{
	0,
	PLAYER_PALNUM,   // 01 Metagrub
	OBJECTS_PALNUM,  // 02 Flip
	PLAYER_PALNUM,   // 03 Boingo
	0
}

#endif
