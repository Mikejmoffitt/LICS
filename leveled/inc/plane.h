#ifndef PLANE_H
#define PLANE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "types.h"
#include "mdgfx.h"
#include "display.h"

#define PLANE_DRAW_X 160
#define PLANE_DRAW_Y 16
#define PLANE_DRAW_W 40
#define PLANE_DRAW_H 28

#define VRAM_DRAW_X 16
#define VRAM_DRAW_Y 16

#define TILESIZE 8
#define CHR_T_W 16
#define CHR_T_H 16
#define CHR_W (CHR_T_W * TILESIZE)
#define CHR_H (CHR_T_H * TILESIZE)
#define CHR_FSIZE ((CHR_W * CHR_H) / 2)

#define PLANE_BORDER_COLOR 255,255,255
#define PLANE_INACTIVE_COLOR 40,40,40
#define PLANE_BORDER_THICKNESS 2

typedef struct plane plane;
struct plane
{
	// Graphics data
	ALLEGRO_BITMAP *tileset_chr;
	ALLEGRO_BITMAP *bg_chr;
	ALLEGRO_BITMAP *level_bg;
	// Level data and information
	u16 *plane_data; // Will be (sizeof(u16) * (32 * w) * (32 * h)
	u32 plane_w; // Width in 32-column screens
	u32 plane_h; // Height in 32-column screens
	u32 tileset_num;
	u32 bg_num;
};

void plane_init(plane *p);

void plane_load_tileset(plane *p, char *tile, char *pal);
void plane_load_bg(plane *p, char *tile, char *pal);
void plane_load_data(plane *p, char *data);

void plane_create_data(plane *p, u32 w, u32 h);

// Draw map contents on screen at x, y in the buffer
void plane_draw_map(plane *p, u32 x, u32 y);

// Draw tiles at x, y in the buffer
void plane_draw_vram(plane *p, u32 x, u32 y);

// Calculate max camera X and Y
void plane_scroll_limits(plane *p, u32 *x, u32 *y);

// Take mouse input for clicking on VRAM, map, etc
void plane_handle_mouse(plane *p);

// Print a text label above a window, complete with opaque black BG
void plane_print_label(u32 x, u32 y, ALLEGRO_COLOR col, const char *msg);

#endif
