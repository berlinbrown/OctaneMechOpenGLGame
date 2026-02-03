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
// global.h
// - we need to keep a running tab
// of everything
// for processing and what not
//
#ifndef _GLOBALS_H_

#include <time.h>

#define LOADED_TRUE 1
#define LOADED_FALSE 0

#define ENABLE_HUD 0

#define HUD_TIME_T 0
#define HUD_ANTS 0
#define HUD_SECS 0
#define HUD_NEST_FOOD 0
#define HUD_GARDEN 0
#define HUD_HEALTH 1
#define HUD_SCORE 1
#define HUD_POS 0

//
// the number of scores to display
// if we have 100 scores it wouldnt
// make much since to display all of them
#define MAX_SCORE_DISPLAY 3

#define MAX_ERRORS 33

//
// config variables --
//
#define LINE_OF_SIGHT ant_globals->line_of_sight
#define ATTACK_RADIUS ant_globals->attack_radius
#define BULLET_DAMAGE ant_globals->bullet_damage
#define MIN_BULLET_SPEED ant_globals->min_bullet_speed
#define USE_ANT_ENGINE ant_globals->use_ant_engine
#define MAX_FIRE_ANTS ant_globals->max_fire_ants
#define MAX_BOTS ant_globals->max_bots
#define USE_GARDEN_AREA ant_globals->use_garden_area
#define MAX_TRAIL_STACK ant_globals->max_trail_stack
#define DYING_STATE ant_globals->dying_state
#define MAX_PHEROMONES ant_globals->max_pheromones
#define PHEROMONE_LIFE ant_globals->pheromone_life
#define PHEROMONE_DROP ant_globals->pheromone_drop
#define MAX_BULLETS ant_globals->max_bullets
#define MAX_FIRE_SPEED ant_globals->max_fire_speed
#define MAX_GARDENS ant_globals->max_gardens
#define BOT_SPEED ant_globals->bot_speed
#define BOT_MAX_SPEED ant_globals->bot_max_speed
#define MIN_TURN_SPEED ant_globals->min_turn_speed
#define CHECK_RESPAWN ant_globals->check_respawn
#define GARD_RESPAWN_RATE ant_globals->gard_respawn_rate
#define MIN_STRAIGHT_STEPS ant_globals->min_straight_steps
#define MAX_STRAIGHT_STEPS ant_globals->max_straight_steps
#define MIN_STRAIGHT_STEPS_2 ant_globals->min_straight_steps_2
#define MAX_STRAIGHT_STEPS_2 ant_globals->max_straight_steps_2
#define INITIAL_ANT_FOOD ant_globals->initial_ant_food
#define INITIAL_GARD_FOOD ant_globals->initial_gard_food
#define FOOD_WIDTH ant_globals->food_width
#define INIT_FOOD_RATE ant_globals->init_food_rate
#define MAX_FOOD_RATE ant_globals->max_food_rate
#define MOVE_FOOD_RATE ant_globals->move_food_rate
#define FOOD_RATE ant_globals->food_rate

//
// config defines
// keep a track of everything
// then write to file
#define ID_FILE_NOT_FOUND 0
#define ID_LINE_OF_SIGHT 1
#define ID_ATTACK_RADIUS 2
#define ID_BULLET_DAMAGE 3
#define ID_MIN_BULLET_SPEED 4
#define ID_USE_ANT_ENGINE 5
#define ID_MAX_FIRE_ANTS 6
#define ID_MAX_BOTS 7
#define ID_USE_GARDEN_AREA 8
#define ID_MAX_TRAIL_STACK 9
#define ID_DYING_STATE 10
#define ID_MAX_PHEROMONES 11
#define ID_PHEROMONE_LIFE 12
#define ID_PHEROMONE_DROP 13
#define ID_MAX_BULLETS 14
#define ID_MAX_FIRE_SPEED 15
#define ID_MAX_GARDENS 16
#define ID_BOT_SPEED 17
#define ID_BOT_MAX_SPEED 18
#define ID_MIN_TURN_SPEED 19
#define ID_CHECK_RESPAWN 20
#define ID_GARD_RESPAWN_RATE 21
#define ID_MIN_STRAIGHT_STEPS 22
#define ID_MAX_STRAIGHT_STEPS 23
#define ID_MIN_STRAIGHT_STEPS_2 24
#define ID_MAX_STRAIGHT_STEPS_2 25
#define ID_INITIAL_ANT_FOOD 26
#define ID_INITIAL_GARD_FOOD 27
#define ID_FOOD_WIDTH 28
#define ID_INIT_FOOD_RATE 29
#define ID_MAX_FOOD_RATE 30
#define ID_MOVE_FOOD_RATE 31
#define ID_FOOD_RATE 32

//
// a score container
//
typedef struct tagScoreObj
{
  int name;
  int kills;
  float score;
} ScoreObj;

//
typedef struct tagAntGlobals
{
  int alive_ants;
  float seconds;
  float time_t;
  float nest_food;
  int garden;
  float player_health;
  DWORD ticks;

  int paused;
  int alive_bots;
  int menu_mode;
  int _menu_state;

  //
  // variables for
  // the scores
  // - we need to allocate
  // a new array however
  ScoreObj* score_obj;

  // config file variables
  float line_of_sight;
  float attack_radius;
  float bullet_damage;
  float min_bullet_speed;
  int use_ant_engine;
  int max_fire_ants;
  int max_bots;
  int use_garden_area;
  int max_trail_stack;
  int dying_state;
  int max_pheromones;
  int pheromone_life;
  int pheromone_drop;
  int max_bullets;
  int max_fire_speed;
  int max_gardens;
  float bot_speed;
  float bot_max_speed;
  float min_turn_speed;
  int check_respawn;
  int gard_respawn_rate;
  int min_straight_steps;
  int max_straight_steps;
  int min_straight_steps_2;
  int max_straight_steps_2;
  int initial_ant_food;
  int initial_gard_food;
  float food_width;
  int init_food_rate;
  int max_food_rate;
  float move_food_rate;
  float food_rate;

} AntGlobals, *AntGlobalsPtr;

//
// Text Library
//
void Super_MainText(void);
void Super_DrawText(void);
void Super_KillText(void);

void Super_BeginPaused(void);
void Draw_HelpScreen(void);
void Draw_IntroScreen(void);
//
// User super_printf for debug code
//
void Super_Printf(char* fmt, ...);
void Print_Score(void);

void Load_Score(float score, float kills, int id, int i);

void Super_LoadGlobals(void);
void Super_KillGlobals(void);

void InitGlobals(void);
void SubtractAnts(int ants);

void TogglePaused(void);

void PrintGlobals(void);
void AddSeconds(float delta);
void AddTime(float delta);
void SetNestFood(float d);
int GetAnts(void);
void GameTick(void);
DWORD GetGameTick(void);
void SetPlayerHealth(float d);

void SetGardenSize(int v);
int GetGardenSize(void);

void AddAnts(int ants);
void DestroyGlobals(void);

void Load_ConfigFile(void);
void Add_ErrorStr(char* str);

extern AntGlobals* ant_globals;
extern float framerate;

extern int mSuper_Loaded;

#endif
