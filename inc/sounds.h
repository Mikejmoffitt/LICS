#ifndef SOUNDS_H
#define SOUNDS_H

#include <genesis.h>

#include "bgm.h"
#include "eif.h"
#include "eef.h"
#include "ewf.h"
#include "sfx.h"

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

static const void *sfx_list[] = {
	(void *)sfx_jump,
	(void *)sfx_walk1,
	(void *)sfx_walk2,
	(void *)sfx_cubetoss,
	(void *)sfx_cubebounce,
	(void *)sfx_fizzle,
	(void *)sfx_cubespawn,
	(void *)sfx_enemyexplode,
	(void *)sfx_enemystrike,
	(void *)sfx_pause,
	(void *)sfx_cubeexplode,
	(void *)sfx_hurt,
	(void *)sfx_cubelift,
	(void *)sfx_powerup_get,
	(void *)sfx_bogologo,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

static const u16 sfx_len[] = {
	2,
	2,
	2,
	9,
	7,
	16,
	38,
	30,
	30,
	18,
	30,
	16,
	9,
	8,
	1

};

static const u16 sfx_priority[] = {
	0,
	0,
	0,
	3,
	1,
	2,
	3,
	5,
	4,
	0,
	6,
	6,
	3,
	4,
	4
};

#endif
