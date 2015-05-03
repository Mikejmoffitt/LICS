#ifndef SPRITES_H
#define SPRITES_H

#include <genesis.h>

// General helper functions for manipulating sprites and maintaining a cache
// of sprite data to be DMA'd.

#define NUM_SPRITES 80

extern u8 next_sprite; // Set to 0 at start of frame
extern u16 sprite_addr;
extern u16 sprite_table[NUM_SPRITES * 4];

// Moslty just set the sprite table address cache
void sprites_init(void);

// DMA up to spr_num, reset next_sprite
void sprites_dma(register spr_num);

// DMA up to next_sprite without passing it
void sprites_dma_simple(void);

// Cut off the sprite list at the last set one
void sprites_clamp_list(u8 num);

// Set properties for sprite in the sprite table
void sprite_set(u8 num, u16 x, u16 y, u8 size, u16 attr, u8 link);

// Simple sprite placement that implies sprite number
void sprite_put(u16 x, u16 y, u8 size, u16 attr);

#endif
