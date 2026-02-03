//
// maps.h
// - see maps.cpp
//
#ifndef _MAPS_H_
#define _MAPS_H_

#include "plist.h"

#define MAX_MAPS 120

#define MAP_WIDTH 7.2f
#define MAP_HEIGHT 7.2f

#define MAP_MIN -30.0f
#define MAP_MAX 30.0f

typedef struct tagMap
{
  int id;

  float x_min;
  float x_max;
  float y_min;
  float y_max;

  PtrList* list;

} Map;

void BuildMap(void);

#define INSERT_MAP(val) InsertFront(map_list, (Map*)val)
#define DESTROY_MAPS        \
  DestroyPtrList(map_list); \
  DestroyMaps()
#define CREATE_MAPS map_list = CreatePtrList()

#endif