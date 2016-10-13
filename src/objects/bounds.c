#include "bounds.h"

static void cube_func(void *v, cube *c);

// Initialization boilerplate
void en_init_bounds(en_bounds *e)
{
	e->head.proc_func = NULL;
	e->head.anim_func = NULL;
	e->head.cube_func = &cube_func;
	e->head.harmful = ENEMY_HARM_NONE;

	e->head.width = 8;
	e->head.height = 6 * 16;

	e->head.y += e->head.height;
	e->head.x += e->head.width;

	e->head.attr[0] = 0;
}

void en_unload_bounds(void)
{
	return;
}

static void cube_func(void *v, cube *c)
{
	(void)v;
	(void)c;
	return;
}
