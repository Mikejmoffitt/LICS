#ifndef PLANE_H
#define PLANE_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <string.h>
#include "types.h"
#include "mdgfx.h"
#include "display.h"
#include "map.h"

#define PLANE_DRAW_X 160
#define PLANE_DRAW_Y 16
#define PLANE_DRAW_W 40
#define PLANE_DRAW_H 30

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

void plane_init(void);
void plane_destroy(void);

void plane_load_fg(void);
void plane_load_bg(void);

// Draw map contents on screen at x, y in the buffer
void plane_draw_map(u32 x, u32 y);

// Draw tiles at x, y in the buffer
void plane_draw_vram(u32 x, u32 y);

// Calculate max camera X and Y
void plane_scroll_limits(u32 *x, u32 *y);

// Take mouse input for clicking on VRAM, map, etc
void plane_handle_mouse(void);

// Show settings, level meta info
void plane_print_info(void);

// Print a text label above a window, complete with opaque black BG
void plane_print_label(u32 x, u32 y, ALLEGRO_COLOR col, const char *msg);

// Handle inputs to trigger saving
void plane_handle_io(void);

#endif
