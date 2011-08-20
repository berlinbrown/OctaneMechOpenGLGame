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
// particles.cpp
// 
// particles or explosion engine
//
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library
#include <gl\glaux.h>		// Header File For The Glaux Library

#include "bot.h"
#include "particles.h"

static ParticleList	*particle_set[MAX_PARTICLE_SET];
static int	particle_index = 0;

// 
// Create Particle
//
ParticleList *CreateParticleList(void) {

    ParticleList *result = (ParticleList *)malloc(
				sizeof(ParticleList));

	result->x = 0;
	result->y = 0;

	result->life = 0;
	result->state = DEAD_STATE;

	return result;

} // end of the function 

//
// Destroy List
//
void DestroyParticleList(ParticleList *list) 
{
    RELEASE_OBJECT(list);

} // end of the function 

//
// New_Speed
//
static void New_Speed(float d[3], float speed)
{

	float	x, y, z;
	float	r;
	float	tmp;

	tmp = 2.0f * speed;

	r = (tmp * ((float)rand())/((float)RAND_MAX)) - speed;
	x = r;

	r = (tmp * ((float)rand())/((float)RAND_MAX)) - speed;
	y = r;

	r = (tmp * ((float)rand())/((float)RAND_MAX)) - speed;
	z = r;

	d[0] = x;
	d[1] = y;
	d[2] = z;

} // end of the function 

//
// Set Explosion
// - just set the position and turn it on
//
void Set_Explosion(ParticleList *list, float x, float y)
{
	int i;

	list->life = MAX_PARTICLE_LIFE;

	list->state = ALIVE_STATE;
	list->x = x;
	list->y = y;

	for (i = 0; i < MAX_PARTICLES; i++)
	{
		list->particles[i].p_id = i;

		list->particles[i].p_pos[0] = x;
		list->particles[i].p_pos[1] = 1.0f;
		list->particles[i].p_pos[2] = y;

		list->particles[i].p_color[0] = 1.0f;
		list->particles[i].p_color[1] = 0.1f;
		list->particles[i].p_color[2] = 0.1f;

		list->particles[i].p_state = ALIVE_STATE;

		// Set the speed
		New_Speed(list->particles[i].p_speed, PARTICLE_SPEED);

	} // end of the for 

	
} // end of the function 

//
// Draw_Particles
//
void Draw_Particles(ParticleList *list)
{
	int i=0;

	if (list->state == ALIVE_STATE)
	{

		glDisable(GL_LIGHTING);

		glPushMatrix();

			glBegin(GL_POINTS);

			for(i =0; i < MAX_PARTICLES; i++)
			{
				if (list->particles[i].p_state == DEAD_STATE)
					continue;

				glColor3fv(list->particles[i].p_color);
				glVertex3fv(list->particles[i].p_pos);

			} // end of the for 

			glEnd();

		glPopMatrix();

		glEnable(GL_LIGHTING);


	} // end of the if 

} // end of the function 


//
// Wrapper Functions
//
void Build_ParticleSet(void)
{
	int i;

	for (i = 0; i < MAX_PARTICLE_SET; i++)
	{
		particle_set[i] = CreateParticleList();
	} // end of the for 

	particle_index = 0;

} // end of the function

//
// Destroy_Particles(void)
//
void Destroy_ParticleSet(void)
{
	int i = 0;

	for (i = 0; i < MAX_PARTICLE_SET; i++)
	{
		DestroyParticleList(particle_set[i]);
	} // end of the of r

} // end of the function 

//
// SetExplosion
//
void SetExplosion(float x, float y)
{
	Set_Explosion(particle_set[particle_index], x, y);

	particle_index++;
	if (particle_index >= MAX_PARTICLE_SET)
		particle_index = 0;

} // end of the function 


//
// AnimateExplisions
//
void Anim_Particles(ParticleList *list)
{
	int i = 0;
	int j = 0;
	float dv = 0;

	if (list->state == ALIVE_STATE)
	{


		for(i =0; i < MAX_PARTICLES; i++)
		{
		
			for (j = 0; j < 3; j++) 
			{
				list->particles[i].p_pos[j] += 
						list->particles[i].p_speed[j];

				// kill if this one hits the ground
				if (list->particles[i].p_pos[1] < 0.0f)
					list->particles[i].p_state = DEAD_STATE;

				dv = 1.0f / 500.0f;
	
				list->particles[i].p_color[j] -= dv;

				if (list->particles[i].p_color[j] < 0.0f)
					list->particles[i].p_color[j] = 0.0f;

			} // end of the for

		} // end of the for 


		// subtract life
		list->life--;

		if (list->life < 0) {

			list->life = 0;
			list->state = DEAD_STATE;

		} // end of the if 

	} // end of big if

} // end of the function

//
// AnimateExplosions
//
void AnimateExplosions(void)
{
	int i = 0;

	for (i = 0; i < MAX_PARTICLE_SET; i++)
	{
		if (particle_set[i]->state == DEAD_STATE)
			continue;

		Anim_Particles(particle_set[i]);

	} // end of the functino 

} // end of the function


//
// Now draw the entire set
//
void DrawExplosions(void)
{
	int i = 0;

	for (i = 0; i < MAX_PARTICLE_SET; i++)
	{
		if (particle_set[i]->state == DEAD_STATE)
			continue;

		Draw_Particles(particle_set[i]);

	} // end of the functino 

} // end of the function 



