#include "mdgfx.h"

ALLEGRO_COLOR mdgfx_color_entry(u16 entry)
{
	u8 r, g, b;

	// Pull R, G, B from nybbles, roughly scale up to 0-255
	r = (entry & 0xE) * 0x12;
	entry = entry >> 4;
	g = (entry & 0xE) * 0x12;
	entry = entry >> 4;
	b = (entry & 0xE) * 0x12;
	return al_map_rgb(r,g,b);

}

void mdgfx_pal_load_loop(ALLEGRO_COLOR *c, ALLEGRO_FILE *pf)
{
	if (!pf)
	{
		printf("Error: palette file is not open.\n");
		return;
	}
	if (!c)
	{
		printf("Error: ALLEGRO_COLOR array is NULL.\n");
		return;
	}
	u32 i = 0;
	while (i < 16 && !al_feof(pf))
	{
		// Get next Motorola style (big-endian) palette entry
		u16 entry = al_fread16be(pf);
		printf("Got pal entry #%X: %3X\n",i,entry);
		*c = mdgfx_color_entry(entry);
		c++; // Hehehehe C++ get it?
		i++;
	}
	printf("Got Megadrive palette.\n");
}

void mdgfx_plot_tile(u8 *t, u16 x, u16 y, ALLEGRO_BITMAP *d, ALLEGRO_COLOR *c)
{
	al_set_target_bitmap(d);
	// Remember x and y refer to the tile position in bitmap d, not pixel position
	// so they represent a pixel position with a factor of 8. 
	// tx and ty refer to pixel within the current tile. Since the bitmap is one
	// large one we can't just address it as a tile directly unless we stored a 
	// buffer for each 8x8 tile.
	u8 *start_t = t;
	for (u32 ty = 0; ty < 8; ty++)
	{
		for (u32 tx = 0; tx < 8; tx++)
		{
			// Which color to use to draw th pixel
			u32 draw_col;
			// Even pixel, use high nybble
			if (tx % 2 == 0)
			{
				draw_col = ((*t) & 0xF0) >> 4;	
			}
			// Odd pixel, use low nybble
			else
			{
				draw_col = (*t) & 0xF;
				// Increment T after each odd pixel for next byte
				t = t + 1;
			}

			u32 plot_x = (x * 8) + tx;
			u32 plot_y = (y * 8) + ty;

			// Place pixel on target bitmap
			al_put_pixel(plot_x, plot_y, c[draw_col]);
		}
	}
}

u32 mdgfx_file_sanity_checks(ALLEGRO_FILE *tf, ALLEGRO_FILE *pf)
{
	if (!pf)
	{
		printf("Could not open palette file for reading.\n");
		return 0;
	}
	if (!tf)
	{
		printf("Could not open tile file for reading.\n");
		return 0;
	}
	if (al_fsize(pf) < 32)
	{
		printf("Warning: Palette file is not of expected size. \n");
		printf("(Expected %d, got %ld)\n",32,al_fsize(pf));
		printf("The palette may be incorrect.\n");
	}
	if (al_fsize(tf) < 32)
	{
		printf("Warning: Tile data file is not of expected size. \n");
		printf(" (Expected at least %d, got %ld)\n",32,al_fsize(tf));
		printf("The tile data may be incorrect.\n");
	}
	return 1;
}


ALLEGRO_BITMAP *mdgfx_load_chr(ALLEGRO_FILE *tf, ALLEGRO_FILE *pf,u32 w,u32 h)
{
	if (!mdgfx_file_sanity_checks(tf,pf))
	{
		return NULL;
	}

	// It is faster to plot to a system memory bitmap
	al_set_new_bitmap_flags(ALLEGRO_MEMORY_BITMAP|ALLEGRO_ALPHA_TEST);
	ALLEGRO_BITMAP *workbuffer = al_create_bitmap(8 * w,8 * h);
	al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP|ALLEGRO_ALPHA_TEST);

	ALLEGRO_BITMAP *ret = al_create_bitmap(8 * w, 8 * h);
	al_set_target_bitmap(workbuffer);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_set_target_bitmap(ret);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	if (!workbuffer)
	{
		printf("Couldn't create CHR buffer.\n");
		return NULL;
	}
	ALLEGRO_COLOR pal[16];
	mdgfx_pal_load_loop(&pal, pf);
	pal[0] = al_map_rgba(0,0,0,0);
	
	// Set a default palette of a greyscale gradient

	printf("mdgfx_load_chr(tf,pf,%d,%d)\n",w,h);
	for (u32 y = 0; y < h; y++)
	{
		if (al_feof(tf))
		{	
			al_set_target_bitmap(ret);
			al_draw_bitmap(workbuffer,0,0,0);
			al_destroy_bitmap(workbuffer);
			printf("File ended early.\n");
			return ret;
		}
		for (u32 x = 0; x < w; x++)
		{
			if (al_feof(tf))
			{
				al_set_target_bitmap(ret);
				al_draw_bitmap(workbuffer,0,0,0);
				al_destroy_bitmap(workbuffer);
				printf("File ended early.\n");
				return ret;
			}
			// Array for one tile's worth of data
			u8 *t = (u8 *)malloc(sizeof(u8) * 32);
			for (u32 z = 0; z < 32; z++)
			{
				t[z] = al_fgetc(tf);
				if (al_feof(tf))
				{
					al_set_target_bitmap(ret);
					al_draw_bitmap(workbuffer,0,0,0);
					al_destroy_bitmap(workbuffer);
					printf("File ended early.\n");
					return ret;
				}
			}
			mdgfx_plot_tile(t, x, y, workbuffer, &pal);
			free(t);
		}
	}
	al_set_target_bitmap(ret);
	al_draw_bitmap(workbuffer,0,0,0);
	al_destroy_bitmap(workbuffer);
	printf("Finished reading tile data.\n");
	return ret;
}
