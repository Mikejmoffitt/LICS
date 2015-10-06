// Minimal Sega Mega Drive / Genesis DMA support functions
// Based on Stephane D.'s SGDK code
// Michael Moffitt
// https://github.com/mikejmoffitt

#ifndef MD_DMA_H
#define MD_DMA_H
#include "md_vdp.h"
#include <stdint.h>

#define DMA_VRAM 0
#define DMA_CRAM 1
#define DMA_VSRAM 2

// Shortcuts for some common DMA operations
#define md_dma_vram(f,t,l) md_dma(DMA_VRAM,f,t,l,2)
#define md_dma_cram(f,t,l) md_dma(DMA_CRAM,f,t,l,2)
#define md_dma_vsram(f,t,l) md_dma(DMA_VSRAM,f,t,l,2)

// Perform a DMA using the VDP; blocking
void md_dma(uint8_t loc, uint32_t src, uint16_t dest, uint16_t len, int16_t step);
// VRAM Fill operation; non-blocking
void md_vram_fill(uint16_t dest, uint16_t len, uint8_t val, int16_t step);
// VRAM copy operation; non-blocking
void md_vram_copy(uint16_t src, uint16_t dest, uint16_t len);

#endif
