#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "types.h"
#include "state.h"

#define BUFFER_W 512
#define BUFFER_H 384
#define PRESCALE 2

#define SEL_SINGLE 0
#define SEL_FULL 1

#define WINDOW_MAP 0
#define WINDOW_VRAM 1
#define WINDOW_OBJ 2

// Editor state variables
extern unsigned int display_w;
extern unsigned int display_h;

extern ALLEGRO_DISPLAY *display;
extern ALLEGRO_BITMAP *main_buffer;
extern ALLEGRO_BITMAP *scale_buffer;

extern ALLEGRO_KEYBOARD_STATE keystate;
extern ALLEGRO_MOUSE_STATE mousestate;

extern ALLEGRO_FONT *font;
extern ALLEGRO_EVENT_QUEUE *queue;

extern unsigned int quit;

extern char display_title[256];

void display_register_queue(void);
void display_handle_queue(void);
void display_update(void);
void display_font_init(void);
unsigned int display_init(void);
void display_shutdown(void);
void display_handle_scroll(void);
unsigned int display_mouse_region(unsigned int x, unsigned int y, unsigned int w, unsigned int h);

#endif
