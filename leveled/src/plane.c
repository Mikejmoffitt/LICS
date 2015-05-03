#include "plane.h"

void plane_init(plane *p)
{
	p->plane_data = NULL;
	p->tileset_chr = NULL;
	p->bg_chr = NULL;
	p->tileset_num = 0;
	p->bg_num = 0;
	p->plane_w = 1;
	p->plane_h = 1;

	// Generate checker backdrop
	if (p->level_bg)
	{
		al_destroy_bitmap(p->level_bg);
	}
	p->level_bg = al_create_bitmap(PLANE_DRAW_W*TILESIZE,PLANE_DRAW_H*TILESIZE);
	al_set_target_bitmap(p->level_bg);
	al_clear_to_color(al_map_rgb(0,0,0));
	for (u32 y = 0; y < PLANE_DRAW_H * 2; y++)
	{
		for (u32 x = 0; x < PLANE_DRAW_W * 2; x++)
		{
			if ((x + y) % 2 ==  0)
			{
				al_draw_filled_rectangle(
					x*TILESIZE/2,y*TILESIZE/2,
					(x+1)*TILESIZE/2,(y+1)*TILESIZE/2,
					al_map_rgb(8,8,8));
			}
		}
	}
}

void plane_load_tileset(plane *p, char *tile, char *pal)
{
	ALLEGRO_FILE *tf = al_fopen(tile,"r");
	if (!tf)
	{
		printf("Error: couldn't open %s for tile data.\n",tile);
		return;
	}
	ALLEGRO_FILE *pf = al_fopen(pal,"r");
	if (!pf)
	{
		printf("Error: couldn't open %s for palette data.\n",pal);
		if (tf) {al_fclose(tf);}
	}
	p->tileset_chr = mdgfx_load_chr(tf,pf,CHR_T_W,CHR_T_H);
	printf("Loaded tileset graphics data.\n");
}

void plane_load_bg(plane *p, char *tile, char *pal)
{
	ALLEGRO_FILE *tf = al_fopen(tile,"r");
	if (!tf)
	{
		printf("Error: couldn't open %s for tile data.\n",tile);
		return;
	}
	ALLEGRO_FILE *pf = al_fopen(pal,"r");
	if (!pf)
	{
		printf("Error: couldn't open %s for palette data.\n",pal);
		if (tf) {al_fclose(tf);}
	}
	p->bg_chr = mdgfx_load_chr(tf,pf,CHR_T_W,CHR_T_H);
	printf("Loaded backdrop graphics data.\n");
}

void plane_load_data(plane *p, char *d)
{
	ALLEGRO_FILE *df = al_fopen(d,"r");
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

void plane_create_data(plane *p, u32 w, u32 h)
{
	p->plane_w = w;
	p->plane_h = h;
	if (p->plane_data)
	{
		free(p->plane_data);
	}
	p->plane_data = (u16 *)malloc(sizeof(u16) * (32*w) * (32*h));
	if (!p->plane_data)
	{
		printf("Couldn't malloc for plane data.\n");
		return;
	}
	for (int i = 0; i < w * h * 32 * 32; i++)
	{
		p->plane_data[i] = i;
	}
}

void plane_draw_map(plane *p, u32 x, u32 y)
{
	al_set_target_bitmap(main_buffer);
	ALLEGRO_BITMAP *chr = p->tileset_chr;
	if (!p->plane_data) { return; }

	// Put a border around the map
	al_draw_rectangle(x - 2, y - 2, 
		x + (TILESIZE * PLANE_DRAW_W) + 2, 
		y + (TILESIZE * PLANE_DRAW_H) + 2,
		al_map_rgb(PLANE_BORDER_COLOR),PLANE_BORDER_THICKNESS);
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
			if (j >= p->plane_w * 32)
			{
				continue;	
			}	
			u32 t_idx = j + scroll_x;
			t_idx += (i + scroll_y) * (32 * p->plane_w);
			u16 t_choice = p->plane_data[(((scroll_y+i)*(32 * p->plane_w)) + j + scroll_x)];
			// determine coords of tile to pull from tileset from buffer
			u32 t_x = TILESIZE * (t_choice % CHR_T_W);
			u32 t_y = TILESIZE * (t_choice/CHR_T_W);
			al_draw_bitmap_region(chr, t_x, t_y, TILESIZE, TILESIZE, 
				x + (TILESIZE * j), y + (TILESIZE * i),0);
		}
	}
}

void plane_draw_vram(plane *p, u32 x, u32 y)
{
	al_set_target_bitmap(main_buffer);
	// Put a border around the VRAM window
	al_draw_rectangle(x - 2, y - 2, 
		x + (CHR_W) + 2, y + (CHR_H) + 2,
		al_map_rgb(PLANE_BORDER_COLOR),PLANE_BORDER_THICKNESS);

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
}

void plane_render(u16 *p, u32 scroll_x, u32 scroll_y)
{
	
}

void plane_scroll_limits(plane *p, u32 *x, u32 *y)
{
	printf("Setting scroll limits. Old values: %d, %d\n",*x,*y);
	*x = ((32*p->plane_w) - (PLANE_DRAW_W));
	*y = ((32*p->plane_h) - (PLANE_DRAW_H));
	printf("New values: %d, %d\n",*x,*y);
}
