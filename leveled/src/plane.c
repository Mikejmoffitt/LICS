#include "plane.h"

static ALLEGRO_BITMAP *checker_bg;
static ALLEGRO_BITMAP *fg_chr;
static ALLEGRO_BITMAP *bg_chr;

static ALLEGRO_EVENT_QUEUE *input_queue;

void plane_init(void)
{
	if (!al_is_system_installed())
	{
		fprintf(stderr, "Allegro is not installed.\n");
		return;
	}
	printf("Plane init!\n");
	fg_chr = NULL;
	bg_chr = NULL;

	// Generate checker backdrop

	printf ("Making checker BG\n");
	if (!checker_bg)
	{
		checker_bg = al_create_bitmap(PLANE_DRAW_W*TILESIZE,PLANE_DRAW_H*TILESIZE);
	}
	al_set_target_bitmap(checker_bg);
	al_clear_to_color(al_map_rgb(40,40,50));
	for (u32 y = 0; y < PLANE_DRAW_H * 2; y++)
	{
		for (u32 x = 0; x < PLANE_DRAW_W * 2; x++)
		{
			if ((x + y) % 2 ==  0)
			{
				al_draw_filled_rectangle(
					x*TILESIZE/2,y*TILESIZE/2,
					(x+1)*TILESIZE/2,(y+1)*TILESIZE/2,
					al_map_rgb(50,50,40));
			}
		}
	}

	input_queue = al_create_event_queue();
	al_install_keyboard();
	if (!input_queue)
	{
		fprintf(stderr,"Couldn't create input event queue. The keyboard will not work.\n");
		return;
	}
	al_register_event_source(input_queue, al_get_keyboard_event_source());
}

void plane_destroy(void)
{
	if (fg_chr)
	{
		al_destroy_bitmap(fg_chr);
	}
	if (bg_chr)
	{
		al_destroy_bitmap(bg_chr);
	}
	if (checker_bg)
	{
		al_destroy_bitmap(checker_bg);
	}
}

// ------- Graphical resource IO -----------
void plane_load_fg(void)
{
	char tile[256];
	char pal[256];
	printf("Tile choice: %d\n",map_header.tileset);
	switch (map_header.tileset)
	{
		default:
		case MAP_SET_OUTSIDE1:
			sprintf(tile,"res/gfx/outside1.bin");
			sprintf(pal,"res/pal/outside1.pal");
			break;
		case MAP_SET_OUTSIDE2:
			sprintf(tile,"res/gfx/outside2.bin");
			sprintf(pal,"res/pal/outside2.pal");
			break;
		case MAP_SET_INSIDE1:
			sprintf(tile,"res/gfx/inside1.bin");
			sprintf(pal,"res/pal/inside1.pal");
			break;
	}

	printf("Opening %s for tile data...\n",tile);
	ALLEGRO_FILE *tf = al_fopen(tile,"rb");
	if (!tf)
	{
		printf("Error: couldn't open %s for tile data.\n",tile);
		return;
	}
	printf("Opening %s for palette data...\n",pal);
	ALLEGRO_FILE *pf = al_fopen(pal,"rb");
	if (!pf)
	{
		printf("Error: couldn't open %s for palette data.\n",pal);
		if (tf) {al_fclose(tf);}
	}
	fg_chr = mdgfx_load_chr(tf,pf,CHR_T_W,CHR_T_H);
	printf("Loaded foreground graphics data.\n");
}

void plane_load_bg(void)
{
	char tile[256];
	char pal[256];
	switch (map_header.tileset)
	{
		default:
		case MAP_SET_OUTSIDE1:
			sprintf(tile,"res/gfx/outside1.bin");
			sprintf(pal,"res/pal/outside1.pal");
			break;
		case MAP_SET_OUTSIDE2:
			sprintf(tile,"res/gfx/outside2.bin");
			sprintf(pal,"res/pal/outside2.pal");
			break;
		case MAP_SET_INSIDE1:
			sprintf(tile,"res/gfx/inside1.bin");
			sprintf(pal,"res/pal/inside1.pal");
			break;
	}

	printf("Opening %s for tile data...\n",tile);
	ALLEGRO_FILE *tf = al_fopen(tile,"rb");
	if (!tf)
	{
		printf("Error: couldn't open %s for tile data.\n",tile);
		return;
	}
	printf("Opening %s for palette data...\n",pal);
	ALLEGRO_FILE *pf = al_fopen(pal,"rb");
	if (!pf)
	{
		printf("Error: couldn't open %s for palette data.\n",pal);
		if (tf) {al_fclose(tf);}
	}
	bg_chr = mdgfx_load_chr(tf,pf,CHR_T_W,CHR_T_H);
	printf("Loaded backdrop graphics data.\n");
}

// Rendering routines
void plane_draw_map(u32 x, u32 y)
{
	if (!map_data) { return; }
	ALLEGRO_COLOR col = (active_window == WINDOW_MAP) ? 
		al_map_rgb(PLANE_BORDER_COLOR) : 
		al_map_rgb(PLANE_INACTIVE_COLOR);
	al_set_target_bitmap(main_buffer);
	ALLEGRO_BITMAP *chr = fg_chr;

	// Put a border around the map
	al_draw_rectangle(x - 4, y - 4, 
		x + (TILESIZE * PLANE_DRAW_W) + 4, 
		y + (TILESIZE * PLANE_DRAW_H) + 4,
		col,PLANE_BORDER_THICKNESS);
	al_draw_bitmap(checker_bg,x,y,0);
	// Render the map
	for (u32 i = 0; i < PLANE_DRAW_H; i++)
	{
		if (i >= map_header.h * 32)
		{
			return;
		}	
		for (u32 j = 0; j < PLANE_DRAW_W; j++)
		{
			if (j >= map_header.w * MAP_WIDTH)
			{
				continue;	
			}	
			u32 t_idx = j + scroll_x;
			t_idx += (i + scroll_y) * (MAP_WIDTH * map_header.w);
			u16 t_choice = map_data[t_idx];
			// determine coords of tile to pull from tileset from buffer
			u32 t_x = TILESIZE * (t_choice % CHR_T_W);
			u32 t_y = TILESIZE * (t_choice/CHR_T_W);
			al_draw_bitmap_region(chr, t_x, t_y, TILESIZE, TILESIZE, 
				x + (TILESIZE * j), y + (TILESIZE * i),0);
		}
	}

	// Text label
	plane_print_label(x, y, col, "Map Data");
}

void plane_draw_vram(u32 x, u32 y)
{
	ALLEGRO_COLOR col = (active_window == WINDOW_VRAM) ? 
		al_map_rgb(PLANE_BORDER_COLOR) : 
		al_map_rgb(PLANE_INACTIVE_COLOR);

	al_set_target_bitmap(main_buffer);
	// Put a border around the VRAM window
	al_draw_rectangle(x - 4, y - 4, 
		x + (CHR_W) + 4, y + (CHR_H) + 4,
		col,PLANE_BORDER_THICKNESS);

	// Draw the VRAM dump
	al_draw_bitmap(fg_chr,x,y,0);

	// Show which is selected
	u32 sel_x = (selection % CHR_T_W) * TILESIZE;
	u32 sel_y = (selection / CHR_T_W) * TILESIZE;
	sel_x += x;
	sel_y += y;
	u32 sel_s = (sel_size == SEL_FULL) ? (2 * TILESIZE) : TILESIZE;
	al_draw_rectangle(sel_x, sel_y, sel_x + sel_s, sel_y + sel_s, 
		al_map_rgba(255,255,0,128), 1);

	// Text label
	plane_print_label(x, y, col, "VRAM Data");

	// Show selection
	char selmsg[16];
	sprintf(&selmsg[0],"Tile: 0x%04X",selection);
	al_draw_text(font,al_map_rgb(255,255,255),x, y + CHR_H - 3 + TILESIZE,0,selmsg);
}

static int height_for_obj(int objnum)
{
	switch (objnum)
	{
		default:
		case 1:
		case 2:
			return TILESIZE * 2;
	}
	return TILESIZE * 2;
}

static int width_for_obj(int objnum)
{
	switch (objnum)
	{
		default:
		case 1:
		case 2:
			return TILESIZE * 2;
	}
	return TILESIZE * 2;
}

void plane_draw_object_list(u32 x, u32 y)
{
	ALLEGRO_COLOR col = (active_window == WINDOW_OBJ) ? 
		al_map_rgb(PLANE_BORDER_COLOR) : 
		al_map_rgb(PLANE_INACTIVE_COLOR);

	al_set_target_bitmap(main_buffer);
	// Put a border around the VRAM window
	al_draw_rectangle(x - 4, y - 4, 
		x + (OBJ_W) + 4, y + (OBJ_H) + 4,
		col,PLANE_BORDER_THICKNESS);
	plane_print_label(x, y, col, "Objects");

	// Now render all of the objects
	for (int i = 0; i < MAP_NUM_OBJS; i++)
	{
		map_obj *o = &map_header.objects[i];
		if (o->type)
		{
			int sx, sy;
			sx = scroll_x * TILESIZE;
			sy = scroll_y * TILESIZE;
			if (o->x - sx > 0 && o->x - sx < PLANE_DRAW_W * TILESIZE && 
				o->y - sy > 0 && o->y - sy < PLANE_DRAW_H * TILESIZE)
			{
				int px, py;
				px = PLANE_DRAW_X + (o->x - sx);
				py = PLANE_DRAW_Y + (o->y - sy);
				al_draw_rectangle(px, py, 
					px + width_for_obj(o->type),
					py + height_for_obj(o->type),
					al_map_rgb(255,255,255),
					2);
		}
	}
}
// Input handling routines

void plane_handle_mouse(void)
{
	// Mouse is in map region

	if (display_mouse_region(
		PLANE_DRAW_X,PLANE_DRAW_Y,
		PLANE_DRAW_W*TILESIZE,PLANE_DRAW_H*TILESIZE))
	{
		active_window = WINDOW_MAP;
		cursor_x = scroll_x + (mouse_x - PLANE_DRAW_X) / TILESIZE;
		cursor_y = scroll_y + (mouse_y - PLANE_DRAW_Y) / TILESIZE;
		// Lock to 16x16 coords
		if (sel_size == SEL_FULL)
		{
			cursor_x = 2 * (cursor_x / 2);
			cursor_y = 2 * (cursor_y / 2);
		}

		// Cursor draw coordinates based on cursor X
		u32 cdx = PLANE_DRAW_X + (cursor_x- scroll_x) * TILESIZE;
		u32 cdy = PLANE_DRAW_Y + (cursor_y - scroll_y) * TILESIZE;

		u32 cdx2 = cdx + ((sel_size == SEL_FULL) ? (2*TILESIZE) : TILESIZE);
		u32 cdy2 = cdy + ((sel_size == SEL_FULL) ? (2*TILESIZE) : TILESIZE);

		al_draw_rectangle(cdx,cdy,cdx2,cdy2,al_map_rgba(255,255,0,128),1);
		if (mousestate.buttons & 1)
		{
			u32 t_idx = cursor_x + ((cursor_y) * (MAP_WIDTH * map_header.w));
			map_data[t_idx] = selection;
			if (sel_size == SEL_FULL)
			{
				t_idx++;
				map_data[t_idx] = selection + 1;
				t_idx += (map_header.w * MAP_WIDTH) - 1;
				map_data[t_idx] = selection + (CHR_T_W);
				t_idx += 1;
				map_data[t_idx] = selection + (CHR_T_W + 1);
			}
			sprintf(display_title,"%s [*]",map_fname);
			al_set_window_title(display, display_title);
		}
	}
	else if (display_mouse_region(VRAM_DRAW_X,VRAM_DRAW_Y,CHR_W,CHR_H))
	{
		active_window = WINDOW_VRAM;
		if (mousestate.buttons & 1)
		{
			u32 sel_x = (mouse_x - VRAM_DRAW_X) / TILESIZE;
			u32 sel_y = (mouse_y - VRAM_DRAW_Y) / TILESIZE;
			// Lock to 16x16 coords
			if (sel_size == SEL_FULL)
			{
				sel_x = 2 * (sel_x / 2);
				sel_y = 2 * (sel_y / 2);
			}
			selection = sel_x + (CHR_T_W * sel_y);
		}
	}

}

void plane_print_info(void)
{
	// Position and selection information
	char msg[64];
	sprintf(msg, "Cursor: %d, %d", cursor_x, cursor_y);
	plane_print_label(PLANE_DRAW_X, BUFFER_H - 24, al_map_rgb(255,255,255), msg);
	sprintf(msg, "Scroll: %d, %d", scroll_x, scroll_y);
	plane_print_label(PLANE_DRAW_X, BUFFER_H - 16, al_map_rgb(192,192,192), msg);
	sprintf(msg, "Room Name: %s (#%d)", map_header.name, map_header.id);
	plane_print_label(PLANE_DRAW_X, BUFFER_H - 8, al_map_rgb(255,128,128), msg);

	// Settings
	if (sel_size == SEL_FULL)
	{
		sprintf(msg, "Brush size: large (16x16)");
	}
	else
	{
		sprintf(msg, "Brush size: small (8x8)");
	}
	plane_print_label(PLANE_DRAW_X, BUFFER_H - 32, al_map_rgb(128,255,128), msg);
}

void plane_scroll_limits(u32 *x, u32 *y)
{
	*x = ((MAP_WIDTH*map_header.w) - (PLANE_DRAW_W));
	*y = ((MAP_HEIGHT*map_header.h) - (PLANE_DRAW_H));
	printf("Scroll limits: %d, %d\n",*x,*y);
}

void plane_print_label(u32 x, u32 y, ALLEGRO_COLOR col, const char *msg)
{
	al_draw_filled_rectangle(
		x, y - 9, 
		x + (8 * strlen(msg)) + 1, y - 1, 
		al_map_rgb(0,0,0));
	al_draw_text(font,col,x, y - 11,0,msg);
}

void plane_handle_io(void)
{
	// User hits save ikey
	while (!al_is_event_queue_empty(input_queue))
	{
		ALLEGRO_EVENT ev;
		al_get_next_event(input_queue, &ev);
	
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
				// Let Ctrl+S save 
				case ALLEGRO_KEY_S:
					if (!al_key_down(&keystate, ALLEGRO_KEY_LCTRL))
					{
						break;
					}
				case ALLEGRO_KEY_F5:
					map_save();
					break;
				case ALLEGRO_KEY_F6:
				case ALLEGRO_KEY_T:
					sel_size = (sel_size == SEL_FULL) ? 0 : SEL_FULL;
					break;
				case ALLEGRO_KEY_UP:
					if (al_key_down(&keystate, ALLEGRO_KEY_LCTRL))
					{
						if (scroll_y < MAP_HEIGHT)
						{
							scroll_y = 0;
							break;
						}
						scroll_y = scroll_y - MAP_HEIGHT;
						scroll_y = MAP_HEIGHT * (scroll_y / MAP_HEIGHT);
					}
					break;
				case ALLEGRO_KEY_DOWN:
					if (al_key_down(&keystate, ALLEGRO_KEY_LCTRL))
					{
						scroll_y = scroll_y + MAP_HEIGHT;
						scroll_y = MAP_HEIGHT * (scroll_y / MAP_HEIGHT);
						if (scroll_y > scroll_max_y)
						{
							scroll_y = scroll_max_y;
						}
					}
					break;
				case ALLEGRO_KEY_LEFT:
					if (al_key_down(&keystate, ALLEGRO_KEY_LCTRL))
					{
						if (scroll_x < MAP_WIDTH)
						{
							scroll_x = 0;
							break;
						}
						scroll_x = scroll_x - MAP_WIDTH;
						scroll_x = MAP_WIDTH * (scroll_x / MAP_WIDTH);
					}
					break;
				case ALLEGRO_KEY_RIGHT:
					if (al_key_down(&keystate, ALLEGRO_KEY_LCTRL))
					{
						scroll_x = scroll_x + MAP_WIDTH;
						scroll_x = MAP_WIDTH * (scroll_x / MAP_WIDTH);
						if (scroll_x > scroll_max_x)
						{
							scroll_x = scroll_max_x;
						}
					}
					break;
			}
		}
	}
}
