#include "music.h"
#include "echo.h"
#include "cdaudio.h"
#include "bgm.h"
#include "eif.h"
#include "eef.h"
#include "ewf.h"
#include "sfx.h"

#include "sounds.h"


static u16 current_bgm;

// When non-zero, SFX won't play (for a fake priority system) if priority < sfx_prio
static u16 sfx_block;
static u16 sfx_prio;


void music_init(void)
{
	// load instrument set into echo
	echo_init((void *)instrument_set);
	current_bgm = 0;
}

// Track 0 is no music
void music_play(u16 num)
{
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
	if (sfx_block >0 && sfx_priority[snd] < sfx_prio)
	{
		return;
	}
	const void *src = sfx_list[snd];
	sfx_block = sfx_len[snd];
	sfx_prio = sfx_priority[snd];
	if (src)
	{
		echo_play_sfx(src);
	}
}

void stopsound(void)
{
	sfx_block = 0;
	sfx_prio = 0;
	echo_stop_sfx();
}

void sfx_counters(void)
{
	if (sfx_block != 0)
	{
		sfx_block--;
	}
	else
	{
		sfx_prio = 0;
	}
}
