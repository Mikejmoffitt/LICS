#include "titlescreen.h"
#include <genesis.h>
#include "pal.h"
#include "gfx.h"
#include "system/music.h"
#include "system/sprites.h"
#include "system/system.h"
#include "system/music.h"
#include "map.h"
#include "state.h"
#include "bg.h"
#include "system/save.h"
#include "cubes.h"

#define TITLE_PLANE_W 64
#define TITLE_PLANE_H 64

#define TITLE_CHOICE_NEWGAME 0
#define TITLE_CHOICE_CONTINUE 1
#define TITLE_CHOICE_SOUNDTEST 2

static u8 title_choice;
static u8 soundtest_choice;

static void title_init(void)
{

	// For the intro, we're going to use a 512x512 plane size for scrolling in
	// the bottom ground area.
	VDP_setEnable(0);
	VDP_setBPlanAddress(0xC000);
	VDP_setWindowAddress(0xE000);
	VDP_setAPlanAddress(0xE000);
	VDP_setSpriteListAddress(0xB000);

	// Inject our font
	VDP_doVRamDMA((u32)gfx_font, 0xA400, 1536);

	VDP_setPlanSize(TITLE_PLANE_W, TITLE_PLANE_H);
	VDP_clearPlan(PLAN_A, 1);
	VDP_waitDMACompletion();
	VDP_clearPlan(PLAN_B, 1);
	VDP_waitDMACompletion();

	VDP_setScreenWidth320();
	system_set_h_split(0, 0, NULL);
	VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);
	VDP_setHilightShadow(0);

	VDP_setVerticalScroll(PLAN_B, 16);
	VDP_setVerticalScroll(PLAN_A, 16);


	VDP_setVerticalScroll(PLAN_B, 256);
	VDP_setVerticalScroll(PLAN_A, 255);

	VDP_setHorizontalScroll(PLAN_B, -30);
	VDP_setHorizontalScroll(PLAN_A, -120);

	// We are jank-utilizing these here.
	sprites_init();
	map_init();

	// Using level 1's tileset / mappings
	map_load_tileset(MAP_SET_OUTSIDE1);
	state.current_room = map_by_id(1);
	state.current_map = (u8 *)&(state.current_room->map_data);

	// Build DMA queue for full-screen level redraw
	map_draw_full(624, 0);

	system_wait_v();

	// DMA graphic tile assets - overwrites totally unused enemy graphics. fine.
	VDP_doVRamDMA((u32)gfx_bogologo, BOGOLOGO_VRAM_SLOT * 32, BOGOLOGO_VRAM_LEN * 16);
	VDP_doVRamDMA((u32)gfx_titlelogo, TITLELOGO_VRAM_SLOT * 32, TITLELOGO_VRAM_LEN * 16);

	// Force it to load backdrop #1
	bg_load(255);
	bg_load(1);

	// Commit the full-screen backdrop mapping
	map_dma();

	// For drawing the green cube on the right
	cube_dma_tiles();
	VDP_doCRamDMA((u32)pal_lyle, 32 * PLAYER_PALNUM, 16);
	state.cam_x = 0;
	state.cam_y = 0;

	// Done doing writes, bring back the VDP
	VDP_setEnable(1);

	// Choose New Game by default if there is no fresh save
	if (sram.fresh_save == 1)
	{
		title_choice = TITLE_CHOICE_NEWGAME;
	}
	// Otherwise, suggest Continue
	else
	{
		title_choice = TITLE_CHOICE_CONTINUE;
	}

}

static inline void place_titlelogo(s16 lx, s16 ly)
{

	sprite_put(lx, ly, SPRITE_SIZE(4, 4), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT));
	sprite_put(lx + 32, ly, SPRITE_SIZE(4, 4), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT + 16));
	sprite_put(lx + 64, ly, SPRITE_SIZE(4, 4), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT + 32));
	sprite_put(lx + 96, ly, SPRITE_SIZE(2, 4), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT + 48));

	sprite_put(lx, ly+32, SPRITE_SIZE(4, 4), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT + 56));
	sprite_put(lx + 32, ly+32, SPRITE_SIZE(4, 4), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT + 56 + 16));

	sprite_put(lx + 64, ly+32, SPRITE_SIZE(4, 4), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT + 56 + 32));

	sprite_put(lx + 96, ly+32, SPRITE_SIZE(2, 4), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT + 56 + 48));

	sprite_put(lx, ly+64, SPRITE_SIZE(4, 1), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT + 112));
	sprite_put(lx + 32, ly+64, SPRITE_SIZE(4, 1), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT + 112 + 4));
	sprite_put(lx + 64, ly+64, SPRITE_SIZE(4, 1), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT + 112 + 8));
	sprite_put(lx + 96, ly+64, SPRITE_SIZE(2, 1), TILE_ATTR_FULL(
		2, 1, 0, 0, TITLELOGO_VRAM_SLOT + 112 + 12));
}

static inline void place_bogologo(s16 lx, s16 ly)
{
	for (u16 y = 0; y < 6; y++)
	{
		for (u16 x = 0; x < 4; x++)
		{
			sprite_put(lx + (x * 32), ly + (y * 8), SPRITE_SIZE(4, 1), TILE_ATTR_FULL(
				2, 1, 0, 0,
				BOGOLOGO_VRAM_SLOT + (x * 4) + (y * 16)));
		}
	}
}

static inline void scroll_movement(fix16 *scroll_dy, fix16 *scroll_y, u16 *logo_falling)
{
	*scroll_dy = fix16Add(*scroll_dy, system_ntsc ? FIX16(0.2) : FIX16(0.24));
	*scroll_y = fix16Add(*scroll_y, *scroll_dy);

	// The bouncing
	if (*scroll_y > FIX16(360))
	{
		if (*scroll_dy > FIX16(2))
		{
			*scroll_dy = -(*scroll_dy >> 1);
			playsound(SFX_CUBE_EXPLODE);
			*scroll_y = FIX16(360);
		}
		else if (*scroll_dy <= FIX16(2) && *scroll_dy > FIX16(0))
		{
			*scroll_dy = FIX16(0);
			playsound(SFX_CUBE_EXPLODE);
			*logo_falling = 0;
			*scroll_y = FIX16(360);
		}
	}
}

static void title_draw_bogologo(u16 i, fix16 scroll_y)
{
	// Logo flashes for 60 frames
	if ((i >= 60 || (i % 4 < 2)) && scroll_y <= FIX16(240))
	{
		place_bogologo(96, 88 - fix16ToInt(scroll_y));
	}
}

static void bogologo_palette(u16 i)
{
	if ((i >> 3) % 2 == 0)
	{
		VDP_doCRamDMA((u32)pal_bogologo, 64, 16);
	}
	else
	{
		VDP_doCRamDMA((u32)pal_bogologo+32, 64, 16);
	}
}

static void title_bg_scroll(fix16 scroll_y)
{
	s16 calc_scroll = 256 + fix16ToInt(scroll_y) - (system_ntsc ? 88 : 105);
	if (scroll_y < FIX16(96))
	{
		calc_scroll = 256;
	}
	if (!system_ntsc)
	{
		calc_scroll++;
	}
	VDP_setVerticalScroll(PLAN_A, calc_scroll);
	if (!system_ntsc && scroll_y >= FIX16(96))
	{
		calc_scroll += 16;
	}
	VDP_setVerticalScroll(PLAN_B, calc_scroll);

}

const char arrow_r[] = {
	'a' - 1, '\0'
};

const char arrow_l[] = {
	'~' + 1, '\0'
};

static void menu_inputs(void)
{
	// Up and down in menu
	if (buttons & BUTTON_UP && !(buttons_prev && BUTTON_UP))
	{
		if (title_choice == TITLE_CHOICE_NEWGAME)
		{
			title_choice = TITLE_CHOICE_SOUNDTEST;
		}
		else
		{
			title_choice--;
		}
	}
	else if (buttons & BUTTON_DOWN && !(buttons_prev && BUTTON_DOWN))
	{
		if (title_choice == TITLE_CHOICE_SOUNDTEST)
		{
			title_choice = TITLE_CHOICE_NEWGAME;
		}
		else
		{
			title_choice++;
		}
	}

	// Left and right for sound test choice
	if (title_choice == TITLE_CHOICE_SOUNDTEST)
	{
		if (buttons & BUTTON_LEFT && !(buttons_prev && BUTTON_LEFT))
		{
			soundtest_choice--;
		}
		if (buttons & BUTTON_RIGHT && !(buttons_prev && BUTTON_RIGHT))
		{
			soundtest_choice++;
		}
	}
}

static void print_choices(void)
{
	u16 menu_y = (system_ntsc ? 19 : 21);
	b_puts(" ", 17, menu_y);
	b_puts(" ", 30, menu_y);
	b_puts(" ", 17, menu_y + 2);
	b_puts(" ", 30, menu_y + 2);
	b_puts(" ", 17, menu_y + 4);
	b_puts(" ", 30, menu_y + 4);
	b_puts(arrow_r, 17, menu_y + (title_choice * 2));
	b_puts(arrow_l, 30, menu_y + (title_choice * 2));

	if (title_choice != TITLE_CHOICE_NEWGAME || system_osc % 16 > 4)
	{
		b_puts("New Game", 20, menu_y);
	}
	else
	{
		b_puts("        ", 20, menu_y);
	}
	if (title_choice != TITLE_CHOICE_CONTINUE || system_osc % 16 > 4)
	{
		b_puts("Continue", 20, menu_y + 2);
	}
	else
	{
		b_puts("        ", 20, menu_y + 2);
	}
	if (title_choice != TITLE_CHOICE_SOUNDTEST || system_osc % 16 > 4)
	{
		b_puts("Sound Test", 19, menu_y + 4);
	}
	else
	{
		b_puts("          ", 19, menu_y + 4);
	}


	if (title_choice == TITLE_CHOICE_SOUNDTEST)
	{
		// Build printable display of choice
		char buffer[3];
		buffer[2] = '\0';
		if ((soundtest_choice & 0xF) >= 0xA)
		{
			buffer[1] = (soundtest_choice & 0xF) - 0xA + 'A';
		}
		else
		{
			buffer[1] = (soundtest_choice & 0xF) + '0';
		}
		if ((soundtest_choice & 0xF0) >= 0xA0)
		{
			buffer[0] = ((soundtest_choice & 0xF0) >> 4) - 0xA + 'A';
		}
		else
		{
			buffer[0] = ((soundtest_choice & 0xF0) >> 4) + '0';
		}
		b_puts(buffer, 23, menu_y + 6);
	}
	else
	{
		b_puts("  ", 23, menu_y + 6);
	}

}

static void draw_green_cube(fix16 scroll_y)
{
	if (scroll_y < FIX16(200))
	{
		return;
	}
	const u16 cube_x = 248;
	const u16 cube_y = 360 + (system_ntsc ? 176 : 192);
	cube_draw_single(cube_x, cube_y - fix16ToInt(scroll_y), CUBE_GREEN);
}

static void soundtest_action(void)
{
	system_wait_v();
	music_play(0);
	music_play(soundtest_choice);
}

static s16 title_menu(void)
{
	VDP_doCRamDMA((u32)pal_titlelogo, 64, 16);
	b_puts("mikejmoffitt.com", 5, (system_ntsc ? 28 : 30));
	while (1)
	{
		place_titlelogo(104, 16);
		cube_draw_single(248, (system_ntsc ? 176 : 192), CUBE_GREEN);
		menu_inputs();
		system_wait_v();
		sprites_dma_simple();
		print_choices();

		if (buttons & BUTTON_FACE && !(buttons_prev & BUTTON_FACE))
		{
			if (title_choice == TITLE_CHOICE_NEWGAME)
			{
				save_clear();
				return 0;
			}
			else if (title_choice == TITLE_CHOICE_CONTINUE)
			{
				return 1;
			}
			else if (title_choice == TITLE_CHOICE_SOUNDTEST)
			{
				soundtest_action();
			}
		}
	}
	return 2;
}

static void logo_bounce(void)
{
	u16 i = 0;
	fix16 scroll_y = FIX16(0);
	fix16 scroll_dy = 0;
	u16 logo_falling = 0;
	// Blank wait
	for (i = 0; i < 30; i++)
	{
		sprites_dma_simple();
		system_wait_v();
	}
	playsound(SFX_BOGOLOGO);
	while (i < (system_ntsc ? 400 : 333))
	{
		if (buttons & BUTTON_START && !(buttons_prev & BUTTON_START))
		{
			i = (system_ntsc ? 400 : 333) - 1;
			scroll_y = FIX16(360);
			scroll_dy = FIX16(0);
			logo_falling = 0;
			system_wait_v();
			// playsound(SFX_MENU);
		}

		// Logo slides up after a delay
		if (i == (system_ntsc ? 120 : 100))
		{
			logo_falling = 1;
		}

		if (logo_falling)
		{
			scroll_movement(&scroll_dy, &scroll_y, &logo_falling);
		}

		title_draw_bogologo(i, scroll_y);
		draw_green_cube(scroll_y);
		system_wait_v();
		title_bg_scroll(scroll_y);
		if (scroll_y <= FIX16(240))
		{
			bogologo_palette(i);
		}

		sprites_dma_simple();
		i++;
	}
}

void title_play_intro(void)
{
	title_init();

	// Logo appears; slides up
	logo_bounce();
	// TODO
	// Hooded dude walks towards cat, snatches; Lyle appears in house

	music_play(14);
	// Title shows; present menu
	title_menu();

	system_wait_v();
	music_play(0);
	VDP_setEnable(0);
	VDP_waitDMACompletion();
	VDP_clearPlan(PLAN_A, 1);
	VDP_waitDMACompletion();
	VDP_clearPlan(PLAN_B, 1);
	VDP_waitDMACompletion();
	VDP_doCRamDMA((u32)pal_black, 0, 16);
	VDP_doCRamDMA((u32)pal_black, 32, 16);
	VDP_doCRamDMA((u32)pal_black, 64, 16);
	VDP_doCRamDMA((u32)pal_black, 96, 16);
	sprites_dma_simple();
	bg_load(255);
	VDP_setEnable(1);
	system_wait_v();


	// Logo slides up as scene slides down
}
