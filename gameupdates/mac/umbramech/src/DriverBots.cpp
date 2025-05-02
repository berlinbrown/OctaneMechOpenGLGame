/**
 * Copyright (c) 2006-2025 Berlin Brown.  All Rights Reserved
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
 * Date: 4/2025
 */

#include "DriverBots.hpp"

#include <cstdio>
#include <cstdlib>

DriverBots::DriverBots() {    
}

void DriverBots::moveFireAnt()
{
	int id = 0;
	float *last_heading = NULL;
	float tmp_heading = 0.0f;

	bool col_flag = false;

	this->numSteps++;

	// when to change direction
	if ((this->numSteps % this->straightSteps) == 0)
	{

		this->state = CHANGE_DIR_STATE;

		return; // process state else where

	} // end of the if

	// movement code
	this->x -= (float)sin(this->heading * PI_180) * this->linearv;
	this->y -= (float)cos(this->heading * PI_180) * this->linearv;

	// use the worst algorithm ever, brute force
	// to check for a bot in the path
	if (SearchEvent(bot))
		return;

	// perform collision test using driver
	col_flag = CheckCollisionBot((DriverBotPtr)bot);

	// we have a hit
	if (col_flag)
	{
		// change state
		this->state = CHANGE_DIR_STATE;

		// reset back
		this->x += (float)sin(this->heading * PI_180) * this->linearv;
		this->y += (float)cos(this->heading * PI_180) * this->linearv;

		return;

	} // end of the if

	this->state = MOVE_STATE;

	return;

} 

//
// FindBot
// - return a number of the first bot found
static int FindBot(DriverBotPtr bot)
{

	int index = 0;

	for (index = 0; index < MAX_FIRE_ANTS; index++)
	{
		// I hope these match up
		if (this->id == index)
			continue; // skip yourself

		// if the guy is dead, skip searching
		if (fire_cluster[index]->alive == DEAD_STATE)
			continue;

		if (CheckSight(bot, fire_cluster[index]))
			return index;

	} // end of the for

	return INVALID_BOT;

} 

//
// SearchEvent
// - use another state
// for processing the search events
//
bool DriverBots::SearchEvent(DriverBotPtr bot)
{
	int res;
	float tmp_targ;

	res = FindBot(bot);

	if (res == -1)
		return false;

	this->enemy_id = res; // save for later

	// we found a valid nme
	// Note: for turret rotation, you have to include
	// the heading direction
	tmp_targ = FindAngle(this->heading,
						 this->x, this->y,
						 fire_cluster[res]->x, fire_cluster[res]->y);

	this->target_dir = tmp_targ;

	// this->state = SET_MOUNT_STATE;
	this->state = GO_MOVE_COMMAND;

	return true;

} 

//
// FindCameraPos
//
void DriverBots::FindCameraPos(DriverBotPtr bot)
{
	float tmp_heading;
	float tmp_x, tmp_y;
	float rad;

	//
	// Set the camera lookat and position
	//
	tmp_heading = this->heading + 90.0f;
	if (tmp_heading > 360.0f)
		tmp_heading -= 360.0f;

	rad = tmp_heading / RAD_TO_DEG;

	tmp_x = LOOKAT_OFFSET * (float)cos(rad) * CAMERA->zoom_factor;
	tmp_y = LOOKAT_OFFSET * (float)sin(rad) * CAMERA->zoom_factor;

	tmp_x = tmp_x + this->x;
	tmp_y = (-tmp_y) + this->y;

	this->look_x = tmp_x;
	this->look_y = tmp_y;

	// the camera pos
	tmp_heading = this->heading + 270.0f;
	if (tmp_heading > 360.0f)
		tmp_heading -= 360.0f;

	rad = tmp_heading / RAD_TO_DEG;

	tmp_x = CAMERA_BOT_OFFSET * (float)cos(rad) * CAMERA->zoom_factor;
	tmp_y = CAMERA_BOT_OFFSET * (float)sin(rad) * CAMERA->zoom_factor;

	tmp_x = tmp_x + this->x;
	tmp_y = (-tmp_y) + this->y;

	this->cam_x = tmp_x;
	this->cam_y = tmp_y;

} 

//
// Process FireEvent
//
void DriverBots::wanderCommand(DriverBotPtr bot)
{

	// bot is dead cant do too much
	if (this->alive == DEAD_STATE)
		return;

	FindCameraPos(bot);

	switch (this->state)
	{
	case MOVE_STATE:
		MoveFireAnt(bot);
		break;

	case CHANGE_DIR_STATE:
		ChangeFireDir(bot);
		break;

	case SET_TURN_STATE:
		// set the turning direction
		// a prelim step for making the actual turn
		SetTurnDirection(bot, TURN_STATE);
		break;

	case TURN_STATE:
		// Note: this is where the bot is
		// physically turning

		// turn state is used to set the direction
		TurnAntState(bot, TURN_STATE, MOVE_STATE);

		this->state = GetStartState(WANDER_COMMAND);
		break;

	case GO_MOVE_COMMAND:
		Generate_Command(bot, MOVE_COMMAND);
		break;

	case EXPLODE_STATE:
		SetExplosion(this->x, this->y);
		this->alive = DEAD_STATE;
		this->state = TEMP_STATE;

		break;

	default:
		break;
	}; // end switch

} 

//
// GenerateMove
// - pick a move spot for attacking
// -
// This has some flaws, one: it
// may cross over the nme to get to some
// spot, hmm, not very effective
// so use GenerateBoxMove also
//
void DriverBots::GenerateMove(DriverBotPtr bot, int next_state)
{

	int res;
	float tmp_targ;
	float tmp_x;
	float tmp_y;
	float radius;
	float rand_rad;

	res = this->enemy_id;

	radius = (((float)(rand() % 100) / 20.0f) - 2.5f);
	radius += ATTACK_RADIUS;

	rand_rad = (((float)(rand() % 1000) / 15.0f) - 33.0f);

	this->attack_angle += rand_rad;

	if (this->attack_angle < 0)
		this->attack_angle += 360.0f;
	else if (this->attack_angle > 360)
		this->attack_angle -= 360.0f;

	// generate a point at a circle away
	// from the nme and then turn
	tmp_x = radius * (float)cos(this->attack_angle);
	tmp_y = radius * (float)sin(this->attack_angle);

	tmp_x = fire_cluster[res]->x + tmp_x;
	tmp_y = fire_cluster[res]->y + tmp_y;

	tmp_targ = FindAngle(this->heading,
						 this->x, this->y, tmp_x, tmp_y);

	this->target_dir = tmp_targ;

	tmp_targ = FindAngle(this->heading,
						 this->x, this->y,
						 fire_cluster[res]->x, fire_cluster[res]->y);

	this->target_dir = tmp_targ;

	// we also need the number of moves it takes to
	// get there
	this->target_moves = (int)(radius / this->linearv);

	if (this->target_moves <= 0)
		this->target_moves = 2;

	this->move_index = 0;

	this->state = next_state;

} 

//
// Generate Box Move
// A little bit more effective
//
void DriverBots::GenerateBoxMove(DriverBotPtr bot, int next_state)
{
	int res;
	float tmp_targ;
	float tmp_x;
	float tmp_y;
	float radius;
	float nme_x, nme_y;
	float dx, dy;
	float zx, zy;
	float tol;

	res = this->enemy_id;
	nme_x = this->x;
	nme_y = this->y;

	zx = fire_cluster[res]->x;
	zy = fire_cluster[res]->y;

	// creat a box on the left
	tmp_x = ((float)(rand() % 100) / 30.0f);
	tmp_y = (((float)(rand() % 100) / 20.0f) - 2.5f);

	dx = zx - this->x;
	dy = zy - this->y;

	radius = (float)sqrt((dx * dx) + (dy * dy));
	tol = LINE_OF_SIGHT / 1.9f;

	if ((zx > this->x) ||
		(radius > tol))
		tmp_x = -tmp_x;

	tmp_x = nme_x + tmp_x;
	tmp_y = nme_y + tmp_y;

	dx = tmp_x - this->x;
	dy = tmp_y - this->y;

	radius = (float)sqrt((dx * dx) + (dy * dy));

	tmp_targ = FindAngle(this->heading,
						 this->x, this->y, tmp_x, tmp_y);

	this->target_dir = tmp_targ;

	tmp_targ = FindAngle(this->heading,
						 this->x, this->y,
						 tmp_x, tmp_y);

	this->target_dir = tmp_targ;

	// we also need the number of moves it takes to
	// get there
	this->target_moves = (int)(radius / this->linearv);

	if (this->target_moves <= 0)
		this->target_moves = 2;

	this->move_index = 0;

	this->state = next_state;

} 

//
// Assault, no move just attack
//
// if you want something basic, this is it
// no thrills, just shoot
//
void DriverBots::assaultMove(DriverBotPtr bot, int next_state)
{
	int res;
	float tmp_targ;

	res = this->enemy_id;

	tmp_targ = FindAngle(this->heading,
						 this->x, this->y, fire_cluster[res]->x, fire_cluster[res]->y);

	this->target_dir = tmp_targ;

	tmp_targ = FindAngle(this->heading,
						 this->x, this->y,
						 fire_cluster[res]->x, fire_cluster[res]->y);

	this->target_dir = tmp_targ;

	// we also need the number of moves it takes to
	// get there
	this->target_moves = 0;
	this->move_index = 0;

	this->state = next_state;

} 

//
// Generate_Turn
// - turn towards the nme again
//
void DriverBots::generateTurn(DriverBotPtr bot)
{
	int res;
	float tmp_targ;

	res = this->enemy_id;

	tmp_targ = FindAngle(this->heading,
						 this->x, this->y,
						 fire_cluster[res]->x, fire_cluster[res]->y);

	this->target_dir = tmp_targ;

	this->state = SET_MOUNT_STATE;

} 

//
// Reach Target
// - travel so many moves based
// on target_moves
//
void DriverBots::Reach_Target(DriverBotPtr bot)
{
	CollisionObj *col_ptr;
	int tol_1;
	int tol_2;

	bool col_flag = false;

	// skip if we have zero move target
	if (this->target_moves <= 0)
	{
		this->state = GENERATE_TURN;
		return;
	} // end of the if

	this->x -= (float)sin(this->heading * PI_180) * this->linearv;
	this->y -= (float)cos(this->heading * PI_180) * this->linearv;

	// perform collision test using driver
	col_flag = CheckCollisionBot((DriverBotPtr)bot);

	// we have a hit
	if (col_flag)
	{
		this->x +=
			(float)sin(this->heading * PI_180) * this->linearv;
		this->y +=
			(float)cos(this->heading * PI_180) * this->linearv;

	} // end of the if

	// Now check how many times we have moved
	this->move_index++;

	tol_1 = this->target_moves - 1;
	tol_2 = this->target_moves + 1;

	if ((this->move_index >= tol_1) && (this->move_index <= tol_2))
	{
		// done, now move to next state
		this->state = GENERATE_TURN;
	} // end of the if

} // end of the function

//
// Move Command
//
void DriverBots::moveCommand(DriverBotPtr bot)
{
	int check_rand;

	// bot is dead cant do too much
	if (this->alive == DEAD_STATE)
		return;

	FindCameraPos(bot);

	switch (this->state)
	{
	case GENERATE_STATE:

		check_rand = rand() % 2;

		// This is the heart of the AI

		// find a location to move to
		//

		if (this->food > DYING_STATE)
		{
			if (check_rand == 1)
				Assault_Move(bot, SET_TURN_STATE);
			else
				GenerateBoxMove(bot, SET_TURN_STATE);
		} // end of the if
		else
		{

			// also increase the speed a little bit
			this->linearv *= 1.1f;
			if (this->linearv >= BOT_MAX_SPEED)
				this->linearv = BOT_MAX_SPEED;

			Assault_Move(bot, SET_TURN_STATE);
		} // end of if

		break;

	case SET_TURN_STATE:
		// set the turning direction
		// a prelim step for making the actual turn
		SetTurnDirection(bot, TURN_STATE);
		break;

	case TURN_STATE:
		// Note: this is where the bot is
		// physically turning

		// turn state is used to set the direction
		TurnAntState(bot, TURN_STATE, MOVE_STATE);

		break;

	case MOVE_STATE:
		Reach_Target(bot);
		break;

	case GENERATE_TURN:
		Generate_Turn(bot);
		break;

	case SET_MOUNT_STATE:
		// set the turning direction
		// a prelim step for making the actual turn
		SetTurnDirection(bot, MOUNT_STATE);
		break;

	case MOUNT_STATE:
		// turn towards the nme
		TurnAndMove(bot, MOUNT_STATE, GO_ATTACK_COMMAND);
		break;

	case GO_ATTACK_COMMAND:
		Generate_Command(bot, ATTACK_COMMAND);
		break;

	case EXPLODE_STATE:
		SetExplosion(this->x, this->y);
		this->alive = DEAD_STATE;
		this->state = TEMP_STATE;

		break;

	default:
		break;
	}; // end switch

} 

//
// Rescan_Enemy
// - check for the nme again
//
void Rescan_Enemy(DriverBotPtr bot)
{
	int res;
	float tmp_targ;

	res = FindBot(bot);

	if (res == -1)
	{
		// no enemy, go back to
		// wandering
		this->state = GO_WANDER_COMMAND;

		return;
	} 

	this->enemy_id = res; // save for later

	// otherwise, go into attack state
	tmp_targ = FindAngle(this->heading,
						 this->x, this->y,
						 fire_cluster[res]->x, fire_cluster[res]->y);

	this->target_dir = tmp_targ;

	this->state = SET_MOUNT_STATE;

} 

//
// Attack Command
//
void DriverBots::attackCommand(DriverBotPtr bot)
{

	// bot is dead cant do too much
	if (this->alive == DEAD_STATE)
		return;

	FindCameraPos(bot);

	switch (this->state)
	{

	case RECHECK_STATE:
		// make sure the nme hasnt moved

		Rescan_Enemy(bot);
		break;

	case SET_MOUNT_STATE:
		// set the turning direction
		// a prelim step for making the actual turn
		SetTurnDirection(bot, MOUNT_STATE);
		break;

	case MOUNT_STATE:
		// turn towards the nme
		TurnAndMove(bot, MOUNT_STATE, SHOOT_STATE);
		break;

	case SHOOT_STATE:
		FireBullets(bot, GO_MOVE_COMMAND);
		break;

	case EXPLODE_STATE:
		SetExplosion(this->x, this->y);
		this->alive = DEAD_STATE;
		this->state = TEMP_STATE;

		break;

	case GO_WANDER_COMMAND:
		// no enemy found, shrug
		Generate_Command(bot, WANDER_COMMAND);
		break;

	case GO_MOVE_COMMAND:
		Generate_Command(bot, MOVE_COMMAND);
		break;
	};

} // end of the function
