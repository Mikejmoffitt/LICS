#include "plane.h"

void plane_init(plane *p)
{
	printf("Plane init!\n");
	p->plane_data = NULL;
	p->tileset_chr = NULL;
	p->bg_chr = NULL;
	p->tileset_num = 0;
	p->bg_num = 0;
	p->plane_w = 1;
	p->plane_h = 1;
	p->level_bg = NULL;

	// Generate checker backdrop

	printf ("Making checker BG\n");
	p->level_bg = al_create_bitmap(PLANE_DRAW_W*TILESIZE,PLANE_DRAW_H*TILESIZE);
	al_set_target_bitmap(p->level_bg);
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
}

void plane_destroy(plane *p)
{
	if (p == NULL)
	{
		return;
	}
	if (p->tileset_chr)
	{
		al_destroy_bitmap(p->tileset_chr);
	}
	if (p->bg_chr)
	{
		al_destroy_bitmap(p->bg_chr);
	}
	if (p->level_bg)
	{
		al_destroy_bitmap(p->level_bg);
	}
	if (p->plane_data)
	{
		free(p->plane_data);
	}
}

// ------- Graphical resource IO -----------
void plane_load_tileset(plane *p, const char *tile, const char *pal)
{
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
	p->tileset_chr = mdgfx_load_chr(tf,pf,CHR_T_W,CHR_T_H);
	printf("Loaded tileset graphics data.\n");
}

void plane_load_bg(plane *p, const char *tile, const char *pal)
{
	ALLEGRO_FILE *tf = al_fopen(tile,"rb");
	if (!tf)
	{
		printf("Error: couldn't open %s for tile data.\n",tile);
		return;
	}
	ALLEGRO_FILE *pf = al_fopen(pal,"rb");
	if (!pf)
	{
		printf("Error: couldn't open %s for palette data.\n",pal);
		if (tf) {al_fclose(tf);}
	}
	p->bg_chr = mdgfx_load_chr(tf,pf,CHR_T_W,CHR_T_H);
	printf("Loaded backdrop graphics data.\n");
}

void plane_load_data(plane *p, const char *d)
{
	ALLEGRO_FILE *df = al_fopen(d,"rb");
	if (!df)
	{
		printf("Error: couldn't open %s for plane data.\n",d);
	}
	int64_t size = al_fsize(df);
	if (size == -1)
	{
		printf("Can't get plane data file size.\n");
		al_fclose(df);
		return;
	}
	p->plane_data = (u16 *)malloc(sizeof(u8) * size);
	if (!p->plane_data) 
	{
		printf("Couldn't malloc for plane data.\n");
		return;
	}
	int64_t idx = 0;
	while (!al_feof(df) && idx < size)
	{
		p->plane_data[idx] = al_fread16be(df);
		idx += 2;
	}
	printf("Loaded plane data.\n");
	al_fclose(df);
}

// --------- Map file IO ----------

// Loads a map from a file. Includes init of hte plane structure
int plane_load_map(plane *p, const char *m)
{
	ALLEGRO_FILE *mf = al_fopen(m, "r");
	if (!mf)
	{
		printf("Couldn't open %s for reading.\n",m);
		return 0;
	}

	if (p)
	{
		plane_destroy(p);
		p = NULL;
	}
	
	// First get dimensions of level
	

	while(!al_feof(mf))
	{
		
	}

	return 1;	
}

int plane_save_map(plane *p, const char *m)
{
	
	return 1;
}


void plane_create_data(plane *p, u32 w, u32 h)
{
	p->plane_w = w;
	p->plane_h = h;
	if (p->plane_data)
	{
		free(p->plane_data);
	}
	p->plane_data = (u16 *)malloc(sizeof(u16) * (40*w) * (32*h));
	if (!p->plane_data)
	{
		printf("Couldn't malloc for plane data.\n");
		return;
	}
	for (int i = 0; i < w * h * 40 * 32; i++)
	{
		p->plane_data[i] = i;
	}
}

void plane_draw_map(plane *p, u32 x, u32 y)
{
	ALLEGRO_COLOR col = (active_window == WINDOW_MAP) ? 
		al_map_rgb(PLANE_BORDER_COLOR) : 
		al_map_rgb(PLANE_INACTIVE_COLOR);
	al_set_target_bitmap(main_buffer);
	ALLEGRO_BITMAP *chr = p->tileset_chr;
	if (!p->plane_data) { return; }

	// Put a border around the map
	al_draw_rectangle(x - 4, y - 4, 
		x + (TILESIZE * PLANE_DRAW_W) + 4, 
		y + (TILESIZE * PLANE_DRAW_H) + 4,
		col,PLANE_BORDER_THICKNESS);
	al_draw_bitmap(p->level_bg,x,y,0);
	// Render the map
	for (u32 i = 0; i < PLANE_DRAW_H; i++)
	{
		if (i >= p->plane_h * 32)
		{
			return;
		}	
		for (u32 j = 0; j < PLANE_DRAW_W; j++)
		{
			if (j >= p->plane_w * 40)
			{
				continue;	
			}	
			u32 t_idx = j + scroll_x;
			t_idx += (i + scroll_y) * (40 * p->plane_w);
			u16 t_choice = p->plane_data[t_idx];
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

void plane_draw_vram(plane *p, u32 x, u32 y)
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
	al_draw_bitmap(p->tileset_chr,x,y,0);

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

void plane_handle_mouse(plane *p)
{
	// Mouse is in map regoin


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
			u32 t_idx = cursor_x + ((cursor_y) * (40 * p->plane_w));
			p->plane_data[t_idx] = selection;
			if (sel_size == SEL_FULL)
			{
				t_idx++;
				p->plane_data[t_idx] = selection + 1;
				t_idx += (p->plane_w * 40) - 1;
				p->plane_data[t_idx] = selection + (CHR_T_W);
				t_idx += 1;
				p->plane_data[t_idx] = selection + (CHR_T_W + 1);
			}
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

void plane_scroll_limits(plane *p, u32 *x, u32 *y)
{
	*x = ((40*p->plane_w) - (PLANE_DRAW_W));
	*y = ((32*p->plane_h) - (PLANE_DRAW_H));
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

void plane_handle_io(plane *p, const char *m)
{
	if (al_key_down(&keystate,ALLEGRO_KEY_F5))
	{
		plane_save_map(p, m);
	}
	// User hits save ikey
}
