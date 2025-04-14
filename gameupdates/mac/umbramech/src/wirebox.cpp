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
// wirebox.cpp
// - do we need a file for each object....yes!
//

#include <stdio.h>
#include <stdlib.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

#undef CURRENT_OBJECT
#define CURRENT_OBJECT wirebox

static void init_wirebox(int list_id);
static void compile_wirebox(void);
static void draw_wirebox(void);
static void render_wirebox(void);
static void draw_wirebox(void);

//
// simple objects library
// - make sure to change the number of objects
// in objects.h
//
DriverObjects CURRENT_OBJECT =
	{
		init_wirebox,	 // init, must be called first
		compile_wirebox, // compile
		draw_wirebox,	 // draw
		render_wirebox,	 // render to scene
		0				 // loaded by INIT
};

//=========================================================
static void draw_wirebox(void)
{
	float size = 0.4f;

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	// Front Face
	glVertex3f(-size, 0.0f, size); // left bottom
	glVertex3f(size, 0.0f, size);  // right bottom
	glVertex3f(size, size, size);  // top right
	glVertex3f(-size, size, size); // top left
	// Back Face

	glVertex3f(-size, 0.0f, -size);
	glVertex3f(-size, size, -size);
	glVertex3f(size, size, -size);
	glVertex3f(size, 0.0f, -size);

	// Top Face
	glVertex3f(-size, size, -size);
	glVertex3f(-size, size, size);
	glVertex3f(size, size, size);
	glVertex3f(size, size, -size);

	// Bottom Face
	glVertex3f(-size, 0.0f, -size);
	glVertex3f(size, 0.0f, -size);
	glVertex3f(size, 0.0f, size);
	glVertex3f(-size, 0.0f, size);

	// Right face
	glVertex3f(size, 0.0f, -size);
	glVertex3f(size, size, -size);
	glVertex3f(size, size, size);
	glVertex3f(size, 0.0f, size);

	// Left Face
	glVertex3f(-size, 0.0f, -size);
	glVertex3f(-size, 0.0f, size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, size, -size);
	glEnd();

} // end of the function

//
// init
// - load anything special about the
// one important function
//
static void init_wirebox(int list_id)
{

	CURRENT_OBJECT.visible = 1;

	// store the id through the function
	// there is probably a better way to do this
	CURRENT_OBJECT.call_id = list_id;

} // end of the functino

//=========================================================
// Now the function to actually draw it
//=========================================================
static void render_wirebox(void)
{
	// glPushMatrix();

	glCallList(CURRENT_OBJECT.call_id);

	// glPopMatrix();

} // end of the function

//=========================================================
// compile
//=========================================================
static void compile_wirebox(void)
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
