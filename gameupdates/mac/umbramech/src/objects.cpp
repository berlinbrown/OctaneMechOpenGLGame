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
// Berlin Brown
// berlin _dot__ brown  __at_ g_mail _ dot_ com
//
// objects.cpp
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

#define SQRT_75 0.8660254038f

GLUquadricObj *quadric; // main Quadric Object

// may not start at 8, see InitObjects
static int mainobjectid = 8; // random number(8)

static void CompileObjects(void);

static float z_t = 0.0f;

static GLfloat grey_ambient[] = {0.0f, 0.3f, 0.4f, 1.0f};
static GLfloat grey_diffuse[] = {0.5f, 0.5f, 0.5f, 1.0f};
static GLfloat grey_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat ino_shininess[] = {0.0f};
static GLfloat ilow_shininess[] = {5.0f};
static GLfloat ihigh_shininess[] = {100.0f};
static GLfloat imat_emission[] = {0.15f, 0.1f, 0.1f, 0.0f};

//
// Array of driver objects
//
DriverObjects *driver_objects[MAX_OBJECTS] =
	{
		&colorcube, // cube object
		&grid,
		&ant,
		&walls,
		&wirebox,
		&norm_cube,
		&pheromone,
		&plane,
		&fireant,
		&pyramid,
		&stars,
};

//
// For the art of saving from adding another object
// file, I decided to place the plane object in this
// source code
// PLANE OBJECT
//---------------------------------------------------------

#undef CURRENT_OBJECT
#define CURRENT_OBJECT plane

static void init_plane(int list_id);
static void compile_plane(void);
static void draw_plane(void);
static void render_plane(void);
static void draw_plane(void);

static void draw_hex(void);

//
// simple objects library
// - make sure to change the number of objects
// in objects.h
//
DriverObjects CURRENT_OBJECT =
	{
		init_plane,	   // init, must be called first
		compile_plane, // compile
		draw_hex,	   // draw
		render_plane,  // render to scene
		0			   // loaded by INIT
};

//
// Actually draw to the scene
//
void RenderPlane(void)
{

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	// set the material for this object
	setmaterial(grey_ambient, grey_diffuse,
				grey_specular, ilow_shininess, imat_emission);

	BEGIN_BOT;

	// draw the object to screen
	driver_objects[PLANE_OBJECT]->render();

	END_BOT;

} // end of the functino

//
// Actually draw to the scene
//
void Bot_Triangle(float x, float y)
{
	float v[3][3] = {0};
	float n[3] = {0};
	float size = 1.2f;
	float height = 2.9f;

	z_t += 0.8f;
	if (z_t > 360.0f)
		z_t -= 360.0f;

	glDisable(GL_LIGHTING);

	BEGIN_BOT;

	// change the size here
	// Note: starts from ground

	glTranslatef(x, 3.2f, y);

	glRotatef(z_t, 0.0f, 1.0f, 0.0f);

	MED_YELLOW;
	glBegin(GL_TRIANGLES);

	// Get the top
	v[0][0] = -size;
	v[0][1] = height;
	v[0][2] = 0.0f;

	v[1][0] = size;
	v[1][1] = height;
	v[1][2] = 0.0f;

	v[2][0] = 0.0;
	v[2][1] = 0.0f;
	v[2][2] = 0.0f;

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	glEnd();

	END_BOT;

	glEnable(GL_LIGHTING);

} // end of the functino

//
// draw_hexplane
//
void draw_hexplane(float x_1, float x_2, float y_1, float size)
{
	float h_2;
	float v[3][3] = {0};
	float n[3] = {0};

	float tol = (size * 2.0f) * 1.1f;

	float x;

	h_2 = 1.8f * size;

	for (x = x_1; x < x_2; x += tol)
	{

		// left bottom front
		v[0][0] = -size + x;
		v[0][1] = 0.0f;
		v[0][2] = size + y_1;

		v[1][0] = -size + x;
		v[1][1] = 0.0f;
		v[1][2] = -size + y_1;

		v[2][0] = size + x;
		v[2][1] = 0.0f;
		v[2][2] = -size + y_1;

		CLR_0;
		// Calc normal and draw
		N_1;
		GET_NORMAL;

		glVertex3fv(v[0]);
		glVertex3fv(v[1]);
		glVertex3fv(v[2]); // triangle left bottom front

		// right tri
		v[0][0] = size + x;
		v[0][1] = 0.0f;
		v[0][2] = -size + y_1;

		v[1][0] = size + x;
		v[1][1] = 0.0f;
		v[1][2] = size + y_1;

		v[2][0] = -size + x;
		v[2][1] = 0.0f;
		v[2][2] = size + y_1;

		CLR_0;
		// Calc normal and draw
		N_1;
		GET_NORMAL;

		glVertex3fv(v[0]);
		glVertex3fv(v[1]);
		glVertex3fv(v[2]); // triangle left bottom front

		// top triangle
		v[0][0] = size + x;
		v[0][1] = 0.0f;
		v[0][2] = -size + y_1;

		v[1][0] = -size + x;
		v[1][1] = 0.0f;
		v[1][2] = -size + y_1;

		v[2][0] = 0.0f + x;
		v[2][1] = 0.0f;
		v[2][2] = -h_2 + y_1;

		CLR_0;
		// Calc normal and draw
		N_1;
		GET_NORMAL;

		glVertex3fv(v[0]);
		glVertex3fv(v[1]);
		glVertex3fv(v[2]); // triangle left bottom front

		// top triangle
		v[0][0] = size + x;
		v[0][1] = 0.0f;
		v[0][2] = size + y_1;

		v[1][0] = -size + x;
		v[1][1] = 0.0f;
		v[1][2] = size + y_1;

		v[2][0] = 0.0f + x;
		v[2][1] = 0.0f;
		v[2][2] = h_2 + y_1;

		CLR_0;
		// Calc normal and draw
		N_0;
		GET_NORMAL;

		glVertex3fv(v[0]);
		glVertex3fv(v[1]);
		glVertex3fv(v[2]); // triangle left bottom front

	} // end of the for

} // end of the function

//=========================================================
//
// Instead of a texured plane,
// draw a hexagon based grid
//
//=========================================================
static void draw_hex(void)
{
	float size = HEX_SIZE;
	float height = HEX_HEIGHT;

	float x_1 = WORLD_X_MIN;
	float x_2 = WORLD_X_MAX;

	float y_1 = WORLD_Y_MIN;
	float y_2 = WORLD_Y_MAX;

	float offset = 0.0f;
	bool s_flag = false;

	float i = 0.0f;

	glBegin(GL_TRIANGLES);

	for (i = y_1; i < y_2; i += height)
	{

		draw_hexplane(x_1 - offset, x_2, i, size);

		if (s_flag == true)
		{
			offset = size;
			s_flag = false;
		}
		else
		{
			offset = 0.0f;
			s_flag = true;
		} // end of the if

	} // end of the for

	glEnd();

} // end of the function

//=========================================================
//=========================================================
static void draw_plane(void)
{

	float size = 3000.0f;
	float bottom = -0.1f;
	float texsize = 20.0f;

	// Bind name texture
	glBindTexture(GL_TEXTURE_2D, GetFunkyTexture());

	glBegin(GL_QUADS);
	glTexCoord2f(-texsize, -texsize); // Texture Coord (Bottom Left)
	glVertex3f(-size, bottom, -size); // Vertex Coord (Bottom Left)

	glTexCoord2f(texsize, -texsize); // Texture Coord (Bottom Right)
	glVertex3f(size, bottom, -size); // Vertex Coord (Bottom Right)

	glTexCoord2f(texsize, texsize); // Texture Coord (Top Right)
	glVertex3f(size, bottom, size); // Vertex Coord (Top Right)

	glTexCoord2f(-texsize, texsize); // Texture Coord (Top Left)
	glVertex3f(-size, bottom, size); // Vertex Coord (Top Left)
	glEnd();

} // end of the function

//
// init
// - load anything special about the
// one important function
//
static void init_plane(int list_id)
{

	CURRENT_OBJECT.visible = 1;

	// store the id through the function
	// there is probably a better way to do this
	CURRENT_OBJECT.call_id = list_id;

} // end of the functino

//=========================================================
// Now the function to actually draw it
//=========================================================
static void render_plane(void)
{
	// glPushMatrix();

	glCallList(CURRENT_OBJECT.call_id);

	// glPopMatrix();

} // end of the function

//=========================================================
// compile
//=========================================================
static void compile_plane(void)
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

//---------------------------------------------------------

//
// RenderGrid
//
void RenderGrid(void)
{
	glDisable(GL_LIGHTING);
	driver_objects[GRID_OBJECT]->render();
	glEnable(GL_LIGHTING);

} // end of the function

//
// For Simple Bounding box tests
//	2D
void RenderBounds(float x, float y, float width)
{

	float h;
	h = width / 2.0f;

	glDisable(GL_LIGHTING);
	BEGIN_BOT;

	glTranslatef(x, 0, y);

	glBegin(GL_LINE_LOOP);

	// Front Face
	glVertex3f(-h, 0.0f, h);  // left bottom
	glVertex3f(h, 0.0f, h);	  // right bottom
	glVertex3f(h, 0.0f, -h);  // top right
	glVertex3f(-h, 0.0f, -h); // top left

	glEnd();

	END_BOT;

	glEnable(GL_LIGHTING);

} // end of the function

//
// RenderWalls
//
void RenderWalls(void)
{
	glDisable(GL_TEXTURE_2D);
	driver_objects[WORLD_OBJECT]->render();

} // end of the function

//
// RenderObjects
//
void RenderObjects(void)
{
	int index = 0;

	for (index = 0; index < MAX_OBJECTS; index++)
	{
		driver_objects[index]->render();
	} // end of the functino

} // end of the function

//
// Init Objects
// - call this to load the objects in InitGL
//
void InitObjects(void)
{
	int index;
	int j = 0;

	// setup the display lists
	mainobjectid = glGenLists(MAX_OBJECTS);

	for (index = mainobjectid; index < (mainobjectid + MAX_OBJECTS);
		 index++)
	{
		driver_objects[j]->init(index);

		j++;
		if (j >= MAX_OBJECTS)
			break;

	} // end of the for

	// go ahead and compile the objects
	CompileObjects();

} // end of the functino

//
// CompileObjects
//
static void CompileObjects(void)
{
	int index = 0;

	for (index = 0; index < MAX_OBJECTS; index++)
	{
		// get the functions from the object driver
		driver_objects[index]->compile();

	} // end of the for

} // end of the function

//=========================================================
// delete objects
// - place in shutdown gl
//---------------------------------------------------------
void DeleteObjects(void)
{
	// get rid of list
	glDeleteLists(mainobjectid, MAX_OBJECTS);

} // end of the function
