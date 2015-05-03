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
	p->tileset_chr = mdgfx_load_chr(tf,pf,CHR_W/TILESIZE,CHR_H*TILESIZE);
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
	p->bg_chr = mdgfx_load_chr(tf,pf,CHR_W/TILESIZE,CHR_H*TILESIZE);
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
	int64_t idx = 0;
	while (!al_feof(df) && idx < size)
	{
		p->plane_data[idx] = al_fread16be(df);
		idx += 2;
	}
	printf("Loaded plane data.\n");
	al_fclose(df);
}

void plane_render_full(u16 *p, u32 scroll_x, u32 scroll_y)
{
	
}

