#include <genesis.h>

#include "system.h"
#include "gameloop.h"

int main(void)
{
	while(1)
	{
		system_init();
		gameloop_main();
	}
	return 0;	
}
