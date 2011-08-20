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
// world.cpp
// the environment
// in which the bots live
//
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#include "bot.h"
#include "gldrawlib.h"
#include "objects.h"

#include "world.h"
#include "lights.h"

//
// ant.cpp
//  - the ant object
//
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#include "gldrawlib.h"
#include "objects.h"


#undef CURRENT_OBJECT
#define CURRENT_OBJECT			walls

static void init_ant(int list_id);
static void compile_ant(void);
static void draw_ant(void);
static void render_ant(void);
static void draw_ant(void);

// Walls needs a reference from
// 
static void init_walls(int list_id);
static void compile_walls(void);
static void draw_walls(void);
static void render_walls(void);

// something simple for once
// because we only need one world?
DriverWorldPtr	world_ptr;

//
// simple objects library
// - make sure to change the number of objects
// in objects.h
//
DriverObjects CURRENT_OBJECT =
{
	init_walls,			// init, must be called first
	compile_walls,		// compile
	draw_walls,			// draw 
	render_walls,			// render to scene
	0					// loaded by INIT
};

/**
 * InitWorld
 */
DriverWorldPtr InitWorld(void)
{
	DriverWorldPtr	world;

	world = (DriverWorldPtr)malloc(sizeof(DriverWorld));
	
	ZeroMemory(world, sizeof(DriverWorld));

	world->x_max = GRID_SIZE	+ 1.0f;
	world->x_min = -GRID_SIZE	- 1.0f;
	world->y_min = -GRID_SIZE	- 1.0f;
	world->y_max = GRID_SIZE	+ 1.0f;

	world->height = WORLD_HEIGHT;

	// what do you think, yellow?
	world->color[0] = 0.5f;
	world->color[1] = 0.5f;
	world->color[2] = 0.5f;

	return world;

} // end of the function

//
// DestroyWorld
//
void DestroyWorld(DriverWorldPtr world)
{
	//free((DriverWorldPtr)world);
	RELEASE_OBJECT(world);

} // end of the function 

// 
// wrapper functions(kind of hidden isnt it)
void CreateWorld(void){world_ptr=InitWorld(); }

void ShutdownWorld(void){


// This is messing me up, maybe need to bring
// back if there are memory leaks
#if 0

	free((DriverWorldPtr)world_ptr);
#endif

} // end of if 




//=========================================================
//=========================================================
static void draw_walls(void)
{
	float v[3][3] = { 0 };
	
	float x_min = world_ptr->x_min;
	float y_min = world_ptr->y_min;
	float x_max = world_ptr->x_max;
	float y_max = world_ptr->y_max;
	float height = world_ptr->height;

	float n[3];


	// change the size here
	// Note: starts from ground

	glBegin(GL_TRIANGLES);

	  // right front of wall
	  v[0][0] = x_min;
	  v[0][1] = height;
	  v[0][2] = y_max;

	  v[1][0] = x_min;
	  v[1][1] = 0.0f;
	  v[1][2] = y_max;

	  v[2][0] = x_max;
	  v[2][1] = height;
	  v[2][2] = y_max;

	  WORLD_COLOR

	  N_1;
	  glNormal3fv(n);

	  // Calc normal and draw
	  glVertex3fv(v[0]);
	  glVertex3fv(v[1]);
	  glVertex3fv(v[2]);	// triangle left bottom front

	  v[0][0] = x_min;
	  v[0][1] = 0.0f;
	  v[0][2] = y_max;

	  v[1][0] = x_max;
	  v[1][1] = 0.0f;
	  v[1][2] = y_max;

	  v[2][0] = x_max;
	  v[2][1] = height;
	  v[2][2] = y_max;

	  WORLD_COLOR

	  N_1;
	  glNormal3fv(n);


	  // Calc normal and draw
	  glVertex3fv(v[0]);
	  glVertex3fv(v[1]);
	  glVertex3fv(v[2]);	// triangle left bottom front

	  // we are only going to draw part of the wall
	  // left side of wall
	  v[0][0] = x_min;
	  v[0][1] = 0.0f;
	  v[0][2] = y_min;

	  v[1][0] = x_min;
	  v[1][1] = height;
	  v[1][2] = y_min;

	  v[2][0] = x_min;
	  v[2][1] = height;
	  v[2][2] = y_max;

	  WORLD_COLOR_2
	  // Calc normal and draw

	  N_0;
	  glNormal3fv(n);

	  glVertex3fv(v[0]);
	  glVertex3fv(v[1]);
	  glVertex3fv(v[2]);	// triangle left bottom front

	   // finish left side of 
	  v[0][0] = x_min;
	  v[0][1] = 0.0f;
	  v[0][2] = y_max;

	  v[1][0] = x_min;
	  v[1][1] = height;
	  v[1][2] = y_max;

	  v[2][0] = x_min;
	  v[2][1] = 0.0f;
	  v[2][2] = y_min;


		WORLD_COLOR_2
	  // Calc normal and draw

	  N_1;
	  glNormal3fv(n);

	  glVertex3fv(v[0]);
	  glVertex3fv(v[1]);
	  glVertex3fv(v[2]);	// triangle left bottom front

	
	glEnd();

} // end of the function


//
// init
// - load anything special about the
// one important function 
//
static void init_walls(int list_id)
{

	CURRENT_OBJECT.visible = 1;

	// store the id through the function
	// there is probably a better way to do this
	CURRENT_OBJECT.call_id = list_id;	
	
} // end of the functino


//=========================================================
// Now the function to actually draw it
//=========================================================
static void render_walls(void)
{
		glPushMatrix();

			glCallList(CURRENT_OBJECT.call_id);

		glPopMatrix();

} // end of the function

//=========================================================
// compile
//=========================================================
static void compile_walls(void)
{
	int id;
	// setup a spot for display list for background
	//object = getcurrentobject();
	id = CURRENT_OBJECT.call_id;

	// apply list
	glNewList(id, GL_COMPILE);

		// call drawing function
		// but this may method make it a little better
		CURRENT_OBJECT.draw();

	glEndList();

} // end of the function

