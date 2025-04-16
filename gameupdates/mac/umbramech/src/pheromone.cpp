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
// pheromone.cpp
//
//

#include <stdio.h>
#include <stdlib.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

#undef CURRENT_OBJECT
#define CURRENT_OBJECT			pheromone

#undef CURRENT_PTR
#define CURRENT_PTR				StaticBotPtr

#undef CURRENT_OBJ
#define CURRENT_OBJ				StaticBot

#undef CURRENT_BOT
#define CURRENT_BOT				trail_set				


//=====================================
static void init_pheromone(int list_id);
static void compile_pheromone(void);
static void draw_pheromone(void);
static void render_pheromone(void);
static void draw_pheromone(void);


static CURRENT_PTR CreatePheromone(int bot_id);
static void RenderPheromone(CURRENT_PTR boid);
static void DestroyPheromone(CURRENT_PTR b);
static void ProcessPheromone(CURRENT_PTR b);


static void Draw_Pheromones(void);
static void Generate_Pheromones(void);
static void Shutdown_Pheromones(void);


//
// simple objects library
// - make sure to change the number of objects
// in objects.h
//
DriverObjects CURRENT_OBJECT =
{
	init_pheromone,			// init, must be called first
	compile_pheromone,		// compile
	draw_pheromone,			// draw 
	render_pheromone,			// render to scene
	0					// loaded by INIT
};


// call 1. garden.generate
// call 2. garden.drawall
// call 3. garden.shutdown
DriverSentinel CURRENT_BOT =
{

	CreatePheromone,		// create
	DestroyPheromone,	// destroy
	RenderPheromone,		// render
	ProcessPheromone,		// process

	Generate_Pheromones,		// generate
	Shutdown_Pheromones,		// shutdown
	Draw_Pheromones,			// drawall

	NULL,				// ptr
	0					// this value is faulty! MAX_PHEROMONES
};

//
// ActivatePheromone
//
// Note: this is a cpu hog, it will find an open
// slot searching to the max possibly
// implement a better method like some sort cache
//
void ActivatePheromone(float x, float y, float dir)
{
	int i = 0;
	for (i = 0; i < MAX_PHEROMONES; i++)
	{
		if (CURRENT_BOT.objects[i]->state == DEAD_STATE)
			break;
	} // end of the for 

	if (i >= MAX_PHEROMONES)
		return;

	// found a dead slot
	CURRENT_BOT.objects[i]->position[0] =x;
	CURRENT_BOT.objects[i]->position[2] =y;
	CURRENT_BOT.objects[i]->rotation[1] = dir;
	CURRENT_BOT.objects[i]->state = ALIVE_STATE;
	CURRENT_BOT.objects[i]->food = PHEROMONE_LIFE;

	CURRENT_BOT.objects[i]->delete_flag = false;


} // end of the function 

//
// Generate Nests
//
static void Generate_Pheromones(void)
{
	int index = 0;


	// This has to be set here because
	// of a config file bug
	CURRENT_BOT.max_items = MAX_PHEROMONES;

	// create the array of pointers
	CURRENT_BOT.objects = (CURRENT_OBJ **)malloc(
			sizeof(CURRENT_OBJ *)*CURRENT_BOT.max_items);

	for (index = 0; index <  CURRENT_BOT.max_items; index++)
	{


		// this bordering on insane
		// allocate an array of bot pointers, duh for nest
		 CURRENT_BOT.objects[index] =  CURRENT_BOT.create(index);

		
	} // end of the for
	

} // end of the function

//
// Shutdown Nests
//
static void Shutdown_Pheromones(void)
{
	int index = 0;

	for (index = 0; index <  CURRENT_BOT.max_items; index++)
	{
		 CURRENT_BOT.destroy( CURRENT_BOT.objects[index]);

	} // end of the for 

	// Shrug, free the ptr-to-ptrs
	//free(CURRENT_BOT.objects);	
	RELEASE_OBJECT(CURRENT_BOT.objects);


} // end of the function


//
// Draw Nests
//
static void Draw_Pheromones(void)
{
	int index = 0;

	for (index = 0; index <  CURRENT_BOT.max_items; index++)
	{
		 CURRENT_BOT.process( CURRENT_BOT.objects[index]);

		 CURRENT_BOT.render( CURRENT_BOT.objects[index]);

	} // end of the for 

} // end of the function 


//
// Process Events
//
static void ProcessPheromone(CURRENT_PTR b)
{
	b->food--;
	if (b->food < 0) {
		b->food = 0;
		b->state = DEAD_STATE;	// pheromone is dead

	} // end of the if 


} // end of the function


//
// Create bot
//
static CURRENT_PTR CreatePheromone(int bot_id)
{
	CURRENT_PTR		bot;

	bot = (CURRENT_PTR)malloc(sizeof(CURRENT_OBJ));

	ZeroMemory((CURRENT_PTR)bot, 
			sizeof(CURRENT_OBJ));
	
	bot->position[0] = 0;
	bot->position[1] = 0;
	bot->position[2] = 0;

	bot->list_id = bot_id;


	bot->rotation[0] = 0;
	bot->rotation[1] = 0;
	bot->rotation[2] = 0;

	bot->size[0] = 1.0f;
	bot->size[1] = 1.0f;
	bot->size[2] = 1.0f;

	bot->color[0] = 1.0f;
	bot->color[1] = 1.0f;
	bot->color[2] = 1.0f;

	bot->state = DEAD_STATE;

	bot->food = PHEROMONE_LIFE;

	bot->delete_flag = false;




	return bot;

} // end of the function

//
// DestroyBot
//
static void DestroyPheromone(CURRENT_PTR b)
{
	free(b);

} // end of the functino 

//
// RenderBot
//
static void RenderPheromone(CURRENT_PTR boid)
{

	if (boid->state == DEAD_STATE)
		return;

	glDisable(GL_LIGHTING);

	BEGIN_BOT;
 
		// Translate then rotate
		glTranslatef(boid->position[0],boid->position[1],
				boid->position[2]);

		// rotate based on the ship struct
		glRotatef(boid->rotation[1], 0.0f, 1.0f, 0.0f);
		glRotatef(boid->rotation[0], 1.0f, 0.0f, 0.0f);
		glRotatef(boid->rotation[2], 0.0f, 0.0f, 1.0f);

		// Scale accordingly

		glScalef(boid->size[0], boid->size[1], boid->size[2]);


		// This may or may not change the color
		glColor3f(boid->color[0], boid->color[1], boid->color[2]);

	
		driver_objects[SQUARE_OBJECT]->render();

	END_BOT;

	glEnable(GL_LIGHTING);

} // end of the function 


//---------------------------------------------------------

//=========================================================
//=========================================================
static void draw_pheromone(void)
{
	float h;
	float width = 0.3f;
	h = width / 2.0f;

		glBegin(GL_LINE_LOOP);

		// Front Face
		glVertex3f(-h,  0.0f,  h);	// left bottom 
		glVertex3f( h,  0.0f,  h);	// right bottom
		glVertex3f( h,  0.0f,  -h);	// top right
		glVertex3f(-h, 0.0f,-h);	// top left

		glEnd();

} // end of the function


//
// init
// - load anything special about the
// one important function 
//
static void init_pheromone(int list_id)
{

	CURRENT_OBJECT.visible = 1;

	// store the id through the function
	// there is probably a better way to do this
	CURRENT_OBJECT.call_id = list_id;	
	
} // end of the functino


//=========================================================
// Now the function to actually draw it
//=========================================================
static void render_pheromone(void)
{
		//glPushMatrix();

			glCallList(CURRENT_OBJECT.call_id);

		//glPopMatrix();

} // end of the function

//=========================================================
// compile
//=========================================================
static void compile_pheromone(void)
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

