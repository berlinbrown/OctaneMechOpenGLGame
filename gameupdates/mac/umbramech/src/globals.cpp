/**
 * Copyright (c) 2006-2026 Berlin Brown.  All Rights Reserved
 *
 * http://www.opensource.org/licenses/bsd-license.php
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of the Botnode.com (Berlin Brown) nor
 * the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written
 * permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
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
 * Contact: Berlin Brown <berlin _dot_ brown at email>
 */

// globals.cpp

#include <GLUT/glut.h>   // GLUT for window/context
#include <OpenGL/gl.h>   // Core OpenGL functions
#include <OpenGL/glu.h>  // OpenGL Utility Library
#include <cstdlib>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <globals.hpp>
#include <defaults.hpp>
#include <text.hpp>
#include <menu.hpp>
#include <fireants.hpp>
#include <bot.hpp>
#include <gldrawlib.hpp>
#include <keys.hpp>
#include <legacy_stubs.hpp>
#include <camera.hpp>

AntGlobals* ant_globals;

static const int HELP_TEXT_LEFT = 230;
static const int HELP_TEXT_RIGHT = 500;
static const int HELP_TEXT_TOP = 180;
static const int HELP_TEXT_BOTTOM = 400;

static const float HELP_BOX_LEFT = 220.0f;
static const float HELP_BOX_TOP = 160.0f;
static const float HELP_BOX_RIGHT = 480.0f;
static const float HELP_BOX_BOTTOM = 450.0f;

// x positions
#define T_COL_1 160
#define T_COL_2 240
#define T_COL_3 300
#define T_COL_4 370
#define T_COL_5 420

#define T_COL_6 60
#define T_COL_7 240
#define T_ROW_1 142
#define T_ROW_2 312
#define T_ROW_3 414
#define T_ROW_4 424

// WRAPPER FUNCTIONS FOR TEXT--

// There are three different text areas
// The top, left text contains the score
// the top, right text contains the fps and any
//	other system specific data --
// NOTE: use Super_Printf for debug code
//		- you also have to call drawText to
//		  actually render to screen

TextBoxPtr main_text = NULL;
TextBoxPtr score_text = NULL;
TextBoxPtr help_text = NULL;

// Set into running mode
// play - game
void Mode_SetRunning(void)
{
  ant_globals->paused = 0;
  ant_globals->menu_mode = MENU_RUN_MODE;

  // Not the first time any more
  ant_globals->_menu_state = FIRST_TIME_FALSE;

  // NOTE NOTE NOTE NOTE!
  // reset the bots here
  // this may take 1a while
  Reset_NetworkBots();
}

// Super_MainText
void Super_MainText(void)
{
  main_text = InitTextBox(10, 200, 400, 480);
  SetTextMode(main_text, TEXT_NONE);
  SetTextColor(main_text, 0, 255, 0);
  Printf(main_text, "** DEBUG TEXT\n");

  // Now prepare the score text- upper left
  score_text = InitTextBox(10, 300, 10, 200);
  SetTextMode(score_text, TEXT_NONE);
  SetTextColor(score_text, 0, 255, 0);

  // Build the help screen text --
  help_text = InitTextBox(HELP_TEXT_LEFT, HELP_TEXT_RIGHT, HELP_TEXT_TOP, HELP_TEXT_BOTTOM);
  SetTextMode(help_text, TEXT_NONE);
  SetTextColor(help_text, 255, 255, 255);

  // Build the help text here
  Printf(help_text, "[ESC] - Main Screen\n\n");
  Printf(help_text, "[P] - Pause / Resume\n\n");
  Printf(help_text, "[Q] - Quit\n\n");
  Printf(help_text, "[TAB] - Cycle Camera Views\n\n");
  Printf(help_text, "[ARROW KEYS] - Move / Turn\n\n");
  Printf(help_text, "[SPACE] - Fire\n\n");
  Printf(help_text, "[S,F,R MOUSE] - Adjust View\n");
  Printf(help_text, "[M] - Mute / Unmute Audio\n\n");
}

// Draw_HelpScreen
void Draw_HelpScreen(void)
{
  glDisable(GL_TEXTURE_2D);

  // Draw a blue screen
  // in the background

  glColor3ub(20, 120, 235);

  glLineWidth(2.0f);
  glBegin(GL_QUADS);
  glVertex3f(HELP_BOX_LEFT, HELP_BOX_TOP, 0.0f);
  glVertex3f(HELP_BOX_RIGHT, HELP_BOX_TOP, 0.0f);

  glVertex3f(HELP_BOX_RIGHT, HELP_BOX_BOTTOM, 0.0f);
  glVertex3f(HELP_BOX_LEFT, HELP_BOX_BOTTOM, 0.0f);

  glEnd();

  // draw outline around object
  glColor3ub(255, 255, 255);
  glBegin(GL_LINE_LOOP);
  glVertex3f(HELP_BOX_LEFT, HELP_BOX_TOP, 0.0f);
  glVertex3f(HELP_BOX_RIGHT, HELP_BOX_TOP, 0.0f);

  glVertex3f(HELP_BOX_RIGHT, HELP_BOX_BOTTOM, 0.0f);
  glVertex3f(HELP_BOX_LEFT, HELP_BOX_BOTTOM, 0.0f);

  glEnd();

  glLineWidth(1.0f);

  TextBegin(help_text);
  DrawText(help_text);
  TextEnd();
}

// Super_Printf
void Super_Printf(const char* fmt, ...)
{
  va_list vlist;
  char buff[MAX_STR];

  // Get output string
  va_start(vlist, fmt);
  vsnprintf(buff, sizeof(buff), fmt, vlist);

  FormatStrCat(main_text, buff);

  va_end(vlist);
}

// Score
void Score_Printf(const char* fmt, ...)
{
  va_list vlist;
  char buff[MAX_STR];

  // Get output string
  va_start(vlist, fmt);
  vsnprintf(buff, sizeof(buff), fmt, vlist);

  FormatStrCat(score_text, buff);

  va_end(vlist);
}

// Super_BeginPaused
//  - once the game is started, begin in paused mode
void Super_BeginPaused(void) { ant_globals->paused = 1; }

// Print_Score
void Print_Score(void)
{
  int i = 0;
  char buffer[80];
  int x, y;

  for (i = 0; i < MAX_SCORE_DISPLAY; i++)
  {
    sprintf(buffer, "BUG\n");

    sprintf(buffer, "P%d %0.2f %d\n", ant_globals->score_obj[i].name,
            ant_globals->score_obj[i].score, ant_globals->score_obj[i].kills);

    TextBegin(score_text);
    DrawString(score_text, 10, 30 + (i * 10), buffer);
    TextEnd();

  }

  // Also, print frames per second
  sprintf(buffer, "FPS: %0.2f", framerate);
  TextBegin(score_text);
  DrawString(score_text, 500, 30, buffer);
  TextEnd();

  // And the score and health
  sprintf(buffer, "Armor: %0.2f", ant_globals->player_health);
  TextBegin(score_text);
  DrawString(score_text, 300, 30, buffer);
  TextEnd();

  // Print the current position
  sprintf(buffer, "%d/%d Bots", ant_globals->alive_bots, (MAX_FIRE_ANTS - 1));
  TextBegin(score_text);
  DrawString(score_text, 300, 44, buffer);
  TextEnd();

  // Print some paused text
  if (ant_globals->paused)
  {
    // find the correct x pos
    x = 12 * 8;
    x /= 2;

    x = (SCREEN_WIDTH / 2) - x;
    y = (SCREEN_HEIGHT / 2);

    // And the score and health
    sprintf(buffer, "- PAUSED -");
    TextBegin(score_text);
    DrawString(score_text, x, y, buffer);
    TextEnd();

    sprintf(buffer, "(GLANTS)");
    TextBegin(score_text);
    DrawString(score_text, x + 8, y + 10, buffer);
    TextEnd();

  }
}

// Draw Text
void Super_DrawText(void)
{
  TextBegin(main_text);
  DrawText(main_text);
  TextEnd();

  Print_NetRun();

  // Also placing print score
  // here, for some reason...
  Print_Score();
}

void Super_KillText(void)
{
  DestroyTextBox(main_text);
  DestroyTextBox(score_text);
  DestroyTextBox(help_text);
}

// Super
void Super_LoadGlobals(void)
{
  ant_globals = (AntGlobals*)malloc(sizeof(AntGlobals));

  ZeroMemory((AntGlobals*)ant_globals, sizeof(AntGlobals));
}

// Super
void Super_KillGlobals(void)
{
  // free the other allocated
  // arrays

  RELEASE_OBJECT(ant_globals->score_obj);

  RELEASE_OBJECT(ant_globals);
}

// Init Globals
void InitGlobals(void)
{
  ant_globals->alive_ants = MAX_FIRE_ANTS;
  ant_globals->seconds = 0;
  ant_globals->time_t = 0;
  ant_globals->nest_food = 0;
  ant_globals->garden = 0;
  ant_globals->player_health = 0;
  ant_globals->paused = 0;
  ant_globals->alive_bots = MAX_FIRE_ANTS - 1;

  ant_globals->menu_mode = MENU_TITLE_MODE;
  ant_globals->_menu_state = FIRST_TIME_TRUE;

  SET_NET_OFF;  // turn network interface off

  ant_globals->score_obj = (ScoreObj*)malloc(MAX_FIRE_ANTS * sizeof(ScoreObj));

  // load with zeros
  ZeroMemory(ant_globals->score_obj, MAX_FIRE_ANTS * sizeof(ScoreObj));
}

// TogglePaused
void TogglePaused(void)
{
  if (ant_globals->paused)
  {
    ant_globals->paused = 0;
  }
  else
  {
    ant_globals->paused = 1;
  }

}

// DestroyGlobals
void DestroyGlobals(void)
{
  // nada?
}

// Load_Score
void Load_Score(float score, float kills, int id, int i)
{
  ant_globals->score_obj[i].score = score;
  ant_globals->score_obj[i].kills = kills;
  ant_globals->score_obj[i].name = id;
}

// SubtractAnts
void SubtractAnts(int ants)
{
  if (ant_globals->alive_ants <= 0)
  {
    ant_globals->alive_ants = 0;
    return;
  }

  ant_globals->alive_ants -= ants;
}

// AddAnts
void AddAnts(int ants) { ant_globals->alive_ants += ants; }

// GetAnts
int GetAnts(void) { return ant_globals->alive_ants; }

// AddSeconds
void AddSeconds(float delta) { ant_globals->seconds += delta; }

// Add Time
// - time to render each frames
void AddTime(float delta) { ant_globals->time_t = delta; }

// Set GardenSize
void SetGardenSize(int v) { ant_globals->garden += v; }

// GetGarden
int GetGardenSize(void) { return ant_globals->garden; }

// SetNestfood
void SetNestFood(float d) { ant_globals->nest_food = d; }

// SetNestfood
void SetPlayerHealth(float d) { ant_globals->player_health = d; }

// AddTick
// - number of ticks in the game
void GameTick(void) { ant_globals->ticks += 1; }

// GetGameTick
DWORD GetGameTick(void) { return ant_globals->ticks; }

static const char* GetAliveStateLabel(int alive)
{
  return (alive == ALIVE_STATE) ? "ALIVE" : "DEAD";
}

// PrintGlobals
void PrintGlobals(void)
{
  int i = 0;
  const int x = 10;
  const int y0 = 80;
  const int dy = 14;

  glRasterPos2i(x, y0 + dy * 1);
  PrintText("ANTS: %d", ant_globals->alive_ants);

  glRasterPos2i(x, y0 + dy * 2);
  PrintText("TIME: %0.1f", ant_globals->seconds);

  glRasterPos2i(x, y0 + dy * 3);
  PrintText("TIME_T: %0.0f", ant_globals->time_t);

  glRasterPos2i(x, y0 + dy * 4);
  PrintText("FOOD: %0.0f", ant_globals->nest_food);

  glRasterPos2i(x, y0 + dy * 5);
  PrintText("GARD: %d", ant_globals->garden);

  glRasterPos2i(x, y0 + dy * 6);
  PrintText("BOT: %0.1f %0.1f", GetBotX(), GetBotY());

  glRasterPos2i(x, y0 + dy * 7);
  PrintText("LIFE: %0.1f", ant_globals->player_health);

  glRasterPos2i(x, y0 + dy * 8);
  {
    int vm = GetCurrentCameraNumber();
    const char* camName = (vm == 1) ? "THIRD" : (vm == 2) ? "FIRST" : (vm == 3) ? "DEMO" : (vm == 4) ? "CLOSE3" : "?";
    PrintText("CAM: %d (%s)", vm, camName);
  }

  glRasterPos2i(x, y0 + dy * 9);
  PrintText("POS: %0.1f %0.1f %0.1f", GetCameraX(), GetCameraY(), GetCameraZ());

  glRasterPos2i(x, y0 + dy * 10);
  PrintText("LOOK: %0.1f %0.1f %0.1f", GetCameraLookX(), GetCameraLookY(), GetCameraLookZ());

  for (i = 1; i < MAX_FIRE_ANTS; i++)
  {
    DriverBotPtr fire_ant = GetFireAnt(i);

    glRasterPos2i(x, y0 + dy * (10 + i));

    if (fire_ant == NULL)
    {
      PrintText("FA%d: NULL", i);
      continue;
    }

    {
      const float dx = fire_ant->x - GetCameraX();
      const float dz = fire_ant->y - GetCameraZ();
      const float dist = sqrtf((dx * dx) + (dz * dz));

      PrintText("FA%d: %s ST:%d HP:%0.1f POS:%0.1f,%0.1f CAM:%0.1f", i,
                GetAliveStateLabel(fire_ant->alive), fire_ant->state, fire_ant->food,
                fire_ant->x, fire_ant->y, dist);
    }
  }
}

// RESET_VALUE
// - if the config file is messed up
// set the appropriate variable
void Reset_Value(int id)
{
  if (id < 0) return;  // no negatives

  switch (id)
  {
    case ID_LINE_OF_SIGHT:

      LINE_OF_SIGHT = D_LINE_OF_SIGHT;
      break;

    case ID_ATTACK_RADIUS:
      ATTACK_RADIUS = D_ATTACK_RADIUS;
      break;

    case ID_BULLET_DAMAGE:
      BULLET_DAMAGE = D_BULLET_DAMAGE;
      break;

    case ID_MIN_BULLET_SPEED:
      MIN_BULLET_SPEED = D_MIN_BULLET_SPEED;
      break;

    case ID_USE_ANT_ENGINE:
      USE_ANT_ENGINE = D_USE_ANT_ENGINE;
      break;

    case ID_MAX_FIRE_ANTS:

      MAX_FIRE_ANTS = D_MAX_FIRE_ANTS;
      break;

    case ID_MAX_BOTS:

      MAX_BOTS = D_MAX_BOTS;

      break;

    case ID_USE_GARDEN_AREA:

      USE_GARDEN_AREA = D_USE_GARDEN_AREA;
      break;

    case ID_MAX_TRAIL_STACK:
      MAX_TRAIL_STACK = D_MAX_TRAIL_STACK;

      break;

    case ID_DYING_STATE:

      DYING_STATE = D_DYING_STATE;
      break;

    case ID_MAX_PHEROMONES:

      MAX_PHEROMONES = D_MAX_PHEROMONES;
      break;

    case ID_PHEROMONE_LIFE:

      PHEROMONE_LIFE = D_PHEROMONE_LIFE;

      break;

    case ID_PHEROMONE_DROP:

      PHEROMONE_DROP = D_PHEROMONE_DROP;
      break;

    case ID_MAX_BULLETS:
      MAX_BULLETS = D_MAX_BULLETS;
      break;

    case ID_MAX_FIRE_SPEED:
      MAX_FIRE_SPEED = D_MAX_FIRE_SPEED;

      break;

    case ID_MAX_GARDENS:

      MAX_GARDENS = D_MAX_GARDENS;

      break;

    case ID_BOT_SPEED:

      BOT_SPEED = D_BOT_SPEED;
      break;

    case ID_BOT_MAX_SPEED:

      BOT_MAX_SPEED = D_BOT_MAX_SPEED;

      break;

    case ID_MIN_TURN_SPEED:

      MIN_TURN_SPEED = D_MIN_TURN_SPEED;

      break;

    case ID_CHECK_RESPAWN:

      CHECK_RESPAWN = D_CHECK_RESPAWN;
      break;

    case ID_GARD_RESPAWN_RATE:

      GARD_RESPAWN_RATE = D_GARD_RESPAWN_RATE;

      break;

    case ID_MIN_STRAIGHT_STEPS:

      MIN_STRAIGHT_STEPS = D_MIN_STRAIGHT_STEPS;

      break;

    case ID_MAX_STRAIGHT_STEPS:

      MAX_STRAIGHT_STEPS = D_MAX_STRAIGHT_STEPS;
      break;

    case ID_MIN_STRAIGHT_STEPS_2:

      break;

    case ID_MAX_STRAIGHT_STEPS_2:

      MAX_STRAIGHT_STEPS_2 = D_MAX_STRAIGHT_STEPS_2;
      break;

    case ID_INITIAL_ANT_FOOD:

      INITIAL_ANT_FOOD = D_INITIAL_ANT_FOOD;
      break;

    case ID_INITIAL_GARD_FOOD:

      INITIAL_GARD_FOOD = D_INITIAL_GARD_FOOD;

      break;

    case ID_FOOD_WIDTH:
      FOOD_WIDTH = D_FOOD_WIDTH;
      break;

    case ID_INIT_FOOD_RATE:

      INIT_FOOD_RATE = D_INIT_FOOD_RATE;
      break;

    case ID_MAX_FOOD_RATE:

      MAX_FOOD_RATE = D_MAX_FOOD_RATE;
      break;

    case ID_MOVE_FOOD_RATE:
      MOVE_FOOD_RATE = D_MOVE_FOOD_RATE;
      break;

    case ID_FOOD_RATE:

      FOOD_RATE = D_FOOD_RATE;

      break;

    default:
      break;
  };
}

// Load all runtime config values directly from code defaults.
// This bypasses config parsing and keeps startup behavior deterministic.
void Load_ConfigDefaultsOnly(void)
{
  int index = 0;

  for (index = ID_LINE_OF_SIGHT; index < MAX_ERRORS; index++)
  {
    Reset_Value(index);
  }
}
