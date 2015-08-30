#include "display.h"

u32 display_w;
u32 display_h;
u32 scroll_x;
u32 scroll_y;
u32 scroll_max_x;
u32 scroll_max_y;

s32 mouse_x;
s32 mouse_y;

u32 cursor_x;
u32 cursor_y;

u32 active_window;

u32 selection;
u32 sel_size;

u32 quit;

ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP *main_buffer;
ALLEGRO_BITMAP *scale_buffer;

ALLEGRO_KEYBOARD_STATE keystate;
ALLEGRO_MOUSE_STATE mousestate;

ALLEGRO_FONT *font;
ALLEGRO_EVENT_QUEUE *queue;

char display_title[256];

void display_register_queue(void)
{
	queue = al_create_event_queue();
	if (!queue)
	{
		printf("Error: Couldn't create event queue. The program can run, but\n");
		printf("window events will not trigger (exit, resize, etc).\n");
		return;
	}
	al_register_event_source(queue,al_get_display_event_source(display));
}

void display_handle_queue(void)
{
	ALLEGRO_EVENT ev;
	while (!al_is_event_queue_empty(queue))
	{
		al_get_next_event(queue, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			quit = 1;
		}
	}
}

void display_update(void)
{
	al_set_target_bitmap(scale_buffer);
	al_draw_scaled_bitmap(main_buffer,
		0,0,BUFFER_W,BUFFER_H,
		0,0,PRESCALE*BUFFER_W,PRESCALE*BUFFER_H,
		0);
	al_set_target_backbuffer(display);
	al_draw_scaled_bitmap(scale_buffer,
		0,0,(PRESCALE*BUFFER_W),(PRESCALE*BUFFER_H),
		0,0,display_w,display_h,
		0);
	al_flip_display();
	al_set_target_bitmap(main_buffer);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_get_keyboard_state(&keystate);
	al_get_mouse_state(&mousestate);

	mouse_x = mousestate.x / (display_w / (1.0 * BUFFER_W));
	mouse_y = mousestate.y / (display_h / (1.0 * BUFFER_H));


}

void display_font_init(void)
{
	al_init_font_addon();
	al_init_ttf_addon();
	font = al_load_ttf_font("font.ttf",-10,ALLEGRO_TTF_MONOCHROME);
	if (!font)
	{
		printf("Error: Couldn't load font.ttf.\n");
	}
}

u32 display_init(void)
{
	display_w = BUFFER_W * PRESCALE;
	display_h = BUFFER_H * PRESCALE;
	scroll_x = 0;
	scroll_y = 0;
	scroll_max_x = 40;
	scroll_max_y = 28;
	cursor_x = 0;
	cursor_y = 0;
	mouse_x = 0;
	mouse_y = 0;
	selection = 0;
	quit = 0;
	sel_size = SEL_FULL;
	display = al_create_display(display_w,display_h);
	if (!display)
	{
		printf("Error: Couldn't create display.\n");
		return 0;
	}
	al_set_target_backbuffer(display);	
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_new_bitmap_flags(ALLEGRO_ALPHA_TEST);
	main_buffer = al_create_bitmap(BUFFER_W,BUFFER_H);	
	if (!main_buffer)
	{
		printf("Error: Couldn't create main buffer.\n");
		al_destroy_display(display);
		return 0;
	}
	al_set_target_bitmap(main_buffer);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_set_new_bitmap_flags(ALLEGRO_ALPHA_TEST | ALLEGRO_MAG_LINEAR | ALLEGRO_MIN_LINEAR);
	scale_buffer = al_create_bitmap(PRESCALE * BUFFER_W, PRESCALE * BUFFER_H);
	if (!scale_buffer)
	{
		printf("Error: Couldn't create scale buffer.\n");
		al_destroy_display(display);
		al_destroy_bitmap(main_buffer);
		return 0;
	}
	al_set_target_bitmap(scale_buffer);
	al_clear_to_color(al_map_rgb(0,0,0));
	al_install_keyboard();
	al_install_mouse();
	al_init_primitives_addon();

	display_font_init();
	if (!font)
	{
		return 0;
	}
	display_register_queue();
	printf("Display environment initialized.\n");


	return 1;
}

void display_shutdown(void)
{
	if (display)
	{
		al_destroy_display(display);
		display = NULL;
	}
	if (main_buffer)
	{
		al_destroy_bitmap(main_buffer);
		main_buffer = NULL;
	}
	if (scale_buffer)
	{
		al_destroy_bitmap(scale_buffer);
		scale_buffer = NULL;
	}
}

void display_handle_scroll(void)
{
	if (al_key_down(&keystate, ALLEGRO_KEY_LCTRL))
	{
		return;
	}
	if (al_key_down(&keystate,ALLEGRO_KEY_RIGHT))
	{
		if (scroll_x < scroll_max_x)
		{
			scroll_x++;
		}
	}
	if (al_key_down(&keystate,ALLEGRO_KEY_LEFT))
	{
		if (scroll_x > 0)
		{
			scroll_x--;
		}
	}
	if (al_key_down(&keystate,ALLEGRO_KEY_DOWN))
	{
		if (scroll_y < scroll_max_y)
		{
			scroll_y++;
		}
	}
	if (al_key_down(&keystate,ALLEGRO_KEY_UP))
	{
		if (scroll_y > 0)
		{
			scroll_y--;
		}
	}
}

u32 display_mouse_region(u32 x, u32 y, u32 w, u32 h)
{
	return ((mouse_x > x) && (mouse_x < (x+w)) && (mouse_y > y) && (mouse_y < (y+h)));
}
