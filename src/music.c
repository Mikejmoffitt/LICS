#include "music.h"
#include "echo.h"
#include "bgm.h"
#include "eif.h"
#include "eef.h"
#include "ewf.h"
#include "sfx.h"
#include "cdaudio.h"

static u16 current_bgm;

static const void *instrument_set[] =
{
	(void *)eif_bass1,			// 00: FM Bass 1
	(void *)eif_sqchords1,		// 01: GM Square 1
	(void *)eif_sawlead,		// 02: GM Saw 1
	(void *)eef_flat,			// 03: PSG flat 
	(void *)ewf_bassdrum1,		// 04: PCM bass drum 1
	(void *)ewf_snare1,			// 05: PCM snare drum 1
	(void *)eef_hat,			// 06: PSG hihat
	(void *)eef_qdecay,			// 07: PSG quick hit
	(void *)ewf_hat1,			// 08: PCM hihat 1
	(void *)ewf_snare2,			// 09: PCM snare drum 2
	(void *)eif_bass2,			// 0A: FM Bass 2
	(void *)eif_saw2,			// 0B: FM Saw 2
	(void *)eif_saw2at,			// 0C: FM Saw 2 (att)
	(void *)eif_buzz,			// 0D: FM Buzz
	0
};

static const void *bgm_set[] = 
{
	0,
	(void *)bgm_bgm1,			// Alone in the dark
	(void *)bgm_bgm2,			// Moskito
	0
};

void music_init(void)
{
	// load instrument set into echo
	echo_init((void *)instrument_set);
	current_bgm = 0;
}

// Track 0 is no music
void music_play(u16 num)
{
	int i;
	if (num == current_bgm)
	{
		return;
	}
	if (num)
	{
		if (cdaudio_is_active())
		{
			cdaudio_stop();
		}
		echo_stop_bgm();
		if (cdaudio_is_active() && cdaudio_play_loop(num))
		{
			// Try playing with CD first
		}
		else
		{
			// Play the track with Echo
			void *bgm_ptr = (void *)bgm_set[num];
			echo_play_bgm(bgm_ptr);
		}
		current_bgm = num;
	}
	else
	{
		current_bgm = 0;
		if (cdaudio_is_active())
		{
			cdaudio_stop();
		}
		echo_stop_bgm();
	}
}

void playsound(u16 snd)
{
	void *src;
	switch (snd)
	{
		default:
		case 0:
			src = (void *)sfx_jump;
			break;
		case 1:
			src = (void *)sfx_walk1;
			break;
		case 2:
			src = (void *)sfx_walk2;
			break;
	}
	echo_play_sfx(src);
}
