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
// bot.cpp
//
// - This really should be called
// objects but it was taken up already
//
// basically bots have attributes and
// use objects for drawing
//
// - This is the meat and potatoes of the
// artificial control
//

#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <math.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

#include "camera.h"
#include "bot.h"
#include "objects.h"
#include "gldrawlib.h"
#include "world.h"
#include "globals.h"
#include "octree.h"
#include "plist.h"
#include "collision.h"

void ProcessBotEvent(DriverBotPtr bot);

static DriverBotPtr *bot_cluster;

// created with the bots
PtrList *trail_stack;

//
// Super_LoadBots
// - if this is not loaded
// we have major problems
//
void Super_LoadBots(void)
{
	bot_cluster = (DriverBotPtr *)malloc(MAX_BOTS *
										 sizeof(DriverBotPtr));
} // end of the function

//
// Super_KillBots(
//
void Super_KillBots(void)
{
	RELEASE_OBJECT(bot_cluster);

} // end of the function

//
// Create So many ants
// - you can use a constant or
// some form of percentage based on the total
// ants that are on the grid
// if you use a food constant, the ants will
// die off quicker, if you use a percentage
// then the ants will respawn at random points
// default is 5%, constant is 10,000
//
void CreateAnts(int food)
{
	int i = 0;
	float tmp = 0;
	int food_tol = 0;
	float perct = 0;

#if USE_FOOD_RESPAWN
	food_tol = FOOD_RESPAWN
#else
	perct = 0.05f * (float)GetAnts();

	if (perct > 1)
	{
		food_tol = (int)perct * INITIAL_ANT_FOOD;
	}
	else
	{
		return; // cant create any ants at the time

	} // end of if-else

#endif

		if (food >= food_tol)
	{

		// generate more ants
		// depending how many ants have died
		for (i = 0; i < MAX_BOTS; i++)
		{
			// look for dead slot
			if (bot_cluster[i]->alive == DEAD_STATE)
			{

				// DestroyBot(bot_cluster[i]);
				// bot_cluster[i] = CreateBot(i);
				ResetBot(bot_cluster[i]);

				GetAntFood(bot_cluster[i]);

				AddAnts(1);

				// dont want to over do it
				tmp += INITIAL_ANT_FOOD;
				if (tmp >= food_tol)
					break;

			} // end of the if

		} // end of the for

	} // end of the if

} // end of the functino

//
// Find Angle
//  - FindAngle between two points with -z = -y, x=x
// (depending on the bots direction)
//
float FindAngle(float dir, float x1, float y1, float x2, float y2)
{

	float dx;
	float dy;
	float dist;
	float angle;

	if (x1 < x2)
	{

		dx = x2 - x1;
		dy = (-y2) - (-y1); // reverse direction of y

		dist = (float)sqrt((dx * dx) + (dy * dy));
		angle = (float)asin(dy / dist);

		angle = RAD_TO_DEG * angle;

		if (angle < 0)
			angle += 360.0f;

		// convert to GL cartesian plane, 0 = 90
		// subtract 90 degrees
		angle -= 90.0f;
		if (angle < 0)
			angle += 360.0f;

		return angle;
	}
	else
	{

		dx = x2 - x1;
		dy = y2 - y1; // reverse direction of y

		dist = (float)sqrt((dx * dx) + (dy * dy));
		angle = (float)asin(dy / dist);

		angle = RAD_TO_DEG * angle;

		if (angle < 0)
			angle += 360.0f;

		// convert to GL cartesian plane, 0 = 90
		// subtract 90 degrees
		angle -= 90.0f;
		if (angle < 0)
			angle += 360.0f;

		// step 3: add 180 because bot has to move toward nest
		angle += 180.0f;

		if (angle >= 360)
			angle -= 360;

		return angle;

	} // end if

	return 0;
} // end of the function

//
// Method to draw food on the ant
// - semi-simple
//
void RenderFood(DriverBotPtr bot)
{

	if (bot->foodstore > 0)
	{

		BEGIN_BOT;

		// place over the ant --
		glTranslatef(bot->x, 0.15f, bot->y);

		glScalef(0.1f, 0.1f, 0.1f);

		driver_objects[NORM_CUBE_OBJECT]->render();

		END_BOT;

	} // end of the if

} // end of the function

//
// GetAntFood
// - intially
//
void GetAntFood(DriverBotPtr bot)
{

	float food;
	float tmp;

	// make sure nest's food is loaded
	bot->food = 0;

	food = INITIAL_ANT_FOOD;

	tmp = nest.objects[0]->food - food;
	if (tmp <= 0)
	{
		// find how much food nest can give
		food = nest.objects[0]->food;

		if (food <= 0)
		{
			nest.objects[0]->food;
			bot->food = 0;

			return;
		} // end of if

	} // end of the if

	bot->food += food;

	// detract from nests pile
	nest.objects[0]->food -= food;

} // end of the function

//
// Once the ant can carry food he should be able
// to eat it on his way back to the nest
//
void EatFood(DriverBotPtr bot, float food_rate)
{
	float food_amt;

	if (bot->foodstore > 0)
	{
		food_amt = food_rate;

		if ((bot->foodstore - food_amt) < 0)
		{
			// try to found how much food is left
			food_amt = bot->foodstore;

			bot->foodstore = 0; // thats all

			// add the foodstore to the edible food
			bot->food += food_amt;

			return;

		} // end of the if

		// add the food store to edible food
		bot->foodstore -= food_amt;
		bot->food += food_amt;

	} // end of the if
	else
		bot->foodstore = 0;

} // end of the function

//
// Metabolize
// - burn food
//
void Metabolize(DriverBotPtr bot, float food_rate)
{
	// no point subtracting when dead
	if (bot->alive == DEAD_STATE)
		return;

	bot->food -= food_rate;

	// you are dead
	if (bot->food <= 0)
	{
		bot->alive = DEAD_STATE;

		SubtractAnts(1);
	} // end of the if

} // end of the function

//
// I really like the idea of a simple state machine for
// the ants, a central processing, queue thing
// a little better device than using a bunch of ifs
// - see state.h
//

//
// ChangeDirection
// - should only be called every once in a while
//
void ChangeDirection(DriverBotPtr bot)
{
	if (bot->go_home == true)
	{
		bot->turn_rand = 3;

		bot->straightSteps = (rand() % __go_home_steps_2) + MIN_STRAIGHT_STEPS_2;
	}
	else
	{

		bot->turn_rand = 30;

		// remove if you dont like calling rand
		bot->straightSteps = (rand() % __straight_steps) + MIN_STRAIGHT_STEPS;
	} // end of the if

	bot->target_dir = bot->heading;

	// pick a random direction
	bot->target_dir += rand() % bot->turn_rand; // 30 degrees ok?

	if (bot->target_dir >= 360)
		bot->target_dir -= 360;

	bot->state = TURN_STATE;

} // end of the function

//
// Turn
//
void TurnBot(DriverBotPtr bot)
{

	float tol;
	float tmp;
	float target_tmp;

	// hmm, turning speed //
	tol = bot->turning_speed + (2.0f * bot->turning_speed);

	bot->heading += bot->turning_speed;

	if (bot->heading >= 360)
		bot->heading -= 360;

	tmp = ABS(bot->heading);
	target_tmp = ABS(bot->target_dir);

	// reached target direction
	if ((tmp > (target_tmp - tol)) && (tmp < (target_tmp + tol)))
	{
		// change state
		bot->state = MOVE_STATE;

		return;
	} // end of the if

	bot->state = TURN_STATE;

	return;

} // end of the function

//
// MoveBot
//
void MoveBot(DriverBotPtr bot)
{

	int id = 0;
	float *last_heading = NULL;
	float tmp_heading = 0.0f;

	CollisionPtr col_ptr;

	// if we have our max food move on
	if (bot->foodstore <= MAX_FOOD_RATE)
	{

		id = BruteCheckFood(bot);

		// check if we have found food
		if (id > 0)
		{

			DropFood(bot, (id - 1), INIT_FOOD_RATE);

			// return home with the food
			bot->go_home = true;
			bot->target_dir =
				FindAngle(bot->heading, bot->x, bot->y, 0.0f, 0.0f);

			bot->state = TURN_STATE;

			return;
		} // end of the if

	} // end of the if

	// check if we are ok to deposit food
	if (bot->foodstore > 0)
	{
		// check if in the nest area
		if ((bot->x > -GET_NEST_HWID) &&
			(bot->x < GET_NEST_HWID) &&
			(bot->y > -GET_NEST_HWID) &&
			(bot->y < GET_NEST_HWID))
		{
			NEST_FOOD_OBJECT += bot->foodstore;
			bot->foodstore = 0;
			bot->go_home = false;

			// Since the bot found food
			// add the direction so that other bots
			// can use it
			//
			// Note: test code, may not use in final
			// implementation

			// get the new heading first
			last_heading = (float *)POP_STACK(trail_stack);

			if (trail_stack->items < MAX_TRAIL_STACK)
				PUSH_STACK(trail_stack, (float *)&bot->heading);

			// Once we have enough food
			// create a new ant
			CreateAnts(NEST_FOOD_OBJECT);

#if HUD_NEST_FOOD
			SetNestFood(NEST_FOOD_OBJECT);
#endif

		} // end of the if

	} // end of the if

	bot->numSteps++;

	// when to change direction
	if ((bot->numSteps % bot->straightSteps) == 0)
	{

		bot->state = CHANGE_DIR_STATE;

		return; // process state else where

	} // end of the if

	// Also We need to drop pheromones on the way home
	// Note: pretty slow algo
	if (bot->go_home)
	{
		if ((bot->numSteps % PHEROMONE_DROP) == 0)
		{
			ActivatePheromone(bot->x, bot->y, bot->heading);
		} // end of the if

	} // end of the if

	// If we have a new heading change direction
	if (last_heading)
	{
		tmp_heading = *last_heading;

		tmp_heading += 180.0f;
		if (tmp_heading > 360.0f)
			tmp_heading -= 360.0f;

		// change direction
		bot->target_dir = tmp_heading;
		bot->turn_rand = 15;

		bot->straightSteps = (rand() % __go_home_steps_2) + MIN_STRAIGHT_STEPS_2;

		bot->state = TURN_STATE;

		return;

	} // end of the if

	bot->x -= (float)sin(bot->heading * PI_180) * bot->linearv;
	bot->y -= (float)cos(bot->heading * PI_180) * bot->linearv;

#if 0
	// perform collision test using driver
	col_ptr = CheckCollisionList((DriverBotPtr)bot, FREE_OBJECT);

	// we have a hit
	if (col_ptr != NULL)
	{
		// check for wall first
		if (col_ptr->move_type == STATIC_OBJECT)
		{
			// change state
			bot->state = CHANGE_DIR_STATE;

			// reset back 
			bot->x += (float)sin(bot->heading*PI_180) * bot->linearv;
			bot->y += (float)cos(bot->heading*PI_180) * bot->linearv;

			return;
		} // end of the if

	} // end of the if
#endif

	bot->state = MOVE_STATE;

	return;

} // end of the function

//
// ProcessBotEvent
// - to keep everything in sync,
// process all bot actions in one fell swoop
//
void ProcessBotEvent(DriverBotPtr bot)
{
	// Eat food first then metabolize
	EatFood(bot, MOVE_FOOD_RATE);

	// burn some food
	Metabolize(bot, FOOD_RATE);

	// bot is dead cant do too much
	if (bot->alive == DEAD_STATE)
		return;

	switch (bot->state)
	{
	case MOVE_STATE:
		MoveBot(bot);
		break;

	case CHANGE_DIR_STATE:
		ChangeDirection(bot);
		break;

	case TURN_STATE:
		TurnBot(bot);
		break;

	default:
		break;
	}; // end switch

} // end of the function

//
// LoadBotParms
// - used by create and reset
//
void LoadBotParms(DriverBotPtr bot_ptr)
{
	// I like to be extra careful
	ZeroMemory((DriverBotPtr)bot_ptr,
			   sizeof(DriverBots));

	bot_ptr->heading = (float)(rand() % 360);
	bot_ptr->target_dir = bot_ptr->heading;

	bot_ptr->linearv = BOT_SPEED;

	bot_ptr->size[0] = 0.2f; // scale
	bot_ptr->size[1] = 0.2f;
	bot_ptr->size[2] = 0.2f;

	bot_ptr->color[0] = 0.8f;
	bot_ptr->color[1] = ((float)(rand() % 1000) / 2000.0f) + 0.2f;
	bot_ptr->color[2] = ((float)(rand() % 1000) / 2000.0f) + 0.2f;

	bot_ptr->turning_speed = ((float)(rand() % 1000) / 2000.0f) + MIN_TURN_SPEED;

	bot_ptr->numSteps = 0;
	bot_ptr->straightSteps = (rand() % __straight_steps) + MIN_STRAIGHT_STEPS;

	bot_ptr->state = MOVE_STATE;
	bot_ptr->alive = ALIVE_STATE;

	// for simplicity, we assume nest is at the center
	// start off the bots at that location
	bot_ptr->x = ((rand() % 1000) / 200.0f) - 2.5f;
	bot_ptr->y = ((rand() % 1000) / 200.0f) - 2.5f;

	// food bot is holding
	bot_ptr->foodstore = 0;

	bot_ptr->turn_rand = 30;

	bot_ptr->go_home = false;

	bot_ptr->score = 0.0f;

	bot_ptr->kills = 0;

	//
	// Crosshair object
	//
	// the crosshairs expand from 1.0f
	// to some value when alive
	//
	bot_ptr->crosshair_state = DEAD_STATE;
	bot_ptr->crosshair_scale = 1.0f;

} // end of the function

//
// CreateBot
// - allocate memory for bot
//
DriverBotPtr CreateBot(int bot_id)
{
	DriverBotPtr bot_ptr;

	bot_ptr = (DriverBotPtr)malloc(sizeof(DriverBots));

	LoadBotParms(bot_ptr);

	bot_ptr->id = bot_id;

	return bot_ptr;

} // end of the function

//
// ResetBot
//
//
// ResetBot
// - allocate memory for bot
//
void ResetBot(DriverBotPtr bot_ptr)
{

	float *last_heading = NULL;
	float tmp_heading = 0;

	int bot_id = bot_ptr->id;

	// I like to be extra careful
	ZeroMemory((DriverBotPtr)bot_ptr,
			   sizeof(DriverBots));

	LoadBotParms(bot_ptr);

	bot_ptr->id = bot_id;

	// automatically generate a new heading
	last_heading = (float *)POP_STACK(trail_stack);

	// If we have a new heading change direction
	if (last_heading)
	{
		tmp_heading = *last_heading;

		tmp_heading += 180.0f;
		if (tmp_heading > 360.0f)
			tmp_heading -= 360.0f;

		// change direction
		bot_ptr->target_dir = tmp_heading;
		bot_ptr->heading = tmp_heading;
		bot_ptr->turn_rand = 15;

		bot_ptr->straightSteps = (rand() % __go_home_steps_2) + MIN_STRAIGHT_STEPS_2;

	} // end of the if

} // end of the function

//
// DestroyBot
//
void DestroyBot(DriverBotPtr b)
{
	RELEASE_OBJECT(b);

} // end of the functino

//
// RenderBot
//
void RenderBot(DriverBotPtr boid)
{

	if (boid->alive == ALIVE_STATE)
	{

		// If there is food, draw that also
		RenderFood(boid);

		BEGIN_BOT;

		// Translate then rotate
		glTranslatef(boid->x, 0, boid->y);

		// rotate based on the ship struct
		glRotatef(boid->heading, 0.0f, 1.0f, 0.0f);

		// Scale accordingly
		glScalef(boid->size[0], boid->size[1], boid->size[2]);

		// This may or may not change the color
		glColor3f(boid->color[0], boid->color[1], boid->color[2]);

		// draw the object to screen
		driver_objects[ANT_OBJECT]->render();

		END_BOT;

	} // end of the if

} // end of the function

//
// GenerateBots
//
void GenerateBots(void)
{
	int index = 0;

	for (index = 0; index < MAX_BOTS; index++)
	{
		bot_cluster[index] = CreateBot(index);

	} // end of the for

	// create a stack for adding pheromone trails
	trail_stack = CREATE_STACK;

} // end of the function

//
// InitFood
// - called after the nest has been loaded
//
void InitFood(void)
{
	int index = 0;

	for (index = 0; index < MAX_BOTS; index++)
	{
		GetAntFood(bot_cluster[index]);

	} // end of the for

} // end of the function

//
// ShutdownBots
//
void ShutdownBots(void)
{
	int index = 0;

	for (index = 0; index < MAX_BOTS; index++)
	{
		DestroyBot(bot_cluster[index]);
	} // end of the for

	// destroy the pheromone trail stack
	DESTROY_STACK(trail_stack);

} // end of the function

//
// Draw Bots
//
void DrawBots(void)
{
	int index = 0;

	for (index = 0; index < MAX_BOTS; index++)
	{
		ProcessBotEvent(bot_cluster[index]);

		RenderBot(bot_cluster[index]);

	}

} // end of the function
