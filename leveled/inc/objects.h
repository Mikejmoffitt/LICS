#ifndef OBJECTS_H
#define OBJECTS_H

#define OBJ_NULL 0
#define OBJ_ROOMPTR 1
#define OBJ_CUBE 2
#define OBJ_METAGRUB 3
#define OBJ_FLIP 4
#define OBJ_BOINGO 5
#define OBJ_ITEM 6
#define OBJ_GAXTER1 7
#define OBJ_GAXTER2 8
#define OBJ_BUGGO1 9
#define OBJ_BUGGO2 10
#define OBJ_DANCYFLOWER 11
#define OBJ_JRAFF 12
#define OBJ_PILLA 13
#define OBJ_HEDGEDOG 14
#define OBJ_SHOOT 15
#define OBJ_LASER 16
#define OBJ_KILLZAM 17
#define OBJ_FLARGY 18
#define OBJ_PLANT 19
#define OBJ_TOSSMUFFIN 20
#define OBJ_TELEPORTER 21
#define OBJ_MAGIBEAR 22
#define OBJ_LAVA 23
#define OBJ_COW 24
#define OBJ_CONTAINER 25
#define OBJ_HOOP 26
#define OBJ_FALSEBLOCK 27
#define OBJ_CP_PAD 28
#define OBJ_CP_METER 29
#define OBJ_DOG 30
#define OBJ_ELEVATOR 31
#define OBJ_ELEVATOR_STOP 32
#define OBJ_FISSINS1 33
#define OBJ_BOSS1 34
#define OBJ_BOSS2 35
#define OBJ_BOSSF1 36
#define OBJ_BOSSF2 37
#define OBJ_EGG 38
#define OBJ_FISSINS2 39
#define OBJ_BOUNDS 40
#define OBJ_SMALLEGG 41
#define OBJ_BASKETBALL 42
#define OBJ_LAVAANIM 43
#define OBJ_SPOOKO 44
#define OBJ_WIP 45

int width_for_obj(int i);
int height_for_obj(int i);
const char *string_for_obj(int i);
int num_obj_types(void);

#endif
