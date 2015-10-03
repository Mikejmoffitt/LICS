#include "music.h"
#include "echo.h"
#include "bgm.h"
#include "eif.h"
#include "eef.h"
#include "ewf.h"
#include "sfx.h"
#include "cdaudio.h"

static u16 current_bgm;

// When non-zero, SFX won't play (for a fake priority system) if priority < sfx_prio
static u16 sfx_block;
static u16 sfx_prio;

static const void *instrument_set[] =
{
	(void *)eif_bass1,          // 00: FM Bass 1
	(void *)eif_sqchords1,      // 01: FM Square 1
	(void *)eif_sawlead,        // 02: FM Saw 1
	(void *)eef_flat,           // 03: PSG flat 
	(void *)ewf_bassdrum1,      // 04: PCM kick drum 1
	(void *)ewf_snare1,	        // 05: PCM snare drum 1
	(void *)eef_hat,            // 06: PSG hihat
	(void *)eef_qdecay,         // 07: PSG quick hit
	(void *)ewf_hat1,           // 08: PCM hihat 1
	(void *)ewf_snare2,         // 09: PCM snare drum 2
	(void *)eif_bass2,          // 0A: FM Bass 2
	(void *)eif_saw2,           // 0B: FM Saw 2
	(void *)eif_saw2at,         // 0C: FM Saw 2 (att)
	(void *)eif_buzz,           // 0D: FM Buzz
	(void *)eif_buzztar,        // 0E: FM Buzztar
	(void *)eif_buzztarc1,      // 0F: FM Buzztar (decay)
	(void *)eif_drone,          // 10: FM Drone
	(void *)eif_unsettling,	    // 11: FM Unsettling
	(void *)ewf_snare3,         // 12: PCM snare drum 3 (quiet)
	(void *)eef_arp1,           // 13: PSG Arp1 
	(void *)eef_arp2,           // 14: PSG Arp2
	0
};

static const void *bgm_set[] = 
{
	0,
	(void *)bgm_bgm1,           // 01: Alone in the dark
	(void *)bgm_bgm2,           // 02: Moskito
	(void *)bgm_bgm3,           // 03: Jttimarsu
	(void *)bgm_bgm4,           // 03: Jttimarsu
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

static const void *sfx_list[] = {
	(void *)sfx_jump,
	(void *)sfx_walk1,
	(void *)sfx_walk2,
	(void *)sfx_cubetoss,
	(void *)sfx_cubebounce,
	(void *)sfx_fizzle,
	(void *)sfx_cubespawn
};

static const u16 sfx_len[] = {
	2,
	2,
	2,
	9,
	7,
	16,
	32
};

static const u16 sfx_priority[] = {
	0,
	0,
	0,
	3,
	1,
	2,
	3
};

void playsound(u16 snd)
{
	if (sfx_block >0 && sfx_priority[snd] < sfx_prio)
	{
		return;
	}
	const void *src = sfx_list[snd];
	sfx_block = sfx_len[snd];
	sfx_prio = sfx_priority[snd];
	echo_play_sfx(src);
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
