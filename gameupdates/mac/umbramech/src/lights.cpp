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
// lights.cpp
//
//

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <math.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

static GLfloat no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
static GLfloat mat_ambient[] = {0.9f, 0.9f, 0.9f, 1.0f};
static GLfloat mat_diffuse[] = {0.9f, 0.8f, 0.8f, 1.0f};
static GLfloat mat_specular[] = {0.0f, 1.0f, 1.0f, 1.0f};
static GLfloat no_shininess[] = {0.0f};
static GLfloat low_shininess[] = {5.0f};
static GLfloat high_shininess[] = {100.0f};
static GLfloat mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

static struct tagDriverLights *light_ptrs[MAX_LIGHTS] = {NULL};

//
// Materials
//
void setmaterial(float amb[], float diff[], float spec[],
				 float shine[], float emiss[])
{
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	glMaterialfv(GL_FRONT, GL_SHININESS, shine);
	glMaterialfv(GL_FRONT, GL_EMISSION, emiss);
} 

//
// Material
//
void InitMaterial(void)
{
	// Change the main properities for all objects
	setmaterial(no_mat, mat_diffuse, mat_specular, low_shininess, no_mat);

} 

//
// CreateBot
// - allocate memory for bot
//
struct tagDriverLights *CreateLight(int id)
{
	struct tagDriverLights *light;

	light = malloc(sizeof(struct tagDriverLights));

	ZeroMemory((struct tagDriverLights *)light,
			   sizeof(struct tagDriverLights));

	light->index_id = id;
	light->light_id = GL_LIGHT0 + id;

	light->position[0] = 3.8f;
	light->position[1] = LIGHT_HEIGHT_0;
	light->position[2] = 2.0f;
	light->position[3] = 1.0f;

	light->state = DEAD_STATE;

	return light;

} 

//
// DestroyLight
//
void DestroyLight(DriverLightsPtr b)
{
	// free(b);
	RELEASE_OBJECT(b);

} // end of the function

//
// RenderWirebox
//
void RenderWirebox(DriverLightsPtr b)
{
	BEGIN_BOT;
	// Translate then rotate
	glTranslatef(b->position[0], b->position[1], b->position[2]);

	// draw the object to screen
	driver_objects[WIREBOX_OBJECT]->render();

	END_BOT;
} 

//
// SetLight
//
void set_Light(DriverLightsPtr bulb)
{

	// Now Prepare for opengl drawing
	//....................................

	// Enablelighting
	glEnable(GL_LIGHTING);

	// Note: I want to keep lighting in general but not this light
	if (bulb->state)
	{
		glEnable(bulb->light_id);

		// Setup the light
		glLightfv(bulb->light_id, GL_POSITION, bulb->position);
		glDisable(GL_LIGHTING);

		// Draw a wire  box....
		RenderWirebox(bulb);

		glEnable(GL_LIGHTING);

	} // end of the if
	else
	{
		glDisable(bulb->light_id);
	} // end of if-else

} 

//
// GenerateBots
//
void GenerateLights(void)
{
	int index = 0;

	for (index = 0; index < MAX_LIGHTS; index++)
	{
		light_ptrs[index] = CreateLight(index);

	} // END of the for

	// for now only set the first one
	light_ptrs[0]->state = ALIVE_STATE;
	light_ptrs[1]->position[1] = 12.0f;

	light_ptrs[1]->state = ALIVE_STATE;
	light_ptrs[1]->position[0] = -1.0f;
	light_ptrs[1]->position[1] = LIGHT_HEIGHT_1;
	light_ptrs[1]->position[2] = 12.0f;

} 

//
// ShutdownBots
//
void ShutdownLights(void)
{
	int index = 0;

	for (index = 0; index < MAX_LIGHTS; index++)
	{
		DestroyLight(light_ptrs[index]);
	} // end of the for

} 
//
// Set
//
void SetLights(void)
{
	int index = 0;

	for (index = 0; index < MAX_LIGHTS; index++)
	{

		set_Light(light_ptrs[index]);

	} // end of the for

} 
