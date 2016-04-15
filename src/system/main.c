#include <genesis.h>

#include "system.h"
#include "titlescreen.h"
#include "gameloop.h"

int main(void)
{
	while(1)
	{
		system_init();
		title_play_intro();
		gameloop_main();
	}
	return 0;	
}
