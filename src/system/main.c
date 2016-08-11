#include <genesis.h>

#include "system.h"
#include "titlescreen.h"
#include "gameloop.h"

int main(void)
{
	system_init();
toplevel_loop:
	title_play_intro();
	gameloop_main();
	goto toplevel_loop;
	return 0;
}
