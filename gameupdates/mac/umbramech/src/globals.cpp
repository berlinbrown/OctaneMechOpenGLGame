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
// globals.cpp
//

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include <sys/time.h>
#include <math.h>

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

#define CONFIG_FILE_NAME "config.ini"
#define ERROR_FILE_NAME "error.log"

FILE *f_config = NULL;
FILE *f_errorlog = NULL;
FILE *f_newfile = NULL;

// false = no error
bool config_errors[MAX_ERRORS];

AntGlobals *ant_globals;

char error_str[MAX_ERRORS][40] = {
	"NULL",
	"LINE_OF_SIGHT",
	"ATTACK_RADIUS",
	"BULLET_DAMAGE",
	"MIN_BULLET_SPEED",
	"USE_ANT_ENGINE",
	"MAX_FIRE_ANTS",
	"MAX_BOTS",
	"USE_GARDEN_AREA",
	"MAX_TRAIL_STACK",
	"DYING_STATE",
	"MAX_PHEROMONES",
	"PHEROMONE_LIFE",
	"PHEROMONE_DROP",
	"MAX_BULLETS",
	"MAX_FIRE_SPEED",
	"MAX_GARDENS",
	"BOT_SPEED",
	"BOT_MAX_SPEED",
	"MIN_TURN_SPEED",
	"CHECK_RESPAWN",
	"GARD_RESPAWN_RATE",
	"MIN_STRAIGHT_STEPS",
	"MAX_STRAIGHT_STEPS",
	"MIN_STRAIGHT_STEPS_2",
	"MAX_STRAIGHT_STEPS_2",
	"INITIAL_ANT_FOOD",
	"INITIAL_GARD_FOOD",
	"FOOD_WIDTH",
	"INIT_FOOD_RATE",
	"MAX_FOOD_RATE",
	"MOVE_FOOD_RATE",
	"FOOD_RATE"};

//
// Config msgs
//
#define MAX_TXT_MSGS 40
char text_msg[MAX_TXT_MSGS][80] =
	{
		"#\n",
		"# glAnts - original config.ini\n",
		"# file, change at your own risk\n",
		"#\n",
		"# if you remove this file, the program\n",
		"# will create another one with the defaults\n",
		"# so removing this file can be a good thing\n",
		"#\n",
		"#\n",
		"# Also of note, if the system finds an error\n",
		"# it goes with the default variable\n",
		"#\n",
		"# FORMAT has to be [VARIABLE_NAME]=VALUE;\n",
		"# oh yeah, add the 'f' tag for float\n",
		"# \n",
		"# [VARIABLE_NAME]=VALUEf;\n",
		"# \n",
		"# COMMENTS are '#'\n",
		"#\n",
		"# Remove the comments below are your own risk\n",
		"# they basically just define the default values\n",
		"# used, if you remove them then you wont know\n",
		"# what good values will look like\n",
		"#\n",
		"# The only things that really need changing\n",
		"# are:\n",
		"#\n",
		"# USE_ANT_ENGINE: take out the little ants \n",
		"#\n",
		"# MAX_FIRE_ANTS: max fighter ants\n",
		"# \n",
		"# MAX_BOTS: number of worker ants\n",
		"#\n",
		"# INITIAL_ANT_FOOD: the health of the fighter\n",
		"#					ants and the workers\n",
		"#					try 10,000 and they will never die\n",
		"#\n",
		"# Also, you may comment one of the variables\n",
		"# and get the default once the program runs\n",
		"#\n"};

//
// intro_str
//
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

//
// text used to fill the network window
//
char network_str_[26][80] = {
	"::Player Name: Player1",	  // 0
	"::IP Address: 127.0.0.1",	  // 1
	"::Send Bots Only: false",	  // 2
	"::Network Bots: 1",		  // 3
	"Total Players to Send: ",	  // 4
	"IP Address-",				  // 5
	"Server-",					  // 6
	"Ping-",					  // 7
	"Cur/Max-",					  // 8
	"Vers-",					  // 9
	"OS",						  // 10
	"Connect To Server",		  // 11
	"Server Settings",			  // 12
	"Client Settings",			  // 13
	"::Server Name: Server1",	  // 14
	"::Current IP: 127.0.0.1",	  // 15
	"::Use Bots Only: false",	  // 16
	"::Host Bots: 1",			  // 17
	"IP Address-",				  // 18
	"Client-",					  // 19
	"Ping-",					  // 20
	"OS-",						  // 21
	"  (Connect to Server)",	  // 22
	"  (Disconnect from Server)", // 23
	"  (Start Server)",			  // 24
	"  (Shutdown Server)"};		  // 25

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

static char *_tmp_str = NULL;
static clock_t curr_Time;
static clock_t next_Time = 0;

// WRAPPER FUNCTIONS FOR TEXT--

// There are three different text areas
// The top, left text contains the score
// the top, right text contains the fps and any
//	other system specific data --
//
// NOTE: use Super_Printf for debug code
//		- you also have to call drawText to
//		  actually render to screen
//

TextBoxPtr main_text = NULL;
TextBoxPtr score_text = NULL;
TextBoxPtr help_text = NULL;
TextBoxPtr intro_text = NULL;
TextBoxPtr network_text = NULL;

//
// Set_TitleScreen
// - put the state in title screen mode
// - usaully used by the network interface
//
void Mode_TitleScreen(void)
{

	ant_globals->paused = 1;
	ant_globals->menu_mode = MENU_TITLE_MODE;

} // end of the function

//
// Set into running mode
// play - game
//
void Mode_SetRunning(void)
{

	ant_globals->paused = 0;
	ant_globals->menu_mode = MENU_RUN_MODE;

	// Not the first time any more
	ant_globals->_menu_state = FIRST_TIME_FALSE;

	// NOTE NOTE NOTE NOTE!
	// reset the bots here
	// this may take 1a while
	// Reset_FireAnts();
	Reset_NetworkBots();

} // end of the function

//
// Super_MainText
//
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

	//
	// Build the help text here
	//
	Printf(help_text, "[ESC] - Main Screen\n\n");
	Printf(help_text, "[P] - Pause\n\n");
	Printf(help_text, "[Q] - Quit\n\n");
	Printf(help_text, "[TAB] - First/Third View Mode\n\n");
	Printf(help_text, "[ARROW KEYS] - Turn\n\n");
	Printf(help_text, "[SPACE] - Fire\n\n");
	Printf(help_text, "[S,F,R MOUSE] - Adjust View\n");
	Printf(help_text, "[F1] - Full Screen\n\n");

	//
	// Intro text
	//
	intro_text = InitTextBox(340, 630, 340, 430);
	SetTextMode(intro_text, TEXT_NONE);
	SetTextColor(intro_text, 255, 255, 255);

	_tmp_str = intro_str;

	//
	// network_text
	//
	network_text = InitTextBox(340, 630, 340, 430);
	SetTextMode(network_text, TEXT_NONE);
	SetTextColor(network_text, 255, 255, 255);

} // end of the function

//
// Draw_TString
void Draw_TString(int x, int y, char *str)
{
	TextBegin(network_text);
	DrawString(network_text, x, y, str);
	TextEnd();

} // end of the func

//
// Set_Net_Menu(int
// - setup the horizontal or vertical positions
// and draw the text --
//
static void Setup_NetMenu(int start_pos, int end_pos, int h_type)
{

	int i;
	float horz_pos;
	float vert_pos;

	//
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

		} // end of the
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

		} // end of the if-else

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

		} // end of the for

		break;

	case STAT_TYPE_:
		break;

	case SERVER_SETTINGS:
		TextBegin(network_text);
		DrawString(network_text, start_pos,
				   end_pos, network_str_[SERVER_SETTINGS]);
		TextEnd();

		break;

	case CLIENT_SETTINGS:
		TextBegin(network_text);
		DrawString(network_text, start_pos,
				   end_pos, network_str_[CLIENT_SETTINGS]);
		TextEnd();

		break;
	};

} // end of the function

//
// Draw_NetworkScreen
//
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
	DrawString(network_text, T_COL_6,
			   T_ROW_1, network_str_[_TEXT_CONNECT_]);
	TextEnd();

	TextBegin(network_text);
	DrawString(network_text, T_COL_7,
			   T_ROW_1, network_str_[_TEXT_DISCONNECT_]);
	TextEnd();

	// Server buttons --
	TextBegin(network_text);
	DrawString(network_text, T_COL_6,
			   T_ROW_2, network_str_[_TEXT_START_]);
	TextEnd();

	TextBegin(network_text);
	DrawString(network_text, T_COL_7,
			   T_ROW_2, network_str_[_TEXT_SHUTDOWN_]);
	TextEnd();

	// And at the bottom
	// the network and help screens and messages
	TextBegin(network_text);

	// get network message
	Get_NetworkMsg(buffer);
	DrawString(network_text, TEXT_HORZ_START,
			   T_ROW_3, buffer);
	TextEnd();

	// print connections if there are any
	printConnections();

} // end of the functino

//
// Draw_HelpScreen
//
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

	// DrawText(intro_text);
	// curr_Time = GetTickCount();
	curr_Time = clock();

	if (curr_Time > next_Time)
	{

		Printf(intro_text, "%c", *_tmp_str);

		next_Time = curr_Time + (800 * 1.4);

		// delay a little bit
		if (*(_tmp_str) == ')')
			next_Time = curr_Time + (8000 * 1.4);

		// update the char
		if (*(++_tmp_str) == '\0')
			_tmp_str = intro_str;

	} // end of the if

	DrawText(intro_text);

} // end of the function

//
// Draw_HelpScreen
//
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

	DrawText(help_text);

} // end of the function

//
// Super_Printf
// - print the debug text screen
//
void Super_Printf(char *fmt, ...)
{
	va_list vlist;
	char buff[MAX_STR];

	// Get output string
	va_start(vlist, fmt);
	vsprintf(buff, fmt, vlist);

	FormatStrCat(main_text, buff);

	va_end(vlist);

} // end of the function

//
// Score
//
void Score_Printf(char *fmt, ...)
{
	va_list vlist;
	char buff[MAX_STR];

	// Get output string
	va_start(vlist, fmt);
	vsprintf(buff, fmt, vlist);

	FormatStrCat(score_text, buff);

	va_end(vlist);

} // end of the function

//
// Super_BeginPaused
//  - once the game is started, begin in paused mode
//
void Super_BeginPaused(void)
{
	ant_globals->paused = 1;

} // end of the fuction

//
// Print_Score
//
void Print_Score(void)
{
	int i = 0;
	char buffer[80];
	int x, y;

	for (i = 0; i < MAX_SCORE_DISPLAY; i++)
	{
		sprintf(buffer, "BUG\n");

		sprintf(buffer, "P%d %0.2f %d\n",
				ant_globals->score_obj[i].name,
				ant_globals->score_obj[i].score,
				ant_globals->score_obj[i].kills);

		TextBegin(score_text);
		DrawString(score_text, 10, 4 + (i * 10), buffer);
		TextEnd();

	} // end of the for

	// Also, print frames per second
	sprintf(buffer, "FPS: %0.2f", framerate);
	TextBegin(score_text);
	DrawString(score_text, 500, 4, buffer);
	TextEnd();

	// And the score and health
	sprintf(buffer, "Armor: %0.2f", ant_globals->player_health);
	TextBegin(score_text);
	DrawString(score_text, 300, 4, buffer);
	TextEnd();

	// Print the current position
	sprintf(buffer, "%d/%d Bots",
			ant_globals->alive_bots, (MAX_FIRE_ANTS - 1));
	TextBegin(score_text);
	DrawString(score_text, 300, 14, buffer);
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

	} // end of the if

} // end of the function

//
// Draw Text
//
void Super_DrawText(void)
{
	DrawText(main_text);

	Print_NetRun();

	// Also placing print score
	// here, for some reason...
	Print_Score();

} // end of the function

//
// Destroy_MainText(void)
//
void Super_KillText(void)
{

	DestroyTextBox(main_text);
	DestroyTextBox(score_text);
	DestroyTextBox(help_text);
	DestroyTextBox(intro_text);

	DestroyTextBox(network_text);

} // end of the function

//
// Super
//
void Super_LoadGlobals(void)
{
	ant_globals = (AntGlobals *)malloc(sizeof(AntGlobals));

	ZeroMemory((AntGlobals *)ant_globals,
			   sizeof(AntGlobals));
} // end of the function

//
// Super
//
void Super_KillGlobals(void)
{
	// free the other allocated
	// arrays
	// free(ant_globals->score_obj);
	// free(ant_globals);

	RELEASE_OBJECT(ant_globals->score_obj);

	RELEASE_OBJECT(ant_globals);

} // end of the function

//
// Init Globals
//
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

	SET_NET_OFF; // turn network interface off

	ant_globals->score_obj = (struct tagScoreObj *)malloc(
		MAX_FIRE_ANTS * sizeof(struct tagScoreObj));

	// load with zeros
	ZeroMemory(ant_globals->score_obj,
			   MAX_FIRE_ANTS * sizeof(ScoreObj));

} // end of the function

//
// TogglePaused
//
void TogglePaused(void)
{

	if (ant_globals->paused)
	{

		ant_globals->paused = 0;
	}
	else
	{

		ant_globals->paused = 1;
	} // end of if-else

} // end of the ufnctio

//
// DestroyGlobals
//
void DestroyGlobals(void)
{
	// nada?
} // end of the function

//
// Load_Score
//
void Load_Score(float score, float kills, int id, int i)
{

	ant_globals->score_obj[i].score = score;
	ant_globals->score_obj[i].kills = kills;
	ant_globals->score_obj[i].name = id;

} // end of the function

//
// SubtractAnts
//
void SubtractAnts(int ants)
{
	if (ant_globals->alive_ants <= 0)
	{
		ant_globals->alive_ants = 0;
		return;
	} // end of if

	ant_globals->alive_ants -= ants;

} // end of the function

//
// AddAnts
//
void AddAnts(int ants)
{

	ant_globals->alive_ants += ants;

} // end of the function

//
// GetAnts
//
int GetAnts(void)
{
	return ant_globals->alive_ants;
} // end of the functino

//
// AddSeconds
//
void AddSeconds(float delta)
{
	ant_globals->seconds += delta;
} // end of the function

//
// Add Time
//
// - time to render each frames
//
void AddTime(float delta)
{
	ant_globals->time_t = delta;
} // end of the function

//
// Set GardenSize
//
void SetGardenSize(int v)
{
	ant_globals->garden += v;

} // end of func

//
// GetGarden
//
int GetGardenSize(void)
{
	return ant_globals->garden;
} // end of func

//
// SetNestfood
//
void SetNestFood(float d)
{
	ant_globals->nest_food = d;

} // end of function

//
// SetNestfood
//
void SetPlayerHealth(float d)
{
	ant_globals->player_health = d;

} // end of function

//
// AddTick
// - number of ticks in the game
void GameTick(void)
{
	ant_globals->ticks += 1;

} // end of the function

//
// GetGameTick
//
DWORD GetGameTick(void)
{
	return ant_globals->ticks;
} // end of the functino

//
// PrintGlobals
//
void PrintGlobals(void)
{
	int i = 0;

	glRasterPos2i(10, 48 + 14);
	PrintText("ANTS: %d", ant_globals->alive_ants);

	glRasterPos2i(10, 48 + 28);
	PrintText("TIME: %0.1f", ant_globals->seconds);

	glRasterPos2i(10, 48 + 42);
	PrintText("TIME_T: %0.0f", ant_globals->time_t);

	glRasterPos2i(10, 48 + 56);
	PrintText("FOOD: %0.0f", ant_globals->nest_food);

	glRasterPos2i(10, 48 + 70);
	PrintText("GARD: %d", ant_globals->garden);

	// note: hud_NEST_FOOD must be turned off
	glRasterPos2i(10, 48 + 56);
	PrintText("x: %0.2f y: %0.2f", GetBotX(), GetBotY());

	glRasterPos2i(10, 48 + 70);
	PrintText("LIFE: %0.2f", ant_globals->player_health);

} // end of the function

//
// CONFIG LOADING FUNCTIONS
//

//
// SetErrorLog
// - call before Load configfile
// or place it in load configfile, whatever
//
// hopefully, run-time errors will not occur
//
void Set_ErrorLog(void)
{
	int i;

	// false means no error
	//
	// we will assume that there were errors
	// except for the file open
	for (i = 0; i < MAX_ERRORS; i++)
		config_errors[i] = true;

	config_errors[ID_FILE_NOT_FOUND] = false;

	f_errorlog = fopen(ERROR_FILE_NAME, "w");

	fprintf(f_errorlog, "-- FILE_LOADED\n\n");

	fclose(f_errorlog);

} // end of the function

//
// Add_ErrorLog
// - add an entry to the error log
//
void Add_ErrorStr(char *str)
{
	f_errorlog = NULL;

	f_errorlog = fopen(ERROR_FILE_NAME, "a");

	fprintf(f_errorlog, "%s\n", str);

	fclose(f_errorlog);

} // end of the functino

//
// Add_ErrorLog
// - add an entry to the error log
//
void Add_ErrorLog(const int error_id)
{
	f_errorlog = NULL;

	f_errorlog = fopen(ERROR_FILE_NAME, "a");

	config_errors[error_id] = true;

	switch (error_id)
	{
	case ID_FILE_NOT_FOUND:
		fprintf(f_errorlog, "error: config FILE_NOT_FOUND\n");
		break;

	case ID_LINE_OF_SIGHT:
		fprintf(f_errorlog, "error: LINE_OF_SIGHT\n");
		break;

	case ID_ATTACK_RADIUS:
		fprintf(f_errorlog, "error: ATTACK_RADIUS\n");
		break;

	case ID_BULLET_DAMAGE:
		fprintf(f_errorlog, "error: BULLET_DAMAGE\n");
		break;

	case ID_MIN_BULLET_SPEED:
		fprintf(f_errorlog, "error: MIN_BULLET_SPEED\n");
		break;

	case ID_USE_ANT_ENGINE:
		fprintf(f_errorlog, "error: USE_ANT_ENGINE:\n");
		break;

	case ID_MAX_FIRE_ANTS:
		fprintf(f_errorlog, "error: MAX_FIRE_ANTS\n");
		break;

	case ID_MAX_BOTS:
		fprintf(f_errorlog, "error: MAX_BOTS\n");
		break;

	case ID_USE_GARDEN_AREA:
		fprintf(f_errorlog, "error: USE_GARDEN_AREA\n");
		break;

	case ID_MAX_TRAIL_STACK:
		fprintf(f_errorlog, "error: MAX_TRAIL_STACK\n");
		break;

	case ID_DYING_STATE:
		fprintf(f_errorlog, "error: DYING_STATE\n");
		break;

	case ID_MAX_PHEROMONES:
		fprintf(f_errorlog, "error: MAX_PHEROMONES\n");
		break;

	case ID_PHEROMONE_LIFE:
		fprintf(f_errorlog, "error: PHEROMONE_LIFE\n");
		break;

	case ID_PHEROMONE_DROP:
		fprintf(f_errorlog, "error: PHEROMONE_DROP\n");
		break;

	case ID_MAX_BULLETS:
		fprintf(f_errorlog, "error: MAX_BULLETS\n");
		break;

	case ID_MAX_FIRE_SPEED:
		fprintf(f_errorlog, "error: MAX_FIRE_SPEED\n");
		break;

	case ID_MAX_GARDENS:
		fprintf(f_errorlog, "error: MAX_GARDENS\n");
		break;

	case ID_BOT_SPEED:
		fprintf(f_errorlog, "error: BOT_SPEED\n");
		break;

	case ID_BOT_MAX_SPEED:
		fprintf(f_errorlog, "error: BOT_MAX_SPEED\n");
		break;

	case ID_MIN_TURN_SPEED:
		fprintf(f_errorlog, "error: MIN_TURN_SPEED\n");
		break;

	case ID_CHECK_RESPAWN:
		fprintf(f_errorlog, "error: CHECK_RESPAWN\n");
		break;

	case ID_GARD_RESPAWN_RATE:
		fprintf(f_errorlog, "error: GARD_RESPAWN_RATE\n");
		break;

	case ID_MIN_STRAIGHT_STEPS:
		fprintf(f_errorlog, "error: MIN_STRAIGHT_STEPS\n");
		break;

	case ID_MAX_STRAIGHT_STEPS:
		fprintf(f_errorlog, "error: MAX_STRAIGHT_STEPS\n");
		break;

	case ID_MIN_STRAIGHT_STEPS_2:
		fprintf(f_errorlog, "error: MIN_STRAIGHT_STEPS_2:\n");
		break;

	case ID_MAX_STRAIGHT_STEPS_2:
		fprintf(f_errorlog, "error: MAX_STRAIGHT_STEPS_2\n");
		break;

	case ID_INITIAL_ANT_FOOD:
		fprintf(f_errorlog, "error: INITIAL_ANT_FOOD\n");
		break;

	case ID_INITIAL_GARD_FOOD:
		fprintf(f_errorlog, "error: INITIAL_GARD_FOOD\n");
		break;

	case ID_FOOD_WIDTH:
		fprintf(f_errorlog, "error: FOOD_WIDTH\n");
		break;

	case ID_INIT_FOOD_RATE:
		fprintf(f_errorlog, "error: INIT_FOOD_RATE\n");
		break;

	case ID_MAX_FOOD_RATE:
		fprintf(f_errorlog, "error: MAX_FOOD_RATE\n");
		break;

	case ID_MOVE_FOOD_RATE:
		fprintf(f_errorlog, "error: MOVE_FOOD_RATE\n");
		break;

	case ID_FOOD_RATE:
		fprintf(f_errorlog, "error: FOOD_RATE\n");
		break;

	default:
		fprintf(f_errorlog, "error: INVALID_ERROR_CODE\n");
		break;
	};

	fclose(f_errorlog);

} // end of the function

//
// Set_Variable
// - set one of the variables
// based on the incoming buffer
//
void Set_Variable(int id, char *val, int f_flag)
{
	float f_val = 0;
	int i_val = 0;

	if (id < 0)
		return; // no negatives

	//
	// also go ahead and set the error
	// should probably set below
	// but that would take more effort
	config_errors[id] = false;

	if (f_flag)
		f_val = atof(val);
	else
		i_val = atoi(val);

	switch (id)
	{
	case ID_LINE_OF_SIGHT:

		if (f_flag)
			LINE_OF_SIGHT = f_val;
		else
			LINE_OF_SIGHT = i_val;
		break;

	case ID_ATTACK_RADIUS:
		if (f_flag)
			ATTACK_RADIUS = f_val;
		else
			ATTACK_RADIUS = i_val;
		break;

	case ID_BULLET_DAMAGE:
		if (f_flag)
			BULLET_DAMAGE = f_val;
		else
			BULLET_DAMAGE = i_val;
		break;

	case ID_MIN_BULLET_SPEED:
		if (f_flag)
			MIN_BULLET_SPEED = f_val;
		else
			MIN_BULLET_SPEED = i_val;
		break;

	case ID_USE_ANT_ENGINE:
		if (f_flag)
			USE_ANT_ENGINE = f_val;
		else
			USE_ANT_ENGINE = i_val;
		break;

	case ID_MAX_FIRE_ANTS:
		if (f_flag)
			MAX_FIRE_ANTS = f_val;
		else
			MAX_FIRE_ANTS = i_val;

		break;

	case ID_MAX_BOTS:
		if (f_flag)
			MAX_BOTS = f_val;
		else
			MAX_BOTS = i_val;
		break;

	case ID_USE_GARDEN_AREA:
		if (f_flag)
			USE_GARDEN_AREA = f_val;
		else
			USE_GARDEN_AREA = i_val;
		break;

	case ID_MAX_TRAIL_STACK:
		if (f_flag)
			MAX_TRAIL_STACK = f_val;
		else
			MAX_TRAIL_STACK = i_val;
		break;

	case ID_DYING_STATE:
		if (f_flag)
			DYING_STATE = f_val;
		else
			DYING_STATE = i_val;
		break;

	case ID_MAX_PHEROMONES:
		if (f_flag)
			MAX_PHEROMONES = f_val;
		else
			MAX_PHEROMONES = i_val;
		break;

	case ID_PHEROMONE_LIFE:
		if (f_flag)
			PHEROMONE_LIFE = f_val;
		else
			PHEROMONE_LIFE = i_val;
		break;

	case ID_PHEROMONE_DROP:
		if (f_flag)
			PHEROMONE_DROP = f_val;
		else
			PHEROMONE_DROP = i_val;
		break;

	case ID_MAX_BULLETS:
		if (f_flag)
			MAX_BULLETS = f_val;
		else
			MAX_BULLETS = i_val;
		break;

	case ID_MAX_FIRE_SPEED:
		if (f_flag)
			MAX_FIRE_SPEED = f_val;
		else
			MAX_FIRE_SPEED = i_val;
		break;

	case ID_MAX_GARDENS:
		if (f_flag)
			MAX_GARDENS = f_val;
		else
			MAX_GARDENS = i_val;
		break;

	case ID_BOT_SPEED:
		if (f_flag)
			BOT_SPEED = f_val;
		else
			BOT_SPEED = i_val;
		break;

	case ID_BOT_MAX_SPEED:
		if (f_flag)
			BOT_MAX_SPEED = f_val;
		else
			BOT_MAX_SPEED = i_val;
		break;

	case ID_MIN_TURN_SPEED:
		if (f_flag)
			MIN_TURN_SPEED = f_val;
		else
			MIN_TURN_SPEED = i_val;
		break;

	case ID_CHECK_RESPAWN:
		if (f_flag)
			CHECK_RESPAWN = f_val;
		else
			CHECK_RESPAWN = i_val;
		break;

	case ID_GARD_RESPAWN_RATE:

		if (f_flag)
			GARD_RESPAWN_RATE = f_val;
		else
			GARD_RESPAWN_RATE = i_val;
		break;

	case ID_MIN_STRAIGHT_STEPS:
		if (f_flag)
			MIN_STRAIGHT_STEPS = f_val;
		else
			MIN_STRAIGHT_STEPS = i_val;
		break;

	case ID_MAX_STRAIGHT_STEPS:
		if (f_flag)
			MAX_STRAIGHT_STEPS = f_val;
		else
			MAX_STRAIGHT_STEPS = i_val;
		break;

	case ID_MIN_STRAIGHT_STEPS_2:
		if (f_flag)
			MIN_STRAIGHT_STEPS_2 = f_val;
		else
			MIN_STRAIGHT_STEPS_2 = i_val;
		break;

	case ID_MAX_STRAIGHT_STEPS_2:
		if (f_flag)
			MAX_STRAIGHT_STEPS_2 = f_val;
		else
			MAX_STRAIGHT_STEPS_2 = i_val;
		break;

	case ID_INITIAL_ANT_FOOD:
		if (f_flag)
			INITIAL_ANT_FOOD = f_val;
		else
			INITIAL_ANT_FOOD = i_val;
		break;

	case ID_INITIAL_GARD_FOOD:
		if (f_flag)
			INITIAL_GARD_FOOD = f_val;
		else
			INITIAL_GARD_FOOD = i_val;
		break;

	case ID_FOOD_WIDTH:
		if (f_flag)
			FOOD_WIDTH = f_val;
		else
			FOOD_WIDTH = i_val;
		break;

	case ID_INIT_FOOD_RATE:
		if (f_flag)
			INIT_FOOD_RATE = f_val;
		else
			INIT_FOOD_RATE = i_val;
		break;

	case ID_MAX_FOOD_RATE:
		if (f_flag)
			MAX_FOOD_RATE = f_val;
		else
			MAX_FOOD_RATE = i_val;
		break;

	case ID_MOVE_FOOD_RATE:
		if (f_flag)
			MOVE_FOOD_RATE = f_val;
		else
			MOVE_FOOD_RATE = i_val;
		break;

	case ID_FOOD_RATE:
		if (f_flag)
			FOOD_RATE = f_val;
		else
			FOOD_RATE = i_val;
		break;

	default:
		break;
	};

} // end of the function

#define V_TRUE *f_flag = 1
//
//
// Get ConfValue
// Note: this file allocates memory
//
void *Get_ConfValue(int id, int *f_flag)
{
	float *f_val = NULL;
	int *i_val = NULL;

	if (id < 0)
		return NULL; // no negatives

	switch (id)
	{
	case ID_LINE_OF_SIGHT:
		f_val = (float *)malloc(sizeof(float));
		*f_val = D_LINE_OF_SIGHT;

		V_TRUE;
		return f_val;
		break;

	case ID_ATTACK_RADIUS:
		f_val = (float *)malloc(sizeof(float));
		*f_val = D_ATTACK_RADIUS;

		V_TRUE;
		return f_val;
		break;

	case ID_BULLET_DAMAGE:
		f_val = (float *)malloc(sizeof(float));
		*f_val = D_BULLET_DAMAGE;

		V_TRUE;
		return f_val;
		break;

	case ID_MIN_BULLET_SPEED:
		f_val = (float *)malloc(sizeof(float));
		*f_val = D_MIN_BULLET_SPEED;

		V_TRUE;
		return f_val;
		break;

	case ID_USE_ANT_ENGINE:
		i_val = (int *)malloc(sizeof(int));
		*i_val = D_USE_ANT_ENGINE;
		return i_val;
		break;

	case ID_MAX_FIRE_ANTS:
		i_val = (int *)malloc(sizeof(int));

		*i_val = D_MAX_FIRE_ANTS;

		return i_val;
		break;

	case ID_MAX_BOTS:
		i_val = (int *)malloc(sizeof(int));
		*i_val = D_MAX_BOTS;

		return i_val;
		break;

	case ID_USE_GARDEN_AREA:
		i_val = (int *)malloc(sizeof(int));
		*i_val = D_USE_GARDEN_AREA;
		return i_val;
		break;

	case ID_MAX_TRAIL_STACK:

		i_val = (int *)malloc(sizeof(int));
		*i_val = D_MAX_TRAIL_STACK;

		return i_val;

		break;

	case ID_DYING_STATE:

		i_val = (int *)malloc(sizeof(int));
		*i_val = D_DYING_STATE;
		return i_val;
		break;

	case ID_MAX_PHEROMONES:
		i_val = (int *)malloc(sizeof(int));
		*i_val = D_MAX_PHEROMONES;

		return i_val;
		break;

	case ID_PHEROMONE_LIFE:

		i_val = (int *)malloc(sizeof(int));
		*i_val = D_PHEROMONE_LIFE;

		return i_val;
		break;

	case ID_PHEROMONE_DROP:
		i_val = (int *)malloc(sizeof(int));
		*i_val = D_PHEROMONE_DROP;

		return i_val;
		break;

	case ID_MAX_BULLETS:
		i_val = (int *)malloc(sizeof(int));
		*i_val = D_MAX_BULLETS;

		return i_val;
		break;

	case ID_MAX_FIRE_SPEED:
		i_val = (int *)malloc(sizeof(int));
		*i_val = D_MAX_FIRE_SPEED;

		return i_val;

		break;

	case ID_MAX_GARDENS:

		i_val = (int *)malloc(sizeof(int));
		*i_val = D_MAX_GARDENS;

		return i_val;

		break;

	case ID_BOT_SPEED:
		f_val = (float *)malloc(sizeof(float));
		*f_val = D_BOT_SPEED;

		V_TRUE;
		return f_val;
		break;

	case ID_BOT_MAX_SPEED:

		f_val = (float *)malloc(sizeof(float));
		*f_val = D_BOT_MAX_SPEED;

		V_TRUE;

		return f_val;

		break;

	case ID_MIN_TURN_SPEED:

		f_val = (float *)malloc(sizeof(float));
		*f_val = D_MIN_TURN_SPEED;

		V_TRUE;

		return f_val;
		break;

	case ID_CHECK_RESPAWN:
		i_val = (int *)malloc(sizeof(int));
		*i_val = D_CHECK_RESPAWN;

		return i_val;
		break;

	case ID_GARD_RESPAWN_RATE:

		i_val = (int *)malloc(sizeof(int));
		*i_val = D_GARD_RESPAWN_RATE;

		return i_val;

		break;

	case ID_MIN_STRAIGHT_STEPS:

		i_val = (int *)malloc(sizeof(int));
		*i_val = D_MIN_STRAIGHT_STEPS;

		return i_val;

		break;

	case ID_MAX_STRAIGHT_STEPS:

		i_val = (int *)malloc(sizeof(int));
		*i_val = D_MAX_STRAIGHT_STEPS;

		return i_val;
		break;

	case ID_MIN_STRAIGHT_STEPS_2:
		i_val = (int *)malloc(sizeof(int));
		*i_val = D_MIN_STRAIGHT_STEPS_2;

		return i_val;

		break;

	case ID_MAX_STRAIGHT_STEPS_2:

		i_val = (int *)malloc(sizeof(int));
		*i_val = D_MAX_STRAIGHT_STEPS_2;

		return i_val;
		break;

	case ID_INITIAL_ANT_FOOD:

		i_val = (int *)malloc(sizeof(int));
		*i_val = D_INITIAL_ANT_FOOD;

		return i_val;
		break;

	case ID_INITIAL_GARD_FOOD:

		i_val = (int *)malloc(sizeof(int));
		*i_val = D_INITIAL_GARD_FOOD;

		return i_val;
		break;

	case ID_FOOD_WIDTH:
		f_val = (float *)malloc(sizeof(float));
		*f_val = D_FOOD_WIDTH;

		V_TRUE;

		return f_val;
		break;

	case ID_INIT_FOOD_RATE:
		i_val = (int *)malloc(sizeof(int));
		*i_val = D_INIT_FOOD_RATE;
		return i_val;
		break;

	case ID_MAX_FOOD_RATE:

		i_val = (int *)malloc(sizeof(int));
		*i_val = D_MAX_FOOD_RATE;
		return i_val;
		break;

	case ID_MOVE_FOOD_RATE:
		f_val = (float *)malloc(sizeof(float));
		*f_val = D_MOVE_FOOD_RATE;

		V_TRUE;

		return f_val;
		break;

	case ID_FOOD_RATE:

		f_val = (float *)malloc(sizeof(float));

		*f_val = D_FOOD_RATE;

		V_TRUE;

		return f_val;
		break;

	default:
		break;
	};

	return NULL;

} // end of the function

//
//
// RESET_VALUE
// - if the config file is messed up
// set the appropriate variable
//
void Reset_Value(int id)
{
	if (id < 0)
		return; // no negatives

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

} // end of the function

//
// Process_ConfigFile
//
int Process_ConfigFile(char *buffer)
{
	int i;
	int res;

	// not case sensitive

	for (i = 0; i < MAX_ERRORS; i++)
	{
		res = strcasecmp(buffer, error_str[i]);

		if (res == 0)
		{
			return i;
		} // end of the if

	} // end of the for

	return -1;

} // end of the function

//
// void Read_ConfigFile
// -
//
// if the error tally comes up
// with any errors it will have
// to create a default variable, sorry
//
void Read_ConfigFile(FILE *f)
{
	char c;

	char buffer[256];
	int c_index = 0;
	int res = 0;
	char var[80];
	int float_flag = 0;

	int lines_read = 0;

	while (!feof(f))
	{

		c = fgetc(f);

		switch (c)
		{

		case '#':

			while (1)
			{
				c = fgetc(f);

				if (c == '\n')
					break;

			} // end of the while

			break;

		case '[':

			c_index = 0;

			while (1)
			{
				c = fgetc(f);

				if (c == ']')
					break;

				buffer[c_index] = c;
				c_index++;

				if (c == '\n')
				{
					lines_read++;
					break;
				} // end of if

			} // end of the while

			// null terminate the command
			buffer[c_index] = '\0';

			res = Process_ConfigFile(buffer);

			// now add the variable with the value
			//
			c = fgetc(f);

			// get the equals
			c_index = 0;
			float_flag = 0;

			while (1)
			{
				c = fgetc(f);

				// we found a float
				if (c == 'f')
				{
					float_flag = 1;
					break;
				} // end of the if

				// break on a ';'
				if (c == ';')
					break;

				var[c_index] = c;
				c_index++;

				if (c == '\n')
					break;

			} // end of the while

			var[c_index] = '\0';

			// the final step, send in a
			// variable
			Set_Variable(res, var, float_flag);

			break;

		default:
			break;

		}; // end switch

	} // end of the while

} // end of the function

//
//
// Rewrite File
//
// if the file doesnt exist
//
void Rewrite_File(void)
{
	int i = 0;
	int f_flag = 0;

	// who needs types
	void *tmp = NULL;
	float *tmp_f = NULL;
	int *tmp_i = NULL;

	f_newfile = fopen(CONFIG_FILE_NAME, "w");

	for (i = 0; i < MAX_TXT_MSGS; i++)
	{
		fprintf(f_newfile, "%s", text_msg[i]);
	} // end of the for

	for (i = 0; i < MAX_ERRORS; i++)
	{
		if (i == 0)
			continue;

		fprintf(f_newfile, "[");
		fprintf(f_newfile, "%s", error_str[i]);
		fprintf(f_newfile, "]=");

		f_flag = 0;

		tmp = Get_ConfValue(i, &f_flag);

		if (f_flag)
		{
			tmp_f = (float *)tmp;
			fprintf(f_newfile, "%0.2ff;\n", *tmp_f);
		}
		else
		{
			tmp_i = (int *)tmp;
			fprintf(f_newfile, "%d;\n", *tmp_i);
		} // end of if-else

		// free(tmp);
		RELEASE_OBJECT(tmp);
		tmp = NULL;

	} // end of the for

	fprintf(f_newfile, "\n\n");

	fclose(f_newfile);

} // end of the function

//
// Check_Errors
// - the config file is messed up somehow
// we need to make sure we have value
// stored for that variable
//
void Check_Errors(void)
{
	int index = 0;

	for (index = 0; index < MAX_ERRORS; index++)
	{
		if (index == ID_FILE_NOT_FOUND)
			continue;

		if (config_errors[index])
		{
			// write the file --
			Add_ErrorLog(index);

			// now get the variable
			Reset_Value(index);

		} // end of the if

	} // end of the for

} // end of the function

//
// load
//
void Load_ConfigFile(void)
{

	Set_ErrorLog(); // set up the error log

	f_config = fopen(CONFIG_FILE_NAME, "r");

	if (f_config == NULL)
	{
		Add_ErrorLog(ID_FILE_NOT_FOUND);

		// create a new file config.ini
		Rewrite_File();

		// Read_ConfigFile(f_newfile);

		// Note: I left the comment above
		// because you may need it later
		// right now, the code writes a file
		// based on the default values
		// then closes the new file
		//
		// Another approach might be to write
		// the new file and then reread the new
		// file and check the variables
		// it is really a matter of
		// "if there are bugs then check here"
		//

		Check_Errors();

		return;

	} // end of the function

	Read_ConfigFile(f_config);

	fclose(f_config);

	Check_Errors();

} // end of the function
