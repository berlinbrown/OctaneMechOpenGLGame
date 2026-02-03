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

#include <GLUT/glut.h>   // GLUT for window/context
#include <OpenGL/gl.h>   // Core OpenGL functions
#include <OpenGL/glu.h>  // OpenGL Utility Library
#include <stdio.h>
#include <stdlib.h>

#undef CURRENT_OBJECT
#define CURRENT_OBJECT norm_cube

static void init_norm_cube(int list_id);
static void compile_norm_cube(void);
static void draw_norm_cube(void);
static void render_norm_cube(void);
static void draw_norm_cube(void);

//
// simple objects library
// - make sure to change the number of objects
// in objects.h
//
DriverObjects CURRENT_OBJECT = {
    init_norm_cube,     // init, must be called first
    compile_norm_cube,  // compile
    draw_norm_cube,     // draw
    render_norm_cube,   // render to scene
    0                   // loaded by INIT
};

//=========================================================
//=========================================================
//=========================================================
// draw cube with normals turned on
// Note: have to use triangles, (dope!)
// - also no particular order when drawing triangles
//=========================================================
static void draw_norm_cube(void)
{
  float v[3][3] = {0};
  float n[3] = {0};

  float size = 1.0f;

  // Note: normals are messed up for now
  // select between n0-n3

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

  CLR_0;
  // Calc normal and draw
  N_2;
  GET_NORMAL;

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

  CLR_1;
  // Calc normal and draw
  N_2;
  GET_NORMAL;

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

  CLR_2;
  // Calc normal and draw
  N_2;
  GET_NORMAL;

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

  MED_PURPLE;
  // Calc normal and draw
  N_2;
  GET_NORMAL;
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left bottom front

  // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?
  //  Draw the right side
  //  Triangle
  v[0][0] = size;
  v[0][1] = 0.0f;
  v[0][2] = size;

  v[1][0] = size;
  v[1][1] = 0.0f;
  v[1][2] = -size;

  v[2][0] = size;
  v[2][1] = size;
  v[2][2] = size;

  MED_BLUE;
  // Calc normal and draw
  N_2;
  GET_NORMAL;

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

  MED_GREEN;
  // Calc normal and draw
  N_2;
  GET_NORMAL;
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

  MED_PURPLE;
  // Calc normal and draw
  N_2;
  GET_NORMAL;
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

  MED_RED;
  // Calc normal and draw
  N_2;
  GET_NORMAL;
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

  CLR_0;
  // Calc normal and draw
  N_2;
  GET_NORMAL;
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

  CLR_3;
  // Calc normal and draw
  N_2;
  GET_NORMAL;
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

  // Calc normal and draw
  N_2;
  GET_NORMAL;
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

  // Calc normal and draw
  N_2;
  GET_NORMAL;
  glVertex3fv(v[0]);
  glVertex3fv(v[1]);
  glVertex3fv(v[2]);  // triangle left side

  glEnd();
}

//
// init
// - load anything special about the
// one important function
//
static void init_norm_cube(int list_id)
{
  CURRENT_OBJECT.visible = 1;

  // store the id through the function
  // there is probably a better way to do this
  CURRENT_OBJECT.call_id = list_id;

}  // end of the function

//=========================================================
// Now the function to actually draw it
//=========================================================
static void render_norm_cube(void)
{
  // glPushMatrix();

  glCallList(CURRENT_OBJECT.call_id);

  // glPopMatrix();
}

//=========================================================
// compile
//=========================================================
static void compile_norm_cube(void)
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
}
