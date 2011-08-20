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
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#include "objects.h"
#include "bot.h"
#include "tree.h"
#include "gldrawlib.h"
#include "plist.h"
#include "octree.h"

Octree **pheromone_tree;

StaticBot	*pheromones;
StaticBot	*p_bot;

void RenderTmpBot(float x, float y, float size);

#define PRINT_FILE			0

Octree **GenerateOctree(void);

int search_count = 0;

int start_delete = 0;
bool delete_flag = true;

//
// CreateOctree
//
Octree *CreateOctree(void)
{
	Octree *ptr;
	ptr = (Octree *)malloc(sizeof(Octree));

	return ptr;

} // end of the function 

//
// DeleteOctree
// 
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

	float i,j;
	float width = 7.5f;
	float height = width;	// square grid
	float x_min,x_max, y_min, y_max;
	int hash_key = 0;
	int count = 0;
	int z = 0;
	int index = 0;
	Octree **tree_ptr;

	for (i = -30.0f; i < 30.0f; i+= height)
		for (j = -30.0f; j < 30.0f; j+= width)
		{
			count++;
		} // end of the for

	// First we have to create an array of pointers
	tree_ptr = (Octree **)malloc(count * sizeof(Octree *));
	if (tree_ptr == NULL)
		printf("HELLO\n");

	// Create each node
	for (index = 0; index < count; index++)
		tree_ptr[index] = CreateOctree();
	

	z = 0;		// reset counter 

	// populate the tree with data
	for (i = -30.0f; i < 30.0f; i+= height)
	{
		for (j = -30.0f; j < 30.0f; j+= width)
		{
			x_min = j;
			y_min = i;
			x_max = j + width;
			y_max = i + width;

			tree_ptr[z]->list = CreatePtrList();
			tree_ptr[z]->x_max= x_max;
			tree_ptr[z]->x_min= x_min;
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
	float x_min,y_min, x_max, y_max;
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
StaticBotPtr SearchListBot(PtrList *list, StaticBotPtr bot)
{
 PtrNode *current_ptr;
 StaticBotPtr x;
 float x_min, x_max, y_min, y_max;

 if (isempty(list))
	return NULL;
 
 current_ptr = list->head;
 
 while(current_ptr != NULL)
 {
	// interesting 
	x = (StaticBotPtr)current_ptr->ptr;

	x_min = x->position[0] - (x->size[0]/2.0f);
	x_max = x->position[0] + (x->size[0]/2.0f);
	y_min = x->position[2] - (x->size[0]/2.0f);
	y_max = x->position[2] + (x->size[0]/2.0f);

	if ((bot->position[0] > x_min) && (bot->position[0] < x_max) &&
		(bot->position[2] > y_min) && (bot->position[2] < y_max))
	{
			return x;
	} // end of the if 
	
	 search_count++;

 	current_ptr = current_ptr->next;

 } // end of while

 return NULL;

} // end of the function 

//
// SearchOctree
// - check if we are in the region
//
StaticBotPtr SearchOctree(Octree **tree_ptr, StaticBotPtr bot)
{
	// find out which bin to search
	int i;
	float x_min,y_min, x_max, y_max;
	int max = tree_ptr[0]->max_elements;

	StaticBotPtr res = NULL;

	for (i = 0; i < max; i++)
	{
		x_min = tree_ptr[i]->x_min;
		x_max = tree_ptr[i]->x_max;
		y_min = tree_ptr[i]->y_min;
		y_max = tree_ptr[i]->y_max;

		search_count++;

		if ((bot->position[0] > x_min) &&
			(bot->position[0] < x_max) &&
			(bot->position[2] > y_min) &&
			(bot->position[2] < y_max))
		{
			// in the area add to list
			res = SearchListBot(tree_ptr[i]->list, bot);
			if (res != NULL)
			{
				printf("list found: (%d)\n", i);
			} // end of the if 

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
void DeleteOctree(Octree **tree_ptr, StaticBotPtr bot)
{
	// find out which bin to search
	int i;
	float x_min,y_min, x_max, y_max;
	int max = tree_ptr[0]->max_elements;

	StaticBotPtr res = NULL;

	for (i = 0; i < max; i++)
	{
		x_min = tree_ptr[i]->x_min;
		x_max = tree_ptr[i]->x_max;
		y_min = tree_ptr[i]->y_min;
		y_max = tree_ptr[i]->y_max;

		search_count++;

		if ((bot->position[0] > x_min) &&
			(bot->position[0] < x_max) &&
			(bot->position[2] > y_min) &&
			(bot->position[2] < y_max))
		{
			// in the area add to list
			res = SearchListBot(tree_ptr[i]->list, bot);
			
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
}// end of the function 

//
// pheromoneSearch
//
StaticBotPtr pheromoneSearch(StaticBotPtr bot)
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

	DeleteOctree(pheromone_tree, bot);

} // end of the function 


//*********************************************************
// TESTING
//*********************************************************
//
// CreateOctreeTest
// - make sure octree is build
//
void CreateOctreeTest(void)
{
	int i =0;

	// create the pheromone tree
	pheromoneBuild();

	pheromones = (StaticBot *)malloc(200 * sizeof(StaticBot));

	for (i = 0; i < 200; i++)
	{
		pheromones[i].position[0] = x1[i].x;
		pheromones[i].position[2] = x1[i].y;
		pheromones[i].size[0] = 
			pheromones[i].size[1] = 
				pheromones[i].size[2] = 0.8f;

			pheromoneInsert(&pheromones[i]);

			pheromones[i].state = ALIVE_STATE;

	} // end of the for 


	p_bot = (StaticBot *)malloc(sizeof(StaticBot));

	p_bot->position[0] = 0;
	p_bot->position[1] = 0;
	p_bot->position[2] = 0;
	p_bot->size[0] = 
			p_bot->size[1] = 
				p_bot->size[2] = 0.8f;


} // end of the function 

void EndOctreeTest(void)
{
	free(pheromones);

	free(p_bot);

	pheromoneDestroy();
} // end of funct

//
// Generate Field
//
void GenerateField(void)
{
	int i = 0;
	int index = 0;

	float width = 0.8f;


	#if PRINT_FILE
	float x,y;

	FILE *f = fopen("t.txt", "w");
	fprintf(f, "Point x1[200]= {");
	#endif

	for (i = 0; i < 200; i++)
	{
	
		#if PRINT_FILE

		x =  ((float)(rand()%12000)/240.0f) - 25.0f;
		y =  ((float)(rand()%12000)/240.0f) - 25.0f;

		fprintf(f, "	%0.3ff, %0.3ff,\n", x, y);
		#endif

		if (pheromones[i].state == ALIVE_STATE)
			RenderBox(x1[i].x, x1[i].y, 0.1f, width);

	} // end of the function

#if PRINT_FILE
	fprintf(f, "};\n");
	fclose(f);
#endif

	// Now lets draw a boid and a target pheremone location
	// now build the bot
	glColor3f(1.0f, 0.0f, 0.0f);
	RenderTmpBot(p_bot->position[0], p_bot->position[2], 0.3f);

	search_count = 0;

	// Check if the bot is in the area
	pheromoneSearch(p_bot);
	printf("SEARCHING\n");

	printf("SEARCHES: %d - %d\n", search_count, 
		pheromone_tree[0]->max_elements);

	if (start_delete)
	{
		if (delete_flag)
		{
			// kill some pheromones
			for (index = 0; index < 100; index++)
			{
				pheromones[index].state = DEAD_STATE;

				pheromoneDelete(&pheromones[index]);
			} // end of the for 


			delete_flag = false;
		} // end of the if 

	} // end of the if 

} // end of the function

//
// RenderTmpBot
//
void RenderTmpBot(float x, float y, float size)
{

	BEGIN_BOT;
 
		// Translate then rotate
		glTranslatef(x, 0.0f, y);

		// Scale accordingly

		glScalef(size, 1.0f, size);

		driver_objects[NORM_CUBE_OBJECT]->render();

	END_BOT;


} // end of the function 

