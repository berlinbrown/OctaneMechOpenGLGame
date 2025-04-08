/**
 * Copyright (c) 2006-2011 Berlin Brown.  All Rights Reserved
 *
 * http://www.opensource.org/licenses/bsd-license.php
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of the Botnode.com (Berlin Brown) nor
 * the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Date: 8/15/2011
 *
 * Description: Simple OpenGL Mech Game
 *
 * Contact: Berlin Brown <berlin dot brown at gmail.com>
 */

//
// maps.cpp
// - this is going to be similar to
// my octree code but a little bit more
// refined
//
// The codes the map grid into sections
// and each section has so many bots inside
//
// - in the end, the map code will be a linked list
// of lists
//
// Note: of course since we are using this mainly for
// line of sight algorithms, then the line of sight
// will extend into several different maps,hmm
// a bit of a pickle...
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bot.h"
#include "plist.h"
#include "maps.h"

PtrList *map_list;

// we need a place to hold the pointers, that is map_ptrs
Map *map_ptrs[MAX_MAPS];
int max_maps;

// Note: max_maps and MAX_MAPS should probably not match

//
// Create Map
// - only sets everything to zero
//
Map *CreateMap(int id_no)
{
	Map *map;

	map = malloc(sizeof(Map));

	// I like to be extra careful
	ZeroMemory((Map *)map,
			   sizeof(Map));

	map->id = id_no;
	map->list = NULL;

	map->list = CreatePtrList();

	map->x_max = map->x_min = 0.0f;
	map->y_max = map->y_min = 0.0f;

	return map;

} // end of the function

//
// Load Map
//
void LoadMap(Map *map, float x_min, float x_max,
			 float y_min, float y_max)
{
	map->x_max = x_max;
	map->x_min = x_min;
	map->y_max = y_max;
	map->y_min = y_min;

} // end of the function

//
// DeleteMap
//
void DeleteMap(Map *map)
{

	// free(map->list);
	// free(map);
	RELEASE_OBJECT(map->list);
	RELEASE_OBJECT(map);

} // end of the function

//
// DestroyMaps
//
void DestroyMaps(void)
{
	int index = 0;

	for (index = 0; index < max_maps; index++)
	{
		DeleteMap(map_ptrs[index]);

	} // end of the functino

	// this might need to go above the for
	DestroyPtrList(map_list);

} // end of the function

//
// PrintList
//
void PrintMapList(PtrList *list)
{
	PtrNode *current_ptr;
	Map *x;

	// if (isempty(list))
	//	return;

	if (list->head == NULL)
		return;

	current_ptr = list->head;

	while (current_ptr != NULL)
	{
		// interesting
		x = (Map *)current_ptr->ptr;

		printf("xmin: %0.2f xmax: %0.2f\n", x->x_min, x->x_max);

		current_ptr = current_ptr->next;
	} // end of while

} // end of the function

//
// Build The map list
//
void BuildMap(void)
{

	float i, j;
	float x_min, x_max;
	float y_min, y_max;
	float map_height, map_width;
	int index = 0;

	map_height = MAP_HEIGHT;
	map_width = MAP_WIDTH;

	// create a new list of maps
	CREATE_MAPS;

	// Build a set a of maps
	for (i = MAP_MIN; i < MAP_MAX; i += map_height)
	{

		for (j = MAP_MIN; j < MAP_MAX; j += map_width)
		{

			x_min = j;
			y_min = i;
			x_max = j + map_width;
			y_max = i + map_height;

			map_ptrs[index] = CreateMap(index);
			LoadMap(map_ptrs[index], x_min, x_max, y_min, y_max);

			INSERT_MAP(map_ptrs[index]);

			index++; // number of maps
			if (index >= MAX_MAPS)
				break;

		} // end of the for

	} // end of the for

	max_maps = index;

	DESTROY_MAPS;

} // end of the function
