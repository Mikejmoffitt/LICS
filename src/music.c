#include "music.h"
#include "echo.h"
#include "bgm.h"
#include "eif.h"

static const u8 *test_instrument_set[] =
{
	(u8*)eif_bass1,
	(u8*)eif_sq1,
	(u8*)eif_saw1,
	0
};

void music_play(u16 num)
{
	// load instrument set into echo
	echo_init((void *)test_instrument_set);
	
	// Play the track
	echo_play_bgm((void *)&bgm_bgm1);
	
	(void)num;
}
