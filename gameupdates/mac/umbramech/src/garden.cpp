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
// garden.cpp
// the place where the ants live
//

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

#undef CURRENT_PTR
#define CURRENT_PTR StaticBotPtr

#undef CURRENT_OBJECT
#define CURRENT_OBJECT StaticBot

#undef CURRENT_BOT
#define CURRENT_BOT garden

#define CUBE_SIZE 1.5f

static CURRENT_PTR CreateGarden(int bot_id);
static void RenderGarden(CURRENT_PTR boid);
static void DestroyGarden(CURRENT_PTR b);
static void ProcessGarden(CURRENT_PTR b);

static void Draw_Gardens(void);
static void Generate_Gardens(void);
static void Shutdown_Gardens(void);

void ResetGarden(CURRENT_PTR bot);
void PlaceGardenArea(CURRENT_PTR bot,
					 float x, float y, float width);

void RandomPlacement(CURRENT_PTR bot);

static DWORD game_ticks = 0;

static int total_respawns = 0;

// place the food between the nest or in front of the nest
// or on the side
float food_pos[3][3] = {
	{0.0f, 15.0f, 6.3f},
	{0.0f, -15.0f, 6.3f},
	{10.0f, 0.0f, 5.4f}};

// call 1. garden.generate
// call 2. garden.drawall
// call 3. garden.shutdown
DriverSentinel CURRENT_BOT =
	{

		CreateGarden,  // create
		DestroyGarden, // destroy
		RenderGarden,  // render
		ProcessGarden, // process

		Generate_Gardens, // generate
		Shutdown_Gardens, // shutdown
		Draw_Gardens,	  // drawall

		NULL, // ptr
		0	  // max_items
};

//
// GardenRespawn
//
void GardenRespawn(void)
{

	int i = 0;
	int tol = (rand() % GARD_RESPAWN_RATE) + 5;

	static int total = 0;
	float dead_plant = 0;

	for (i = 0; i < MAX_GARDENS; i++)
	{

		if (CURRENT_BOT.objects[i]->state == DEAD_STATE)
		{

			ResetGarden(CURRENT_BOT.objects[i]);

			SetGardenSize(1); // send to globals for HUD

			total_respawns++;
			if (total_respawns > (tol - 1))
			{
				total_respawns = 0;
				break;
			} // end of the if

		} // end of if

		// randomly check and kill garden
		// to keep a fresh garden
		dead_plant = INITIAL_GARD_FOOD * 0.28f;
		if (CURRENT_BOT.objects[i]->food < dead_plant)
		{
			if ((rand() % 3) == 1)
			{
				CURRENT_BOT.objects[i]->state = DEAD_STATE;
				SetGardenSize(-1);
			} // end of the if

		} // end of the if

	} // end of the for

} // end of the function

//
// Respawing the garden has to be done here
// after so many game ticks respawn the garden
// with so much food
//
void CheckRespawn(void)
{

	if (game_ticks++ >= CHECK_RESPAWN)
	{

		GardenRespawn();

		game_ticks = 0;

	} // end of the if

} 

//
// A brute force method for checking whether
// or not a bot is on some food
// - we need this test against the optimized version
// if there will be one
// calcs involved: MAX_BOTS * MAX_GARDENS: 300 * 15 =4500
//
// - returns: index + 1
// other wise zero
//
int BruteCheckFood(DriverBotPtr bot)
{

	float x_min, x_max;
	float y_min, y_max;
	float x_width;
	float y_width;
	int i;

	for (i = 0; i < MAX_GARDENS; i++)
	{

		// save some iterations
		if (CURRENT_BOT.objects[i]->state == DEAD_STATE) {
			continue;
		}

		x_width = FOOD_WIDTH * CUBE_SIZE;
		y_width = FOOD_WIDTH * CUBE_SIZE;

		x_min = CURRENT_BOT.objects[i]->position[0] -
				x_width;
		x_max = CURRENT_BOT.objects[i]->position[0] +
				x_width;

		y_min = CURRENT_BOT.objects[i]->position[2] -
				y_width;
		y_max = CURRENT_BOT.objects[i]->position[2] +
				y_width;

		if ((bot->x > x_min) && (bot->x < x_max) &&
			(bot->y > y_min) && (bot->y < y_max))
		{
			return i + 1;
		} // end of the if

	} // end of the for

	return 0;

} 

//
// Collect Food
// - mainly pertains to the ant
//
void CollectFood(DriverBotPtr bot, float food_rate)
{
	// food ant is carrying
	bot->foodstore += food_rate;
} 

//
// Drop Food
// - extract the food and then have the bot eat the food
//
void DropFood(DriverBotPtr bot, int id, float food_rate)
{

	float perc;
	float food_amt = 0;
	int t;
	float tmp;

	t = (int)(INIT_FOOD_RATE / 10);

	tmp = rand() % t;

	food_rate = (float)INIT_FOOD_RATE - tmp;

	food_amt = food_rate;

	// no, food quit
	if (CURRENT_BOT.objects[id]->food <= 0)
	{

		CURRENT_BOT.objects[id]->state = DEAD_STATE;
		return;
	} // end of the if

	if ((CURRENT_BOT.objects[id]->food - food_amt) < 0)
	{
		food_amt = CURRENT_BOT.objects[id]->food;

		CURRENT_BOT.objects[id]->food = 0;

		// depleted all the food
		CURRENT_BOT.objects[id]->state = DEAD_STATE;
		SetGardenSize(-1);

	} // end of if
	else
	{

		CURRENT_BOT.objects[id]->food -= food_amt;

	} // end of if-else

	// decrease the size of the piece of food
	perc = ((float)INITIAL_GARD_FOOD - food_amt) /
		   (float)INITIAL_GARD_FOOD;

	CURRENT_BOT.objects[id]->size[0] *= perc;
	CURRENT_BOT.objects[id]->size[1] *= perc;
	CURRENT_BOT.objects[id]->size[2] *= perc;

	// eat the food
	CollectFood(bot, food_amt);

} 

//
// Generate Nests
//
static void Generate_Gardens(void)
{
	int index = 0;

	CURRENT_BOT.max_items = MAX_GARDENS;

	// create the array of pointers
	CURRENT_BOT.objects = (CURRENT_OBJECT **)malloc(
		sizeof(CURRENT_OBJECT *) * CURRENT_BOT.max_items);

	for (index = 0; index < CURRENT_BOT.max_items; index++)
	{

		SetGardenSize(1);

		// this bordering on insane
		// allocate an array of bot pointers, duh for nest
		CURRENT_BOT.objects[index] = CURRENT_BOT.create(index);

		if (USE_GARDEN_AREA)
		{
			int sel = 0;
			int r = rand() % 3;

			if (r == 2)
				sel = 2;
			else if (r == 1)
				sel = 1;
			else
				sel = 0;

			PlaceGardenArea(CURRENT_BOT.objects[index],
							food_pos[sel][0], food_pos[sel][1], food_pos[sel][2]);
		}
		else
		{
			RandomPlacement(CURRENT_BOT.objects[index]);
		} // end of if

	} // end of the for

} 

//
// Shutdown Nests
//
static void Shutdown_Gardens(void)
{
	int index = 0;

	for (index = 0; index < CURRENT_BOT.max_items; index++)
	{
		CURRENT_BOT.destroy(CURRENT_BOT.objects[index]);

	} // end of the for

	// Shrug, free the ptr-to-ptrs
	// free(CURRENT_BOT.objects);
	RELEASE_OBJECT(CURRENT_BOT.objects);

} 

//
// Draw Nests
//
static void Draw_Gardens(void)
{
	int index = 0;

	for (index = 0; index < CURRENT_BOT.max_items; index++)
	{
		CURRENT_BOT.process(CURRENT_BOT.objects[index]);

		CURRENT_BOT.render(CURRENT_BOT.objects[index]);

	} // end of the for

} 

//
// Process Events
//
static void ProcessGarden(CURRENT_PTR b)
{
	// just rotate
	b->rotation[1] += 0.5f;
	if (b->rotation[1] >= 360)
		b->rotation[1] -= 360;

} 

//
// - place the garden somewhere in a predefined
// region
// - used to test the pheromone collection
//
void PlaceGardenArea(CURRENT_PTR bot, float x, float y, float width)
{
	float x_min;
	float y_min;
	float h_width;
	int rand_num = 0;
	float res;

	// bot->position[0] += tmp_x;
	// bot->position[2] += tmp_y;

	// place between the min and max randomly
	h_width = width / 2.0f;
	y_min = y - h_width;
	x_min = x - h_width;

	// we need a random number for the max
	rand_num = (int)(width * 1000.0f);
	res = (float)(rand() % rand_num) / 1000.0f;

	bot->position[0] = x_min + res;

	res = (float)(rand() % rand_num) / 1000.0f;
	bot->position[2] = y_min + res;

} 

//
// RandomPlacement
// - put the garden anywhere
//
void RandomPlacement(CURRENT_PTR bot)
{
	float tmp_x;
	float tmp_y;

	// position a bit of away from nest
	bot->position[0] =
		(((float)(rand() % 4000) / 100.0f) - 20.0f);

	tmp_x = ((float)(rand() % 4000) / 400.0f);
	if (bot->position[0] < 0)
		tmp_x = -tmp_x;

	bot->position[1] = 0.0f;

	tmp_y = ((float)(rand() % 4000) / 400.0f);
	bot->position[2] =
		(((float)(rand() % 4000) / 100.0f) - 20.0f);

	if (bot->position[2] < 0)
		tmp_y = -tmp_y;

	// attempt to pos away from the nest
	bot->position[0] += tmp_x;
	bot->position[2] += tmp_y;

} 

//
// LoadGardParms
//
void LoadGardParms(CURRENT_PTR bot)
{
	ZeroMemory((CURRENT_PTR)bot,
			   sizeof(CURRENT_OBJECT));

	bot->position[0] = 0;
	bot->position[1] = 0;
	bot->position[2] = 0;

	bot->rotation[0] = 0;
	bot->rotation[1] = 0;
	bot->rotation[2] = 0;

	bot->size[0] = 0.35f;
	bot->size[1] = 1.2f;
	bot->size[2] = 0.35f;

	bot->color[0] = 1.0f;
	bot->color[1] = 1.0f;
	bot->color[2] = 0.0f;

	bot->state = ALIVE_STATE;

	bot->food = INITIAL_GARD_FOOD;

} 

//
// Create bot
//
static CURRENT_PTR CreateGarden(int bot_id)
{
	CURRENT_PTR bot;

	bot = (CURRENT_PTR)malloc(sizeof(CURRENT_OBJECT));

	LoadGardParms(bot);

	bot->list_id = bot_id;

	return bot;

} 

//
// Reset Garden
// - similar to above
//
void ResetGarden(CURRENT_PTR bot)
{

	// the only thing we need to save
	int bot_id = bot->list_id;

	ZeroMemory((CURRENT_PTR)bot,
			   sizeof(CURRENT_OBJECT));

	LoadGardParms(bot);

	bot->list_id = bot_id;

	if (USE_GARDEN_AREA)
	{
		int sel = 0;
		int r = rand() % 3;

		if (r == 2)
			sel = 2;
		else if (r == 1)
			sel = 1;
		else
			sel = 0;

		PlaceGardenArea(bot,
						food_pos[sel][0], food_pos[sel][1], food_pos[sel][2]);
	}
	else
	{
		RandomPlacement(bot);
	}

} 

//
// DestroyBot
//
static void DestroyGarden(CURRENT_PTR b)
{
	// free(b);
	RELEASE_OBJECT(b);

} // end of the function

//
// RenderBot
//
static void RenderGarden(CURRENT_PTR boid)
{

	if (boid->state == DEAD_STATE)
		return;

	BEGIN_BOT;

	// Translate then rotate
	glTranslatef(boid->position[0], boid->position[1],
				 boid->position[2]);

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
	// gluSphere(quadric, 0.5f, 18, 8);	// draw sphere for hood
	driver_objects[NORM_CUBE_OBJECT]->render();

	END_BOT;

} 
