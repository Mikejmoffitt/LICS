#include <stdio.h>
#include <allegro5/allegro.h>
#include "mdgfx.h"
#include "types.h"

#define BUFFER_W 512
#define BUFFER_H 384
#define PRESCALE 2

u32 display_w;
u32 display_h;

ALLEGRO_DISPLAY *display;
ALLEGRO_BITMAP *main_buffer;
ALLEGRO_BITMAP *scale_buffer;

void update_display(void)
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
}

u32 display_init(void)
{
	display_w = BUFFER_W * PRESCALE;
	display_h = BUFFER_H * PRESCALE;
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

int main(int argc, char **argv)
{
	al_init();
	if (!display_init())
	{
		return 0;
	}
	if (argc < 3)
	{
		printf("Usage: leveled [tiledata] [paldata]\n");
		display_shutdown();
		return 0;
	}

	plane pl;
	plane_init(&pl);
	plane_load_tileset(&pl,argv[1],argv[2]);

	for (;;)
	{
		al_set_target_bitmap(main_buffer);
		al_draw_bitmap(vram,0,0,0);
		update_display();
	}
	
}
