#ifndef SPRITES_H
#define SPRITES_H

#include <genesis.h>

// General helper functions for manipulating sprites and maintaining a cache
// of sprite data to be DMA'd.

extern u16 sprite_addr;
extern u16 sprite_table[64 * 4];

// Moslty just set the sprite table address cache
void sprites_init(void);

// DMA up to spr_num
void sprites_dma(register spr_num);

// Set properties for sprite in the sprite table
void sprite_set(u8 num, u16 x, u16 y, u8 size, u16 attr, u8 link);

#endif
