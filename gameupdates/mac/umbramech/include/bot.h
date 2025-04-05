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
// bot.h
//
#ifndef _BOT_H_
#define _BOT_H_

// used for network
#define __VERSION__                         6
#define __OS__                              "LIN"

typedef unsigned long DWORD;
typedef int bool;

#define ZeroMemory(S, N) memset((S), 0, (N))

#define true 1
#define false 0

// the number of commands in the 
// command process
//
#define ATTACK_COMMAND		1
#define WANDER_COMMAND		2
#define MOVE_COMMAND		3

//
// use TAB to toggle between modes
//
#define THIRD_PERSON_MODE	1
#define FIRST_PERSON_MODE	2

//
// crosshairs
// 
#define CROSSHAIRS_SCALE	4.5f
#define CROSSHAIRS_GROWTH	1.3f

#define LOOKAT_OFFSET		5.0f
#define CAMERA_BOT_OFFSET	8.0f
#define CAMERA_HEIGHT		3.2f

#define RELEASE_OBJECT(x)	\
	if (x != NULL)			\
		free(x);			\
	x = NULL				\

//
// The multiplier for kills or 
// just normal hits
//
#define SCORE_NORMAL		2.0f
#define SCORE_KILL			10.0f


// stars.cpp
//
#define MAX_RAND			4096	// 4096?
#define MAX_STARS			1200	// 2000
#define STAR_RADIUS			710.0
#define STAR_ROT_SPEED		0.1f;


#define __straight_steps	(MAX_STRAIGHT_STEPS-MIN_STRAIGHT_STEPS)	

#define __go_home_steps_2	(MAX_STRAIGHT_STEPS_2-MIN_STRAIGHT_STEPS_2)	

//
// based on how much food is avaible
// respawn so many ants
#define USE_FOOD_RESPAWN	0
#define ANT_RESPAWN			8
#define FOOD_RESPAWN	(ANT_RESPAWN * INITIAL_ANT_FOOD)


#define BEGIN_BOT		glPushMatrix()
#define END_BOT			glPopMatrix()

#define TURN_RIGHT			1
#define TURN_LEFT			2


#define MOVE_STATE			0
#define TURN_STATE			1
#define CHANGE_DIR_STATE	2
#define MOUNT_STATE			3
#define SHOOT_STATE			4
#define	SET_TURN_STATE		6
#define SET_MOUNT_STATE		7
#define GENERATE_STATE		8
#define GENERATE_TURN		9
#define RECHECK_STATE		10

#define TEMP_STATE			99
#define GO_MOVE_COMMAND		999
#define GO_ATTACK_COMMAND	998
#define GO_WANDER_COMMAND	997
#define EXPLODE_STATE		995

#define ALIVE_STATE			1
#define DEAD_STATE			0	
#define	READY_STATE			2


#define GET_NEST_WIDTH		(1.5f*nest.objects[0]->size[0])
#define GET_NEST_HWID		(GET_NEST_WIDTH/2.0f)
#define NEST_FOOD_OBJECT	nest.objects[0]->food

#define INVALID_BOT			-1

// static bot
//
typedef struct tagStaticBot {

	int		list_id;
	float	position[3];
	float	rotation[3];
	float	size[3];
	float	color[3];
	float	state;

	float	food;

	bool	delete_flag;		// deleted from pheromone list

	float	heading;		// you can use rotation or heading
	float	linearv;

	//
	// for the collision
	//
	float	travel;		// distance traveled
	float	final_x;
	float	final_y;
	float	max_dist;

	
	//
	// 
	float	old_x;
	float	old_y;			// for drawing a line strip

	// used with bullets
	float	virt_x;
	float	virt_y;
	float	virt_heading;
	int		owner;			// owner of this bullet

	struct tagStaticBot *next;

} StaticBot, *StaticBotPtr;

// functions for static bot
typedef struct tagDriverSentinel {

	StaticBotPtr (*create)(int bot_id);
	void (*destroy)(StaticBotPtr b);
	void (*render)(StaticBotPtr boid);
	void (*process)(StaticBotPtr b);

	void (*generate)(void);
	void (*shutdown)(void);
	void (*drawall)(void);

	StaticBot	**objects;

	int		max_items;

} DriverSentinel, *DriverSentinelPtr;


typedef struct tagDriverBots {
		
	float	x;
	float	y;
	float	linearv;	// speed
	float	size[3];	// scale 
	float	heading;		// direction
	float	target_dir;	// target heading
	float	color[3];
	int		id;
	int		alive;
	int		numSteps;	// how many steps ant has moved
	float	turning_speed;

	int		state;			// what is bot doing
	int		straightSteps;	// steps before changing direction
	float	food;
	float	foodstore;
	int		turn_rand;
	int		turn_direction;	

	bool	go_home;

	int		gun_reset;		// delay befor firing
	int		gun_index;

	bool	move_back;		// for move and turn


	// camera helper variables
	float	look_x;
	float	look_y;
	float	look_h;		// look height, actually the y
	float	cam_x;
	float	cam_y;
	int		view_mode;		// first or third person

	float	score;
	int		kills;

	//
	// the rectangle of the bot
	// Note: you have to add
	// the x,y to this value, but dont
	// do to this variable, use temps
	float	x_min;
	float	x_max;
	float	y_min;
	float	y_max;

	//
	// crosshair object 
	//
	int		crosshair_state;
	float	crosshair_scale;

	// Add the fire ant bullets
	//StaticBot	bullets[MAX_BULLETS];
	StaticBot	*bullets;

	// Command interface
	void (*run)(struct tagDriverBots *bot);

	int		last_command;
	int		command;
	float	attack_angle;
	int		target_moves;
	int		move_index;
	int		enemy_id;

} DriverBots, *DriverBotPtr;

void Super_LoadBots(void);
void Super_KillBots(void);

void Super_FireAnts(void);
void Super_KillFires(void);

// fireants.cpp
void Reset_FireAnts(void);
void Player_Control(bool *keys);

void LoadBotParms(DriverBotPtr bot_ptr);

void Wander_Command(DriverBotPtr bot);
void Move_Command(DriverBotPtr bot);
void Attack_Command(DriverBotPtr bot);

int GetStartState(int cmd);
int GetLastState(int cmd);
void Generate_Command(DriverBotPtr bot, int cmd);

void GenerateBots(void);
void ShutdownBots(void);

void DrawBots(void);
void InitFood(void);

int BruteCheckFood(DriverBotPtr bot);
void DropFood(DriverBotPtr bot, int id, float food_rate);

void CreateAnts(int food);

float FindAngle(float dir, float x1, float y1, float x2, float y2);

void CheckRespawn(void);

void ActivatePheromone(float x, float y, float dir);

DriverBotPtr CreateBot(int bot_id);
void DestroyBot(DriverBotPtr b);
void GetAntFood(DriverBotPtr bot);
void ResetBot(DriverBotPtr bot_ptr);

void GenerateFireAnts(void);
void ShutdownFireAnts(void);
void DrawFireAnts(void);
void AnimFireAnts(void);

bool CheckSight(DriverBotPtr bot, DriverBotPtr nme);

void MoveFire0(int dir, int turn);

void PositionBot(DriverBotPtr bot);

void AnimNetworkBots(void);


extern DriverSentinel nest;
extern DriverSentinel garden;
extern DriverSentinel trail_set; 


#endif

