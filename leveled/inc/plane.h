#ifndef PLANE_H
#define PLANE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "types.h"
#include "mdgfx.h"

#define PLANE_DRAW_X 272
#define PLANE_DRAW_Y 8

#define VRAM_DRAW_X 8
#define VRAM_DRAW_Y 8

#define TILESIZE 8
#define CHR_W (16 * TILESIZE)
#define CHR_H (16 * TILESIZE)
#define CHR_FSIZE ((CHR_W * CHR_H) / 2)

typedef struct plane plane;
struct plane
{
	u16 *plane_data; // Will be (sizeof(u16) * (32 * w) * (32 * h)
	ALLEGRO_BITMAP *tileset_chr;
	ALLEGRO_BITMAP *bg_chr;
	ALLEGRO_BITMAP *plane_render; // will be (32 * w * 8) by (32 * h * 8)
	u32 tileset_num;
	u32 bg_num;
	u32 plane_w; // Width in 32-column screens
	u32 plane_h; // Height in 32-column screens
};

void plane_init(plane *p);

void plane_load_tileset(plane *p, char *tile, char *pal);
void plane_load_bg(plane *p, char *tile, char *pal);
void plane_load_data(plane *p, char *data);

// Draw map contents on screen at x, y in the buffer
void plane_draw_map(plane *p, u16 x, u16 y);

// Draw tiles at x, y in the buffer
void plane_draw_vram(plane *p, u16 x, u16 y);

void plane_render(u16 *p, u32 scroll_x, u32 scroll_y);


#endif
