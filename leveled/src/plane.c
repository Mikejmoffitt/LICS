#include "plane.h"
#include "objects.h"

static ALLEGRO_BITMAP *checker_bg;
static ALLEGRO_BITMAP *fg_chr;

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

	// Generate checker backdrop

	printf ("Making checker BG\n");
	if (!checker_bg)
	{
		checker_bg = al_create_bitmap(PLANE_DRAW_W*TILESIZE,PLANE_DRAW_H*TILESIZE);
	}
	al_set_target_bitmap(checker_bg);
	al_clear_to_color(al_map_rgb(40,40,50));
	for (unsigned int y = 0; y < PLANE_DRAW_H * 2; y++)
	{
		for (unsigned int x = 0; x < PLANE_DRAW_W * 2; x++)
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
		case MAP_SET_SANDY1:
			sprintf(tile,"res/gfx/sandy1.bin");
			sprintf(pal,"res/pal/sandy1.pal");
			break;
		case MAP_SET_TELEPORTER:
			sprintf(tile,"res/gfx/teleporter.bin");
			sprintf(pal,"res/pal/teleporter.pal");
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
	if (fg_chr)
	{
		al_destroy_bitmap(fg_chr);
	}
	fg_chr = mdgfx_load_chr(tf,pf,CHR_T_W,CHR_T_H);
	printf("Loaded foreground graphics data.\n");
}

// Rendering routines
void plane_draw_map(unsigned int x, unsigned int y)
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
	for (unsigned int i = 0; i < PLANE_DRAW_H; i++)
	{
		if (i >= map_header.h * 32)
		{
			return;
		}	
		for (unsigned int j = 0; j < PLANE_DRAW_W; j++)
		{
			if (j >= map_header.w * MAP_WIDTH)
			{
				continue;	
			}	
			unsigned int t_idx = j + scroll_x;
			t_idx += (i + scroll_y) * (MAP_WIDTH * map_header.w);
			unsigned int t_choice = map_data[t_idx];
			// determine coords of tile to pull from tileset from buffer
			unsigned int t_x = TILESIZE * (t_choice % CHR_T_W);
			unsigned int t_y = TILESIZE * (t_choice/CHR_T_W);
			al_draw_bitmap_region(chr, t_x, t_y, TILESIZE, TILESIZE, 
				x + (TILESIZE * j), y + (TILESIZE * i),0);
		}
	}

	// Text label
	plane_print_label(x, y, col, "Map Data");
}

void plane_draw_vram(unsigned int x, unsigned int y)
{
	ALLEGRO_COLOR col = (active_window == WINDOW_VRAM) ? 
		al_map_rgb(PLANE_BORDER_COLOR) : 
		al_map_rgb(PLANE_INACTIVE_COLOR);

	al_set_target_bitmap(main_buffer);
	// Put a border around the VRAM window
	al_draw_rectangle(x - 4, y - 4, 
		x + (CHR_W) + 4, y + (CHR_H) + 4,
		col,PLANE_BORDER_THICKNESS);

	ALLEGRO_COLOR sel_col = (edit_mode == MODE_TILES) ? al_map_rgba(0,96,255,0) : al_map_rgba(128,128,128,0);

	// Draw the VRAM dump
	al_draw_bitmap(fg_chr,x,y,0);

	// Show which is selected
	unsigned int sel_x = (tile_sel% CHR_T_W) * TILESIZE;
	unsigned int sel_y = (tile_sel/ CHR_T_W) * TILESIZE;
	sel_x += x;
	sel_y += y;
	unsigned int sel_s = (tile_src_size == SEL_FULL) ? (2 * TILESIZE) : TILESIZE;
	al_draw_rectangle(sel_x, sel_y, sel_x + sel_s, sel_y + sel_s, 
		sel_col, 1);

	// Text label
	char selmsg[16];
	sprintf(selmsg,"VRAM Data: $%02X",tile_sel);
	plane_print_label(x, y, col, selmsg);
}

static void plane_object_window(unsigned int x, unsigned int y)
{
	ALLEGRO_COLOR sel_col = (edit_mode == MODE_OBJECTS) ? al_map_rgba(0,96,255,0) : al_map_rgba(128,128,128,0);
	for (int i = 0; i < OBJ_H / TILESIZE; i++)
	{
		unsigned int idx = i + obj_list_scroll;
		if (idx >= MAP_NUM_OBJS)
		{
			break;
		}
		map_obj *o = &map_header.objects[idx];

		ALLEGRO_COLOR col = (o->type) ? al_map_rgb(255,255,255) : al_map_rgb(80,80,80);
		char print_name[18];
		snprintf(print_name, 17, "%02X %s %04X", idx, string_for_obj(o->type), o->data);
		plane_print_label(x, y + (i * TILESIZE), col, print_name);
		if (obj_list_sel == idx)
		{
			al_draw_filled_rectangle(x, y + (i * TILESIZE) - 9, x + (OBJ_W), 
			(y + ((i + 1) * TILESIZE)) - 10, 
			sel_col);
		}
	}
}

static void plane_meta_object_text(unsigned int x, unsigned int y)
{
	// Prints a more descriptive readout of an object's data
	map_obj *o = &map_header.objects[obj_list_sel];
	char name[1 + (META_DRAW_W / TILESIZE)];
	char desc[1 + (META_DRAW_W / TILESIZE)];
	char dat1[1 + (META_DRAW_W / TILESIZE)];
	char dat2[1 + (META_DRAW_W / TILESIZE)];
	char posd[1 + (META_DRAW_W / TILESIZE)];

	sprintf(name,"Object Type: %02X, named %s",o->type, string_for_obj(o->type));
	sprintf(posd,"X: %04X Y: %04X",o->x, o->y);
	sprintf(dat1,"                       Raw Data: 0x%04X",o->data);

	switch (o->type)
	{
		case OBJ_NULL:
			sprintf(desc," ");
			sprintf(dat2," ");
			break;
		case OBJ_ROOMPTR:
			sprintf(dat2,"Door #%X (LSN)",o->data & 0x000F);
			sprintf(desc,"To (%2X, %X)",(o->data & 0xFF00) >> 8, (o->data & 0x00F0) >> 4);
			break;
		case OBJ_CUBE:
			sprintf(dat2,"");
			if (o->data == 0x0100)
			{
				sprintf(desc,"Blue Cube (destructable)");
			}
			else if (o->data == 0x0200)
			{
				sprintf(desc,"Phantom Cube (don't place)");
			}
			else if (o->data == 0x0300)
			{
				sprintf(desc,"Green Cube (bouncy)");
			}
			else if (o->data == 0x0400)
			{
				sprintf(desc,"Red Cube (explody)");
			}
			else if (o->data == 0x1000)
			{
				sprintf(desc,"Orange Cube (big)");
			}
			else if (o->data == 0x2000)
			{
				sprintf(desc,"Spawner");
			}
			else if ((o->data & 0x0F00) == 0x0800)
			{
				sprintf(desc,"Yellow Cube (item)");
				if ((o->data & 0x00FF) == 0x00)
				{
					sprintf(dat2,"HP UP");
				}
				else if ((o->data & 0x00FF) == 0x01)
				{
					sprintf(dat2,"HP UP 2X");
				}
				else if ((o->data & 0x00FF) == 0x20)
				{
					sprintf(dat2,"CP UP");
				}
				else if ((o->data & 0x00FF) == 0x21)
				{
					sprintf(dat2,"CP UP 2X");
				}
				else if ((o->data & 0x00F0) == 0x40)
				{
					sprintf(dat2,"CP ORB #%X",o->data & 0x000F);
				}
				else if ((o->data & 0x00F0) == 0x80)
				{
					sprintf(dat2,"HP ORB #%X",o->data & 0x000F);
				}
			}
			break;
		case OBJ_METAGRUB:
			sprintf(desc,"Lunges side to side");
			sprintf(dat2," ");
			break;
	}

	plane_print_label(x, y + 40, al_map_rgb(255,255,255), name);
	plane_print_label(x, y + 48, al_map_rgb(128,128,255), desc);
	plane_print_label(x, y + 56, al_map_rgb(128,255,128), dat1);
	plane_print_label(x, y + 64, al_map_rgb(255,192,128), dat2);
	plane_print_label(x, y + 72, al_map_rgb(128,64,192), posd);

	// Flashing edit cursor
	if (meta_cursor_pos >= 0 && osc & 0x00000004)
	{
		int cx = x + (TILESIZE * (35 + meta_cursor_pos));
		al_draw_filled_rectangle(cx + 1, y + 47, cx + TILESIZE, y + 54,
			al_map_rgba(255, 0, 0, 0));

	}
}

void plane_draw_meta(unsigned int x, unsigned int y)
{
	ALLEGRO_COLOR col = (active_window == WINDOW_META) ? 
		al_map_rgb(PLANE_BORDER_COLOR) : 
		al_map_rgb(PLANE_INACTIVE_COLOR);

	al_set_target_bitmap(main_buffer);
	// Put a border around the VRAM window
	al_draw_rectangle(x - 4, y - 4, 
		x + (META_DRAW_W) + 4, y + (META_DRAW_H) + 4,
		col,PLANE_BORDER_THICKNESS);
	plane_print_label(x, y, col, "Metadata");

	// Print room meta-information
	char msg[128];
	// "It's just temporary code" he said
	// "Something more elegant will replace it later" he said
	sprintf(msg, "ID %02X: %s\n\n", map_header.id, map_header.name); 
	plane_print_label(x, y + 8, al_map_rgb(255, 192, 255), msg);

	sprintf(msg, "TS: %02X SP: %02X BG: %02X M: %02X",
		map_header.tileset, map_header.sprite_palette, map_header.background, map_header.music); 
	plane_print_label(x, y + 16, al_map_rgb(192, 255, 255), msg);

	sprintf(msg, "Size: %Xx%X Loc: %d,%d",
		map_header.w, map_header.h, map_header.map_x, map_header.map_y);
	plane_print_label(x, y + 24, al_map_rgb(255, 255, 192), msg);
	
	// Edit mode information
	sprintf(msg, "(%03X,%03X-%c)",
		cursor_x, cursor_y, tile_src_size == SEL_FULL ? 'L' : 's');
	plane_print_label(x + (TILESIZE * 28), y + 16, al_map_rgb(192, 255, 192), msg);

	switch (edit_mode)
	{
		case MODE_TILES:
			sprintf(msg, "Tile Mode");
			break;
		case MODE_OBJECTS:
			sprintf(msg, " Obj Mode");
			break;
		default:
			sprintf(msg, "???? Mode");
			break;
	}
		
	plane_print_label(x + (8 * 31), y + 24, al_map_rgb(255,192,192), msg);

	plane_meta_object_text(x,y);

}

void plane_draw_object_list(unsigned int x, unsigned int y)
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
			int px, py;
			px = PLANE_DRAW_X + o->x - sx;
			py = PLANE_DRAW_Y + o->y - sy;
				al_draw_rectangle(px, py, 
					px + width_for_obj(o->type),
					py + height_for_obj(o->type),
					al_map_rgba(255,255,255,128),
					(obj_list_sel == i) ? 2 : 1);
		}
	}
	plane_object_window(x, y + TILESIZE);
}
// Input handling routines

static void mouse_in_map(void)
{
	if (edit_mode == MODE_TILES)
	{
		cursor_x = scroll_x + (mouse_x - PLANE_DRAW_X) / TILESIZE;
		cursor_y = scroll_y + (mouse_y - PLANE_DRAW_Y) / TILESIZE;
		// Lock to 16x16 coords
		if (tile_src_size == SEL_FULL)
		{
			cursor_x = 2 * (cursor_x / 2);
			cursor_y = 2 * (cursor_y / 2);
		}

		// Cursor draw coordinates based on cursor X
		unsigned int cdx = PLANE_DRAW_X + (cursor_x- scroll_x) * TILESIZE;
		unsigned int cdy = PLANE_DRAW_Y + (cursor_y - scroll_y) * TILESIZE;

		unsigned int cdx2 = cdx + ((tile_src_size == SEL_FULL) ? (2*TILESIZE) : TILESIZE);
		unsigned int cdy2 = cdy + ((tile_src_size == SEL_FULL) ? (2*TILESIZE) : TILESIZE);

		al_draw_rectangle(cdx,cdy,cdx2,cdy2,al_map_rgba(255,255,0,128),1);
		unsigned int t_idx = cursor_x + ((cursor_y) * (MAP_WIDTH * map_header.w));

		if (mousestate.buttons & 1)
		{
			map_data[t_idx] = tile_sel;
			if (tile_src_size == SEL_FULL)
			{
				t_idx++;
				map_data[t_idx] = tile_sel + 1;
				t_idx += (map_header.w * MAP_WIDTH) - 1;
				map_data[t_idx] = tile_sel + (CHR_T_W);
				t_idx += 1;
				map_data[t_idx] = tile_sel + (CHR_T_W + 1);
			}
			sprintf(display_title,"%s [*]",map_fname);
			al_set_window_title(display, display_title);
		}
		else if (mousestate.buttons & 2)
		{
			tile_sel = map_data[t_idx];
		}
	}
	else if (edit_mode == MODE_OBJECTS)
	{
		if (mousestate.buttons & 1)
		{
			cursor_x = scroll_x * TILESIZE + (mouse_x - PLANE_DRAW_X);
			cursor_y = scroll_y * TILESIZE + (mouse_y - PLANE_DRAW_Y);
			if (tile_src_size == SEL_FULL)
			{
				cursor_x = (cursor_x / TILESIZE) * TILESIZE;
				cursor_y = (cursor_y / TILESIZE) * TILESIZE;
			}
			map_obj *o = &map_header.objects[obj_list_sel];	
			o->x = cursor_x;
			o->y = cursor_y;
		}
	}
}

static void mouse_in_vram(void)
{
	// User clicks in VRAM window; go to map edit mode
	if (mousestate.buttons & 1)
	{
		edit_mode = MODE_TILES;	
	}
	if (edit_mode == MODE_TILES)
	{
		if (mousestate.buttons & 1)
		{
			unsigned int sel_x = (mouse_x - VRAM_DRAW_X) / TILESIZE;
			unsigned int sel_y = (mouse_y - VRAM_DRAW_Y) / TILESIZE;
			// Lock to 16x16 coords
			if (tile_src_size == SEL_FULL)
			{
				sel_x = 2 * (sel_x / 2);
				sel_y = 2 * (sel_y / 2);
			}
			tile_sel= sel_x + (CHR_T_W * sel_y);
		}
	}
}

static void mouse_in_objlist(void)
{
	// user clicks in obj window, switch modes
	if (mousestate.buttons & 1)
	{
		edit_mode = MODE_OBJECTS;	
		obj_list_sel = ((mouse_y - OBJ_DRAW_Y) / TILESIZE) + obj_list_scroll;
		if (obj_list_sel >= MAP_NUM_OBJS)
		{
			obj_list_sel = MAP_NUM_OBJS - 1;
		}
	}
}

static void mouse_in_meta(void)
{
	// User clicks on object meta info, switch to meta mode
	if (mousestate.buttons & 1)
	{
		edit_mode = MODE_OBJECTS;	
	}
}

void plane_handle_mouse(void)
{
	// Mouse is in map region

	if (display_mouse_region(
		PLANE_DRAW_X,PLANE_DRAW_Y,
		PLANE_DRAW_W*TILESIZE,PLANE_DRAW_H*TILESIZE))
	{
		active_window = WINDOW_MAP;
		mouse_in_map();
	}
	// Mouse is in the VRAM region
	else if (display_mouse_region(VRAM_DRAW_X,VRAM_DRAW_Y,CHR_W,CHR_H))
	{
		active_window = WINDOW_VRAM;
		mouse_in_vram();
	}
	// Mouse is in the object list region
	else if (display_mouse_region(OBJ_DRAW_X, OBJ_DRAW_Y, OBJ_W, OBJ_H))
	{
		mouse_in_objlist();
		active_window = WINDOW_OBJ;
	}
	// Mouse is in the meta region
	else if (display_mouse_region(META_DRAW_X, META_DRAW_Y, META_DRAW_W, META_DRAW_H))
	{
		mouse_in_meta();
		active_window = WINDOW_META;
	}

}

void plane_scroll_limits(unsigned int *x, unsigned int *y)
{
	*x = ((MAP_WIDTH*map_header.w) - (PLANE_DRAW_W));
	*y = ((MAP_HEIGHT*map_header.h) - (PLANE_DRAW_H));
	printf("Scroll limits: %d, %d\n",*x,*y);
}

void plane_print_label(unsigned int x, unsigned int y, ALLEGRO_COLOR col, const char *msg)
{
	al_draw_filled_rectangle(
		x, y - 9, 
		x + (8 * strlen(msg)) + 1, y - 1, 
		al_map_rgb(0,0,0));
	al_draw_text(font,col,x, y - 11,0,msg);
}

void plane_handle_io(void)
{
	// Scrolling about the map
	if (active_window == WINDOW_MAP && !al_key_down(&keystate, ALLEGRO_KEY_LCTRL))
	{
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
	else if (active_window == WINDOW_OBJ)
	{
		if (al_key_down(&keystate, ALLEGRO_KEY_DOWN) && obj_list_scroll < (MAP_NUM_OBJS - OBJ_LIST_LEN))
		{
			obj_list_scroll++;
		}
		else if (al_key_down(&keystate, ALLEGRO_KEY_UP) && obj_list_scroll > 0)
		{
			obj_list_scroll--;
		}
	}

	// User hits save ikey
	while (!al_is_event_queue_empty(input_queue))
	{
		ALLEGRO_EVENT ev;
		al_get_next_event(input_queue, &ev);
		uint8_t entry_val;
	
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
				// Let Ctrl+S save 
				case ALLEGRO_KEY_F5:
					map_save();
					break;
				case ALLEGRO_KEY_F6:
				case ALLEGRO_KEY_T:
					tile_src_size = (tile_src_size == SEL_FULL) ? 0 : SEL_FULL;
					break;
				case ALLEGRO_KEY_F8:
				case ALLEGRO_KEY_I:
					map_data_interview();
					plane_load_fg();

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
				// Entering hex values for object data
				case ALLEGRO_KEY_0:
				case ALLEGRO_KEY_PAD_0:
					entry_val = 0;
					goto do_hex_entry;
				case ALLEGRO_KEY_1:
				case ALLEGRO_KEY_PAD_1:
					entry_val = 1;
					goto do_hex_entry;
				case ALLEGRO_KEY_2:
				case ALLEGRO_KEY_PAD_2:
					entry_val = 2;
					goto do_hex_entry;
				case ALLEGRO_KEY_3:
				case ALLEGRO_KEY_PAD_3:
					entry_val = 3;
					goto do_hex_entry;
				case ALLEGRO_KEY_4:
				case ALLEGRO_KEY_PAD_4:
					entry_val = 4;
					goto do_hex_entry;
				case ALLEGRO_KEY_5:
				case ALLEGRO_KEY_PAD_5:
					entry_val = 5;
					goto do_hex_entry;
				case ALLEGRO_KEY_6:
				case ALLEGRO_KEY_PAD_6:
					entry_val = 6;
					goto do_hex_entry;
				case ALLEGRO_KEY_7:
				case ALLEGRO_KEY_PAD_7:
					entry_val = 7;	
					goto do_hex_entry;
				case ALLEGRO_KEY_8:
				case ALLEGRO_KEY_PAD_8:
					entry_val = 8;
					goto do_hex_entry;
				case ALLEGRO_KEY_9:
				case ALLEGRO_KEY_PAD_9:
					entry_val = 9;
					goto do_hex_entry;
				case ALLEGRO_KEY_A:
					entry_val = 0xA;
					goto do_hex_entry;
				case ALLEGRO_KEY_B:
					entry_val = 0xB;
					goto do_hex_entry;
				case ALLEGRO_KEY_C:
					entry_val = 0xC;
					goto do_hex_entry;
				case ALLEGRO_KEY_D:
					entry_val = 0xD;
					goto do_hex_entry;
				case ALLEGRO_KEY_E:
					entry_val = 0xE;
					goto do_hex_entry;
				case ALLEGRO_KEY_F:
					entry_val = 0xF;
					goto do_hex_entry;
do_hex_entry:
					if (edit_mode == MODE_OBJECTS)
					{
						if (meta_cursor_pos >= 0 && meta_cursor_pos < 4)
						{
							map_obj *o = &map_header.objects[obj_list_sel];
							o->data = o->data & (~(0xF << (3 - meta_cursor_pos) * 4));
							o->data += entry_val << (3 - meta_cursor_pos) * 4;
							meta_cursor_pos++;
							if (meta_cursor_pos >= 4)
							{
								meta_cursor_pos = -1;
							}
						}
					}
					break;
				case ALLEGRO_KEY_ENTER:
					if (edit_mode == MODE_OBJECTS)
					{
						if (meta_cursor_pos == -1)
						{
							meta_cursor_pos = 0;
						}
						else
						{
							meta_cursor_pos = -1;
						}
					}
					break;
				case ALLEGRO_KEY_PAD_PLUS:
				case ALLEGRO_KEY_EQUALS:
					if (edit_mode == MODE_OBJECTS)
					{
						map_obj *o = &map_header.objects[obj_list_sel];
						o->type++;
					}
					break;
				case ALLEGRO_KEY_PAD_MINUS:
				case ALLEGRO_KEY_MINUS:
					if (edit_mode == MODE_OBJECTS)
					{
						map_obj *o = &map_header.objects[obj_list_sel];
						o->type--;
					}
					break;
			}
		}
	}
}
