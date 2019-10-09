// sprites.h
// Michael Moffitt
// Routines for abstracting away sprite placement into a cache in main memory,
// which is then transferred to VRAM during the vertical blank using DMA.
// Unless sprites are placed manually, priority is determinde by the order in
// which sprites have been drawn.
#ifndef SPRITES_H
#define SPRITES_H

#define NUM_SPRITES 80

#include <genesis.h>

extern u16 next_spr;
extern u16 sprite_addr;
extern u16 sprite_table[NUM_SPRITES * 4];

// General helper functions for manipulating sprites and maintaining a cache
// of sprite data to be DMA'd.

u16 sprites_get_addr(void);

// Moslty just set the sprite table address cache
void sprites_init(void);

// DMA up to spr_num, reset next_sprite
void sprites_dma(u32 spr_num);

// DMA up to next_sprite without passing it
void sprites_dma_simple(void);

// Cut off the sprite list at the last set one
void sprites_clamp_list(u8 num);

// Set properties for sprite in the sprite table
static inline void sprite_set(u8 num, s16 x, s16 y, u8 size, u16 attr, u8 link)
{
	// Sprite table only holds 80 sprites
	if (num >= NUM_SPRITES)
	{
		return;
	}
	u16 *addr = &sprite_table[num << 2];
	*addr = 128 + y;
	addr++;
	*addr = (size << 8) + link;
	addr++;
	*addr = attr;
	addr++;
	*addr = 128 + x;
}

// Simple sprite placement that implies sprite number
static inline void sprite_put(s16 x, s16 y, u8 size, u16 attr)
{
	sprite_set(next_spr, x, y, size, attr, next_spr + 1);
	next_spr++;
}

// Accessors to internal variables
u8 sprites_get_next_sprite(void);
u16 sprites_get_sprite_addr(void);
u16 *sprites_get_table(void);

#endif
