#ifndef OBJTYPES_H
#define OBJTYPES_H

#include <genesis.h>

typedef struct object object;
struct object
{
	u16 type;	
};

#define OBJ_PLAYER 1
#define OBJ_CUBE 2

#endif
