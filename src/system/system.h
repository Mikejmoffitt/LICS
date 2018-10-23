// system.h
// A few basic routines for setting up the system and managing vblank timing
#ifndef GLOBALSYS_H
#define GLOBALSYS_H

#include <genesis.h>

#ifdef WANT_DEBUG_COLOR
#define DEBUG_BGCOL(x) VDP_setPaletteColor(0,x);
#else
#define DEBUG_BGCOL(x) 
#endif

#define SYSTEM_PLANE_W 64
#define SYSTEM_PLANE_H 32

#define BUTTON_FACE (BUTTON_START|BUTTON_A|BUTTON_C|BUTTON_B|BUTTON_X|BUTTON_Y|BUTTON_Z)

extern u16 buttons;
extern u16 buttons_prev;
extern u16 debug_en;


// This just counts up forever. It is often used for animations.
extern u16 system_osc;

// Is the system NTSC (60fps)?
extern u16 system_ntsc;
extern u16 ntsc_counter; // Counts from 0-5 when in NTSC mode for H-movement speed hack

void system_init(void);
void w_puts(const char *s, u16 x, u16 y);
void a_puts(const char *s, u16 x, u16 y);
void b_puts(const char *s, u16 x, u16 y);
void system_wait_v(void);
void system_set_debug(u16 val);
void system_debug_cpu_meter(void);
void system_set_h_split(u16 line, u16 num, u16 *p);

void palette_att(u16 *pal, u16 amnt);
void fade_out(void);
void fade_in(void);
void fade_in_to(u16 *orig_pal);

#endif
