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

#include <stdio.h>
#include <stdlib.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

#include "bot.h"
#include "gldrawlib.h"
#include "objects.h"
#include "world.h"
#include "lights.h"

#undef CURRENT_OBJECT
#define CURRENT_OBJECT walls

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

//
// something simple for once
// because we only need one world?
DriverWorldPtr world_ptr;

//
// simple objects library
// - make sure to change the number of objects
// in objects.h
//
DriverObjects CURRENT_OBJECT =
	{
		init_walls,	   // init, must be called first
		compile_walls, // compile
		draw_walls,	   // draw
		render_walls,  // render to scene
		0			   // loaded by INIT
};

//
// InitWorld
//
DriverWorldPtr InitWorld(void)
{
	DriverWorldPtr world;

	world = (DriverWorldPtr)malloc(sizeof(DriverWorld));

	ZeroMemory(world, sizeof(DriverWorld));

	world->x_max = GRID_SIZE + 1.0f;
	world->x_min = -GRID_SIZE - 1.0f;
	world->y_min = -GRID_SIZE - 1.0f;
	world->y_max = GRID_SIZE + 1.0f;

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
	// free((DriverWorldPtr)world);
	RELEASE_OBJECT(world);

} // end of the function

//
// wrapper functions(kind of hidden isnt it)
void CreateWorld(void) { world_ptr = InitWorld(); }

void ShutdownWorld(void)
{

} // end of if

//=========================================================
//=========================================================
static void draw_walls(void)
{

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

} // end of the function

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
	// object = getcurrentobject();
	id = CURRENT_OBJECT.call_id;

	// apply list
	glNewList(id, GL_COMPILE);

	// call drawing function
	// but this may method make it a little better
	CURRENT_OBJECT.draw();

	glEndList();

} // end of the function
