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
//
// grid.cpp
//
// - draw a simple grid in the background
//
#include <GL/gl.h>   // Header File For The OpenGL32 Library
#include <GL/glu.h>  // Header File For The GLu32 Library
#include <stdio.h>
#include <stdlib.h>

#include "objects.h"

#undef CURRENT_OBJECT
#define CURRENT_OBJECT grid

static void draw_grid(void);
static void compile_grid(void);
static void render_grid(void);
static void init_grid(int list_id);

//
// simple objects library
// - make sure to change the number of objects
// in objects.h
//
DriverObjects grid = {
    init_grid,     // init, must be called first
    compile_grid,  // compile
    draw_grid,     // draw
    render_grid,   // render to scene
    0              // loaded by INIT
};

//---------------------------------------------------------
// Note: these functions are misc objects
// draw background
//---------------------------------------------------------
//=========================================================
// draw the backdrops
//=========================================================
static void drawbackxy(float gridsize, float sections)
{
  float i = 0.0f;
  float j = 0.0f;

  for (i = -gridsize; i <= gridsize; i += sections)
  {
    glBegin(GL_LINES);
    glVertex3f(i, 0.0f, -gridsize);
    glVertex3f(i, gridsize, -gridsize);
    glEnd();
  }  // end of the for

  // now draw the other lines --
  for (j = 0.0f; j <= gridsize; j += sections)
  {
    glBegin(GL_LINES);
    glVertex3f(-gridsize, j, -gridsize);
    glVertex3f(gridsize, j, -gridsize);
    glEnd();
  }  // end of the functino

}  // end of function

//=========================================================
// draw back drop yz
//=========================================================
static void drawbackyz(float gridsize, float sections)
{
  float i = 0.0f;
  float j = 0.0f;

  for (i = -gridsize; i <= gridsize; i += sections)
  {
    glBegin(GL_LINES);
    glVertex3f(gridsize, 0.0f, i);
    glVertex3f(gridsize, gridsize, i);
    glEnd();
  }  // end of the for

  // now draw the other lines --
  for (j = 0.0f; j <= gridsize; j += sections)
  {
    glBegin(GL_LINES);
    glVertex3f(gridsize, j, -gridsize);
    glVertex3f(gridsize, j, gridsize);
    glEnd();
  }  // end of the functino

}  // end of the functin

//=========================================================
// draw grid
// - draw grid are the points on the plane
// that show the ground
//=========================================================
static void drawbackxz(float gridsize, float sections)
{
  float i = 0.0f;
  float j = 0.0f;

  for (i = -gridsize; i <= gridsize; i += sections)
  {
    glBegin(GL_LINES);
    glVertex3f(i, 0.0f, -gridsize);
    glVertex3f(i, 0.0f, gridsize);
    glEnd();
  }  // end of the for

  // now draw the other lines
  for (j = -gridsize; j <= gridsize; j += sections)
  {
    glBegin(GL_LINES);
    glVertex3f(-gridsize, 0.0f, j);
    glVertex3f(gridsize, 0.0f, j);
    glEnd();
  }  // end of the functino

}  // end of the function

//=========================================================
// drawbackground
// - grids in the back for testing
//=========================================================
static void draw_grid(void)
{
#if 0
   //drawbackxz(GRID_SIZE, 1.0f);
#endif
  drawbackxy(GRID_SIZE, 1.0f);
  drawbackyz(GRID_SIZE, 1.0f);

}  // end of the function

//
// init
// - load anything special about the
// one important function
//
static void init_grid(int list_id)
{
  // store the id through the function
  // there is probably a better way to do this
  CURRENT_OBJECT.call_id = list_id;

}  // end of the functino

//=========================================================
// Now the function to actually draw it
//=========================================================
static void render_grid(void)
{
  glPushMatrix();

  // standard color - green
  glColor3f(0.0f, 0.8f, 0.0f);

  glCallList(CURRENT_OBJECT.call_id);

  glPopMatrix();

}  // end of the function

//=========================================================
// compile
//=========================================================
static void compile_grid(void)
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

}  // end of the function
