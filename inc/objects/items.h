#ifndef ITEMS_H
#define ITEMS_H

// Even though this is for an ability-gaining item, 

#include <genesis.h>
#include "enemy.h"

#define ITEM_FLOAT_LEN 32
#define ITEM_ANIM_LEN 12

#define ITEM_MAP 0
#define ITEM_LIFT 1
#define ITEM_JUMP 2
#define ITEM_PHANTOM 3
#define ITEM_KICK 4
#define ITEM_ORANGE 5

typedef struct en_item en_item;
struct en_item
{
	en_header head;
	u16 item_type;
	u16 anim_cnt;
	u16 float_cnt;
};

void en_init_item(en_item *e, u16 type);
void en_unload_item(void);

#endif
