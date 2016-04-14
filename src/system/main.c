#include <genesis.h>

#include "system.h"
#include "titlescreen.h"
#include "gameloop.h"

int main(void)
{
	while(1)
	{
		title_play_intro();
		system_init();
		gameloop_main();
	}
	return 0;	
}
