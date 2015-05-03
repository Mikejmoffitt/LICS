#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "types.h"

#define BUFFER_W 512
#define BUFFER_H 384
#define PRESCALE 2

#define SEL_SINGLE 0
#define SEL_FULL 1

#define WINDOW_MAP 0
#define WINDOW_VRAM 1

// Editor state variables
extern u32 display_w;
extern u32 display_h;
extern u32 scroll_x;
extern u32 scroll_y;
extern u32 scroll_max_x;
extern u32 scroll_max_y;

extern u32 cursor_x;
extern u32 cursor_y;

extern u32 active_window;

extern u32 selection;
extern u32 sel_size;

extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_BITMAP *main_buffer;
extern ALLEGRO_BITMAP *scale_buffer;

extern ALLEGRO_KEYBOARD_STATE keystate;


void display_update(void);
u32 display_init(void);
void display_shutdown(void);
void display_handle_scroll(void);

#endif
