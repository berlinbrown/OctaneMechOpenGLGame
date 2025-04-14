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
// bsp.cpp
// This may border on octree but they are similar
//
// the goal is to use this code with the pheremones
// when they are dropped by the ants
// if an ant drops 100 pheremones on the way to the nest
// 100 * 500, it would be silly to have to test all of these
// so some algorithm is needed to save cpu time
//
// - the octree is an array of pointers to seperate lists
// - the lists contains the position of the pheremone
// - whenever a pheremone is created, it is added to the list
// - whenever a pheremone is deleted, is is removed from the list
// - whenever the bot moves, he searches the list
// - better than brute force, of course
// - since we are using a hash function, possibly O(1)
//

#include <stdlib.h>
#include <stdio.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

Octree **pheromone_tree;
Octree **GenerateOctree(void);

//
// CreateOctree
//
Octree *CreateOctree(void)
{
	Octree *ptr;
	ptr = (Octree *)malloc(sizeof(Octree));

	return ptr;

} // end of the function

void DeleteOctree(Octree **tree_ptr)
{
	// delete the lists on the tree
	int i;
	int max;
	max = tree_ptr[0]->max_elements;

	for (i = 0; i < max; i++)
	{
		DestroyPtrList(tree_ptr[i]->list);

	} // end of the for

	// have to delete every node individually
	for (i = 0; i < max; i++)
	{
		free(tree_ptr[i]);
	} // end of the for

	free(tree_ptr);

} // end of the function

//
// GenerateOctree
//  - struct has xmin,xmax,ymin, ymax
// and a pointer to a list
//
Octree **GenerateOctree(void)
{

	float i, j;
	float width = 7.5f;
	float height = width; // square grid
	float x_min, x_max, y_min, y_max;
	int hash_key = 0;
	int count = 0;
	int z = 0;
	int index = 0;
	Octree **tree_ptr;

	for (i = -30.0f; i < 30.0f; i += height)
		for (j = -30.0f; j < 30.0f; j += width)
		{
			count++;
		} // end of the for

	// First we have to create an array of pointers
	tree_ptr = (Octree **)malloc(count * sizeof(Octree *));

	// Create each node
	for (index = 0; index < count; index++)
		tree_ptr[index] = CreateOctree();

	z = 0; // reset counter

	// populate the tree with data
	for (i = -30.0f; i < 30.0f; i += height)
	{
		for (j = -30.0f; j < 30.0f; j += width)
		{
			x_min = j;
			y_min = i;
			x_max = j + width;
			y_max = i + width;

			tree_ptr[z]->list = CreatePtrList();
			tree_ptr[z]->x_max = x_max;
			tree_ptr[z]->x_min = x_min;
			tree_ptr[z]->y_max = y_max;
			tree_ptr[z]->y_min = y_min;
			tree_ptr[z]->max_elements = count;

			z++;
		} // end of the for

	} // end of the for

	return tree_ptr;

} // end of the function

//
// InsertOctree
// - insert a pheremone into the tree
//
void InsertOctree(Octree **tree_ptr, StaticBotPtr bot)
{
	int i;
	float x_min, y_min, x_max, y_max;
	int max = tree_ptr[0]->max_elements;

	for (i = 0; i < max; i++)
	{
		x_min = tree_ptr[i]->x_min;
		x_max = tree_ptr[i]->x_max;
		y_min = tree_ptr[i]->y_min;
		y_max = tree_ptr[i]->y_max;

		if ((bot->position[0] > x_min) &&
			(bot->position[0] < x_max) &&
			(bot->position[2] > y_min) &&
			(bot->position[2] < y_max))
		{
			// in the area add to list
			InsertFront(tree_ptr[i]->list, (StaticBotPtr)bot);
			return;
		} // end of the if

	} // end of the for

} // end of the function

//
// SearchListBot
// - search the list for a static bot
//
StaticBotPtr SearchListBot(PtrList *list, DriverBotPtr bot)
{
	PtrNode *current_ptr;
	StaticBotPtr x;
	float x_min, x_max, y_min, y_max;

	// if (isempty(list))
	//	return NULL;
	if (list->head == NULL)
		return NULL;

	current_ptr = list->head;

	while (current_ptr != NULL)
	{
		// interesting
		x = (StaticBotPtr)current_ptr->ptr;

		x_min = x->position[0] - (x->size[0] / 2.0f);
		x_max = x->position[0] + (x->size[0] / 2.0f);
		y_min = x->position[2] - (x->size[0] / 2.0f);
		y_max = x->position[2] + (x->size[0] / 2.0f);

		if ((bot->x > x_min) && (bot->x < x_max) &&
			(bot->y > y_min) && (bot->y < y_max))
		{
			return x;
		} // end of the if

		current_ptr = current_ptr->next;

	} // end of while

	return NULL;

} // end of the function

//
// SearchOctree
// - check if we are in the region
//
StaticBotPtr SearchOctree(Octree **tree_ptr, DriverBotPtr bot)
{
	// find out which bin to search
	int i;
	float x_min, y_min, x_max, y_max;
	int max = tree_ptr[0]->max_elements;

	StaticBotPtr res = NULL;

	for (i = 0; i < max; i++)
	{
		x_min = tree_ptr[i]->x_min;
		x_max = tree_ptr[i]->x_max;
		y_min = tree_ptr[i]->y_min;
		y_max = tree_ptr[i]->y_max;

		if ((bot->x > x_min) &&
			(bot->x < x_max) &&
			(bot->y > y_min) &&
			(bot->y < y_max))
		{
			// in the area add to list
			res = SearchListBot(tree_ptr[i]->list, bot);

			// Search the list in this region
			return res;
		} // end of the if

	} // end of the for

	return NULL;

} // end of the function

//
// DeleteOctree
// - delete a node from the tree
//
void DeleteOctreeNode(Octree **tree_ptr, StaticBotPtr bot)
{
	// find out which bin to search
	int i;
	float x_min, y_min, x_max, y_max;
	int max = tree_ptr[0]->max_elements;

	for (i = 0; i < max; i++)
	{
		x_min = tree_ptr[i]->x_min;
		x_max = tree_ptr[i]->x_max;
		y_min = tree_ptr[i]->y_min;
		y_max = tree_ptr[i]->y_max;

		if ((bot->position[0] > x_min) &&
			(bot->position[0] < x_max) &&
			(bot->position[2] > y_min) &&
			(bot->position[2] < y_max))
		{

			// delete the node
			DeletePtrNode(tree_ptr[i]->list, bot);

		} // end of the if

	} // end of the for

} // end of the function

//
// Wrapper Functions
// - you should probably only have to deal
// with these functions unless you want more
// trees
void pheromoneBuild(void)
{
	pheromone_tree = GenerateOctree();
} // end of the function

//
// octreeDestroy
// - make sure this called
//
void pheromoneDestroy(void)
{
	DeleteOctree(pheromone_tree);
} // end of the function

//
// pheromoneInsert(StaticBotPtr bot)
//
void pheromoneInsert(StaticBotPtr bot)
{
	InsertOctree(pheromone_tree, bot);
} // end of the function

//
// pheromoneSearch
//
StaticBotPtr pheromoneSearch(DriverBotPtr bot)
{
	StaticBotPtr ptr;
	ptr = SearchOctree(pheromone_tree, bot);

	return ptr;
} // end of the function

//
// pheromoneDelete
// - delete a pheromone
//
void pheromoneDelete(StaticBotPtr bot)
{

	DeleteOctreeNode(pheromone_tree, bot);

} // end of the function
