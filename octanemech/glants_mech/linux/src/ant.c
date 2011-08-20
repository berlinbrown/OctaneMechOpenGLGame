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
// ant.cpp
//  - the ant object
//
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include "gldrawlib.h"
#include "objects.h"
#include "lights.h"

#undef CURRENT_OBJECT
#define CURRENT_OBJECT			ant

static void init_ant(int list_id);
static void compile_ant(void);
static void draw_ant(void);
static void render_ant(void);
static void draw_ant(void);


GLfloat xmat_ambient[] = { 0.0f, 0.9f, 0.0f, 1.0f };
GLfloat xmat_diffuse[] = { 0.9f, 0.8f, 0.8f, 1.0f };
GLfloat xmat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat xno_shininess[] = { 0.0f };
GLfloat xlow_shininess[] = { 5.0f };
GLfloat xhigh_shininess[] = { 100.0f};
GLfloat xmat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

//
// simple objects library
// - make sure to change the number of objects
// in objects.h
//
DriverObjects CURRENT_OBJECT =
{
	init_ant,			// init, must be called first
	compile_ant,		// compile
	draw_ant,			// draw 
	render_ant,			// render to scene
	0					// loaded by INIT
};


//=========================================================
//=========================================================
static void draw_ant(void)
{
	float v[3][3];
	float size = 1.0f;
	float height = 0.6f;
	
	float n[3];		// remember to free

#if ENABLE_LIGHTS
		// set the material for this object
		setmaterial(xmat_ambient, xmat_diffuse, 
				xmat_specular, xlow_shininess, xmat_emission);


#endif


	// change the size here
	// Note: starts from ground

	glBegin(GL_TRIANGLES);


	 // Get the top
	  v[0][0] = -size;
	  v[0][1] = height;
	  v[0][2] = size;

	  v[1][0] = size;
	  v[1][1] = height;
	  v[1][2] = size;

	  v[2][0] = 0.0;
	  v[2][1] = height;
	  v[2][2] = -size;

	  N_0;
	  glNormal3fv(n);


	  glVertex3fv(v[0]);
	  glVertex3fv(v[1]);
	  glVertex3fv(v[2]);	// triangle left bottom front

	 
	// left bottom front
	  v[0][0] = -size;
	  v[0][1] = 0.0f;
	  v[0][2] = size;

	  v[1][0] = size;
	  v[1][1] = 0.0f;
	  v[1][2] = size;

	  v[2][0] = size;
	  v[2][1] = height;
	  v[2][2] = size;

	  MED_BLUE;	
	  // Calc normal and draw
	  N_1;
	  glNormal3fv(n);
		
	  glVertex3fv(v[0]);
	  glVertex3fv(v[1]);
	  glVertex3fv(v[2]);	// triangle left bottom front

	  v[0][0] = -size;
	  v[0][1] = 0.0f;
	  v[0][2] = size;

	  v[1][0] = -size;
	  v[1][1] = height;
	  v[1][2] = size;

	  v[2][0] = size;
	  v[2][1] = height;
	  v[2][2] = size;

	  MED_BLUE;	
	  // Calc normal and draw
	  N_0;
	  glNormal3fv(n);


	  glVertex3fv(v[0]);
	  glVertex3fv(v[1]);
	  glVertex3fv(v[2]);	// triangle left bottom front

	 


	  // BOTTOM
	  v[0][0] = -size;
	  v[0][1] = 0.0f;
	  v[0][2] = size;

	  v[1][0] = size;
	  v[1][1] = 0.0f;
	  v[1][2] = size;

	  v[2][0] = 0.0f;
	  v[2][1] = 0.0f;
	  v[2][2] = -size;

	  MED_YELLOW;	
	  // Calc normal and draw]
	  N_0;
	  glNormal3fv(n);

	  glVertex3fv(v[0]);
	  glVertex3fv(v[1]);
	  glVertex3fv(v[2]);	// triangle left bottom front


	  // right side
	  v[0][0] = size;
	  v[0][1] = 0.0f;
	  v[0][2] = size;

	  v[1][0] = 0.0f;
	  v[1][1] = 0.0f;
	  v[1][2] = -size;

	  v[2][0] = 0.0f;
	  v[2][1] = height;
	  v[2][2] = -size;

	  MED_CYAN;	
	  // Calc normal and draw
	  N_1;
	  glNormal3fv(n);

	  glVertex3fv(v[0]);
	  glVertex3fv(v[1]);
	  glVertex3fv(v[2]);	// triangle left bottom front
	  
	  // still right side
	  v[0][0] = size;
	  v[0][1] = 0.0f;
	  v[0][2] = size;

	  v[1][0] = size;
	  v[1][1] = height;
	  v[1][2] = size;

	  v[2][0] = 0.0f;
	  v[2][1] = height;
	  v[2][2] = -size;

	  MED_CYAN;	
	  // Calc normal and draw
	  N_0;
	  glNormal3fv(n);
		
	  glVertex3fv(v[0]);
	  glVertex3fv(v[1]);
	  glVertex3fv(v[2]);	// triangle left bottom front

	  
	  // right side
	  v[0][0] = -size;
	  v[0][1] = 0.0f;
	  v[0][2] = size;

	  v[1][0] = 0.0f;
	  v[1][1] = 0.0f;
	  v[1][2] = -size;

	  v[2][0] = 0.0f;
	  v[2][1] = height;
	  v[2][2] = -size;

	  MED_PURPLE;
	  N_2;
	  glNormal3fv(n);

	  // Calc normal and draw
	  glVertex3fv(v[0]);
	  glVertex3fv(v[1]);
	  glVertex3fv(v[2]);	// triangle left bottom front

	  // still right side
	  v[0][0] = -size;
	  v[0][1] = 0.0f;
	  v[0][2] = size;

	  v[1][0] = -size;
	  v[1][1] = height;
	  v[1][2] = size;

	  v[2][0] = 0.0f;
	  v[2][1] = height;
	  v[2][2] = -size;

	  MED_PURPLE;	
	  N_1;
	  glNormal3fv(n);

	  // Calc normal and draw
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
static void init_ant(int list_id)
{

	CURRENT_OBJECT.visible = 1;

	// store the id through the function
	// there is probably a better way to do this
	CURRENT_OBJECT.call_id = list_id;	
	
} // end of the functino


//=========================================================
// Now the function to actually draw it
//=========================================================
static void render_ant(void)
{
		//glPushMatrix();

			glCallList(CURRENT_OBJECT.call_id);

		//glPopMatrix();

} // end of the function

//=========================================================
// compile
//=========================================================
static void compile_ant(void)
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

