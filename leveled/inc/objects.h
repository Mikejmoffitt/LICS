#ifndef OBJECTS_H
#define OBJECTS_H

#define OBJ_NULL 0
#define OBJ_ROOMPTR 1
#define OBJ_CUBE 2
#define OBJ_METAGRUB 3

int width_for_obj(int i);
int height_for_obj(int i);
const char *string_for_obj(int i);

#endif
