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
// pyramid.cpp
//
// - note: at present pyramid is not really a pyramid
// but a box
//

#include <stdio.h>
#include <stdlib.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

#undef CURRENT_OBJECT
#define CURRENT_OBJECT pyramid

static void init_pyramid(int list_id);
static void compile_pyramid(void);
static void draw_pyramid(void);
static void render_pyramid(void);
static void draw_pyramid(void);

GLfloat dmat_ambient[] = {0.0f, 0.0f, 0.9f, 1.0f};
GLfloat dmat_diffuse[] = {0.3f, 0.8f, 0.8f, 1.0f};
GLfloat dmat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat dno_shininess[] = {0.0f};
GLfloat dlow_shininess[] = {5.0f};
GLfloat dhigh_shininess[] = {100.0f};
GLfloat dmat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

//
// here is the level
// 14 walls, 5 cols
// 60.0f is a good height
//
#define LEVEL_MAX_WALLS 10
static float level_0[LEVEL_MAX_WALLS][5] =
	{
		{-200.0f, 260.0f, 100.0f, 30.0f, 70.0f}, // 1
		{-110.0f, 200.0f, 50.0f, 40.0f, 70.0f},	 // 2
		{-70.0f, 0.0f, 20.0f, 40.0f, 80.0f},	 // 3
		{70.0f, 0.0f, 20.0f, 40.0f, 56.0f},		 // 4
		{0.0f, 70.0f, 50.0f, 30.0f, 66.0f},		 // 5
		{0.0f, -70.0f, 50.0f, 30.0f, 60.0f},	 // 6
		{-220.0f, -240.0f, 70.0f, 40.0f, 70.0f}, // 7
		{180.0f, -100.0f, 50.0f, 30.0f, 55.0f},	 // 8
		{260.0f, 100.0f, 80.0f, 30.0f, 68.0f},	 // 9
		{220.0f, 80.0f, 40.0f, 30.0f, 55.0f}	 // 10

};

//
// simple objects library
// - make sure to change the number of objects
// in objects.h
//
DriverObjects CURRENT_OBJECT =
	{
		init_pyramid,	 // init, must be called first
		compile_pyramid, // compile
		draw_pyramid,	 // draw
		render_pyramid,	 // render to scene
		0				 // loaded by INIT
};

static CollisionList *wall_list;

//
// WALLSOBJECTS GO HERE
//=========================================================

//
static void SetupWall(CollisionObj **ptr)
{
	(*ptr) = CreateCollisionObj();

	(*ptr)->id = wall_list->objects;

	InsertColFront(wall_list, *ptr);

} // end of the function

//
// InsertWall
//
void InsertWall(float x, float y,
				float width, float height, float height_2)
{
	float x_min, x_max, y_min, y_max;

	// set up the struct
	CollisionObj *ptr = NULL;

	SetupWall(&(ptr)); // inserted into standard list

	ptr->movement_type = PLANE_COL_TYPE; // moves

	ptr->box_x = x;
	ptr->box_y = y;

	if (width <= 0)
		width = 1.0f;

	if (height <= 0)
		height = 1.0f;

	ptr->size[0] = width;
	ptr->size[1] = height_2;
	ptr->size[2] = height;

	// increase the width a little so it doesnt
	// look like the objects are crossing over
	width *= 1.1f;
	height *= 1.1f;

	x_min = x - (width / 2.0f);
	x_max = x + (width / 2.0f);

	y_min = y - (height / 2.0f);
	y_max = y + (height / 2.0f);

	//
	// In order to insert a wall of the box
	// we need the xmins and maxes and the normals
	// 4 differnt walls
	//

	// front wall
	InsertColSegment(x_min, y_max, x_max, y_max);

	// right wall
	InsertColSegment(x_max, y_min, x_max, y_max);

	// back wall (top)
	InsertColSegment(x_min, y_min, x_max, y_min);

	// left wall
	InsertColSegment(x_min, y_min, x_min, y_max);

} // end of the function

//
// Create Walls
//
void CreateWalls(void)
{
	int i = 0;
	// InsertWall(4.0f, -10.0f, 20.0f, 0.5f, 55.0f);

	for (i = 0; i < LEVEL_MAX_WALLS; i++)
	{

		InsertWall(level_0[i][0], level_0[i][1],
				   level_0[i][2], level_0[i][3],
				   level_0[i][4]);
	} // end of the for

} // end of teh fucntion

//
// PrintList
//
void Draw_Walls(CollisionList *list)
{

	CollisionObj *current_ptr;

	if (list->front == NULL)
		return;

	current_ptr = list->front;

	while (current_ptr != NULL)
	{

		// draw the wall
		glPushMatrix();

		glTranslatef(current_ptr->box_x, 0.0f, current_ptr->box_y);

		glScalef(current_ptr->size[0], current_ptr->size[1],
				 current_ptr->size[2]);

		driver_objects[PYRAMID_OBJECT]->render();

		glPopMatrix();

		current_ptr = current_ptr->next;

	} // end of while

} // end of the function

//
// Create wall list
//
void Create_Wall_List(void)
{
	wall_list = CreateCollisionList();
} // end of the function

//
// Delelet Col List
//
void Delete_Wall_List(void)
{
	DestroyColList(wall_list);
} // end of the function

//
// Print_Col_List
//
void Print_Wall_List(void)
{
	PrintCollisionList(wall_list);

} // end of the function

//
// Draw_Wall_List
//
void Draw_Wall_List(void)
{
	Draw_Walls(wall_list);
} // end of the functino

//
// END WALLOBJECTS
//=========================================================
static void draw_pyramid(void)
{
	float v[3][3] = {0};
	float n[3] = {0};

	float size = 0.5f;

	// set the material for this object
	setmaterial(dmat_ambient, dmat_diffuse,
				dmat_specular, dlow_shininess, dmat_emission);

	// Note: normals are messed up for now
	// select between n0-n3

	// change the size here
	// Note: starts from ground

	glBegin(GL_TRIANGLES);

	// left bottom front
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = size;

	v[2][0] = size;
	v[2][1] = size;
	v[2][2] = size;

	CLR_0;
	// Calc normal and draw
	N_2;
	GET_NORMAL;

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// Finish the front
	v[0][0] = size;
	v[0][1] = size;
	v[0][2] = size;

	v[1][0] = -size;
	v[1][1] = size;
	v[1][2] = size;

	v[2][0] = -size;
	v[2][1] = 0.0f;
	v[2][2] = size;

	CLR_1;
	// Calc normal and draw
	N_2;
	GET_NORMAL;

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// Draw the back triangle
	//-----------------------------
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = -size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = -size;

	v[2][0] = size;
	v[2][1] = size;
	v[2][2] = -size;

	CLR_2;
	// Calc normal and draw
	N_2;
	GET_NORMAL;
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom bac

	// Finish the back
	v[0][0] = size;
	v[0][1] = size;
	v[0][2] = -size;

	v[1][0] = -size;
	v[1][1] = size;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = 0.0f;
	v[2][2] = -size;

	MED_PURPLE;
	// Calc normal and draw
	N_2;
	GET_NORMAL;
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?
	//  Draw the right side
	//  Triangle
	v[0][0] = size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = -size;

	v[2][0] = size;
	v[2][1] = size;
	v[2][2] = size;

	MED_BLUE;
	// Calc normal and draw
	N_2;
	GET_NORMAL;
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom bac

	// FINISh the right side of the box
	v[0][0] = size;
	v[0][1] = 0.0f;
	v[0][2] = -size;

	v[1][0] = size;
	v[1][1] = size;
	v[1][2] = -size;

	v[2][0] = size;
	v[2][1] = size;
	v[2][2] = size;

	MED_GREEN;
	// Calc normal and draw
	N_2;
	GET_NORMAL;
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom bac

	// FINISh the left side of the box
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = -size;

	v[1][0] = -size;
	v[1][1] = size;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = size;
	v[2][2] = size;

	MED_PURPLE;
	// Calc normal and draw
	N_2;
	GET_NORMAL;
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom bac

	// Draw the left side
	// Triangle
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = -size;
	v[1][1] = 0.0f;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = size;
	v[2][2] = size;

	MED_RED;
	// Calc normal and draw
	N_2;
	GET_NORMAL;
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left side

	// Draw the top and bottom
	v[0][0] = size;
	v[0][1] = size;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = size;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = size;
	v[2][2] = -size;

	CLR_0;
	// Calc normal and draw
	N_2;
	GET_NORMAL;
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left side

	// Draw one of the bottom triangles
	v[0][0] = size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = 0.0f;
	v[2][2] = -size;

	CLR_3;
	// Calc normal and draw
	N_2;
	GET_NORMAL;
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left side

	// Lets finish the bottom with the second triangle
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = size;

	v[2][0] = -size;
	v[2][1] = 0.0f;
	v[2][2] = -size;

	// Calc normal and draw
	N_2;
	GET_NORMAL;
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left side

	// Go back and finish the top
	v[0][0] = -size;
	v[0][1] = size;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = size;
	v[1][2] = size;

	v[2][0] = -size;
	v[2][1] = size;
	v[2][2] = -size;

	// Calc normal and draw
	N_2;
	GET_NORMAL;
	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left side

	glEnd();

} // end of the function

//
// init
// - load anything special about the
// one important function
//
static void init_pyramid(int list_id)
{

	CURRENT_OBJECT.visible = 1;

	// store the id through the function
	// there is probably a better way to do this
	CURRENT_OBJECT.call_id = list_id;

} // end of the functino

//=========================================================
// Now the function to actually draw it
//=========================================================
static void render_pyramid(void)
{
	// glPushMatrix();

	glCallList(CURRENT_OBJECT.call_id);

	// glPopMatrix();

} // end of the function

//=========================================================
// compile
//=========================================================
static void compile_pyramid(void)
{
	int id;
	// setup a spot for display list for background
	// object = getcurrentobject();
	id = CURRENT_OBJECT.call_id;

	// apply list
	glNewList(id, GL_COMPILE);

	// call drawing function
	// but this may method make it a little better
	CURRENT_OBJECT.draw();

	glEndList();

} // end of the function
