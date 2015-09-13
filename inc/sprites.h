// sprites.h
// Michael Moffitt
// Routines for abstracting away sprite placement into a cache in main memory,
// which is then transferred to VRAM during the vertical blank using DMA.
// Unless sprites are placed manually, priority is determinde by the order in
// which sprites have been drawn.
#ifndef SPRITES_H
#define SPRITES_H

#include <genesis.h>

// General helper functions for manipulating sprites and maintaining a cache
// of sprite data to be DMA'd.

#define NUM_SPRITES 80

// Moslty just set the sprite table address cache
void sprites_init(void);
// DMA up to spr_num, reset next_sprite
void sprites_dma(u32 spr_num);

// DMA up to next_sprite without passing it
void sprites_dma_simple(void);

// Cut off the sprite list at the last set one
void sprites_clamp_list(u8 num);

// Set properties for sprite in the sprite table
void sprite_set(u8 num, s16 x, s16 y, u8 size, u16 attr, u8 link);

// Simple sprite placement that implies sprite number
void sprite_put(s16 x, s16 y, u8 size, u16 attr);

// Accessors to internal variables
u8 sprites_get_next_sprite(void);
u16 sprites_get_sprite_addr(void);
u16 *sprites_get_table(void);

#endif
