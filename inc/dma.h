#ifndef DMA_H
#define DMA_H

#include <stdint.h>

void dma_set_inc(uint16_t inc);
void dma_to_vram(uint32_t src, uint16_t dest, uint16_t len);

#endif
