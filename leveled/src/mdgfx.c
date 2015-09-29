#include "mdgfx.h"

ALLEGRO_COLOR mdgfx_color_entry(uint16_t entry)
{
	uint8_t r, g, b;

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
	unsigned int i = 0;
	while (i < 16 && !al_feof(pf))
	{
		// Get next Motorola style (big-endian) palette entry
		uint16_t entry = al_fread16be(pf);
		if (i % 8 == 0)
		{
			printf("\n");
		}
		printf("[%3X] ",entry);
		*c = mdgfx_color_entry(entry);
		c++; // Hehehehe C++ get it?
		i++;
	}
}

void mdgfx_plot_tile(uint8_t *t, uint16_t x, uint16_t y, ALLEGRO_BITMAP *d, ALLEGRO_COLOR *c)
{
	al_set_target_bitmap(d);
	// Remember x and y refer to the tile position in bitmap d, not pixel position
	// so they represent a pixel position with a factor of 8. 
	// tx and ty refer to pixel within the current tile. Since the bitmap is one
	// large one we can't just address it as a tile directly unless we stored a 
	// buffer for each 8x8 tile.
	uint8_t *start_t = t;
	for (unsigned int ty = 0; ty < 8; ty++)
	{
		for (unsigned int tx = 0; tx < 8; tx++)
		{
			// Which color to use to draw th pixel
			unsigned int draw_col;
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

			unsigned int plot_x = (x * 8) + tx;
			unsigned int plot_y = (y * 8) + ty;

			// Place pixel on target bitmap
			al_put_pixel(plot_x, plot_y, c[draw_col]);
		}
	}
}

unsigned int mdgfx_file_sanity_checks(ALLEGRO_FILE *tf, ALLEGRO_FILE *pf)
{
	if (!pf)
	{
		printf("[mdgfx] Could not open palette file for reading.\n");
		return 0;
	}
	if (!tf)
	{
		printf("[mdgfx] Could not open tile file for reading.\n");
		return 0;
	}
	if (al_fsize(pf) < 32)
	{
		printf("[mdgfx] Warning: Palette file is not of expected size. \n");
		printf("[mdgfx] (Expected %d, got %ld)\n",32,al_fsize(pf));
		printf("[mdgfx] The palette may be incorrect.\n");
	}
	if (al_fsize(tf) < 32)
	{
		printf("[mdgfx] Warning: Tile data file is not of expected size. \n");
		printf("[mdgfx]  (Expected at least %d, got %ld)\n",32,al_fsize(tf));
		printf("[mdgfx] The tile data may be incorrect.\n");
	}
	return 1;
}


ALLEGRO_BITMAP *mdgfx_load_chr(ALLEGRO_FILE *tf, ALLEGRO_FILE *pf,unsigned int w,unsigned int h)
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
		printf("[mdgfx] Couldn't create CHR buffer.\n");
		return NULL;
	}
	ALLEGRO_COLOR pal[16];
	mdgfx_pal_load_loop(&pal[0], pf);
	pal[0] = al_map_rgba(0,0,0,0);
	
	// Set a default palette of a greyscale gradient

	printf("[mdgfx] mdgfx_load_chr(tf,pf,%d,%d)\n",w,h);
	for (unsigned int y = 0; y < h; y++)
	{
		if (al_feof(tf))
		{	
			al_set_target_bitmap(ret);
			al_draw_bitmap(workbuffer,0,0,0);
			al_destroy_bitmap(workbuffer);
			printf("[mdgfx] File ended early.\n");
			return ret;
		}
		for (unsigned int x = 0; x < w; x++)
		{
			if (al_feof(tf))
			{
				al_set_target_bitmap(ret);
				al_draw_bitmap(workbuffer,0,0,0);
				al_destroy_bitmap(workbuffer);
				printf("[mdgfx] File ended early.\n");
				return ret;
			}
			// Array for one tile's worth of data
			uint8_t *t = (uint8_t *)malloc(sizeof(uint8_t) * 32);
			for (unsigned int z = 0; z < 32; z++)
			{
				t[z] = al_fgetc(tf);
				if (al_feof(tf))
				{
					al_set_target_bitmap(ret);
					al_draw_bitmap(workbuffer,0,0,0);
					al_destroy_bitmap(workbuffer);
					printf("[mdgfx] File ended early.\n");
					return ret;
				}
			}
			mdgfx_plot_tile(t, x, y, workbuffer, &pal[0]);
			free(t);
		}
	}
	al_set_target_bitmap(ret);
	al_draw_bitmap(workbuffer,0,0,0);
	al_destroy_bitmap(workbuffer);
	printf("[mdgfx] Finished reading tile data.\n");
	return ret;
}
