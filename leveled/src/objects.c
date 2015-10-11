#include "objects.h"

static const char *obj_name[] =
{
	"        ",
	"Room Ptr",
	"Cube    ",
	"Metagrub",
	"Flip    ",
	"Boingo  ",
	0
};

static const int obj_width[] =
{
	0,
	16,
	16,
	24,
	24,
	16,
	0
};

static const int obj_height[] = 
{
	0,
	32,
	16,
	8,
	16,
	16,
	0
};

int width_for_obj(int i)
{
	return obj_width[i];
}

int height_for_obj(int i)
{
	return obj_height[i];
}

const char *string_for_obj(int i)
{
	return obj_name[i];
}
