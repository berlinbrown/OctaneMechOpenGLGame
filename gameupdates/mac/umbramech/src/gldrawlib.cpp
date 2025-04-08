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
// bigbinc@hotmail.com
// glDrawLib.cpp
//
// a couple of drawing routines
// opengl library
//

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context


#include "gldrawlib.h"

// Reduces a normal vector specified as a set of three coordinates,
// to a unit normal vector of length one.
void ReduceToUnit(float vector[3])
{
	float length;

	// Calculate the length of the vector
	length = (float)sqrt((vector[0] * vector[0]) +
						 (vector[1] * vector[1]) +
						 (vector[2] * vector[2]));

	// Keep the program from blowing up by providing an exceptable
	// value for vectors that may calculated too close to zero.
	if (length == 0.0f)
		length = 1.0f;

	// Dividing each element by the length will result in a
	// unit normal vector.
	vector[0] /= length;
	vector[1] /= length;
	vector[2] /= length;
} // end of the function

// Points p1, p2, & p3 specified in counter clock-wise order
void Calc_Normal(float v[3][3], float out[3])
{
	float v1[3], v2[3];
	static const int x = 0;
	static const int y = 1;
	static const int z = 2;

	// Calculate two vectors from the three points
	v1[x] = v[0][x] - v[1][x];
	v1[y] = v[0][y] - v[1][y];
	v1[z] = v[0][z] - v[1][z];

	v2[x] = v[1][x] - v[2][x];
	v2[y] = v[1][y] - v[2][y];
	v2[z] = v[1][z] - v[2][z];

	// Take the cross product of the two vectors to get
	// the normal vector which will be stored in out
	out[x] = v1[y] * v2[z] - v1[z] * v2[y];
	out[y] = v1[z] * v2[x] - v1[x] * v2[z];
	out[z] = v1[x] * v2[y] - v1[y] * v2[x];

	// Normalize the vector (shorten length to one)
	ReduceToUnit(out);

} // end of the function

//
// Normalize
//
void Normalize(float p[3])
{
	float length;

	length = (float)sqrt((p[0] * p[0]) + (p[1] * p[1]) + (p[2] * p[2]));

	if (length != 0)
	{
		p[0] /= length;
		p[1] /= length;
		p[2] /= length;
	}
	else
	{
		p[0] = 0;
		p[1] = 0;
		p[2] = 0;
	}

} // end of the function

//
// calcnormal
//
void CalcNormal(float p[3], float p1[3], float p2[3], float n[3])
{
	float pa[3];
	float pb[3];

	pa[0] = p1[0] - p[0];
	pa[1] = p1[1] - p[1];
	pa[2] = p1[2] - p[2];

	pb[0] = p2[0] - p[0];
	pb[1] = p2[1] - p[1];
	pb[2] = p2[2] - p[2];

	n[0] = pa[1] * pb[2] - pa[2] * pb[1];
	n[1] = pa[2] * pb[0] - pa[0] * pb[2];
	n[2] = pa[0] * pb[1] - pa[1] * pb[0];

	Normalize(n);

} // end of the function

//
// get clock
//
unsigned long getclock(void)
{
	// this function returns the current tick count

	// return tim

	// return(GetTickCount());

} // end Get_Clock

//
// Draw Objects
//
void DrawObjects(void)
{
	glLoadIdentity();				  // Reset The Current Modelview Matrix
	glTranslatef(-1.5f, 0.0f, -6.0f); // Move Left 1.5 Units And Into The Screen 6.0
	// glRotatef(rtri,0.0f,1.0f,0.0f);						// Rotate The Triangle On The Y axis ( NEW )

	glBegin(GL_TRIANGLES);			 // Start Drawing A Triangle
	glColor3f(1.0f, 0.0f, 0.0f);	 // Red
	glVertex3f(0.0f, 1.0f, 0.0f);	 // Top Of Triangle (Front)
	glColor3f(0.0f, 1.0f, 0.0f);	 // Green
	glVertex3f(-1.0f, -1.0f, 1.0f);	 // Left Of Triangle (Front)
	glColor3f(0.0f, 0.0f, 1.0f);	 // Blue
	glVertex3f(1.0f, -1.0f, 1.0f);	 // Right Of Triangle (Front)
	glColor3f(1.0f, 0.0f, 0.0f);	 // Red
	glVertex3f(0.0f, 1.0f, 0.0f);	 // Top Of Triangle (Right)
	glColor3f(0.0f, 0.0f, 1.0f);	 // Blue
	glVertex3f(1.0f, -1.0f, 1.0f);	 // Left Of Triangle (Right)
	glColor3f(0.0f, 1.0f, 0.0f);	 // Green
	glVertex3f(1.0f, -1.0f, -1.0f);	 // Right Of Triangle (Right)
	glColor3f(1.0f, 0.0f, 0.0f);	 // Red
	glVertex3f(0.0f, 1.0f, 0.0f);	 // Top Of Triangle (Back)
	glColor3f(0.0f, 1.0f, 0.0f);	 // Green
	glVertex3f(1.0f, -1.0f, -1.0f);	 // Left Of Triangle (Back)
	glColor3f(0.0f, 0.0f, 1.0f);	 // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f); // Right Of Triangle (Back)
	glColor3f(1.0f, 0.0f, 0.0f);	 // Red
	glVertex3f(0.0f, 1.0f, 0.0f);	 // Top Of Triangle (Left)
	glColor3f(0.0f, 0.0f, 1.0f);	 // Blue
	glVertex3f(-1.0f, -1.0f, -1.0f); // Left Of Triangle (Left)
	glColor3f(0.0f, 1.0f, 0.0f);	 // Green
	glVertex3f(-1.0f, -1.0f, 1.0f);	 // Right Of Triangle (Left)
	glEnd();						 // Done Drawing The Pyramid

	glLoadIdentity();				 // Reset The Current Modelview Matrix
	glTranslatef(1.5f, 0.0f, -7.0f); // Move Right 1.5 Units And Into The Screen 7.0
	// glRotatef(rquad,1.0f,1.0f,1.0f);					// Rotate The Quad On The X axis ( NEW )

	glBegin(GL_QUADS);				 // Draw A Quad
	glColor3f(0.0f, 1.0f, 0.0f);	 // Set The Color To Blue
	glVertex3f(1.0f, 1.0f, -1.0f);	 // Top Right Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, -1.0f);	 // Top Left Of The Quad (Top)
	glVertex3f(-1.0f, 1.0f, 1.0f);	 // Bottom Left Of The Quad (Top)
	glVertex3f(1.0f, 1.0f, 1.0f);	 // Bottom Right Of The Quad (Top)
	glColor3f(1.0f, 0.5f, 0.0f);	 // Set The Color To Orange
	glVertex3f(1.0f, -1.0f, 1.0f);	 // Top Right Of The Quad (Bottom)
	glVertex3f(-1.0f, -1.0f, 1.0f);	 // Top Left Of The Quad (Bottom)
	glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Quad (Bottom)
	glVertex3f(1.0f, -1.0f, -1.0f);	 // Bottom Right Of The Quad (Bottom)
	glColor3f(1.0f, 0.0f, 0.0f);	 // Set The Color To Red
	glVertex3f(1.0f, 1.0f, 1.0f);	 // Top Right Of The Quad (Front)
	glVertex3f(-1.0f, 1.0f, 1.0f);	 // Top Left Of The Quad (Front)
	glVertex3f(-1.0f, -1.0f, 1.0f);	 // Bottom Left Of The Quad (Front)
	glVertex3f(1.0f, -1.0f, 1.0f);	 // Bottom Right Of The Quad (Front)
	glColor3f(1.0f, 1.0f, 0.0f);	 // Set The Color To Yellow
	glVertex3f(1.0f, -1.0f, -1.0f);	 // Top Right Of The Quad (Back)
	glVertex3f(-1.0f, -1.0f, -1.0f); // Top Left Of The Quad (Back)
	glVertex3f(-1.0f, 1.0f, -1.0f);	 // Bottom Left Of The Quad (Back)
	glVertex3f(1.0f, 1.0f, -1.0f);	 // Bottom Right Of The Quad (Back)
	glColor3f(0.0f, 0.0f, 1.0f);	 // Set The Color To Blue
	glVertex3f(-1.0f, 1.0f, 1.0f);	 // Top Right Of The Quad (Left)
	glVertex3f(-1.0f, 1.0f, -1.0f);	 // Top Left Of The Quad (Left)
	glVertex3f(-1.0f, -1.0f, -1.0f); // Bottom Left Of The Quad (Left)
	glVertex3f(-1.0f, -1.0f, 1.0f);	 // Bottom Right Of The Quad (Left)
	glColor3f(1.0f, 0.0f, 1.0f);	 // Set The Color To Violet
	glVertex3f(1.0f, 1.0f, -1.0f);	 // Top Right Of The Quad (Right)
	glVertex3f(1.0f, 1.0f, 1.0f);	 // Top Left Of The Quad (Right)
	glVertex3f(1.0f, -1.0f, 1.0f);	 // Bottom Left Of The Quad (Right)
	glVertex3f(1.0f, -1.0f, -1.0f);	 // Bottom Right Of The Quad (Right)
	glEnd();						 // Done Drawing The Quad

} // end of the functino
