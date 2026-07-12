/**
 * Copyright (c) 2006-2026 Berlin Brown.  All Rights Reserved
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
 * Contact: Berlin Brown <berlin _dot_ brown at email>
 */

// ant.cpp
//  - the ant object

#include <GLUT/glut.h>   // GLUT for window/context
#include <OpenGL/gl.h>   // Core OpenGL functions
#include <OpenGL/glu.h>  // OpenGL Utility Library
#include <stdio.h>
#include <stdlib.h>
#include <objects.hpp>
#include <legacy_stubs.hpp>

#undef CURRENT_OBJECT
#define CURRENT_OBJECT norm_cube

static void init_norm_cube(int list_id);
static void compile_norm_cube(void);
static void draw_norm_cube(void);
static void render_norm_cube(void);
static void draw_norm_cube(void);

// simple objects library
// - make sure to change the number of objects
// in objects.h
DriverObjects CURRENT_OBJECT = {
    init_norm_cube,     // init, must be called first
    compile_norm_cube,  // compile
    draw_norm_cube,     // draw
    render_norm_cube,   // render to scene
    0                   // loaded by INIT
};

// draw cube with normals turned on
// Note: have to use triangles, (dope!)
// - also no particular order when drawing triangles
static void draw_norm_cube(void)
{
  float size = 1.0f;

  glColor3f(0.55f, 0.60f, 0.65f);

  glBegin(GL_TRIANGLES);

  // Front face (+Z), normal = (0, 0, 1)
  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-size, 0.0f,  size);
  glVertex3f( size, 0.0f,  size);
  glVertex3f( size,  size,  size);

  glNormal3f(0.0f, 0.0f, 1.0f);
  glVertex3f( size,  size,  size);
  glVertex3f(-size,  size,  size);
  glVertex3f(-size, 0.0f,  size);

  // Back face (-Z), normal = (0, 0, -1)
  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex3f( size, 0.0f, -size);
  glVertex3f(-size, 0.0f, -size);
  glVertex3f(-size,  size, -size);

  glNormal3f(0.0f, 0.0f, -1.0f);
  glVertex3f(-size,  size, -size);
  glVertex3f( size,  size, -size);
  glVertex3f( size, 0.0f, -size);

  // Right face (+X), normal = (1, 0, 0)
  glNormal3f(1.0f, 0.0f, 0.0f);
  glVertex3f( size, 0.0f,  size);
  glVertex3f( size, 0.0f, -size);
  glVertex3f( size,  size, -size);

  glNormal3f(1.0f, 0.0f, 0.0f);
  glVertex3f( size,  size, -size);
  glVertex3f( size,  size,  size);
  glVertex3f( size, 0.0f,  size);

  // Left face (-X), normal = (-1, 0, 0)
  glNormal3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(-size, 0.0f, -size);
  glVertex3f(-size, 0.0f,  size);
  glVertex3f(-size,  size,  size);

  glNormal3f(-1.0f, 0.0f, 0.0f);
  glVertex3f(-size,  size,  size);
  glVertex3f(-size,  size, -size);
  glVertex3f(-size, 0.0f, -size);

  // Top face (+Y), normal = (0, 1, 0)
  glNormal3f(0.0f, 1.0f, 0.0f);
  glVertex3f(-size,  size,  size);
  glVertex3f( size,  size,  size);
  glVertex3f( size,  size, -size);

  glNormal3f(0.0f, 1.0f, 0.0f);
  glVertex3f( size,  size, -size);
  glVertex3f(-size,  size, -size);
  glVertex3f(-size,  size,  size);

  // Bottom face (-Y), normal = (0, -1, 0)
  glNormal3f(0.0f, -1.0f, 0.0f);
  glVertex3f(-size, 0.0f, -size);
  glVertex3f( size, 0.0f, -size);
  glVertex3f( size, 0.0f,  size);

  glNormal3f(0.0f, -1.0f, 0.0f);
  glVertex3f( size, 0.0f,  size);
  glVertex3f(-size, 0.0f,  size);
  glVertex3f(-size, 0.0f, -size);

  glEnd();
}

// init
// - load anything special about the
// one important function
static void init_norm_cube(int list_id)
{
  CURRENT_OBJECT.visible = 1;

  // Store the display list id for this object.
  CURRENT_OBJECT.call_id = list_id;

}

// Now the function to actually draw it
static void render_norm_cube(void)
{

  glCallList(CURRENT_OBJECT.call_id);

}

// compile
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
