#include "map.h"
#include "display.h"

const char *map_fname;

uint16_t *map_data;
map_file map_header;

void map_new()
{
	char in_buffer[MAP_NAME_LEN];
	memset(in_buffer, 0, sizeof(char) * MAP_NAME_LEN);
	memset(&map_header, 0, sizeof(map_file));

	printf("[map] Creating new room.\n");
	printf("\nRoom title? (%d chars)\n",MAP_NAME_LEN - 1);
	printf("<");
	for (int i = 0; i < MAP_NAME_LEN - 1; i++)
	{
		printf("-");
	}
	printf(">\n>");
	fgets(in_buffer, MAP_NAME_LEN, stdin);
	memset(map_header.name,0,MAP_NAME_LEN);
	snprintf(map_header.name,MAP_NAME_LEN - 1,"%s",in_buffer);
	printf("\nRoom ID? (integer)\n>");
	fgets(in_buffer, 3, stdin);
	map_header.id = atoi(in_buffer);
	while (map_header.w == 0 || map_header.w >= MAP_MAX_WIDTH)
	{
		printf("\nWidth in %d-column screens? (integer)\n>",MAP_WIDTH);
		fgets(in_buffer, 3, stdin);
		map_header.w = atoi(in_buffer);
	}
	while (map_header.h == 0 || map_header.h >= MAP_MAX_HEIGHT)
	{
		printf("\nHeight in %d-row screens? (integer)\n>",MAP_HEIGHT);
		fgets(in_buffer, 3, stdin);
		map_header.h = atoi(in_buffer);
	}

	if (map_data)
	{
		free(map_data);
		map_data = NULL;
	}
	printf(" ( creating a map %d by %d screens)\n",map_header.w,map_header.h);
	size_t map_nwords = (map_header.w * MAP_WIDTH) * (map_header.h * 32);
	map_data = (uint16_t *)calloc(sizeof(uint16_t),map_nwords);

	printf("\nTop-left map X? (integer)\n>");
	fgets(in_buffer, 3, stdin);
	map_header.map_x = atoi(in_buffer);

	printf("\nTop-left map Y? (integer)\n>");
	fgets(in_buffer, 3, stdin);
	map_header.map_y = atoi(in_buffer);

	printf("\nTileset chioce? (integer)\n>");
	fgets(in_buffer, 3, stdin);
	map_header.tileset = atoi(in_buffer);

	printf("\nSprite palette? (integer)\n>");
	fgets(in_buffer, 3, stdin);
	map_header.sprite_palette = atoi(in_buffer);

	printf("\nBackground? (integer)\n>");
	fgets(in_buffer, 3, stdin);
	map_header.background = atoi(in_buffer);
}

void map_save()
{
	printf("[map] Saving room to %s\n", map_fname);
	FILE *f = fopen(map_fname,"wb");
	if (!f)
	{
		printf("[map] Error: Couldn't open %s for writing.\n", map_fname);
		return;
	}
	if (!map_data)
	{
		printf("[map] Error: No map data to write to %s.\n", map_fname);
		return;
	}
	fwrite(&map_header,sizeof(map_header),1,f);
	// Swap endianness again during save
	size_t map_nwords = (map_header.w * MAP_WIDTH) * (map_header.h * MAP_HEIGHT);
	for (unsigned int i = 0; i < map_nwords; i++)
	{
		fputc(map_data[i] >> 8,f); // MSB first
		fputc((unsigned char)map_data[i],f); // LSB first
	}
	printf("[map] Done writing to %s at %lX.\n", map_fname,ftell(f));

	sprintf(display_title,"%s",map_fname);
	al_set_window_title(display, display_title);
	fclose(f);
}

void map_load()
{
	printf("[map] Loading room from %s\n", map_fname);
	FILE *f = fopen(map_fname,"rb");
	if (!f)
	{
		printf("[map] Couldn't open %s for reading.\n", map_fname);
		map_new();
		map_save();
		return;
	}
	else
	{
		fread(&map_header,sizeof(map_header),1,f);
		if (map_data)
		{
			free(map_data);
			map_data = NULL;
		}
		size_t map_nwords = (map_header.w * MAP_WIDTH) * (map_header.h * MAP_HEIGHT);
		printf("Got map sized %d by %d\n",map_header.w * MAP_WIDTH,map_header.h * MAP_HEIGHT);
		map_data = (uint16_t *)calloc(sizeof(uint16_t),map_nwords);

		// Read in file, swapping from little endian to big endian on the fly
		for (unsigned int i = 0; i < map_nwords; i++)
		{
			uint16_t val;
			val = fgetc(f) << 8; // MSB first
			val += fgetc(f); // LSB first
			map_data[i] = val;
		}
		printf("[map] Done loading to %s at %lX.\n", map_fname, ftell(f));
		printf("[map] Map name: %s\n",map_header.name);
		printf("[map] ID: %d\n",map_header.id);
		printf("[map] W: %d\n",map_header.w);
		printf("[map] H: %d\n",map_header.w);
		printf("[map] X: %d\n",map_header.map_x);
		printf("[map] Y: %d\n",map_header.map_y);
		printf("[map] Tileset: %d\n",map_header.tileset);
		printf("[map] Sprite Pal: %d\n",map_header.sprite_palette);
		printf("[map] Background: %d\n",map_header.background);
		fclose(f);
	}
}
