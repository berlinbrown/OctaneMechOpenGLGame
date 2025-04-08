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
// fireants.cpp
//
// - These ants filter out other ants that arent
//
// carrying any food for a long time
//
// - may also incorporate some messaging techniques
//
//
// The code for the ai is also here hidden
// amongst the other stuff
//
// ant.cpp
//  - the ant object
//
#include <stdio.h>
#include <stdlib.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

#include <math.h>

#include "gldrawlib.h"
#include "objects.h"
#include "lights.h"
#include "bot.h"
#include "camera.h"
#include "world.h"
#include "particles.h"
#include "collision.h"
#include "globals.h"
#include "camera.h"
#include "fireants.h"
#include "menu.h"
#include "sound.h"

#include "network/include/msg.h"
#include "network/network.h"
#include "network/include/connect.h"

#undef CURRENT_OBJECT
#define CURRENT_OBJECT fireant

#define DEATH_WAIT_COUNT 120

static void init_fireant(int list_id);
static void compile_fireant(void);
static void draw_fireant(void);
static void render_fireant(void);
static void draw_fireant(void);

//
// For the bot
//
GLfloat red_ambient[] = {0.6f, 0.6f, 0.6f, 1.0f};
GLfloat red_diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat red_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat rno_shininess[] = {0.0f};
GLfloat rlow_shininess[] = {5.0f};
GLfloat rhigh_shininess[] = {100.0f};
GLfloat rmat_emission[] = {0.3f, 0.3f, 0.3f, 0.0f};

//
// For the nme bot
//
GLfloat blue_ambient[] = {0.0f, 0.0f, 0.7f, 1.0f};
GLfloat blue_diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat blue_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat bno_shininess[] = {0.0f};
GLfloat blow_shininess[] = {5.0f};
GLfloat bhigh_shininess[] = {100.0f};
GLfloat bmat_emission[] = {0.0f, 0.1f, 0.3f, 0.0f};

//
// For the cannon
//
GLfloat color_ambient[] = {0.4f, 0.4f, 0.4f, 1.0f};
GLfloat color_diffuse[] = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat color_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat yno_shininess[] = {0.0f};
GLfloat ylow_shininess[] = {5.0f};
GLfloat yhigh_shininess[] = {100.0f};
GLfloat ymat_emission[] = {0.1f, 0.1f, 0.1f, 0.0f};

static int FindBot(DriverBotPtr bot);
static bool SearchEvent(DriverBotPtr bot);

void ResetBullet(StaticBotPtr boid);
void Player_Shoot(DriverBotPtr bot);

#define MAX_NET_BOTS1 32
static int net_bot_ids[MAX_NET_BOTS1];
static int net_bot_index = 0;

#define MAX_RAND_POS 3
float rand_bot_pos[MAX_RAND_POS][4] = {
	{60, -110, (WORLD_X_MAX + 177), 177}, // 1
	{-150, 120, 104, 200},				  // 2
	{130, 180, 320, 160}};				  // 3

//
// Create a fire ant struct
//
static DriverBotPtr *fire_cluster;

//
// - used with networking,
// the network bots need some kind of conversion tool
// to convert from a network id to a local id
//
// Convert_SetID
void Convert_SetID(int val)
{
	net_bot_ids[net_bot_index++] = val;

} // end of the function

int Convert_GetID(int msg_id)
{
	int i;

	for (i = 0; i < net_bot_index; i++)
	{
		if (msg_id == net_bot_ids[i])
		{
			return i;
		} // end of the if

	} // en dof the fo r

	return 0; // cheating!

} // end of the function

//
// Note: if this code doesnt work
// the next best thing is to test against
// the objects at the beginning to
// see if a bot landed in any of them
// this way is more complete
//
// We want to keep the bots
// in a specified region in order to keep
// the bots from being placed in buildings
//
void BotPlacement(DriverBotPtr bot, float x, float y,
				  float width, float height)
{
	float x_min;
	float y_min;
	float h_width;
	float h_height;
	int rand_num = 0;
	float res;

	float tmp;
	int rand_arg;
	float rand_arg_1, rand_arg_2;

	// place between the min and max randomly
	h_width = width / 2.0f;
	h_height = height / 2.0f;
	y_min = y - h_width;
	x_min = x - h_width;

	// we need a random number for the max
	rand_num = (int)(width * 1000.0f);
	res = (float)(rand() % rand_num) / 1000.0f;

	bot->x = x_min + res;

	res = (float)(rand() % rand_num) / 1000.0f;
	bot->y = y_min + res;

	//
	// code to fix boundary issues
	//
	tmp = WORLD_X_MAX * 0.08f;

	// get new value --
	if ((bot->x < -tmp) || (bot->x > tmp) ||
		(bot->y < -tmp) || (bot->y > tmp))
	{

		rand_arg_1 = WORLD_X_MAX - tmp;
		rand_arg_2 = rand_arg_1 / 2.0f;

		// get the int component
		rand_arg = (int)(rand_arg_2 * rand_arg_1 * 2.0f);

		// place the fire ants a little farther away
		bot->x = ((rand() % rand_arg) / rand_arg_1) - rand_arg_2;
		bot->y = ((rand() % rand_arg) / rand_arg_1) - rand_arg_2;

	} // end of the if

} // end of the function

//
// PositionBot
// - set the bots initial position
//
void PositionBot(DriverBotPtr bot)
{
	int rand_no;

	rand_no = rand() % MAX_RAND_POS;

	BotPlacement(bot,
				 rand_bot_pos[rand_no][0],
				 rand_bot_pos[rand_no][1],
				 rand_bot_pos[rand_no][2],
				 rand_bot_pos[rand_no][3]);

} // end of the function

//
// Supers
//
void Super_FireAnts(void)
{
	fire_cluster = (DriverBotPtr *)malloc(MAX_FIRE_ANTS *
										  sizeof(DriverBotPtr));
} // end of the function

//
// Super_KillFires
//
void Super_KillFires(void)
{
	// ifree(fire_cluster);
	RELEASE_OBJECT(fire_cluster);
} // end of the function

//
// Render_ShootLine
//
void Render_ShootLine(float x_1, float y_1,
					  float x_2, float y_2)
{
	float height = 2.0f;

	glDisable(GL_LIGHTING);

	MED_RED;

	glBegin(GL_LINE_LOOP);

	glVertex3f(x_1, height, y_1); // left bottom
	glVertex3f(x_2, height, y_2); // top left

	glEnd();

	glEnable(GL_LIGHTING);

} // end of the function

//
// simple objects library
// - make sure to change the number of objects
// in objects.h
//
DriverObjects CURRENT_OBJECT =
	{
		init_fireant,	 // init, must be called first
		compile_fireant, // compile
		draw_fireant,	 // draw
		render_fireant,	 // render to scene
		0				 // loaded by INIT
};

//
//
void Player_Control(bool *keys)
{
	// cant move when dead
	if (fire_cluster[0]->alive == DEAD_STATE)
		return;

	//
	// Rotate the camera according to
	// 'S' and 'F'
	if ((keys[keyCodes[VK_f]]) || (keys[keyCodes[VK_F]]))
	{
		AngleCamera(0.0f, 2.0f, 0.0f);

	} // end of the if

	// s
	if ((keys[keyCodes[VK_S]]) || (keys[keyCodes[VK_s]]))
	{
		AngleCamera(0.0f, -2.0f, 0.0f);

	} // end of the if

	// R  to reset
	if ((keys[keyCodes[VK_R]]) || (keys[keyCodes[VK_r]]))
	{
		// cheap trick --
		CAMERA->rotation[1] = OFFSET_ROTATION;

	} // end of the if

	//
	//  Control the robot
	//
	if (keys[keyCodes[VK_UP]])
	{
		MoveFire0(1, 0);

		// keys[keyCodes[VK_UP]] = false;

	} // end of the if

	if (keys[keyCodes[VK_DOWN]])
	{
		MoveFire0(-1, 0);

		// keys[keyCodes[VK_DOWN]] = false;

	} // end of the if

	if (keys[keyCodes[VK_LEFT]])
	{
		MoveFire0(0, 1);

		// keys[keyCodes[VK_LEFT]] = false;

	} // end of the if

	if (keys[keyCodes[VK_RIGHT]])
	{
		MoveFire0(0, -1);

		// keys[keyCodes[VK_RIGHT]] = false;
	} // end of the if

	if (keys[keyCodes[VK_SPACE]])
	{

		Player_Shoot(fire_cluster[0]);

		// keys[keyCodes[VK_SPACE]] = false;
	} // end of the if

} // end of the function

//
// Handle_Player
// - do any other processing
// like handle explosions
//
void Handle_Player(DriverBotPtr bot)
{
	float rad;
	float tmp_heading;
	float tmp_x;
	float tmp_y;
	float ang;
	int i;

	SetPlayerHealth(bot->food);

	// another trick to get the camera to wait
	// before switching bots
	if (bot->state == TEMP_STATE)
	{

		// increase a little bit
		CAMERA->zoom_factor *= 1.05f;
		if (CAMERA->zoom_factor > MAX_ZOOM)
			CAMERA->zoom_factor = MAX_ZOOM;

		// another little trick
		// to make sure we see the
		// explosion
		if (bot->numSteps++ > DEATH_WAIT_COUNT)
		{
			CAMERA->zoom_factor = CAMERA->old_zoom;
			bot->state = GO_WANDER_COMMAND;

			// At this point the bot is dead,
			// dead, dead
			// place the death sign
			ant_globals->_menu_state = MENU_DEAD_MODE;

			Reset_DeadText();

		} // end of the if

	} // end of the if

	if ((bot->alive == DEAD_STATE) &&
		(bot->state == GO_WANDER_COMMAND))
	{

		// set a new camera bot
		for (i = 0; i < MAX_FIRE_ANTS; i++)
		{

			GetCameraBot(fire_cluster[i]);

			if (fire_cluster[i]->alive == ALIVE_STATE)
				break;

		} // end of the for

	} // end of the if

	if (bot->alive == DEAD_STATE)
		return;

	// draw a player indicator
	//
	Bot_Triangle(bot->x, bot->y);

	if (bot->view_mode == THIRD_PERSON_MODE)
	{

		// nOTE: // THIRD PERSON MODE!!!

		//
		// calculate the lookat point
		//
		ang = CAMERA->rotation[1] + bot->heading;

		// now the next point of the triangle
		// shift by 45 degrees
		tmp_heading = ang + 90.0f;
		if (tmp_heading > 360.0f)
			tmp_heading -= 360.0f;

		rad = tmp_heading / RAD_TO_DEG;

		// also include the zoom
		tmp_x = LOOKAT_OFFSET * (float)cos(rad) * CAMERA->zoom_factor;
		tmp_y = LOOKAT_OFFSET * (float)sin(rad) * CAMERA->zoom_factor;

		tmp_x = tmp_x + bot->x;
		tmp_y = (-tmp_y) + bot->y;

		bot->look_x = tmp_x;
		bot->look_y = tmp_y;

		// get the camera position now

		tmp_heading = ang + 270.0f;
		if (tmp_heading > 360.0f)
			tmp_heading -= 360.0f;

		rad = tmp_heading / RAD_TO_DEG;

		tmp_x = CAMERA_BOT_OFFSET * (float)cos(rad) * CAMERA->zoom_factor;
		tmp_y = CAMERA_BOT_OFFSET * (float)sin(rad) * CAMERA->zoom_factor;

		tmp_x = tmp_x + bot->x;
		tmp_y = (-tmp_y) + bot->y;

		bot->cam_x = tmp_x;
		bot->cam_y = tmp_y;
	}
	else if (bot->view_mode == FIRST_PERSON_MODE)
	{

		// In first person the look at is directed
		// a little higher and a little farther

		// offset the camera a little bit
		// in order to see the bullet
		ang = bot->heading + 5.0f;

		// now the next point of the triangle
		// shift by 45 degrees
		tmp_heading = ang + 90.0f;
		if (tmp_heading > 360.0f)
			tmp_heading -= 360.0f;

		rad = tmp_heading / RAD_TO_DEG;

		// also include the zoom
		tmp_x = PERSPECTIVE_Z * (float)cos(rad);
		tmp_y = PERSPECTIVE_Z * (float)sin(rad);

		tmp_x = tmp_x + bot->x;
		tmp_y = (-tmp_y) + bot->y;

		bot->look_x = tmp_x;
		bot->look_y = tmp_y;

		// get the camera position now

		tmp_heading = ang + 270.0f;
		if (tmp_heading > 360.0f)
			tmp_heading -= 360.0f;

		rad = tmp_heading / RAD_TO_DEG;

		// get that first person look
		//
		tmp_x = -FIRST_PERSON_Z * (float)cos(rad);
		tmp_y = -FIRST_PERSON_Z * (float)sin(rad);

		tmp_x = tmp_x + bot->x;
		tmp_y = (-tmp_y) + bot->y;

		bot->cam_x = tmp_x;
		bot->cam_y = tmp_y;

		// end first person mode

	} // end of if else

	switch (bot->state)
	{
	case EXPLODE_STATE:
		SetExplosion(bot->x, bot->y);
		bot->alive = DEAD_STATE;
		bot->state = TEMP_STATE;

		// save zoom factor for later use
		CAMERA->old_zoom = CAMERA->zoom_factor;

		bot->numSteps = 0;

		break;
	};

} // end of the function

//
// - handle the net player
//
// the only big difference, is the client
// will send packets to the server
//
void Net_Player(DriverBotPtr bot)
{
	float rad;
	float tmp_heading;
	float tmp_x;
	float tmp_y;
	float ang;
	int i;

	SetPlayerHealth(bot->food);

	// another trick to get the camera to wait
	// before switching bots
	if (bot->state == TEMP_STATE)
	{

		// increase a little bit
		CAMERA->zoom_factor *= 1.05f;
		if (CAMERA->zoom_factor > MAX_ZOOM)
			CAMERA->zoom_factor = MAX_ZOOM;

		// another little trick
		// to make sure we see the
		// explosion
		if (bot->numSteps++ > DEATH_WAIT_COUNT)
		{
			CAMERA->zoom_factor = CAMERA->old_zoom;
			bot->state = GO_WANDER_COMMAND;

			// At this point the bot is dead,
			// dead, dead
			// place the death sign
			ant_globals->_menu_state = MENU_DEAD_MODE;

			Reset_DeadText();

		} // end of the if

	} // end of the if

	if ((bot->alive == DEAD_STATE) &&
		(bot->state == GO_WANDER_COMMAND))
	{

		// set a new camera bot
		for (i = 0; i < MAX_NETWORK_BOTS; i++)
		{

			GetCameraBot(fire_cluster[i]);

			if (fire_cluster[i]->alive == ALIVE_STATE)
				break;

		} // end of the for

	} // end of the if

	if (bot->alive == DEAD_STATE)
		return;

	// draw a player indicator
	//
	Bot_Triangle(bot->x, bot->y);

	if (bot->view_mode == THIRD_PERSON_MODE)
	{

		// nOTE: // THIRD PERSON MODE!!!

		//
		// calculate the lookat point
		//
		ang = CAMERA->rotation[1] + bot->heading;

		// now the next point of the triangle
		// shift by 45 degrees
		tmp_heading = ang + 90.0f;
		if (tmp_heading > 360.0f)
			tmp_heading -= 360.0f;

		rad = tmp_heading / RAD_TO_DEG;

		// also include the zoom
		tmp_x = LOOKAT_OFFSET * (float)cos(rad) * CAMERA->zoom_factor;
		tmp_y = LOOKAT_OFFSET * (float)sin(rad) * CAMERA->zoom_factor;

		tmp_x = tmp_x + bot->x;
		tmp_y = (-tmp_y) + bot->y;

		bot->look_x = tmp_x;
		bot->look_y = tmp_y;

		// get the camera position now

		tmp_heading = ang + 270.0f;
		if (tmp_heading > 360.0f)
			tmp_heading -= 360.0f;

		rad = tmp_heading / RAD_TO_DEG;

		tmp_x = CAMERA_BOT_OFFSET * (float)cos(rad) * CAMERA->zoom_factor;
		tmp_y = CAMERA_BOT_OFFSET * (float)sin(rad) * CAMERA->zoom_factor;

		tmp_x = tmp_x + bot->x;
		tmp_y = (-tmp_y) + bot->y;

		bot->cam_x = tmp_x;
		bot->cam_y = tmp_y;
	}
	else if (bot->view_mode == FIRST_PERSON_MODE)
	{

		// In first person the look at is directed
		// a little higher and a little farther

		// offset the camera a little bit
		// in order to see the bullet
		ang = bot->heading + 5.0f;

		// now the next point of the triangle
		// shift by 45 degrees
		tmp_heading = ang + 90.0f;
		if (tmp_heading > 360.0f)
			tmp_heading -= 360.0f;

		rad = tmp_heading / RAD_TO_DEG;

		// also include the zoom
		tmp_x = PERSPECTIVE_Z * (float)cos(rad);
		tmp_y = PERSPECTIVE_Z * (float)sin(rad);

		tmp_x = tmp_x + bot->x;
		tmp_y = (-tmp_y) + bot->y;

		bot->look_x = tmp_x;
		bot->look_y = tmp_y;

		// get the camera position now

		tmp_heading = ang + 270.0f;
		if (tmp_heading > 360.0f)
			tmp_heading -= 360.0f;

		rad = tmp_heading / RAD_TO_DEG;

		// get that first person look
		//
		tmp_x = -FIRST_PERSON_Z * (float)cos(rad);
		tmp_y = -FIRST_PERSON_Z * (float)sin(rad);

		tmp_x = tmp_x + bot->x;
		tmp_y = (-tmp_y) + bot->y;

		bot->cam_x = tmp_x;
		bot->cam_y = tmp_y;

		// end first person mode

	} // end of if else

	switch (bot->state)
	{
	case EXPLODE_STATE:
		SetExplosion(bot->x, bot->y);
		bot->alive = DEAD_STATE;
		bot->state = TEMP_STATE;

		// save zoom factor for later use
		CAMERA->old_zoom = CAMERA->zoom_factor;

		bot->numSteps = 0;

		break;
	};

} // end of the function

//
// Reset_CrossHairs
// - reset the crosshairs on fire
//
void Reset_CrossHairs(DriverBotPtr bot)
{
	// can only change when the hairs are dead
	if (bot->crosshair_state == ALIVE_STATE)
		return;
	else
	{

		bot->crosshair_state = ALIVE_STATE;
		bot->crosshair_scale = 1.0f;

	} // end of if - else

} // end of the function

//
// We need to use the Bot driver structure in order
// to create, render and destroy the fire ants
//=========================================================

//
// DeductHit
//
static bool DeductHit(DriverBotPtr bot, float hit)
{
	bot->food -= hit;

	if (bot->food < 0)
	{

		// have to explode first
		bot->state = EXPLODE_STATE;

		return true;
	} // end of the if

	// set an explostion at this point

	return false;
} // end of the function

//
// Check Collisions
// - This needs improvement,
// brute force for checking the collions on the other
// bots
//
void CheckCollisions(StaticBotPtr boid)
{
	int index = 0;
	float x, y;
	float dx, dy;
	float rand_hit;

	float score = 0.0f;

	bool b_res;
	float length;

	CollisionPtr col_ptr = NULL;

	x = boid->position[0];
	y = boid->position[2];

	// perform collision test using driver
	col_ptr = CheckCollisionMoving((StaticBotPtr)boid);

	// we have a hit
	if (col_ptr != NULL)
	{

		// check for wall first
		if (col_ptr->movement_type == MOVING_COL_TYPE)
		{

			dx = ABS(boid->virt_x) - ABS(x);
			dy = ABS(boid->virt_y) - ABS(y);

			// based on the length
			// decrease the impact of the bullet
			length = (float)sqrt((dx * dx) + (dy * dy));

			// impact is actually food in this case
			length *= 0.9f; // this helps for cheating
			boid->food = boid->linearv / length;

			// we negate close-up shots, sorry!
			if (boid->food > 0.75)
				boid->food *= 0.65; // drop to 60%

			// food is really sall at this
			// point, 0.04->0.03 etc
			boid->food *= BULLET_DAMAGE;

			// at least 70%
			rand_hit = ((float)(rand() % 1000) / 3000.0f) + 0.7f;

			boid->food *= rand_hit;

			// deduct the hit from the bot
			b_res = DeductHit(col_ptr->bot_ptr, boid->food);

			//**
			// This is probably the most
			// immediate way to send a I got
			// hit command
			// But it is not that intuitive
			// more of a trick that cheats
			// our interface, but for now it works

			// who shot me?
			col_ptr->bot_ptr->enemy_id = boid->owner;

			score = boid->food * SCORE_NORMAL;
			if (col_ptr->bot_ptr->food < 0)
			{
				score = boid->food * SCORE_KILL;

				// got a kill
				fire_cluster[boid->owner]->kills++;
			} // end of the if

			fire_cluster[boid->owner]->score += score;

			// switch to new command
			if (col_ptr->bot_ptr->command == WANDER_COMMAND)
				Generate_Command(col_ptr->bot_ptr, MOVE_COMMAND);
			//**

			x = col_ptr->collision_x;
			y = col_ptr->collision_y;
			SetExplosion(x, y);

			boid->state = DEAD_STATE;

			// another extra
			// clip the bullet so that it stops
			// at the place of impact, this may
			// or may not working depending on
			// several things
			boid->old_x = x;
			boid->old_y = y;

			return;

		} // end of the if

	} // end of if

} // end of the function

//
// MoveBullets
//
void MoveBullets(StaticBotPtr boid)
{

	float x, y;
	float v;
	float d_h;

	if (boid->state != ALIVE_STATE)
		return;

	x = (float)sin(boid->virt_heading * PI_180) * boid->linearv;
	y = (float)cos(boid->virt_heading * PI_180) * boid->linearv;

	// by using the rotation of the turret
	// we create problems for finding the actual
	// position of the bullet
	boid->position[0] -= x;
	boid->position[2] -= y;

	// NOTE: check the collisions with bots first!!
	// check collisions with bots
	CheckCollisions(boid);

	// before the move draw
	// a line strip
	boid->old_x = boid->position[0] - (BULLET_LEN * x);
	boid->old_y = boid->position[2] - (BULLET_LEN * y);

	// boid->travel+=boid->linearv;
	boid->travel += boid->linearv;

	//
	// by default kill the bullet in case somebody
	// else doesnt turn it off
	if (boid->travel > MAX_BULLET_TRAVEL)
		boid->state = DEAD_STATE;

	if (boid->max_dist == 0)
	{
		boid->max_dist = 0.000001f;
	} // end of the if

	v = boid->travel / boid->max_dist;

	//
	// select different values for
	// the percent traveled
	// in the end, it really doenst matter
	if (v > 0.82f)
	{
		// kill the bullet
		SetExplosion(boid->final_x, boid->final_y);
		boid->state = DEAD_STATE;

		boid->old_x = boid->final_x;
		boid->old_y = boid->final_y;

	} // end of the if

} // end of the function

//
// ResetBullets
//
void ResetBullets(DriverBotPtr bot)
{
	int index = 0;

	bot->gun_reset = 0; // delay befor firing
	bot->gun_index = 0;

	for (index = 0; index < MAX_BULLETS; index++)
	{
		bot->bullets[index].state = READY_STATE;

		bot->bullets[index].position[0] = bot->x;
		bot->bullets[index].position[2] = bot->y;

	} // end of the for

} // end of the function

//
// FireBullets for the player
//
void Player_Shoot(DriverBotPtr bot)
{
	CollisionPtr col_ptr = NULL;

	int gun_index = bot->gun_index;
	float rand_rot = 0.0f;

	if (bot->alive == DEAD_STATE)
		return; // cant shoot if dead

	// delay for rapid fire
	if (bot->gun_reset++ > MAX_FIRE_SPEED)
	{
		bot->gun_reset = 0;

	} // end of the if

	if (bot->gun_reset != 0)
		return;

	// turns a bullet on
	if (bot->bullets[gun_index].state == READY_STATE)
	{

		Do_FireSound();

		// fire the crosshairs also
		// also add the crosshair
		Reset_CrossHairs(bot);

		bot->bullets[gun_index].state = ALIVE_STATE;

		rand_rot = (float)((rand() % 1000) / 100.0f) - 5.0f;

		// save the virtual settings
		bot->bullets[gun_index].virt_heading = bot->heading;
		bot->bullets[gun_index].virt_x = bot->x;
		bot->bullets[gun_index].virt_y = bot->y;
		bot->bullets[gun_index].owner = bot->id;

		// also set the initial position
		bot->bullets[gun_index].position[0] = bot->x;
		bot->bullets[gun_index].position[2] = bot->y;

		bot->bullets[gun_index].linearv =
			(float)(rand() % 1000) / 5000.0f + MIN_BULLET_SPEED;

		bot->bullets[gun_index].travel = 0;

		// with our new algorithm
		// for collision detection
		// we should check for collision once
		// at bullet launch
		col_ptr = CheckCollisionList((StaticBotPtr)&bot->bullets[gun_index],
									 RAY_COL_TYPE);

		if (col_ptr != NULL)
		{
			bot->bullets[gun_index].final_x =
				col_ptr->collision_x;
			bot->bullets[gun_index].final_y =
				col_ptr->collision_y;

			bot->bullets[gun_index].max_dist =
				col_ptr->dist;

		} // end of the if

	} // end of the if

	// keep track of bullets fired
	bot->gun_index++;
	if (bot->gun_index >= MAX_BULLETS)
	{
		bot->gun_index = 0;

	} // end of the if

} // end of the function

//
// FireBullets
//
void FireBullets(DriverBotPtr bot, int next_state)
{
	int gun_index = bot->gun_index;
	float rand_rot = 0.0f;

	CollisionPtr col_ptr;

	if (bot->alive == DEAD_STATE)
		return; // cant shoot if dead

	// delay for rapid fire
	if (bot->gun_reset++ > MAX_FIRE_SPEED)
	{
		bot->gun_reset = 0;

	} // end of the if

	if (bot->gun_reset != 0)
		return;

	// turns a bullet on
	if (bot->bullets[gun_index].state == READY_STATE)
	{

		Do_FireSound();

		// also add the crosshair
		Reset_CrossHairs(bot);

		bot->bullets[gun_index].state = ALIVE_STATE;

		rand_rot = (float)((rand() % 1000) / 100.0f) - 5.0f;

		// save the virtual settings
		bot->bullets[gun_index].virt_heading = bot->heading;
		bot->bullets[gun_index].virt_x = bot->x;
		bot->bullets[gun_index].virt_y = bot->y;
		bot->bullets[gun_index].owner = bot->id;

		// also set the initial position
		bot->bullets[gun_index].position[0] = bot->x;
		bot->bullets[gun_index].position[2] = bot->y;

		bot->bullets[gun_index].linearv =
			(float)(rand() % 1000) / 5000.0f + MIN_BULLET_SPEED;

		bot->bullets[gun_index].travel = 0;

		// with our new algorithm
		// for collision detection
		// we should check for collision once
		// at bullet launch
		col_ptr = CheckCollisionList((StaticBotPtr)&bot->bullets[gun_index],
									 RAY_COL_TYPE);

		if (col_ptr != NULL)
		{
			bot->bullets[gun_index].final_x =
				col_ptr->collision_x;
			bot->bullets[gun_index].final_y =
				col_ptr->collision_y;

			bot->bullets[gun_index].max_dist =
				col_ptr->dist;

		} // end of the if

	} // end of the if

	// keep track of bullets fired
	bot->gun_index++;
	if (bot->gun_index >= MAX_BULLETS)
	{
		bot->gun_index = 0;

	} // end of the if

	// fire 2 shots and then return to normal
	if ((bot->gun_index % 2) == 0)
	{
		// This may hurt us
		// we are if Attack state
		bot->state = next_state;

	} // end of the if

} // end of the function

//
// Render_LineStrip
//
void Render_LineStrip(float x_1, float y_1, float x_2, float y_2,
					  float height1, float height2)
{
	float v[3][3] = {0};

	glBegin(GL_TRIANGLES);

	// left bottom front
	v[0][0] = x_1;
	v[0][1] = height1;
	v[0][2] = y_1;

	v[1][0] = x_2;
	v[1][1] = height1;
	v[1][2] = y_2;

	v[2][0] = x_2;
	v[2][1] = height2;
	v[2][2] = y_2;

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// finish the strip
	v[0][0] = x_1;
	v[0][1] = height1;
	v[0][2] = y_1;

	v[1][0] = x_1;
	v[1][1] = height2;
	v[1][2] = y_1;

	v[2][0] = x_2;
	v[2][1] = height2;
	v[2][2] = y_2;

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	glEnd();

} // end of the functino

//
// DrawBullets
//
void DrawBullets(StaticBotPtr boid)
{

	MoveBullets(boid);

	BEGIN_BOT;

	glDisable(GL_LIGHTING);
	MED_GREEN;

	// Only draw a ball for right now
	// glTranslatef(boid->position[0], 0.1f, boid->position[2]);
	// gluSphere(quadric, 0.08f, 18, 8);
	Render_LineStrip(boid->position[0], boid->position[2],
					 boid->old_x, boid->old_y,
					 BULLET_H1, BULLET_H2);

	glEnable(GL_LIGHTING);

	END_BOT;

} // end of the function

//
// RenderBullets, DrawBullets, MoveBullets, LoadBullets
//
void RenderBullets(DriverBotPtr bot)
{
	int index = 0;

	for (index = 0; index < MAX_BULLETS; index++)
	{
		if (bot->bullets[index].state == ALIVE_STATE)
		{
			DrawBullets(&bot->bullets[index]);

		} // end of the if

		// now check for ready state
		// in order to reset
		if (bot->bullets[index].state == DEAD_STATE)
		{
			bot->bullets[index].state = READY_STATE;

			bot->bullets[index].position[0] = bot->x;
			bot->bullets[index].position[2] = bot->y;

		} // end of the if

	} // end of the for

} // end of the function

//
// ChangeDirection
// - should only be called every once in a while
//
static void ChangeFireDir(DriverBotPtr bot)
{

	bot->target_dir = bot->heading;

	// pick a random direction
	bot->target_dir += rand() % bot->turn_rand; // 30 degrees ok?

	if (bot->target_dir >= 360)
		bot->target_dir -= 360;

	bot->state = SET_TURN_STATE;

} // end of the function

//
// Turn
//  and Move at the same time
//
static void TurnAndMove(DriverBotPtr bot,
						int old_state, int new_state)
{

	float tol;
	float tmp;
	float target_tmp;

	bool col_flag = false;

	// hmm, turning speed //
	tol = bot->turning_speed + (2.0f * bot->turning_speed);

	// if course if your goal is to turn right
	if (bot->turn_direction == TURN_LEFT)
	{
		bot->heading += bot->turning_speed;

		if (bot->heading >= 360)
			bot->heading -= 360.0f;
	}
	else
	{
		bot->heading -= bot->turning_speed;

		if (bot->heading < 0)
			bot->heading += 360.0f;

	} // end of the if-else

	// move and turn at same time --
	if (bot->move_back)
	{
		bot->x += (float)sin(bot->heading * PI_180) * bot->linearv;
		bot->y += (float)cos(bot->heading * PI_180) * bot->linearv;
	}
	else
	{
		bot->x -= (float)sin(bot->heading * PI_180) * bot->linearv;
		bot->y -= (float)cos(bot->heading * PI_180) * bot->linearv;
	} // end of the if=else

	// perform collision test using driver
	col_flag = CheckCollisionBot((DriverBotPtr)bot);

	// we have a hit
	if (col_flag)
	{
		// reset back
		if (bot->move_back)
		{
			bot->x -=
				(float)sin(bot->heading * PI_180) * bot->linearv;
			bot->y -=
				(float)cos(bot->heading * PI_180) * bot->linearv;
		}
		else
		{
			bot->x +=
				(float)sin(bot->heading * PI_180) * bot->linearv;
			bot->y +=
				(float)cos(bot->heading * PI_180) * bot->linearv;
		} // end if-else

		return;

	} // end of the if

	// end move code --

	tmp = ABS(bot->heading);
	target_tmp = ABS(bot->target_dir);

	// reached target direction
	if ((tmp > (target_tmp - tol)) && (tmp < (target_tmp + tol)))
	{
		// change state
		bot->state = new_state;

		return;
	} // end of the if

	bot->state = old_state;

	return;

} // end of the function

//
// Turn
// - same as turn fire ant
// except that you can change the state
//
static void TurnAntState(DriverBotPtr bot,
						 int old_state, int new_state)
{

	float tol;
	float tmp;
	float target_tmp;

	// hmm, turning speed //
	tol = bot->turning_speed + (2.0f * bot->turning_speed);

	// if course if your goal is to turn right
	if (bot->turn_direction == TURN_LEFT)
	{
		bot->heading += bot->turning_speed;

		if (bot->heading >= 360)
			bot->heading -= 360.0f;
	}
	else
	{
		bot->heading -= bot->turning_speed;

		if (bot->heading < 0)
			bot->heading += 360.0f;

	} // end of the if-else

	tmp = ABS(bot->heading);
	target_tmp = ABS(bot->target_dir);

	// reached target direction
	if ((tmp > (target_tmp - tol)) && (tmp < (target_tmp + tol)))
	{
		// change state
		bot->state = new_state;

		return;
	} // end of the if

	bot->state = old_state;

	return;

} // end of the function

//
// SetTurnDirection
// - select the direction to turn
// left or right
//
static void SetTurnDirection(DriverBotPtr bot, int state)
{
	float dist;
	int no_turns_left;
	int no_turns_right;

	// set whether to move backwards or forwards
	if ((rand() % 4) == 2)
	{
		bot->move_back = false;
	}
	else
	{
		bot->move_back = true;

	} // end of the if

	dist = bot->target_dir - bot->heading;

	if (dist < 0)
		dist += 360.0f;

	no_turns_left = (int)(dist / bot->turning_speed);

	// now check the right
	dist = bot->heading - bot->target_dir;

	if (dist < 0)
		dist += 360.0f;

	no_turns_right = (int)(dist / bot->turning_speed);

	if (no_turns_right < no_turns_left)
		bot->turn_direction = TURN_RIGHT;
	else
		bot->turn_direction = TURN_LEFT;

	bot->state = state;

} // end of the function

//
// MoveFire0
// - for testing
void MoveFire0(int dir, int turn)
{
	int move_type = 0;

	bool b_dir = false;
	bool col_flag = false;

	if (dir > 0)
	{
		// movement code
		fire_cluster[0]->x -= (float)sin(fire_cluster[0]->heading * PI_180) *
							  fire_cluster[0]->linearv;
		fire_cluster[0]->y -= (float)cos(fire_cluster[0]->heading * PI_180) *
							  fire_cluster[0]->linearv;

		b_dir = false;

		move_type = move_type | MOVE_FORWARD;
	}
	else if (dir < 0)
	{

		// movement code
		fire_cluster[0]->x += (float)sin(fire_cluster[0]->heading *
										 PI_180) *
							  fire_cluster[0]->linearv;
		fire_cluster[0]->y += (float)cos(fire_cluster[0]->heading *
										 PI_180) *
							  fire_cluster[0]->linearv;

		b_dir = true;

		move_type = move_type | MOVE_BACKWARD;

	} // end of if-else

	if (turn > 0)
	{
		fire_cluster[0]->heading += fire_cluster[0]->turning_speed;

		if (fire_cluster[0]->heading > 360)
			fire_cluster[0]->heading -= 360.0f;

		move_type = move_type | MOVE_TURNLEFT;

	} // end of if
	else if (turn < 0)
	{
		fire_cluster[0]->heading -= fire_cluster[0]->turning_speed;

		if (fire_cluster[0]->heading < 0)
			fire_cluster[0]->heading += 360.0f;

		move_type = move_type | MOVE_TURNRIGHT;

	} // end of if-else

	// used with collisions
	// perform collision test using driver
	col_flag = CheckCollisionBot((DriverBotPtr)fire_cluster[0]);

	// we have a hit
	if (col_flag)
	{
		// reset back
		if (b_dir)
		{
			fire_cluster[0]->x -=
				(float)sin(fire_cluster[0]->heading * PI_180) * fire_cluster[0]->linearv;
			fire_cluster[0]->y -=
				(float)cos(fire_cluster[0]->heading * PI_180) *
				fire_cluster[0]->linearv;
		}
		else
		{
			fire_cluster[0]->x +=
				(float)sin(fire_cluster[0]->heading * PI_180) *
				fire_cluster[0]->linearv;
			fire_cluster[0]->y +=
				(float)cos(fire_cluster[0]->heading * PI_180) *
				fire_cluster[0]->linearv;
		} // end if-else

	} // end of the if

	// send the message if we are in network mode --
	Build_MoveMsg(fire_cluster[0]->x, fire_cluster[0]->y, fire_cluster[0]->heading, move_type);

} // end of func

//
// MoveBot
//
static void MoveFireAnt(DriverBotPtr bot)
{

	int id = 0;
	float *last_heading = NULL;
	float tmp_heading = 0.0f;

	bool col_flag = false;

	bot->numSteps++;

	// when to change direction
	if ((bot->numSteps % bot->straightSteps) == 0)
	{

		bot->state = CHANGE_DIR_STATE;

		return; // process state else where

	} // end of the if

	// movement code
	bot->x -= (float)sin(bot->heading * PI_180) * bot->linearv;
	bot->y -= (float)cos(bot->heading * PI_180) * bot->linearv;

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
		bot->state = CHANGE_DIR_STATE;

		// reset back
		bot->x += (float)sin(bot->heading * PI_180) * bot->linearv;
		bot->y += (float)cos(bot->heading * PI_180) * bot->linearv;

		return;

	} // end of the if

	bot->state = MOVE_STATE;

	return;

} // end of the function

//
// FindBot
// - return a number of the first bot found
static int FindBot(DriverBotPtr bot)
{

	int index = 0;

	for (index = 0; index < MAX_FIRE_ANTS; index++)
	{
		// I hope these match up
		if (bot->id == index)
			continue; // skip yourself

		// if the guy is dead, skip searching
		if (fire_cluster[index]->alive == DEAD_STATE)
			continue;

		if (CheckSight(bot, fire_cluster[index]))
			return index;

	} // end of the for

	return INVALID_BOT;

} // end of the function

//
// SearchEvent
// - use another state
// for processing the search events
//
static bool SearchEvent(DriverBotPtr bot)
{
	int res;
	float tmp_targ;

	res = FindBot(bot);

	if (res == -1)
		return false;

	bot->enemy_id = res; // save for later

	// we found a valid nme
	// Note: for turret rotation, you have to include
	// the heading direction
	tmp_targ = FindAngle(bot->heading,
						 bot->x, bot->y,
						 fire_cluster[res]->x, fire_cluster[res]->y);

	bot->target_dir = tmp_targ;

	// bot->state = SET_MOUNT_STATE;
	bot->state = GO_MOVE_COMMAND;

	return true;

} // end of the function

//
// FindCameraPos
//
void FindCameraPos(DriverBotPtr bot)
{
	float tmp_heading;
	float tmp_x, tmp_y;
	float rad;

	//
	// Set the camera lookat and position
	//
	tmp_heading = bot->heading + 90.0f;
	if (tmp_heading > 360.0f)
		tmp_heading -= 360.0f;

	rad = tmp_heading / RAD_TO_DEG;

	tmp_x = LOOKAT_OFFSET * (float)cos(rad) * CAMERA->zoom_factor;
	tmp_y = LOOKAT_OFFSET * (float)sin(rad) * CAMERA->zoom_factor;

	tmp_x = tmp_x + bot->x;
	tmp_y = (-tmp_y) + bot->y;

	bot->look_x = tmp_x;
	bot->look_y = tmp_y;

	// the camera pos
	tmp_heading = bot->heading + 270.0f;
	if (tmp_heading > 360.0f)
		tmp_heading -= 360.0f;

	rad = tmp_heading / RAD_TO_DEG;

	tmp_x = CAMERA_BOT_OFFSET * (float)cos(rad) * CAMERA->zoom_factor;
	tmp_y = CAMERA_BOT_OFFSET * (float)sin(rad) * CAMERA->zoom_factor;

	tmp_x = tmp_x + bot->x;
	tmp_y = (-tmp_y) + bot->y;

	bot->cam_x = tmp_x;
	bot->cam_y = tmp_y;

} // end of the function

//
// Process FireEvent
//
void Wander_Command(DriverBotPtr bot)
{

	// bot is dead cant do too much
	if (bot->alive == DEAD_STATE)
		return;

	FindCameraPos(bot);

	switch (bot->state)
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

		bot->state = GetStartState(WANDER_COMMAND);
		break;

	case GO_MOVE_COMMAND:
		Generate_Command(bot, MOVE_COMMAND);
		break;

	case EXPLODE_STATE:
		SetExplosion(bot->x, bot->y);
		bot->alive = DEAD_STATE;
		bot->state = TEMP_STATE;

		break;

	default:
		break;
	}; // end switch

} // end of the function

//
// GenerateMove
// - pick a move spot for attacking
// -
// This has some flaws, one: it
// may cross over the nme to get to some
// spot, hmm, not very effective
// so use GenerateBoxMove also
//
void GenerateMove(DriverBotPtr bot, int next_state)
{

	int res;
	float tmp_targ;
	float tmp_x;
	float tmp_y;
	float radius;
	float rand_rad;

	res = bot->enemy_id;

	radius = (((float)(rand() % 100) / 20.0f) - 2.5f);
	radius += ATTACK_RADIUS;

	rand_rad = (((float)(rand() % 1000) / 15.0f) - 33.0f);

	bot->attack_angle += rand_rad;

	if (bot->attack_angle < 0)
		bot->attack_angle += 360.0f;
	else if (bot->attack_angle > 360)
		bot->attack_angle -= 360.0f;

	// generate a point at a circle away
	// from the nme and then turn
	tmp_x = radius * (float)cos(bot->attack_angle);
	tmp_y = radius * (float)sin(bot->attack_angle);

	tmp_x = fire_cluster[res]->x + tmp_x;
	tmp_y = fire_cluster[res]->y + tmp_y;

	tmp_targ = FindAngle(bot->heading,
						 bot->x, bot->y, tmp_x, tmp_y);

	bot->target_dir = tmp_targ;

	tmp_targ = FindAngle(bot->heading,
						 bot->x, bot->y,
						 fire_cluster[res]->x, fire_cluster[res]->y);

	bot->target_dir = tmp_targ;

	// we also need the number of moves it takes to
	// get there
	bot->target_moves = (int)(radius / bot->linearv);

	if (bot->target_moves <= 0)
		bot->target_moves = 2;

	bot->move_index = 0;

	bot->state = next_state;

} // end of the function

//
// Generate Box Move
// A little bit more effective
//
void GenerateBoxMove(DriverBotPtr bot, int next_state)
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

	res = bot->enemy_id;
	nme_x = bot->x;
	nme_y = bot->y;

	zx = fire_cluster[res]->x;
	zy = fire_cluster[res]->y;

	// creat a box on the left
	tmp_x = ((float)(rand() % 100) / 30.0f);
	tmp_y = (((float)(rand() % 100) / 20.0f) - 2.5f);

	dx = zx - bot->x;
	dy = zy - bot->y;

	radius = (float)sqrt((dx * dx) + (dy * dy));
	tol = LINE_OF_SIGHT / 1.9f;

	if ((zx > bot->x) ||
		(radius > tol))
		tmp_x = -tmp_x;

	tmp_x = nme_x + tmp_x;
	tmp_y = nme_y + tmp_y;

	dx = tmp_x - bot->x;
	dy = tmp_y - bot->y;

	radius = (float)sqrt((dx * dx) + (dy * dy));

	tmp_targ = FindAngle(bot->heading,
						 bot->x, bot->y, tmp_x, tmp_y);

	bot->target_dir = tmp_targ;

	tmp_targ = FindAngle(bot->heading,
						 bot->x, bot->y,
						 tmp_x, tmp_y);

	bot->target_dir = tmp_targ;

	// we also need the number of moves it takes to
	// get there
	bot->target_moves = (int)(radius / bot->linearv);

	if (bot->target_moves <= 0)
		bot->target_moves = 2;

	bot->move_index = 0;

	bot->state = next_state;

} // end of the function

//
// Assault, no move just attack
//
// if you want something basic, this is it
// no thrills, just shoot
//
void Assault_Move(DriverBotPtr bot, int next_state)
{
	int res;
	float tmp_targ;

	res = bot->enemy_id;

	tmp_targ = FindAngle(bot->heading,
						 bot->x, bot->y, fire_cluster[res]->x, fire_cluster[res]->y);

	bot->target_dir = tmp_targ;

	tmp_targ = FindAngle(bot->heading,
						 bot->x, bot->y,
						 fire_cluster[res]->x, fire_cluster[res]->y);

	bot->target_dir = tmp_targ;

	// we also need the number of moves it takes to
	// get there
	bot->target_moves = 0;
	bot->move_index = 0;

	bot->state = next_state;

} // end of the function

//
// Generate_Turn
// - turn towards the nme again
//
void Generate_Turn(DriverBotPtr bot)
{
	int res;
	float tmp_targ;

	res = bot->enemy_id;

	tmp_targ = FindAngle(bot->heading,
						 bot->x, bot->y,
						 fire_cluster[res]->x, fire_cluster[res]->y);

	bot->target_dir = tmp_targ;

	bot->state = SET_MOUNT_STATE;

} // end of the function

//
// Reach Target
// - travel so many moves based
// on target_moves
//
void Reach_Target(DriverBotPtr bot)
{
	CollisionObj *col_ptr;
	int tol_1;
	int tol_2;

	bool col_flag = false;

	// skip if we have zero move target
	if (bot->target_moves <= 0)
	{
		bot->state = GENERATE_TURN;
		return;
	} // end of the if

	bot->x -= (float)sin(bot->heading * PI_180) * bot->linearv;
	bot->y -= (float)cos(bot->heading * PI_180) * bot->linearv;

	// perform collision test using driver
	col_flag = CheckCollisionBot((DriverBotPtr)bot);

	// we have a hit
	if (col_flag)
	{
		bot->x +=
			(float)sin(bot->heading * PI_180) * bot->linearv;
		bot->y +=
			(float)cos(bot->heading * PI_180) * bot->linearv;

	} // end of the if

	// Now check how many times we have moved
	bot->move_index++;

	tol_1 = bot->target_moves - 1;
	tol_2 = bot->target_moves + 1;

	if ((bot->move_index >= tol_1) && (bot->move_index <= tol_2))
	{
		// done, now move to next state
		bot->state = GENERATE_TURN;
	} // end of the if

} // end of the functino

//
// Move Command
//
void Move_Command(DriverBotPtr bot)
{
	int check_rand;

	// bot is dead cant do too much
	if (bot->alive == DEAD_STATE)
		return;

	FindCameraPos(bot);

	switch (bot->state)
	{
	case GENERATE_STATE:

		check_rand = rand() % 2;

		// This is the heart of the AI

		// find a location to move to
		//

		if (bot->food > DYING_STATE)
		{
			if (check_rand == 1)
				Assault_Move(bot, SET_TURN_STATE);
			else
				GenerateBoxMove(bot, SET_TURN_STATE);
		} // end of the if
		else
		{

			// also increase the speed a little bit
			bot->linearv *= 1.1f;
			if (bot->linearv >= BOT_MAX_SPEED)
				bot->linearv = BOT_MAX_SPEED;

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
		SetExplosion(bot->x, bot->y);
		bot->alive = DEAD_STATE;
		bot->state = TEMP_STATE;

		break;

	default:
		break;
	}; // end switch

} // end of the function

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
		bot->state = GO_WANDER_COMMAND;

		return;
	} // end of the function

	bot->enemy_id = res; // save for later

	// otherwise, go into attack state
	tmp_targ = FindAngle(bot->heading,
						 bot->x, bot->y,
						 fire_cluster[res]->x, fire_cluster[res]->y);

	bot->target_dir = tmp_targ;

	bot->state = SET_MOUNT_STATE;

} // end of the function

//
// Attack Command
//
void Attack_Command(DriverBotPtr bot)
{

	// bot is dead cant do too much
	if (bot->alive == DEAD_STATE)
		return;

	FindCameraPos(bot);

	switch (bot->state)
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
		SetExplosion(bot->x, bot->y);
		bot->alive = DEAD_STATE;
		bot->state = TEMP_STATE;

		break;

	case GO_WANDER_COMMAND:
		// no enemy found, shrug
		Generate_Command(bot, WANDER_COMMAND);
		break;

	case GO_MOVE_COMMAND:
		Generate_Command(bot, MOVE_COMMAND);
		break;
	};

} // end of the functino

//
// Render_Tri1
// x-offset
void Render_Tri1(float size, float offset)
{
	float v[3][3] = {0};

	glBegin(GL_TRIANGLES);

	// left bottom front
	v[0][0] = -size - offset;
	v[0][1] = -size;
	v[0][2] = 0.0f;

	v[1][0] = -size - offset;
	v[1][1] = size;
	v[1][2] = 0.0f;

	v[2][0] = size - offset;
	v[2][1] = 0.0f;
	v[2][2] = 0.0f;

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	glEnd();

} // end of the function

//
// Render_Tri1
//
void Render_Tri2(float size, float offset)
{
	float v[3][3] = {0};

	glBegin(GL_TRIANGLES);

	// left bottom front
	v[0][0] = size + offset;
	v[0][1] = -size;
	v[0][2] = 0.0f;

	v[1][0] = size + offset;
	v[1][1] = size;
	v[1][2] = 0.0f;

	v[2][0] = -size + offset;
	v[2][1] = 0.0f;
	v[2][2] = 0.0f;

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	glEnd();

} // end of the function

//
// Render_Tri3
//
void Render_Tri3(float size, float offset)
{
	float v[3][3] = {0};

	glBegin(GL_TRIANGLES);

	// left bottom front
	v[0][0] = -size;
	v[0][1] = size + offset;
	v[0][2] = 0.0f;

	v[1][0] = size;
	v[1][1] = size + offset;
	v[1][2] = 0.0f;

	v[2][0] = 0.0f;
	v[2][1] = -size + offset;
	v[2][2] = 0.0f;

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	glEnd();

} // end of the function

//
// Render_Tri
//
void Render_Tri(float size, float offset)
{

	Render_Tri1(size, offset);
	Render_Tri2(size, offset);
	Render_Tri3(size, offset);

} // end of the function

// RenderFireAnt
//
static void RenderFireAnt(DriverBotPtr boid)
{
	float len = 0;

	if (boid->alive == ALIVE_STATE)
	{

		BEGIN_BOT;

		// Translate then rotate
		glTranslatef(boid->x, 0.0f, boid->y);

		// rotate based on the ship struct
		glRotatef(boid->heading, 0.0f, 1.0f, 0.0f);

		// Scale accordingly
		glScalef(boid->size[0], boid->size[1], boid->size[2]);

		// This may or may not change the color
		glColor3f(boid->color[0], boid->color[1], boid->color[2]);

		// draw the object to screen
		//
		// also check the color:

		// set the material for this object
		if (boid->id == 0)
			setmaterial(red_ambient, red_diffuse,
						red_specular, rlow_shininess, rmat_emission);
		else
			setmaterial(blue_ambient, blue_diffuse,
						blue_specular, blow_shininess, bmat_emission);

		driver_objects[FIREANT_OBJECT]->render();

		//
		// Create a shooter, aimer object thing
		// - crosshairs

		if (boid->crosshair_state == ALIVE_STATE)
		{
			// while alive grow to some max
			boid->crosshair_scale *= CROSSHAIRS_GROWTH;

			if (boid->crosshair_scale >= CROSSHAIRS_SCALE)
			{
				boid->crosshair_scale = 1.0f;
				boid->crosshair_state = DEAD_STATE;
			} // end of the if

			BEGIN_BOT;

			glDisable(GL_LIGHTING);

			MED_BLUE;
			glTranslatef(0.0f, 0.5f, -1.8f);

			glScalef(boid->crosshair_scale,
					 boid->crosshair_scale,
					 boid->crosshair_scale);

			Render_Tri(0.2f, 0.5f);

			glEnable(GL_LIGHTING);

			END_BOT;

		} // end of the if

		// this is the turret
		BEGIN_BOT;

		len = 0.8f;

		glTranslatef(0.0f, 0.0f, len);

		// rotate the turrets head
		glRotatef(0.0f, 0.0f, 1.0f, 0.0f);

		// translate back to original pos
		glTranslatef(0.0f, 0.0f, -len);

		glTranslatef(0.0f, 0.4f, -0.9f);

		glScalef(0.1f, 0.2f, 0.25f);

		// set the material for this object
		setmaterial(color_ambient, color_diffuse,
					color_specular, ylow_shininess, ymat_emission);

		driver_objects[NORM_CUBE_OBJECT]->render();
		END_BOT;

		END_BOT;

	} // end of the if

} // end of the function

//
// LoadFireAnts_A
//
static void LoadFireAnts_A(DriverBotPtr bot)
{
	int index = 0;

	float rand_arg_1;
	float rand_arg_2;
	int rand_arg;
	float tmp;
	float size;

	bot->linearv *= 0.8f; // make a little slower than normal bot

	// increase the turning speed
	bot->turning_speed *= 1.4f;

	// give the bot food
	bot->food = INITIAL_ANT_FOOD;

	//
	// we also need some way to check if the
	// bot winds up in a building
	//
	tmp = WORLD_X_MAX * 0.10f;
	rand_arg_1 = WORLD_X_MAX - tmp;
	rand_arg_2 = rand_arg_1 / 2.0f;

	// get the int component
	rand_arg = (int)(rand_arg_2 * rand_arg_1 * 2.0f);

	// place the fire ants a little farther away
	bot->x = ((rand() % rand_arg) / rand_arg_1) - rand_arg_2;
	bot->y = ((rand() % rand_arg) / rand_arg_1) - rand_arg_2;

	// This is redundant code
	// a better placement algorithm for the bot
	PositionBot(bot);

	bot->attack_angle = 0.0f;

	bot->view_mode = THIRD_PERSON_MODE;

	//
	// Change the size
	bot->size[0] = 3.2f;
	bot->size[1] = 3.2f;
	bot->size[2] = 3.2f;

	// somehow, we have to use the size
	// given here to calculate the min/max box
	size = FIRE_ANT_SIZE * 0.5f;

	bot->x_min = -(bot->size[0] * size * 1.52f);
	bot->x_max = bot->size[0] * size * 1.52f;
	bot->y_min = -(bot->size[2] * size * 1.52f);
	bot->y_max = bot->size[2] * size * 1.52f;

	bot->look_h = FIRST_HEIGHT; // first person y

	ResetBullets(bot);

	// command interface --
	//
	Generate_Command(bot, WANDER_COMMAND);

} // end of the function

//
// Load FireAnts
//  - fire ants are a little different
// than the normal bots
//
static void LoadFireAnts(DriverBotPtr bot)
{

	// also allocate bullets
	// dont forget to free bullets
	bot->bullets = (StaticBot *)malloc(MAX_BULLETS *
									   sizeof(StaticBot));

	LoadFireAnts_A(bot);

} // end of the function

// GenerateBots
//
void GenerateFireAnts(void)
{
	int index = 0;

	for (index = 0; index < MAX_FIRE_ANTS; index++)
	{
		fire_cluster[index] = CreateBot(index);

		LoadFireAnts(fire_cluster[index]);

		// used for collision testing, be careful
		// InsertColListA(fire_cluster[index]);
		Insert_MovingObj(fire_cluster[index]);

	} // end of the for

	GetCameraBot(fire_cluster[0]);

} // end of the function

//
// Reset_Fire_Ant
//
void Reset_Fire_Ant(DriverBotPtr bot, int id)
{
	LoadBotParms(bot);

	bot->id = id;

	// make sure to kill the bullets
	// free(bot->bullets);
	RELEASE_OBJECT(bot->bullets);

	bot->bullets = (StaticBot *)malloc(MAX_BULLETS *
									   sizeof(StaticBot));

	LoadFireAnts_A(bot);

} // end of the function

//
// Reset_FireAnts
// - this will be used to start a new game
//
void Reset_FireAnts(void)
{
	int index = 0;

	for (index = 0; index < MAX_FIRE_ANTS; index++)
	{
		Reset_Fire_Ant(fire_cluster[index], index);

	} // end of the for

	GetCameraBot(fire_cluster[0]);

} // end of the function

//
// - Run_NetworkBots
// - collect the messages as they come in
//
void Run_NetworkBots(void)
{
	int index = 0;

	Msg *msg_ptr = NULL;
	int msg_count;
	int msg_objectid;
	int tmp_id;

	float x;
	float y;
	float heading;

	//
	// and we also have to set the positions
	//
	msg_ptr = Get_Messages(&msg_count, &msg_objectid);

	if (msg_count < 0)
		msg_count = 0;

	// on what we have from the server
	for (index = (msg_count - 1); index >= 0; index--)
	{

		tmp_id = msg_ptr[index].object_id;

		// skip the current bot
		if (tmp_id == 0)
			continue;

		x = msg_ptr[index].pos_x;
		y = msg_ptr[index].pos_y;
		heading = msg_ptr[index].heading;

		fire_cluster[tmp_id]->x = x;
		fire_cluster[tmp_id]->y = y;
		fire_cluster[tmp_id]->heading = heading;

	} // end of for

	// reset the message count
	Reset_MessageCount();

} // end of the function

//
// Reset_NetworkBots
//
void Reset_NetworkBots(void)
{
	int index = 0;

	Msg *msg_ptr = NULL;
	int msg_count;
	int msg_objectid;
	int tmp_id;

	float x;
	float y;
	float heading;

	for (index = 0; index < MAX_NETWORK_BOTS; index++)
	{
		Reset_Fire_Ant(fire_cluster[index], index);

	} // end of the for

	//
	// and we also have to set the positions
	//
	msg_ptr = Get_Messages(&msg_count, &msg_objectid);

	// get the id first
	Convert_SetID(msg_objectid);

	// for safety

	if (msg_count < 0)
		msg_count = 0;

	//
	// convert ids first
	for (index = (msg_count - 1); index >= 0; index--)
	{
		tmp_id = msg_ptr[index].object_id;

		// if we dont have the id already
		// add it to the queue
		if (msg_objectid != tmp_id)
			Convert_SetID(tmp_id);

	} // end of the for

	// now get the pos, based
	// on what we have from the server
	for (index = (msg_count - 1); index >= 0; index--)
	{

		tmp_id = Convert_GetID(index);

		x = msg_ptr[index].pos_x;
		y = msg_ptr[index].pos_y;
		heading = msg_ptr[index].heading;

		fire_cluster[tmp_id]->x = x;
		fire_cluster[tmp_id]->y = y;
		fire_cluster[tmp_id]->heading = heading;

	} // end of for

	GetCameraBot(fire_cluster[0]);

	//
	// we are done with the messages
	// so reset them
	Reset_MessageCount();

} // end of the fuction

//
//
// Set up demo, mode, everything is
// reset, but the first bot is dead
//
void Prepare_DemoMode(void)
{
	int index = 0;

	for (index = 0; index < MAX_FIRE_ANTS; index++)
	{
		Reset_Fire_Ant(fire_cluster[index], index);

	} // end of the for

	fire_cluster[0]->food = -1000;
	fire_cluster[0]->alive = DEAD_STATE;

	GetCameraBot(fire_cluster[1]);

} // end of the function

//
// ShutdownBots
//
void ShutdownFireAnts(void)
{
	int index = 0;

#if 0
	for (index = 0; index < MAX_FIRE_ANTS; index++)
	{
		//free(fire_cluster[index]->bullets);
		RELEASE_OBJECT(fire_cluster[index]->bullets);

		DestroyBot(fire_cluster[index]);

	} // end of the for
#endif

	for (index = 0; index < MAX_SAVED_BOTS; index++)
	{
		// free(fire_cluster[index]->bullets);
		RELEASE_OBJECT(fire_cluster[index]->bullets);

		DestroyBot(fire_cluster[index]);

	} // end of the for

} // end of the function

//
// AnimNetworkBots
//
void AnimNetworkBots(void)
{
	int index = 0;
	int j = 0;
	float max = -10000;
	int id = 0;
	int max_id;

	//
	// Check Network snapshots first
	Perform_Snapshots();

	ant_globals->alive_bots = 0; // reset

	for (index = 0; index < MAX_NETWORK_BOTS; index++)
	{

		// run the command library
		// unless the robot is robot zero
		if (index > 0)
		{

			// get data from the network
			// Run_NetworkBots();
		}
		else if (index == 0)
		{

			// only difference here is you send out commands

			Net_Player(fire_cluster[0]);

		} // end of if else

		// check if bot is alive for score
		// count

		if (fire_cluster[index]->alive == ALIVE_STATE)
		{

			if (index != 0)
				ant_globals->alive_bots++;

		} // end of the if

	} // end of the for

	// get the top two scores in addition
	// to the main bot
	Load_Score(fire_cluster[0]->score,
			   fire_cluster[0]->kills, 0, 0);

	// now the next 2
	for (index = 1; index < MAX_SCORE_DISPLAY; index++)
	{
		max = -100000;

		for (j = 1; j < MAX_NETWORK_BOTS; j++)
		{
			// skip the already highest
			if (j == max_id)
				continue;

			if (fire_cluster[j]->alive == DEAD_STATE)
				continue;

			// get the next highest score
			if (fire_cluster[j]->score > max)
			{
				Load_Score(fire_cluster[j]->score,
						   fire_cluster[j]->kills, j, index);
				max = fire_cluster[j]->score;

				id = j;

			} // end of the if

		} // end of the for

		max_id = id;

	} // end of the for

} // end of the function

//
// Draw Bots
//
void AnimFireAnts(void)
{
	int index = 0;
	int j = 0;
	float max = -10000;
	int id = 0;
	int max_id;

	ant_globals->alive_bots = 0; // reset

	for (index = 0; index < MAX_FIRE_ANTS; index++)
	{

		// run the command library
		// unless the robot is robot zero
		if (index > 0)
		{

			if (fire_cluster[index]->alive == ALIVE_STATE)
				fire_cluster[index]->run(fire_cluster[index]);
		}
		else if (index == 0)
		{

			Handle_Player(fire_cluster[0]);

		} // end of if else

		// check if bot is alive for score
		// count

		if (fire_cluster[index]->alive == ALIVE_STATE)
		{

			if (index != 0)
				ant_globals->alive_bots++;

		} // end of the if

	} // end of the for

	// get the top two scores in addition
	// to the main bot
	Load_Score(fire_cluster[0]->score,
			   fire_cluster[0]->kills, 0, 0);

	// now the next 2
	for (index = 1; index < MAX_SCORE_DISPLAY; index++)
	{
		max = -100000;

		for (j = 1; j < MAX_FIRE_ANTS; j++)
		{
			// skip the already highest
			if (j == max_id)
				continue;

			if (fire_cluster[j]->alive == DEAD_STATE)
				continue;

			// get the next highest score
			if (fire_cluster[j]->score > max)
			{
				Load_Score(fire_cluster[j]->score,
						   fire_cluster[j]->kills, j, index);
				max = fire_cluster[j]->score;

				id = j;

			} // end of the if

		} // end of the for

		max_id = id;

	} // end of the for

} // end of the function

//
// DrawFireAnts
//
void DrawFireAnts(void)
{
	int index = 0;

	if (ant_globals->paused == 1)
	{
		RenderFireAnt(fire_cluster[0]);
	}
	else
	{

		if (fire_cluster[0]->view_mode == THIRD_PERSON_MODE)
			RenderFireAnt(fire_cluster[0]);

	} // end of the if-else

	RenderBullets(fire_cluster[0]);

#if 0
	for (index = 1; index < MAX_FIRE_ANTS; index++)
	{
	
		RenderFireAnt(fire_cluster[index]);
		RenderBullets(fire_cluster[index]);

	} // end of the function

#endif

	if ((CHECK_NET_CLIENT) || (CHECK_NET_SERVER))
	{

		for (index = 1; index < MAX_NETWORK_BOTS; index++)
		{

			RenderFireAnt(fire_cluster[index]);
			RenderBullets(fire_cluster[index]);

		} // end of the function

	} // end of the if
	else
	{

		for (index = 1; index < MAX_FIRE_ANTS; index++)
		{

			RenderFireAnt(fire_cluster[index]);
			RenderBullets(fire_cluster[index]);

		} // end of the function

	} // end of if

} // end of the function

// INTERFACE END ==========================================

//
// draw fire ant
//
static void draw_fireant(void)
{
	float v[3][3] = {0};
	float size = FIRE_ANT_SIZE;
	float height = 0.9f;

	float f_size = size / 2.0f;
	float in_size = size * 0.65f;

	float in_size_2 = size * 0.35f;

	float height_2 = height * 0.3f;

	float height_3 = height * 1.8f;

	float height_4 = height * 0.8f;

	float n[3]; // remember to free

	// change the size here
	// Note: starts from ground

	glBegin(GL_TRIANGLES);

	// BOTTOM SECTION ---
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = size;

	v[2][0] = f_size;
	v[2][1] = 0.0f;
	v[2][2] = -size;

	N_0;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// Getthe top
	v[0][0] = -f_size;
	v[0][1] = 0.0f;
	v[0][2] = -size;

	v[1][0] = f_size;
	v[1][1] = 0.0f;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = 0.0f;
	v[2][2] = size;

	N_1;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// END OF SECTION --

	// build a floor on the bottom of the ship
	// (left-side)
	v[0][0] = -f_size;
	v[0][1] = 0.0f;
	v[0][2] = -size;

	v[1][0] = -f_size;
	v[1][1] = height_2;
	v[1][2] = -size;

	v[2][0] = -size;
	v[2][1] = 0.0f;
	v[2][2] = size;

	N_2;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// (left-side) - 2
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = -size;
	v[1][1] = height_2;
	v[1][2] = size;

	v[2][0] = -f_size;
	v[2][1] = height_2;
	v[2][2] = -size;

	N_1;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	//
	// BACK WALL
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = 0.0f;
	v[1][2] = size;

	v[2][0] = size;
	v[2][1] = height_2;
	v[2][2] = size;

	N_1;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// BACK WALL-2
	v[0][0] = -size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = -size;
	v[1][1] = height_2;
	v[1][2] = size;

	v[2][0] = size;
	v[2][1] = height_2;
	v[2][2] = size;

	N_0;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// Front
	v[0][0] = -f_size;
	v[0][1] = 0.0f;
	v[0][2] = -size;

	v[1][0] = f_size;
	v[1][1] = 0.0f;
	v[1][2] = -size;

	v[2][0] = f_size;
	v[2][1] = height_2;
	v[2][2] = -size;

	N_0;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// FRONT - 2
	v[0][0] = -f_size;
	v[0][1] = 0.0f;
	v[0][2] = -size;

	v[1][0] = -f_size;
	v[1][1] = height_2;
	v[1][2] = -size;

	v[2][0] = f_size;
	v[2][1] = height_2;
	v[2][2] = -size;

	N_1;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	//
	//

	// (left-side)
	v[0][0] = f_size;
	v[0][1] = 0.0f;
	v[0][2] = -size;

	v[1][0] = f_size;
	v[1][1] = height_2;
	v[1][2] = -size;

	v[2][0] = size;
	v[2][1] = 0.0f;
	v[2][2] = size;

	N_2;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// (left-side) - 2
	v[0][0] = size;
	v[0][1] = 0.0f;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = height_2;
	v[1][2] = size;

	v[2][0] = f_size;
	v[2][1] = height_2;
	v[2][2] = -size;

	N_1;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	//
	// NOW, build the top part of the ship
	//

	// back
	v[0][0] = -size;
	v[0][1] = height_2;
	v[0][2] = size;

	v[1][0] = size;
	v[1][1] = height_2;
	v[1][2] = size;

	v[2][0] = f_size;
	v[2][1] = height;
	v[2][2] = in_size;

	N_1;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// back
	v[0][0] = -size;
	v[0][1] = height_2;
	v[0][2] = size;

	v[1][0] = -f_size;
	v[1][1] = height;
	v[1][2] = in_size;

	v[2][0] = f_size;
	v[2][1] = height;
	v[2][2] = in_size;

	N_0;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	//
	// On the right
	//
	v[0][0] = -size;
	v[0][1] = height_2;
	v[0][2] = size;

	v[1][0] = -f_size;
	v[1][1] = height_2;
	v[1][2] = -size;

	v[2][0] = -f_size;
	v[2][1] = height;
	v[2][2] = in_size;

	N_3;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	//
	// On the right

	v[0][0] = size;
	v[0][1] = height_2;
	v[0][2] = size;

	v[1][0] = f_size;
	v[1][1] = height_2;
	v[1][2] = -size;

	v[2][0] = f_size;
	v[2][1] = height;
	v[2][2] = in_size;

	N_1;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	//
	// Place in the front
	//
	v[0][0] = -f_size;
	v[0][1] = height_2;
	v[0][2] = -size;

	v[1][0] = f_size;
	v[1][1] = height_2;
	v[1][2] = -size;

	v[2][0] = f_size;
	v[2][1] = height;
	v[2][2] = in_size;

	N_1;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	//
	// On the left
	//
	v[0][0] = -f_size;
	v[0][1] = height;
	v[0][2] = in_size;

	v[1][0] = f_size;
	v[1][1] = height;
	v[1][2] = in_size;

	v[2][0] = -f_size;
	v[2][1] = height_2;
	v[2][2] = -size;

	N_0;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// This is where I get weird
	// semi-anime inspired
	//
	v[0][0] = -f_size;
	v[0][1] = height_2;
	v[0][2] = -in_size;

	v[1][0] = -f_size;
	v[1][1] = height_2;
	v[1][2] = size;

	v[2][0] = -size;
	v[2][1] = height_3;
	v[2][2] = size;

	N_1;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// now on the left-side
	v[0][0] = f_size;
	v[0][1] = height_2;
	v[0][2] = -in_size;

	v[1][0] = f_size;
	v[1][1] = height_2;
	v[1][2] = size;

	v[2][0] = size;
	v[2][1] = height_3;
	v[2][2] = size;

	N_2;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	// place a gun in the front
	v[0][0] = 0.0f;
	v[0][1] = height_2;
	v[0][2] = -size;

	v[1][0] = 0.0f;
	v[1][1] = height_2;
	v[1][2] = -in_size_2;

	v[2][0] = 0.0f;
	v[2][1] = height_4;
	v[2][2] = -in_size_2;

	N_2;
	glNormal3fv(n);

	glVertex3fv(v[0]);
	glVertex3fv(v[1]);
	glVertex3fv(v[2]); // triangle left bottom front

	glEnd();

} // end of the function

//
// init
// - load anything special about the
// one important function
//
static void init_fireant(int list_id)
{

	CURRENT_OBJECT.visible = 1;

	// store the id through the function
	// there is probably a better way to do this
	CURRENT_OBJECT.call_id = list_id;

} // end of the functino

//=========================================================
// Now the function to actually draw it
//=========================================================
static void render_fireant(void)
{
	// glPushMatrix();

	glCallList(CURRENT_OBJECT.call_id);

	// glPopMatrix();

} // end of the function

//=========================================================
// compile
//=========================================================
static void compile_fireant(void)
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

} // end of the function
