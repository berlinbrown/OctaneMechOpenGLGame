//
// octree.h
//
#ifndef _OCTREE_H_
#define _OCTREE_H_

#include "bot.h"
#include "gldrawlib.h"
#include "plist.h"

#define MAX_TRAIL_STEPS 500

#define MAX_OCTREE 2000

//
// Dont tell you CS instructor that this
// is a octree, it is merely a hashtable
// the key is generated from the xmin,xmax, etc
// - good number of elements is 2000
typedef struct tagOctree
{
  float x_min;
  float x_max;
  float y_min;
  float y_max;

  int max_elements;
  PtrList* list;

} Octree;

void pheromoneBuild(void);
void pheromoneDestroy(void);
void pheromoneInsert(StaticBotPtr bot);
void pheromoneDelete(StaticBotPtr bot);
StaticBotPtr pheromoneSearch(DriverBotPtr bot);

#endif
