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
// stars.cpp
//  - the ant object
//
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#include "objects.h"
#include "bot.h"

#undef CURRENT_OBJECT
#define CURRENT_OBJECT			stars

static void init_stars(int list_id);
static void compile_stars(void);
static void draw_stars(void);
static void render_stars(void);
static void draw_stars(void);

//
// simple objects library
// - make sure to change the number of objects
// in objects.h
//
DriverObjects CURRENT_OBJECT =
{
	init_stars,			// init, must be called first
	compile_stars,		// compile
	draw_stars,			// draw 
	render_stars,			// render to scene
	0					// loaded by INIT
};


//=========================================================
//=========================================================
static void draw_stars(void)
{

	int cnt	= MAX_STARS;
	double radius = STAR_RADIUS; 

	int step = cnt / 3;
	int i;

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);


	glColor3f(1.0, 1.0, 1.0);
	glPointSize(0.5);
	glBegin(GL_POINTS);
		for (i = 0; i < step; i++) {
			glColor3f(1.0, 1.0, 1.0);
			glVertex3f(
				radius * ((float)(rand() % MAX_RAND)/(MAX_RAND)) - radius / 2.0,
				radius * ((float)(rand() % MAX_RAND)/(MAX_RAND)) - radius / 2.0,
				radius * ((float)(rand() % MAX_RAND)/(MAX_RAND)) - radius / 2.0
			);
		}
	glEnd();
	glPointSize(1.0);
	glBegin(GL_POINTS);
		for (i = 0; i < step; i++) {
			glVertex3f(
				radius * ((float)(rand() % MAX_RAND)/(MAX_RAND)) - radius / 2.0,
				radius * ((float)(rand() % MAX_RAND)/(MAX_RAND)) - radius / 2.0,
				radius * ((float)(rand() % MAX_RAND)/(MAX_RAND)) - radius / 2.0
			);
		}
	glEnd();
	glPointSize(1.5);
	glBegin(GL_POINTS);
		for (i = 0; i < step; i++) {
			glVertex3f(
				radius * ((float)(rand() % MAX_RAND)/(MAX_RAND)) - radius / 2.0,
				radius * ((float)(rand() % MAX_RAND)/(MAX_RAND)) - radius / 2.0,
				radius * ((float)(rand() % MAX_RAND)/(MAX_RAND)) - radius / 2.0
			);
		}
	glEnd();

	glEnable(GL_LIGHTING);

} // end of the function


//
// init
// - load anything special about the
// one important function 
//
static void init_stars(int list_id)
{

	CURRENT_OBJECT.visible = 1;

	// store the id through the function
	// there is probably a better way to do this
	CURRENT_OBJECT.call_id = list_id;	
	
} // end of the functino


//=========================================================
// Now the function to actually draw it
//=========================================================
static void render_stars(void)
{
		glPushMatrix();

			glCallList(CURRENT_OBJECT.call_id);

		glPopMatrix();

} // end of the function

//=========================================================
// compile
//=========================================================
static void compile_stars(void)
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

