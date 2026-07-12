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
#include <bot.hpp>
#include <gldrawlib.hpp>
#include <keys.hpp>
#include <legacy_stubs.hpp>
#include <camera.hpp>

AntGlobals* ant_globals;

// intro_str
char intro_str[] = {
    "glAnts is a mech game.\n"
    "It is loosely based on spectreVR.\n"
    "glAnts was created with GCCv2.96... \n"
    "just kidding, it will be though... \n"
    "visit glants.sourceforge.net for updates\n"
    "Linux version coming soon..."
    "\n(berlin ltd.)\n\n\n\n"
    "Microsoft has issued\n"
    "yet another critical update.\n"
    "Software Piracy is \nGood for Microsoft\n\n"};

// text used to fill the network window
char network_str_[26][80] = {"::Player Name: Player1",      // 0
                             "::IP Address: 127.0.0.1",     // 1
                             "::Send Bots Only: false",     // 2
                             "::Network Bots: 1",           // 3
                             "Total Players to Send: ",     // 4
                             "IP Address-",                 // 5
                             "Server-",                     // 6
                             "Ping-",                       // 7
                             "Cur/Max-",                    // 8
                             "Vers-",                       // 9
                             "OS",                          // 10
                             "Connect To Server",           // 11
                             "Server Settings",             // 12
                             "Client Settings",             // 13
                             "::Server Name: Server1",      // 14
                             "::Current IP: 127.0.0.1",     // 15
                             "::Use Bots Only: false",      // 16
                             "::Host Bots: 1",              // 17
                             "IP Address-",                 // 18
                             "Client-",                     // 19
                             "Ping-",                       // 20
                             "OS-",                         // 21
                             "  (Connect to Server)",       // 22
                             "  (Disconnect from Server)",  // 23
                             "  (Start Server)",            // 24
                             "  (Shutdown Server)"};        // 25

#define HORZ_TYPE_ 2
#define VERT_TYPE_ 4
#define STAT_TYPE_ 6

#define N_TITLE_X 200
#define N_TITLE_YY1 200
#define N_TITLE_YY2 220

// strlength is taken into consideration
#define TEXT_HORZ_START 60.0f
#define TEXT_HORZ_WIDTH 40
#define TEXT_VERT_START 60.0f
#define TEXT_VERT_STARTY2 240
#define TEXT_VERT_HEIGHT 16
#define TXT_V_START 160.0f
#define TXT_V_STARTYY2 330

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

static char* _tmp_str = NULL;
static clock_t curr_Time;
static clock_t next_Time = 0;

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
TextBoxPtr intro_text = NULL;
TextBoxPtr network_text = NULL;

// Set_TitleScreen
// - put the state in title screen mode
// - usaully used by the network interface
void Mode_TitleScreen(void)
{
  ant_globals->paused = 1;
  ant_globals->menu_mode = MENU_TITLE_MODE;
}

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
  // Reset_NetworkBots();  // Not needed for testing framework
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
  help_text = InitTextBox(230, 500, 180, 370);
  SetTextMode(help_text, TEXT_NONE);
  SetTextColor(help_text, 255, 255, 255);

  // Build the help text here
  Printf(help_text, "[ESC] - Main Screen\n\n");
  Printf(help_text, "[P] - Pause\n\n");
  Printf(help_text, "[Q] - Quit\n\n");
  Printf(help_text, "[TAB] - First/Third View Mode\n\n");
  Printf(help_text, "[ARROW KEYS] - Turn\n\n");
  Printf(help_text, "[SPACE] - Fire\n\n");
  Printf(help_text, "[S,F,R MOUSE] - Adjust View\n");
  Printf(help_text, "[F1] - Full Screen\n\n");

  // Intro text
  intro_text = InitTextBox(340, 630, 340, 430);
  SetTextMode(intro_text, TEXT_NONE);
  SetTextColor(intro_text, 255, 255, 255);

  _tmp_str = intro_str;

  // network_text
  network_text = InitTextBox(340, 630, 340, 430);
  SetTextMode(network_text, TEXT_NONE);
  SetTextColor(network_text, 255, 255, 255);
}

// Draw_TString
void Draw_TString(int x, int y, const char* str)
{
  TextBegin(network_text);
  DrawString(network_text, x, y, str);
  TextEnd();

}

// Set_Net_Menu(int
// - setup the horizontal or vertical positions
// and draw the text --
static void Setup_NetMenu(int start_pos, int end_pos, int h_type)
{
  int i;
  float horz_pos;
  float vert_pos;

  // set the text changes before the code below

  switch (h_type)
  {
    case HORZ_TYPE_:

      // we only have two diferent cases to handle
      if (start_pos == C_HORZ_MENU)
      {
        horz_pos = TEXT_HORZ_START;
        vert_pos = TXT_V_START;

        TextBegin(network_text);
        DrawString(network_text, horz_pos, vert_pos, network_str_[C_HORZ_MENU]);
        TextEnd();

        // next col --
        horz_pos = T_COL_1;
        vert_pos = TXT_V_START;

        TextBegin(network_text);
        DrawString(network_text, horz_pos, vert_pos, network_str_[C_HORZ_MENU + 1]);
        TextEnd();

        // next col --
        horz_pos = T_COL_2;
        vert_pos = TXT_V_START;

        TextBegin(network_text);
        DrawString(network_text, horz_pos, vert_pos, network_str_[C_HORZ_MENU + 2]);
        TextEnd();

        // next col --
        horz_pos = T_COL_3;
        vert_pos = TXT_V_START;

        TextBegin(network_text);
        DrawString(network_text, horz_pos, vert_pos, network_str_[C_HORZ_MENU + 3]);
        TextEnd();

        // next col --
        horz_pos = T_COL_4;
        vert_pos = TXT_V_START;

        TextBegin(network_text);
        DrawString(network_text, horz_pos, vert_pos, network_str_[C_HORZ_MENU + 4]);
        TextEnd();

        // next col --
        horz_pos = T_COL_5;
        vert_pos = TXT_V_START;

        TextBegin(network_text);
        DrawString(network_text, horz_pos, vert_pos, network_str_[C_HORZ_MENU + 5]);
        TextEnd();

      }
      else
      {
        horz_pos = TEXT_HORZ_START;
        vert_pos = TXT_V_STARTYY2;
        TextBegin(network_text);
        DrawString(network_text, horz_pos, vert_pos, network_str_[S_SERV_HORZ]);
        TextEnd();

        // next col
        horz_pos = T_COL_1;
        vert_pos = TXT_V_STARTYY2;

        TextBegin(network_text);
        DrawString(network_text, horz_pos, vert_pos, network_str_[S_SERV_HORZ + 1]);
        TextEnd();

        horz_pos = T_COL_2;
        vert_pos = TXT_V_STARTYY2;

        TextBegin(network_text);
        DrawString(network_text, horz_pos, vert_pos, network_str_[S_SERV_HORZ + 2]);
        TextEnd();

        horz_pos = T_COL_3;
        vert_pos = TXT_V_STARTYY2;

        TextBegin(network_text);
        DrawString(network_text, horz_pos, vert_pos, network_str_[S_SERV_HORZ + 3]);
        TextEnd();

      }

      break;

    case VERT_TYPE_:

      if (start_pos == CLIENT_NET_MENU)
        vert_pos = TEXT_VERT_START;
      else
        vert_pos = TEXT_VERT_STARTY2;

      for (i = start_pos; i <= end_pos; i++)
      {
        TextBegin(network_text);
        DrawString(network_text, TEXT_HORZ_START, vert_pos, network_str_[i]);
        TextEnd();

        vert_pos = vert_pos + TEXT_VERT_HEIGHT;

      }

      break;

    case STAT_TYPE_:
      break;

    case SERVER_SETTINGS:
      TextBegin(network_text);
      DrawString(network_text, start_pos, end_pos, network_str_[SERVER_SETTINGS]);
      TextEnd();

      break;

    case CLIENT_SETTINGS:
      TextBegin(network_text);
      DrawString(network_text, start_pos, end_pos, network_str_[CLIENT_SETTINGS]);
      TextEnd();

      break;
  };
}

// Draw_NetworkScreen
void Draw_NetworkScreen(void)
{
  const float box_x_min = 50.0f;
  const float box_x_max = 500.0f;
  const float box_y_min = 50.0f;
  const float box_y_max = 440.0f;

  const float msg_y = box_y_max - 32.0f;

  char buffer[80];

  glDisable(GL_TEXTURE_2D);

  // Draw a blue screen
  // in the background

  glColor3ub(20, 120, 235);

  glLineWidth(3.0f);
  glBegin(GL_QUADS);
  glVertex3f(box_x_min, box_y_min, 0.0f);
  glVertex3f(box_x_max, box_y_min, 0.0f);

  glVertex3f(box_x_max, box_y_max, 0.0f);
  glVertex3f(box_x_min, box_y_max, 0.0f);

  glEnd();

  // draw outline around object
  glColor3ub(255, 255, 255);
  glBegin(GL_LINE_LOOP);
  glVertex3f(box_x_min, box_y_min, 0.0f);
  glVertex3f(box_x_max, box_y_min, 0.0f);

  glVertex3f(box_x_max, box_y_max, 0.0f);
  glVertex3f(box_x_min, box_y_max, 0.0f);
  glEnd();

  // The next line
  // will be used for the message screen
  glBegin(GL_LINE_LOOP);
  glVertex3f(box_x_min, msg_y, 0.0f);
  glVertex3f(box_x_max, msg_y, 0.0f);
  glEnd();

  glLineWidth(1.0f);

  // before drawing the name, save selection
  Display_NetSel(network_str_);

  // Print the actual text
  Setup_NetMenu(CLIENT_NET_MENU, END_CLIENT_NET, VERT_TYPE_);
  Setup_NetMenu(C_HORZ_MENU, END_CLIENT_NET, HORZ_TYPE_);

  Setup_NetMenu(N_TITLE_X, N_TITLE_YY2, SERVER_SETTINGS);
  Setup_NetMenu(SERVER_NET_MENU, END_SERVER_NET, VERT_TYPE_);
  Setup_NetMenu(S_SERV_HORZ, E_SERV_HORZ, HORZ_TYPE_);

  // Draw the static text
  TextBegin(network_text);
  DrawString(network_text, T_COL_6, T_ROW_1, network_str_[_TEXT_CONNECT_]);
  TextEnd();

  TextBegin(network_text);
  DrawString(network_text, T_COL_7, T_ROW_1, network_str_[_TEXT_DISCONNECT_]);
  TextEnd();

  // Server buttons --
  TextBegin(network_text);
  DrawString(network_text, T_COL_6, T_ROW_2, network_str_[_TEXT_START_]);
  TextEnd();

  TextBegin(network_text);
  DrawString(network_text, T_COL_7, T_ROW_2, network_str_[_TEXT_SHUTDOWN_]);
  TextEnd();

  // And at the bottom
  // the network and help screens and messages
  TextBegin(network_text);

  // get network message
  Get_NetworkMsg(buffer);
  DrawString(network_text, TEXT_HORZ_START, T_ROW_3, buffer);
  TextEnd();

  // print connections if there are any
  printConnections();

}

// Draw_HelpScreen
void Draw_IntroScreen(void)
{
  glDisable(GL_TEXTURE_2D);

  // Draw a blue screen
  // in the background
  glColor3ub(20, 120, 235);

  glLineWidth(2.0f);
  glBegin(GL_QUADS);
  glVertex3f(320.0f, 320.0f, 0.0f);
  glVertex3f(635.0f, 320.0f, 0.0f);

  glVertex3f(635.0f, 440.0f, 0.0f);
  glVertex3f(320.0f, 440.0f, 0.0f);

  glEnd();

  // draw outline around object
  glColor3ub(255, 255, 255);
  glBegin(GL_LINE_LOOP);
  glVertex3f(320.0f, 320.0f, 0.0f);
  glVertex3f(635.0f, 320.0f, 0.0f);

  glVertex3f(635.0f, 440.0f, 0.0f);
  glVertex3f(320.0f, 440.0f, 0.0f);

  glEnd();

  glLineWidth(1.0f);

  TextBegin(intro_text);

  // curr_Time = GetTickCount();
  curr_Time = clock();

  if (curr_Time > next_Time)
  {
    Printf(intro_text, "%c", *_tmp_str);

    next_Time = curr_Time + (800 * 1.4);

    // delay a little bit
    if (*(_tmp_str) == ')') next_Time = curr_Time + (8000 * 1.4);

    // update the char
    if (*(++_tmp_str) == '\0') _tmp_str = intro_str;

  }

  DrawText(intro_text);
  TextEnd();
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
  glVertex3f(220.0f, 160.0f, 0.0f);
  glVertex3f(480.0f, 160.0f, 0.0f);

  glVertex3f(480.0f, 320.0f, 0.0f);
  glVertex3f(220.0f, 320.0f, 0.0f);

  glEnd();

  // draw outline around object
  glColor3ub(255, 255, 255);
  glBegin(GL_LINE_LOOP);
  glVertex3f(220.0f, 160.0f, 0.0f);
  glVertex3f(480.0f, 160.0f, 0.0f);

  glVertex3f(480.0f, 320.0f, 0.0f);
  glVertex3f(220.0f, 320.0f, 0.0f);

  glEnd();

  glLineWidth(1.0f);

  TextBegin(help_text);
  DrawText(help_text);
  TextEnd();
}

// Super_Printf
// - print the debug text screen
void Super_Printf(const char* fmt, ...)
{
  va_list vlist;
  char buff[MAX_STR];

  // Get output string
  va_start(vlist, fmt);
  vsprintf(buff, fmt, vlist);

  FormatStrCat(main_text, buff);

  va_end(vlist);
}

// Score
void Score_Printf(char* fmt, ...)
{
  va_list vlist;
  char buff[MAX_STR];

  // Get output string
  va_start(vlist, fmt);
  vsprintf(buff, fmt, vlist);

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
  DestroyTextBox(intro_text);

  DestroyTextBox(network_text);
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
