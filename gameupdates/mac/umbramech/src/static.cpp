
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
// static.cpp
// - This really should be called
// objects but it was taken up already
// basically bots have attributes and
// use objects for drawing
// - This is the meat and potatoes of the
// artificial control

#include <GLUT/glut.h>   // GLUT for window/context
#include <OpenGL/gl.h>   // Core OpenGL functions
#include <OpenGL/glu.h>  // OpenGL Utility Library
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <DriverBots.hpp>
#include <gldrawlib.hpp>
#include <bot.hpp>
#include <lights.hpp>
#include <objects.hpp>

#undef CURRENT_PTR
#define CURRENT_PTR StaticBotPtr

#undef CURRENT_OBJECT
#define CURRENT_OBJECT StaticBot

#undef CURRENT_BOT
#define CURRENT_BOT nest

#define MAX_NESTS 1

static CURRENT_PTR CreateSentinel(int bot_id);
static void RenderSentinel(CURRENT_PTR boid);
static void DestroySentinel(CURRENT_PTR b);
static void ProcessNest(CURRENT_PTR b);

static void DrawNests(void);
static void GenerateNests(void);
static void ShutdownNests(void);

GLfloat funk_ambient[] = {0.0f, 0.0f, 0.8f, 1.0f};
GLfloat funk_diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat funk_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat zno_shininess[] = {0.0f};
GLfloat zlow_shininess[] = {5.0f};
GLfloat zhigh_shininess[] = {100.0f};
GLfloat zmat_emission[] = {0.3f, 0.3f, 0.2f, 0.0f};

// call 1. nest.generate
// call 2. nest.drawall
// call 3. nest.shutdown
DriverSentinel CURRENT_BOT = {

    CreateSentinel,   // create
    DestroySentinel,  // destroy
    RenderSentinel,   // render
    ProcessNest,      // process

    GenerateNests,  // generate
    ShutdownNests,  // shutdown
    DrawNests,      // drawll

    NULL,  // ptr
    0      // max_items
};

// each ant gets 1000 food pieces
#define INITIAL_NEST_FOOD (MAX_BOTS * INITIAL_ANT_FOOD)

// Generate Nests
static void GenerateNests(void)
{
  int index = 0;

  CURRENT_BOT.max_items = MAX_NESTS;

  // create the array of pointers
  CURRENT_BOT.objects = (CURRENT_OBJECT**)malloc(sizeof(CURRENT_OBJECT*));

  for (index = 0; index < CURRENT_BOT.max_items; index++)
  {
    // this bordering on insane
    // allocate an array of bot pointers, duh for nest
    CURRENT_BOT.objects[index] = CURRENT_BOT.create(index);

  }

  // Since I know first hand that there
  // is only one nest, create initial food offering
  // based on number of ants
  nest.objects[0]->food = INITIAL_NEST_FOOD;
}

// Shutdown Nests
static void ShutdownNests(void)
{
  int index = 0;

  for (index = 0; index < CURRENT_BOT.max_items; index++)
  {
    CURRENT_BOT.destroy(CURRENT_BOT.objects[index]);

  }
}

// Draw Nests
static void DrawNests(void)
{
  int index = 0;

  for (index = 0; index < CURRENT_BOT.max_items; index++)
  {
    CURRENT_BOT.process(CURRENT_BOT.objects[index]);

    CURRENT_BOT.render(CURRENT_BOT.objects[index]);

  }
}

// Process Events
static void ProcessNest(CURRENT_PTR b)
{
  // just rotate
  b->rotation[1] += 0.5f;
  if (b->rotation[1] >= 360) b->rotation[1] -= 360;
}

// Create bot
static CURRENT_PTR CreateSentinel(int bot_id)
{
  CURRENT_PTR bot;

  bot = (CURRENT_PTR)malloc(sizeof(CURRENT_OBJECT));
  ZeroMemory((CURRENT_PTR)bot, sizeof(CURRENT_OBJECT));

  bot->position[0] = 0;
  bot->position[1] = 0;
  bot->position[2] = 0;

  bot->rotation[0] = 0;
  bot->rotation[1] = 0;
  bot->rotation[2] = 0;

  bot->size[0] = 2.0f;
  bot->size[1] = 2.0f;
  bot->size[2] = 2.0f;

  bot->color[0] = 1.0f;
  bot->color[1] = 1.0f;
  bot->color[2] = 0.0f;

  bot->state = ALIVE_STATE;

  return bot;
}

// DestroyBot
static void DestroySentinel(CURRENT_PTR b) { free(b); }

// RenderBot
static void RenderSentinel(CURRENT_PTR boid)
{
  BEGIN_BOT;

  // Translate then rotate
  glTranslatef(boid->position[0], boid->position[1], boid->position[2]);

  // rotate based on the ship struct
  glRotatef(boid->rotation[1], 0.0f, 1.0f, 0.0f);
  glRotatef(boid->rotation[0], 1.0f, 0.0f, 0.0f);
  glRotatef(boid->rotation[2], 0.0f, 0.0f, 1.0f);

  // Scale accordingly
  glScalef(boid->size[0], boid->size[1], boid->size[2]);

  // This may or may not change the color
  glColor3f(boid->color[0], boid->color[1], boid->color[2]);

  // draw the object to screen
  // driver_objects[ANT_OBJECT]->render();

  // set the material for this object
  setmaterial(funk_ambient, funk_diffuse, funk_specular, zlow_shininess, zmat_emission);

  gluSphere(quadric, 0.5f, 18, 8);  // draw sphere for hood

  END_BOT;
}
