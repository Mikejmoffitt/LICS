#include "plane.h"
#include "objects.h"

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
	sprintf(dat2,"                                       ");

	switch (o->type)
	{
		case OBJ_NULL:
			sprintf(desc," ");
			break;
		case OBJ_ROOMPTR:
			sprintf(desc,"To (%2X, %X)",(o->data & 0xFF00) >> 8, (o->data & 0x00F0) >> 4);
			sprintf(dat2,"Door #%X (LSN)",o->data & 0x000F);
			break;
		case OBJ_DOG:
			sprintf(dat2,"Releases object after eating 3 eggs");
			goto cube_desc_area;
		case OBJ_CONTAINER:
			sprintf(dat2,"A background element that can be struck.");
			break;
		case OBJ_CUBE:
cube_desc_area:
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
				else if ((o->data & 0x00FF) == 0x10)
				{
					sprintf(dat2,"HP UP 2X");
				}
				else if ((o->data & 0x00FF) == 0x20)
				{
					sprintf(dat2,"CP UP");
				}
				else if ((o->data & 0x00FF) == 0x30)
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
			else
			{
				sprintf(dat2,"invalid (start at $0100)");
			}
			break;
		case OBJ_METAGRUB:
			sprintf(desc,"Lunges side to side");
			break;
		case OBJ_FLIP:
			sprintf(desc,"Flies left to right 100px");
			break;
		case OBJ_BOINGO:
			sprintf(desc,"Jumps left and right");
			if (o->data == 0x0001)
			{
				sprintf(dat2, "Angry version");
			}
			else if (o->data == 0x0002)
			{
				sprintf(dat2, "Cube version");
			}
			else
			{
				sprintf(dat2, "Normal version");
			}
			break;
		case OBJ_ITEM:
			if (o->data == 0x0000)
			{
				sprintf(desc, "Map screen");
			}
			else if (o->data == 0x0001)
			{
				sprintf(desc, "Cube lifting");
			}
			else if (o->data == 0x0002)
			{
				sprintf(desc, "Cube jumping");
			}
			else if (o->data == 0x0003)
			{
				sprintf(desc, "Phantom cubes");
			}
			else if (o->data == 0x0004)
			{
				sprintf(desc, "Kicking cubes");
			}
			else if (o->data == 0x0005)
			{
				sprintf(desc, "Orange cube lifting");
			}
			else
			{
				sprintf(desc, "Invalid item (defaults to map)");
			}
			break;
		case OBJ_GAXTER1:
			sprintf(desc,"Flies around, homing at player");
			break;
		case OBJ_GAXTER2:
			sprintf(desc,"Flies left and right, shoots at player");
			sprintf(dat2,"Shots angle against floor");
			break;
		case OBJ_BUGGO1:
			sprintf(desc,"Walks on ceiling left and right");
			sprintf(dat2,"Fires spike down when player approaches");
			break;
		case OBJ_BUGGO2:
			sprintf(desc,"Walks on floor left and right, firing sparks");
			sprintf(dat2,"Can only be hurt by a kicked cube");
			break;
		case OBJ_DANCYFLOWER:
			sprintf(desc,"Just sits there, waiting to get destroyed");
			break;
		case OBJ_JRAFF:
			sprintf(desc,"Walks left and right constrained by BG");
			sprintf(dat2,"Takes two hits");
			break;
		case OBJ_PILLA:
			sprintf(desc,"Walks left and right constrained by BG");
			if (o->data)
			{
				sprintf(dat2,"(head unit)");
			}
			else
			{
				sprintf(dat2,"(tail unit)");
			}
			break;
		case OBJ_HEDGEDOG:
			sprintf(desc,"Jumps up and fires a few shots");
			break;
		case OBJ_SHOOT:
			sprintf(desc,"Like Flip, but swoops down when player approaches");
			break;
		case OBJ_LASER:
			sprintf(desc,"A laser effect that fades in and out.");
			sprintf(dat2,"Hurts player when visible and touched.");
			break;
		case OBJ_KILLZAM:
			sprintf(desc,"Fizzles in, shoots at player, fizzles out.");
			sprintf(dat2,"Only vulnerable when visible");
			break;
		case OBJ_FLARGY:
			sprintf(desc,"Walks in a small tight zone, deflects cubes");
			sprintf(dat2,"Only vulnerable from behind");
			break;
		case OBJ_PLANT:
			sprintf(desc,"Evel plant fires shots at player");
			sprintf(dat2,"Takes two hits");
			break;
		case OBJ_TOSSMUFFIN:
			sprintf(desc,"Walks slowly, picks up and throws cubes");
			sprintf(dat2,"Takes two hits");
			break;
		case OBJ_TELEPORTER:
			sprintf(desc,"Triggers room event, and player exit.");
			if (o->data & 0xFF00)
			{
				sprintf(dat2, "Number %02X (activator)", o->data & 0xFF);
			}
			else
			{
				sprintf(dat2, "Number %02X", o->data & 0xFF);
			}
			break;
		case OBJ_MAGIBEAR:
			sprintf(desc,"Walks slowly, barfs shots at player");
			sprintf(dat2,"Takes three hits");
			break;
		case OBJ_LAVA:
			sprintf(desc,"Generates a column of lava");
			break;
		case OBJ_COW:
			sprintf(desc,"Will protect lyle from lava");
			sprintf(dat2,"Attack too much and he'll charge");
			break;
		case OBJ_HOOP:
			sprintf(desc,"When a ball is put through it vanishes.");
			sprintf(dat2,"Triggers a room event on vanish.");
			break;
		case OBJ_FALSEBLOCK:
			sprintf(desc,"A block that vanishes on room event.");
			break;
		case OBJ_CP_PAD:
			sprintf(desc,"Active area for the CP orb sucker");
			break;
		case OBJ_CP_METER:
			sprintf(desc,"Indicator for CP meter position");
			break;
		case OBJ_ELEVATOR:
			sprintf(desc,"Allows lyle transport up/down");
			break;
		case OBJ_ELEVATOR_STOP:
			sprintf(desc,"Indicates the elevator is to stop here");
			break;
		case OBJ_FISSINS1:
			sprintf(desc,"Jumps up/down from one spot");
			break;
		case OBJ_BOSS1:
			sprintf(desc,"Boss 1 room indicator");
			break;
		case OBJ_BOSS2:
			sprintf(desc,"Boss 2 room indicator");
			break;
		case OBJ_BOSSF1:
			sprintf(desc,"Final boss 1 room indicator");
			break;
		case OBJ_BOSSF2:
			sprintf(desc,"Final boss 2 room indicator");
			break;
		case OBJ_EGG:
			sprintf(desc,"Egg that will drop down when hit");
			break;
		case OBJ_FISSINS2:
			sprintf(desc,"Jumps up from sand; bound by marker");
			if (o->data)
			{
				sprintf(dat2,"This is a marker");
			}
			else
			{
				sprintf(dat2,"This is the fissins");
			}
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
	sprintf(msg, "(%03X,%03X-%c)%c",
		cursor_x, cursor_y, tile_src_size == SEL_FULL ? 'L' : 's', tile_prio ? 'P' : ' ');
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
