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
 *
 * https://github.com/berlinbrown/OctaneMechOpenGLGame
 */

#include <gl\gl.h>     // Header File For The OpenGL32 Library
#include <gl\glaux.h>  // Header File For The Glaux Library
#include <gl\glu.h>    // Header File For The GLu32 Library
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "objects.h"

static void compilecube(void);
static void drawcube(void);
static void init_cube(int list_id);
static void rendercube(void);

#undef CURRENT_OBJECT
#define CURRENT_OBJECT colorcube

//
// simple objects library
//
DriverObjects colorcube = {
    init_cube,    // init, must be called first
    compilecube,  // compile
    drawcube,     // draw
    rendercube,   // render to scene
    0             // loaded by INIT
};

//
// init cube
// - load anything special about the cube
// one important function
//
static void init_cube(int list_id)
{
  CURRENT_OBJECT.visible = 0;

  // store the id through the function
  // there is probably a better way to do this
  CURRENT_OBJECT.call_id = list_id;

}  // end of the function

//=========================================================
// Draw cube with normals turned on
// Note: have to use triangles, (dope!)
// - also no particular order when drawing triangles
//=========================================================
static void drawcube(void)
{
  float v[3][3] = {0};
  float size = 1.0f;

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

  glColor3f(1.0f, 0.0f, 0.0f);
  // Calc normal and draw
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left bottom front

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

  // Calc normal and draw
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left bottom front

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

  glColor3f(1.0f, 1.0f, 0.0f);
  // Calc normal and draw
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left bottom bac

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

  // Calc normal and draw
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left bottom front

  // Draw the right side
  // Triangle
  v[0][0] = size;
  v[0][1] = 0.0f;
  v[0][2] = size;

  v[1][0] = size;
  v[1][1] = 0.0f;
  v[1][2] = -size;

  v[2][0] = size;
  v[2][1] = size;
  v[2][2] = size;

  glColor3f(0.0f, 0.5f, 1.0f);
  // Calc normal and draw
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left bottom bac

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

  // Calc normal and draw
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left bottom bac

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

  glColor3f(1.0f, 0.5f, 1.0f);
  // Calc normal and draw
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left bottom bac

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

  // Calc normal and draw
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left side

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

  glColor3f(0.6f, 0.6f, 0.6f);
  // Calc normal and dra
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left side

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

  // Calc normal and draw
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left side

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

  glColor3f(0.03f, 0.3f, 0.3f);
  // Calc normal and dra
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left side

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

  // Calc normal and dra
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left side

  glEnd();

}  // end of the function

//=========================================================
// Now the function to actually draw it
//=========================================================
static void rendercube(void)
{
  static float x = 0.0f;

  if (CURRENT_OBJECT.visible)
  {
    glPushMatrix();

    glRotatef(x, 0.0f, 1.0f, 0.0f);
    glCallList(colorcube.call_id);
    x += 2.1f;
    glPopMatrix();
  }  // end of the if

}  // end of the function

//=========================================================
// compile cube
//=========================================================
static void compilecube(void)
{
  int cube_id;
  // setup a spot for display list for background
  // cubeobject = getcurrentobject();
  cube_id = colorcube.call_id;

  // apply list
  glNewList(cube_id, GL_COMPILE);

  // call drawing function
  // we could use (drawcube)
  // but this may method make it a little better
  colorcube.draw();

  glEndList();

}  // end of the function
