#ifndef MDGFX_H
#define MDGFX_H

#include <allegro5/allegro.h>
#include <stdio.h>

// Support functions for loading graphics in Megadrive format to bitmaps
// ALLEGRO can understand and manipulate

ALLEGRO_COLOR mdgfx_color_entry(uint16_t entry);

// Load 16 palettes (16be) from pf into c (ALLEGRO_COLOR[16])
void mdgfx_pal_load_loop(ALLEGRO_COLOR *c, ALLEGRO_FILE *pf);

// t is 32 bytes; render a MD tile using palette c at 8x, 8y to d
void mdgfx_plot_tile(uint8_t *t, uint16_t x, uint16_t y, ALLEGRO_BITMAP *d, ALLEGRO_COLOR *c);

unsigned int mdgfx_file_sanity_checks(ALLEGRO_FILE *tf, ALLEGRO_FILE *pf);

// Create an ALLEGRO_BITMAP representing the tile data loaded from tf, using 
// the palette contained in pf. w and h are in terms of tiles (w = 16 is a 
// typical amount used for VRAM dumps)
ALLEGRO_BITMAP *mdgfx_load_chr(ALLEGRO_FILE *tf, ALLEGRO_FILE *pf, unsigned int w, unsigned int h);

#endif
