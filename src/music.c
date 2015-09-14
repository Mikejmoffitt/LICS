#include "music.h"
#include "echo.h"
#include "bgm.h"
#include "eif.h"
#include "eef.h"
#include "ewf.h"

static const void *test_instrument_set[] =
{
	(void *)eif_bass1,
	(void *)eif_sqchords1,
	(void *)eif_sawlead,
	(void *)eef_flat,
	(void *)ewf_bassdrum1,
	(void *)ewf_snare1,
	(void *)eef_hat,
	(void *)eef_qdecay,
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
