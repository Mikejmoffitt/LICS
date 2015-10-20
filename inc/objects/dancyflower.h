#ifndef DANCYFLOWER_H
#define DANCYFLOWER_H

#include <genesis.h>
#include <enemy.h>

#define DANCYFLOWER_ANIM_LEN 3
#define DANCYFLOWER_ANIM_DELAY 14

#define DANCYFLOWER_WIDTH 8
#define DANCYFLOWER_HEIGHT 48

typedef struct en_dancyflower en_dancyflower;
struct en_dancyflower
{
	en_header head;
	u16 anim_cnt;
	u16 anim_frame;
};

void en_init_dancyflower(en_dancyflower *e);
void en_unload_dancyflower(void);

#endif
