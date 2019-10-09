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
	(void *)ewf_snare1,         // 05: PCM snare drum 1
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
	(void *)eif_unsettling,     // 11: FM Unsettling
	(void *)ewf_snare3,         // 12: PCM snare drum 3 (quiet)
	(void *)eef_arp1,           // 13: PSG Arp1
	(void *)eef_arp2,           // 14: PSG Arp2
	(void *)eif_sawdecay,       // 15: FM Saw 2 Decay
	(void *)eif_modhorn,        // 16: FM Modhorn
	(void *)eif_sine,           // 17: FM Sine decay
	(void *)eif_sqpluck,        // 18: FM Square pluck
	(void *)eif_evilbass,       // 19: FM Evil Bass
	(void *)ewf_bassdrum2,      // 1A: PCM kick drum 2 (chippy)
	(void *)ewf_snare4,         // 1B: PCM snare 4 (chippy)
	(void *)ewf_hat2,           // 1C: PCM hihat 2
	0
};

static const void *bgm_set[] =
{
	0,
	(void *)bgm_bgm1,           // 01: A Sad Touch
	(void *)bgm_bgm2,           // 02: Moskito
	(void *)bgm_bgm3,           // 03: Jttimarsu
	(void *)bgm_bgm4,           // 04: Modern Talking
	(void *)bgm_bgm5,           // 05: Saturn
	(void *)bgm_bgm6,           // 06: Heidi #7
	0,
	(void *)bgm_bgm8,           // 08: ST-Style
	0,
	0,
	0,
	0,
	0,
	(void *)bgm_bgm14,           // 14: Alone in the Dark
	(void *)bgm_bgm15,           // 15: Elevator Room drone
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
	(void *)sfx_elevatormove,
	0
};

static const u16 sfx_len[] = {
	2,  // jump
	2,  // walk1
	2,  // walk2
	9,  // cubetoss
	7,  // cubebounce
	16, // fizzle
	38, // cubespawn
	30, // enemyexplode
	30, // enemystrike
	18, // pause
	30, // cubeexplode
	16, // hurt
	9,  // cubelift
	8,  // powerup_get
	1,  // bogologo
	3   // elevatormove

};

static const u16 sfx_priority[] = {
	0,  // jump
	0,  // walk1
	0,  // walk2
	3,  // cubetoss
	1,  // cubebounce
	2,  // fizzle
	3,  // cubespawn
	5,  // enemyexplode
	4,  // enemystrike
	0,  // pause
	6,  // cubeexplode
	6,  // hurt
	3,  // cubelift
	4,  // powerup_get
	4,  // bogologo
	8   // elevatormove
};

#endif
