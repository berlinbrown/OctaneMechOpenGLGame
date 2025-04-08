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
// los.cpp
//
// - line of sight algorithm for the fire ant AI
//
// it is simple, returns a value of 1 or 2
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

#include "globals.h"
#include "bot.h"
#include "gldrawlib.h"
#include "fireants.h"

#define UP 1
#define DOWN 2
#define DIRECTION(dir, val) dir = val

// Our line of sight is simple, it is composed of a triangle
// if the point is within that triangle, return a one
// otherwise return a zero

//
// Draw Line of Sight
// where y is actually the z
//
void DrawLineOfSight(float x1, float y1, float x2, float y2)
{
	float height = 0.1f;

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);

	// Front Face
	glVertex3f(x1, height, y1); // left bottom
	glVertex3f(x2, height, y2); // right bottom

	// Back Face

	glEnd();

	glEnable(GL_LIGHTING);

} // end of the function

//
// The algo just checks the y=mx+b of the line
// and depending on the location of the vertex, etc
// it is in the triangle or not
// - no clipping
//
bool CheckSight(DriverBotPtr bot, DriverBotPtr nme)
{
	float rad;
	float tmp_heading;
	float tmp_x2;
	float tmp_y2;
	float tmp_x3;
	float tmp_y3;
	float a1, b1, a2, b2, a3, b3, a4, b4;
	float m1, m2, m3;
	float bb1, bb2, bb3;
	float center_x, center_y;
	int direction;
	bool AB_vert;
	bool BC_vert;
	bool CA_vert;
	int inside = 0;

	AB_vert = BC_vert = CA_vert = false;

	// now the next point of the triangle
	// shift by 45 degrees
	tmp_heading = bot->heading + 125.0f;
	if (tmp_heading > 360.0f)
		tmp_heading -= 360.0f;

	rad = tmp_heading / RAD_TO_DEG;

	tmp_x2 = LINE_OF_SIGHT * (float)cos(rad);
	tmp_y2 = LINE_OF_SIGHT * (float)sin(rad);

	tmp_x2 = tmp_x2 + bot->x;
	tmp_y2 = (-tmp_y2) + bot->y;

	// Now the final point
	tmp_heading = bot->heading + 55.0f;
	if (tmp_heading > 360.0f)
		tmp_heading -= 360.0f;

	rad = tmp_heading / RAD_TO_DEG;

	tmp_x3 = LINE_OF_SIGHT * (float)cos(rad);
	tmp_y3 = LINE_OF_SIGHT * (float)sin(rad);

	tmp_x3 = tmp_x3 + bot->x;
	tmp_y3 = (-tmp_y3) + bot->y;

	a1 = tmp_x2;
	b1 = tmp_y2;

	a2 = tmp_x3;
	b2 = tmp_y3;

	a3 = bot->x;
	b3 = bot->y;

	a4 = nme->x;
	b4 = nme->y;

	// find the slope of the different lines
	// have to check for divide by zero also
	if ((a2 - a1) != 0)
	{
		m1 = (b2 - b1) / (a2 - a1); // a->b
		bb1 = (b1) - (m1 * a1);
	}
	else if ((a2 - a1) == 0)
	{
		AB_vert = true;
	} // end of the if-else

	// y = mx + b
	if ((a3 - a2) != 0)
	{
		m2 = (b3 - b2) / (a3 - a2);
		bb2 = (b2) - (m2 * a2);
	}
	else if ((a3 - a2) == 0)
	{
		BC_vert = true;
	} // end of if-else

	// y = mx + b
	if ((a1 - a3) != 0)
	{
		m3 = (b1 - b3) / (a1 - a3);
		bb3 = (b3) - (m3 * a3);
	}
	else if ((a3 - a2) == 0)
	{
		CA_vert = true;
	} // end of the if-else

	center_x = (a1 + a2 + a3) / 3.0f;
	center_y = (b1 + b2 + b3) / 3.0f;

	// a->b
	if (((m1 * center_x) + bb1) >= center_y)
		DIRECTION(direction, UP);
	else
		DIRECTION(direction, DOWN);

	if (AB_vert == true)
	{
		if ((a1 < a4) && (a1 < center_x))
			inside++;
		else if ((a1 > a4) && (a1 > center_x))
			inside++;
	}
	else
	{
		if (direction == UP)
		{
			if (b4 <= ((m1 * a4) + bb1))
				inside++;
		}
		else if (direction == DOWN)
		{
			if (b4 >= ((m1 * a4) + bb1))
				inside++;
		} // end of if-else

	} // end of the if - else

	// b->c
	if (((m2 * center_x) + bb2) >= center_y)
		DIRECTION(direction, UP);
	else
		DIRECTION(direction, DOWN);

	if (BC_vert == true)
	{
		if ((a2 < a4) && (a2 < center_x))
			inside++;
		else if ((a2 > a4) && (a2 > center_x))
			inside++;
	}
	else
	{
		if (direction == UP)
		{
			if (b4 <= ((m2 * a4) + bb2))
				inside++;
		}
		else if (direction == DOWN)
		{

			if (b4 >= ((m2 * a4) + bb2))
				inside++;

		} // end of the if-else

	} // end of the if-else

	// c->a
	if (((m3 * center_x) + bb3) >= center_y)
		DIRECTION(direction, UP);
	else
		DIRECTION(direction, DOWN);

	if (CA_vert == true)
	{
		if ((a3 < a4) && (a3 < center_x))
			inside++;
		else if ((a3 > a4) && (a3 > center_x))
			inside++;
	}
	else
	{

		if (direction == UP)
		{
			if (b4 <= ((m3 * a4) + bb3))
				inside++;
		}
		else if (direction == DOWN)
		{
			if (b4 >= ((m3 * a4) + bb3))
				inside++;
		} // end of the if-else

	} // end of the if-else

#if DRAW_LINE_SIGHT
	// DrawLineOfSight(bot->x, bot->y, tmp_x, tmp_y);
	DrawLineOfSight(tmp_x2, tmp_y2, tmp_x3, tmp_y3);
	DrawLineOfSight(bot->x, bot->y, tmp_x2, tmp_y2);
	DrawLineOfSight(bot->x, bot->y, tmp_x3, tmp_y3);
#endif

	// check if point lies inside
	if (inside == 3)
	{

		return true;
	}
	else
	{
		return false;
	} // end of if-else

} // end of the function
