#include "enemy.h"

static en_generic enemies[ENEMIES_NUM];

void enemy_dma_tiles(void)
{
	u16 i = 0;

	// Enemy palette
	VDP_doCRamDMA((u32)pal_enemy, ENEMY_PALNUM * 32, 16);

	// Enemy graphics
	while (enemy_vram_len[i] != 0)
	{
		VDP_doVRamDMA(enemy_vram_src[i], enemy_vram_dest[i], enemy_vram_len[i]);
		i++;
	}
}

void enemy_init(void)
{
	u16 i = ENEMIES_NUM;
	while (i--)
	{
		en_generic *e = &enemies[i];
		e->head.active = 0;
	}
}
