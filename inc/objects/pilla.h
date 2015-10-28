#ifndef PILLA_H
#define PILLA_H

#include <genesis.h>
#include "enemy.h"

typedef struct en_pilla en_pilla;
struct en_pilla
{
	en_header head;
	fix16 dy;
	u16 is_head;
	u16 anim_cnt;
	u16 anim_frame;
};

void en_init_pilla(en_pilla *e, u16 head);
void en_unload_pilla(void);

#endif
