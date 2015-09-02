#ifndef HUD_H
#define HUD_H

#include <genesis.h>

#define HUD_HP_X 8
#define HUD_HP_Y 8
#define HUD_HP_SPACING 8

#define HUD_CP_X 8
#define HUD_CP_Y 216

#define HUD_CP_BAR_HEIGHT 48


// Set up HUD graphics in VRAM
void hud_dma_tiles(void);

// Draw health bar
void hud_draw_health(u16 max, u16 val);

// Draw cube power bar
void hud_draw_cp(u16 val);

#endif
