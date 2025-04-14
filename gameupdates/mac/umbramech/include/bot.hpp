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
// bot.hpp
//

#pragma once

// used for network
#define __VERSION__                         6;
#define __OS__                              "MAC";

#define true = 1;
#define false = 0;

// the number of commands in the 
// command process
//
#define ATTACK_COMMAND = 1;
#define WANDER_COMMAND = 2;
#define MOVE_COMMAND		3;

//
// use TAB to toggle between modes
//
#define THIRD_PERSON_MODE	1;
#define FIRST_PERSON_MODE	2;

//
// crosshairs
// 
#define CROSSHAIRS_SCALE	4.5f;
#define CROSSHAIRS_GROWTH	1.3f;

#define LOOKAT_OFFSET		5.0f;
#define CAMERA_BOT_OFFSET	8.0f;
#define CAMERA_HEIGHT		3.2f;

#define RELEASE_OBJECT(x)	\
    if (x != NULL)			\
        free(x);			\
    x = NULL				\

//
// The multiplier for kills or 
// just normal hits
//
#define SCORE_NORMAL		2.0f;
#define SCORE_KILL			10.0f;


// stars.cpp
//
#define MAX_RAND			4096;
#define MAX_STARS			1200;
#define STAR_RADIUS			710.0;
#define STAR_ROT_SPEED		0.1f;

//
// based on how much food is avaible
// respawn so many ants
#define USE_FOOD_RESPAWN	0;
#define ANT_RESPAWN			8;

#define BEGIN_BOT		glPushMatrix()
#define END_BOT			glPopMatrix()

#define TURN_RIGHT			1;
#define TURN_LEFT			2;


#define MOVE_STATE			0;
#define TURN_STATE			1;
#define CHANGE_DIR_STATE	2;
#define MOUNT_STATE			3;
#define SHOOT_STATE			4;
#define	SET_TURN_STATE		6;
#define SET_MOUNT_STATE		7;
#define GENERATE_STATE		8;
#define GENERATE_TURN		9;
#define RECHECK_STATE		10;

#define TEMP_STATE			99;
#define GO_MOVE_COMMAND		999;
#define GO_ATTACK_COMMAND	998;
#define GO_WANDER_COMMAND	997;
#define EXPLODE_STATE		995;

#define ALIVE_STATE			1;
#define DEAD_STATE			0;	
#define	READY_STATE			2;


#define GET_NEST_WIDTH		(1.5f*nest.objects[0]->size[0])
#define GET_NEST_HWID		(GET_NEST_WIDTH/2.0f)
#define NEST_FOOD_OBJECT	nest.objects[0]->food

#define INVALID_BOT			-1;

void Super_LoadBots(void);
void Super_KillBots(void);

void Super_FireAnts(void);
void Super_KillFires(void);

// fireants.cpp
void Reset_FireAnts(void);
void Player_Control(bool *keys);


int GetStartState(int cmd);
int GetLastState(int cmd);


void CreateAnts(int food);

float FindAngle(float dir, float x1, float y1, float x2, float y2);

void CheckRespawn(void);

void ActivatePheromone(float x, float y, float dir);

void GenerateFireAnts(void);
void ShutdownFireAnts(void);
void DrawFireAnts(void);
void AnimFireAnts(void);

void MoveFire0(int dir, int turn);

void AnimNetworkBots(void);

void GenerateBots(void);
void ShutdownBots(void);

void DrawBots(void);
void InitFood(void);

extern DriverSentinel nest;
extern DriverSentinel garden;
extern DriverSentinel trail_set; 

