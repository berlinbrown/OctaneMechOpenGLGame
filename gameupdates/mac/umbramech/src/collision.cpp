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
// collision.cpp
//
// - test a collision against any object
//
//
//
// At present, everything is based on
// ray and line intersection
//
// For example, once a bullet is fired
// it generates a ray, and the ray is tested
// against the lines that make up the wall
//
// A moving object for example one of the
// ships has a
//
// There may several different functions
// to check for collisions
//
// There may also be several different
// ways to insert a segment to perform
// a collision against
//		- Berlin Brown
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

#include "collision.h"
#include "gldrawlib.h"
#include "camera.h"
#include "fireants.h"

static void Reset_DistStack(void);
static void Insert_DistStack(CollisionPtr ptr);
static CollisionPtr Find_DistStack(float x, float y);

static bool CheckLineHit(CollisionPtr ptr,
						 float x_1, float y_1, float x_2, float y_2);

static void Intersect_Lines(float x0, float y0, float x1, float y1,
							float x2, float y2, float x3, float y3,
							float *xi, float *yi);
//
// insert
static void InsertFront(CollisionList *list, CollisionObj *col_obj);

//
// We have a little problem
// since we doing a lot of line collision algorithms
// we need to find the closest collision
// by using a stack and then finding the
// shortest distance on the stack
//
// allocate an array of ptrs
static CollisionPtr dist_stack[MAX_DIST_STACK];
static int dist_stack_ctr = 0;

static float mOrig_x = 0, mOrig_y = 0;
static float mDest_x = 0, mDest_y = 0;
static float mCol_x = 0, mCol_y = 0;

#define CLOCKWISE -1
#define COUNTER_CLOCKWISE 1
#define LINE 0

#define BOT_LIST moving_list

// main list for collision objects
//
CollisionList *collision_list;

//
// another list for moving objects
//
CollisionList *moving_list;

// Note: insert and delete are the two most dangerous
// functions

//
// similar to plist.cpp, except uses CollisionObj
//

//
// Reset_DistStack(void)
static void Reset_DistStack(void)
{
	dist_stack_ctr = 0;
} // end of the functino

// Insert_DistStack
//
static void Insert_DistStack(CollisionPtr ptr)
{
	dist_stack[dist_stack_ctr] = ptr;

	// Note: we are not checking for the max, sorry
	dist_stack_ctr++;

} // end of the function

//
// Find_DistStack
// - find the shortest from the given
// point
//
static CollisionPtr Find_DistStack(float x, float y)
{
	float min = 10000;
	float res;
	int i = 0;
	float x2, y2;
	float tmp1, tmp2;
	int min_id = -1;

	CollisionPtr ptr;

	for (i = 0; i < dist_stack_ctr; i++)
	{
		ptr = dist_stack[i];

		x2 = ptr->collision_x;
		y2 = ptr->collision_y;

		tmp1 = x2 - x;
		tmp2 = y2 - y;

		// get the distance
		res = (float)sqrt((tmp1 * tmp1) + (tmp2 * tmp2));

		if (res < min)
		{
			// get new min
			min = res;
			min_id = i;

			dist_stack[min_id]->dist = res;
		} // end of the if

	} // end of the function

	return dist_stack[min_id];

} // end of the functino

//
// IsEmpty
//
int IsEmpty(CollisionList *list)
{

	if (list->front == NULL)
		return 1; /* first pointer null, list is empty */
	else
		return 0;

} /* end of the fcuntion */

//
// Create CollisionList
//
CollisionList *CreateCollisionList(void)
{

	CollisionList *result = (CollisionList *)malloc(
		sizeof(CollisionList));

	result->front = NULL;
	result->objects = 0;

	return result;

} // end of the function

//
// DestroyColList
//
void DestroyColList(CollisionList *list)
{

	CollisionObj *pos, *next;
	pos = list->front;

	while (pos != NULL)
	{

		next = pos->next;

		// need deletecollisionobject
		DeleteCollisionObj(pos);

		pos = next;

	} // end of the while

	RELEASE_OBJECT(list);

} // end of the function

//
// Insert Front
// - we will assume that you are created the object
//
void InsertColFront(CollisionList *list, CollisionObj *col_obj)
{
	CollisionObj *new_node = NULL;

	new_node = col_obj;

	if (IsEmpty(list))

		list->front = new_node;

	else
	{

		new_node->next = list->front;
		list->front = new_node;

	} // end if

	list->objects++;

} // end of the function

//
// void InsertCol
// - normal setup function
void SetupInsert(CollisionObj **ptr)
{
	(*ptr) = CreateCollisionObj();

	(*ptr)->id = collision_list->objects;

	InsertColFront(collision_list, *ptr);

} // end of the function

//
// Note: use this with the moving collision
// objects list
//
// Setup_Moving
//
void Setup_Moving(CollisionObj **ptr)
{

	(*ptr) = CreateCollisionObj();

	(*ptr)->id = BOT_LIST->objects;

	InsertColFront(BOT_LIST, *ptr);

} // end of the function

//
// Insert_MovingObj
//
void Insert_MovingObj(DriverBotPtr bot)
{

	// set up the struct
	CollisionObj *ptr = NULL;

	Setup_Moving(&ptr); // insert into moving obj list

	ptr->movement_type = MOVING_COL_TYPE; // moves

	ptr->bot_ptr = bot;

} // end of the function

//
// Next Library function
//
//  Check_MovingHit
// - check a moving object against a bullet
//
bool Check_MovingHit(CollisionPtr ptr, StaticBotPtr boid)
{
	float orig[2];
	float dest[2];
	float res_x, res_y;
	float x, y;
	float dv;
	int i = 0;
	float o[2];
	float d[2];

	// this value
	// can changed to get a more accurate
	// collision

	// This value is too high, but it
	// almost works --
	dv = 1.1f * BULLET_LEN;

	x = (float)sin(boid->virt_heading * PI_180) * dv;
	y = (float)cos(boid->virt_heading * PI_180) * dv;

	// build first line --
	orig[0] = boid->position[0];
	orig[1] = boid->position[2];

	dest[0] = boid->position[0] - x;
	dest[1] = boid->position[2] - y;

	if (ptr->bot_ptr->alive == DEAD_STATE)
		return false;

	// perform two different tests
	//
	for (i = 0; i < 2; i++)
	{

		switch (ptr->movement_type)
		{
		// we already know the type
		// but to be consistent
		case MOVING_COL_TYPE:

			// change some of the parms in the ptr
			// depending which iteration
			if (i == 0)
			{
				// build second line
				o[0] = ptr->bot_ptr->x + ptr->bot_ptr->x_min;
				o[1] = ptr->bot_ptr->y + ptr->bot_ptr->y_max;

				d[0] = ptr->bot_ptr->x + ptr->bot_ptr->x_max;
				d[1] = ptr->bot_ptr->y + ptr->bot_ptr->y_min;
			}
			else
			{

				// build line
				o[0] = ptr->bot_ptr->x + ptr->bot_ptr->x_min;
				o[1] = ptr->bot_ptr->y + ptr->bot_ptr->y_min;

				d[0] = ptr->bot_ptr->x + ptr->bot_ptr->x_max;
				d[1] = ptr->bot_ptr->y + ptr->bot_ptr->y_max;

			} // end of the if

			ptr->pos_0[0] = o[0];
			ptr->pos_0[1] = o[1];

			ptr->pos_1[0] = d[0];
			ptr->pos_1[1] = d[1];

			if (CheckLineHit(ptr, orig[0], orig[1],
							 dest[0], dest[1]))
			{

				// also get the point of intersection
				Intersect_Lines(
					ptr->pos_0[0], ptr->pos_0[1],
					ptr->pos_1[0], ptr->pos_1[1],
					orig[0], orig[1],
					dest[0], dest[1],
					&res_x, &res_y);

				ptr->collision_x = res_x;
				ptr->collision_y = res_y;

				return true;

			} // end of the if

			break;

		default:
			break;
		};

	} // end of the for

	return false;

} // end of the function

//
// CheckCollisionMoving
// - check for bullets versus moving ships
//
CollisionPtr CheckCollisionMoving(StaticBotPtr test_obj)
{

	CollisionObj *current_ptr;

	// we should never assume list is empty but, ahh..
	if (IsEmpty(BOT_LIST))
		return NULL;

	current_ptr = BOT_LIST->front;

	// sorry have to seek down the entire list
	while (current_ptr != NULL)
	{
		if (Check_MovingHit(current_ptr, test_obj))
		{

			return current_ptr;

		} // end of the if

		current_ptr = current_ptr->next;

	} // end of while

	return NULL;

} // end of the function

//
// Insert a Line segment
//
// we are working in 2d space pretty much
// A line segment can be a wall and this
// function converts that wall into a plane
//
// you need to provide the normal
//
void InsertColSegment(float x_1, float y_1, float x_2, float y_2)
{

	// set up the struct
	CollisionObj *ptr = NULL;

	SetupInsert(&(ptr)); // inserted into standard list

	ptr->pos_0[0] = x_1;
	ptr->pos_0[1] = y_1;

	ptr->pos_1[0] = x_2;
	ptr->pos_1[1] = y_2;

	// calculate distance from 0,0
	ptr->movement_type = PLANE_COL_TYPE; // does not move

} // end of the function

//
// Test for intersection of the line
// assuming they intersect
//
void Intersect_Lines(float x0, float y0, float x1, float y1,
					 float x2, float y2, float x3, float y3,
					 float *xi, float *yi)
{

	float a1, b1, c1,
		a2, b2, c2,
		det_inv,
		m1, m2;

	if ((x1 - x0) != 0)
		m1 = (y1 - y0) / (x1 - x0);
	else
		m1 = (float)90000;

	if ((x3 - x2) != 0)
		m2 = (y3 - y2) / (x3 - x2);
	else
		m2 = (float)1e+10; // close enough to infinity

	// compute constants

	a1 = m1;
	a2 = m2;

	b1 = -1;
	b2 = -1;

	c1 = (y0 - m1 * x0);
	c2 = (y2 - m2 * x2);

	// compute the inverse of the determinate

	det_inv = 1 / (a1 * b2 - a2 * b1);

	// use Kramers rule to compute xi and yi

	*xi = ((b1 * c2 - b2 * c1) * det_inv);
	*yi = ((a2 * c1 - a1 * c2) * det_inv);

} // end Intersect_Lines

//
// Check for Clock direction
//
int CheckClockDir(float pt1[2], float pt2[2], float pt3[2])
{
	float test = 0;
	float tmp1;
	float tmp2;
	float tmp3;
	float tmp4;

	tmp1 = (pt2[0] - pt1[0]);
	tmp2 = (pt3[1] - pt1[1]);
	tmp3 = (pt3[0] - pt1[0]);
	tmp4 = (pt2[1] - pt1[1]);

	test = ((tmp1 * tmp2) - (tmp3 * tmp4));

	if (test > 0)
		return COUNTER_CLOCKWISE;
	else if (test < 0)
		return CLOCKWISE;
	else
		return LINE;

	return -99;

} // end of the function

//
// CheckLineHit
//
bool CheckLineHit(CollisionPtr ptr,
				  float x_1, float y_1, float x_2, float y_2)
{
	int test1_a, test1_b, test2_a, test2_b;

	float p0_x, p0_y;
	float p1_x, p1_y;

	float a_1p1[2], a_1p2[2], a_2p1[2], a_2p2[2];

	p0_x = ptr->pos_0[0];
	p0_y = ptr->pos_0[1];

	p1_x = ptr->pos_1[0];
	p1_y = ptr->pos_1[1];

	a_1p1[0] = p0_x;
	a_1p1[1] = p0_y; // point 1

	a_1p2[0] = p1_x; // point 2
	a_1p2[1] = p1_y;

	// -- next line --
	a_2p1[0] = x_1; // point 1
	a_2p1[1] = y_1;

	a_2p2[0] = x_2; // point 2
	a_2p2[1] = y_2;

	test1_a = CheckClockDir(a_1p1, a_1p2, a_2p1);
	test1_b = CheckClockDir(a_1p1, a_1p2, a_2p2);

	if (test1_a != test1_b)
	{
		test2_a = CheckClockDir(a_2p1, a_2p2, a_1p1);
		test2_b = CheckClockDir(a_2p1, a_2p2, a_1p2);

		if (test2_a != test2_b)
		{

			return true;
		} // end of the if

	} // end of the if

	return false;

} // end of the function

//
// CheckHitBot
// - check for a collision with a wall and a bot
//
// we assume that the type is a moving bot
//
// with this function, we are testing
// two lines, the lines are a cross over the
// the bot
//
bool CheckHitBot(CollisionPtr ptr, DriverBotPtr bot)
{
	float orig[2];
	float dest[2];

	int i = 0;

	// perform two different calculations

	// save some calculations if dead
	if (bot->alive == DEAD_STATE)
		return false;

	for (i = 0; i < 2; i++)
	{

		if (i == 0)
		{
			// build first line --
			orig[0] = (bot->x + bot->x_min);
			orig[1] = (bot->y_min + bot->y);
			dest[0] = bot->x + bot->x_max;
			dest[1] = bot->y_max + bot->y;
		}
		else
		{

			// build second line
			orig[0] = bot->x + bot->x_min;
			orig[1] = bot->y + bot->y_max;

			dest[0] = bot->x + bot->x_max;
			dest[1] = bot->y + bot->y_min;

		} // end of the if

		switch (ptr->movement_type)
		{
		case PLANE_COL_TYPE:

			if (CheckLineHit(ptr, orig[0], orig[1],
							 dest[0], dest[1]))
			{
				return true;

			} // end of the if

			break;

		default:
			break;
		};

	} // end of the for

	return false;

} // end of the function

//
// CheckHitLines
//
bool CheckHitLines(CollisionPtr ptr, void *test_obj, int type)
{
	StaticBotPtr static_ptr = NULL;
	float orig[2];
	float dest[2];
	float dx, dy;
	float res_x, res_y;

	int owner = -1;

	if (type == RAY_COL_TYPE)
	{

		static_ptr = (StaticBotPtr)test_obj;

		orig[0] = static_ptr->virt_x;
		orig[1] = static_ptr->virt_y;

		// get the next point
		dx = (float)sin(static_ptr->virt_heading * PI_180) * MAX_BULLET_DEST;
		dy = (float)cos(static_ptr->virt_heading * PI_180) * MAX_BULLET_DEST;

		dest[0] = orig[0] - dx;
		dest[1] = orig[1] - dy;

		owner = static_ptr->owner;

	} // end of the if

	switch (ptr->movement_type)
	{
	case PLANE_COL_TYPE:

		if (CheckLineHit(ptr, orig[0], orig[1],
						 dest[0], dest[1]))
		{
			// find the point of intersection
			//
			Intersect_Lines(
				ptr->pos_0[0], ptr->pos_0[1],
				ptr->pos_1[0], ptr->pos_1[1],
				orig[0], orig[1],
				dest[0], dest[1],
				&res_x, &res_y);

			ptr->collision_x = res_x;
			ptr->collision_y = res_y;

			return true;

		} // end of the if

		break;

	default:
		break;
	};

	return false;

} // end of the function

//
// CheckCollisionList
// - the meat and potatoes of the function
// - check for a collision with our list
//
// returns: NULL means no collision
// - cpu intensive function
//
CollisionPtr CheckCollisionList(void *test_obj, int type)
{

	CollisionObj *current_ptr;

	StaticBotPtr static_ptr = NULL;
	float x = 0, y = 0;

	// we should never assume list is empty but, ahh..
	if (IsEmpty(collision_list))
		return NULL;

	current_ptr = collision_list->front;

	Reset_DistStack();
	// sorry have to seek down the entire list
	while (current_ptr != NULL)
	{
		if (CheckHitLines(current_ptr, test_obj, type))
		{
			// we have a hit, abandon ship
			Insert_DistStack(current_ptr);

		} // end of the if

		current_ptr = current_ptr->next;

	} // end of while

	// NULL means no collision
	if (dist_stack_ctr == 0)
		return NULL;

	// Now check for the shortest collision

	if (type == RAY_COL_TYPE)
	{

		static_ptr = (StaticBotPtr)test_obj;

		x = static_ptr->virt_x;
		y = static_ptr->virt_y;

	} // end of the if

	current_ptr = Find_DistStack(x, y);

	return current_ptr;

} // end of the function

//
//**
// Next Collision Test
// for bot with walls
//**
//
bool CheckCollisionBot(DriverBotPtr test_obj)
{

	CollisionObj *current_ptr;

	// we should never assume list is empty but, ahh..
	if (IsEmpty(collision_list))
		return NULL;

	current_ptr = collision_list->front;

	// sorry have to seek down the entire list
	while (current_ptr != NULL)
	{
		if (CheckHitBot(current_ptr, test_obj))
		{

			return true;

		} // end of the if

		current_ptr = current_ptr->next;

	} // end of while

	return false;

} // end of the function

//
// Remove Front
//
void RemoveFront(CollisionList *list)
{

	CollisionObj *temp_ptr = NULL;

	if (IsEmpty(list))
		return;
	else
	{
		temp_ptr = list->front;

		if (list->front->next == NULL)
			list->front = NULL; // reset
		else
			list->front = list->front->next;

		// free(temp_ptr);
		RELEASE_OBJECT(temp_ptr);

		list->objects--;

	} // end of the if-else

} // end of the function

//
// PrintList
//
void PrintCollisionList(CollisionList *list)
{

	CollisionObj *current_ptr;

	CollisionObj *x = NULL;

	if (IsEmpty(list))
		return;

	current_ptr = list->front;

	while (current_ptr != NULL)
	{
		// interesting
		x = current_ptr;

		printf("ID: %d\n", x->id);

		current_ptr = current_ptr->next;

	} // end of while

} // end of the function

//
// CreateCollisionObj
//
CollisionPtr CreateCollisionObj(void)
{
	CollisionPtr ptr = NULL;

	ptr = (CollisionPtr)malloc(sizeof(CollisionObj));

	ZeroMemory(ptr, sizeof(CollisionObj));

	ptr->movement_type = PLANE_COL_TYPE; // static or moving

	ptr->next = NULL;
	ptr->static_ptr = NULL;

	return ptr;

} // end of the function

//
// DeleteCollisionObj
//
void DeleteCollisionObj(CollisionObj *ptr)
{
	RELEASE_OBJECT(ptr);
} // end of the function

//
// WRAPPER FUNCTIONS
//
void Create_Col_List(void)
{
	collision_list = CreateCollisionList();

	// also create the moving list
	moving_list = CreateCollisionList();
} // end

//
// Delelet Col List
//
void Delete_Col_List(void)
{
	DestroyColList(collision_list);

	DestroyColList(moving_list);

} // end of the function

//
// Print_Col_List
//
void Print_Col_List(void)
{
	PrintCollisionList(collision_list);
} // end of the function
