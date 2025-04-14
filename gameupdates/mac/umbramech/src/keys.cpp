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
// keys.c
//
// a typing interface
// for chat or other purposes
// - mostly used with the networking settings
//
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <float.h> // used for _control

#include <OpenGL/gl.h>      // Core OpenGL functions
#include <OpenGL/glu.h>     // OpenGL Utility Library
#include <GLUT/glut.h>      // GLUT for window/context

// bad, bad! using globals
extern char network_str_[22][80];

static int current_mode = SEL_CLIENT_MODE;
static int current_sel = START_SEL;
static int current_char[4][2];

// the different key entry types
static int client_sel[6] = {
    SEL_TYPE_TEXT,
    SEL_TYPE_IP,
    SEL_TYPE_BOOL,
    SEL_TYPE_NUMERIC,
    SEL_TYPE_APPLY,
    SEL_TYPE_APPLY};

static int client_str[6] = {
    CLIENT_NET_MENU,
    CLIENT_NET_MENU + 1,
    CLIENT_NET_MENU + 2,
    CLIENT_NET_MENU + 3,
    _TEXT_CONNECT_,
    _TEXT_DISCONNECT_};

// server variables //
static int server_str[6] = {
    SERVER_NET_MENU,
    SERVER_NET_MENU + 1,
    SERVER_NET_MENU + 2,
    SERVER_NET_MENU + 3,
    _TEXT_START_,
    _TEXT_SHUTDOWN_};

//
// final_str
static char final_str[4][80];

//
// The string has to begin at random points
//
static int client_start[4] = {
    15,
    14,
    18,
    16};

static int server_start[4] = {
    15,
    14,
    17,
    13};

static int server_sel[6] = {
    SEL_TYPE_TEXT,
    SEL_TYPE_NOCHANGE,
    SEL_TYPE_BOOL,
    SEL_TYPE_NUMERIC,
    SEL_TYPE_APPLY,
    SEL_TYPE_APPLY};

static void Dec_Selection(void);
static void Inc_Selection(void);

static void Reset_StartChar(void)
{
  int i = 0;

  for (i = 0; i < 4; i++)
  {
    current_char[i][0] = 0;
    current_char[i][1] = 0;
  } // end of the for

} // end of func

//
// Finalize_Selection
// - connect/disconnect/etc
//
static void Finalize_Selection(void)
{
  int text_start = 0;
  int tmp_selection = 0;
  int tmp_mode = 0;
  int i = 0, j = 0;
  char c = ' ';

  // the mode effects how to handle input characters
  if (current_mode == SEL_CLIENT_MODE)
  {

    tmp_mode = 0;

    // collect the current user strings
    for (i = 0; i < 4; i++)
    {
      tmp_selection = client_str[i];
      text_start = client_start[i];

      c = ' ';
      for (j = 0; c != '\0'; j++)
      {

        c = network_str_[tmp_selection][text_start + j];
        final_str[i][j] = c;

      } // end of the inner for

      // make sure the string is null terminated
      final_str[i][j] = '\0';

    } // end of the for
  }
  else
  {

    tmp_mode = 1;

    // collect the current user strings
    for (i = 0; i < 4; i++)
    {
      tmp_selection = server_str[i];
      text_start = server_start[i];

      c = ' ';
      for (j = 0; c != '\0'; j++)
      {

        c = network_str_[tmp_selection][text_start + j];
        final_str[i][j] = c;

      } // end of the inner for

      // make sure the string is null terminated
      final_str[i][j] = '\0';

    } // end of the for

  } // end of th if - else

} // end of the function

//
// Char_Keys(
// - handle normal keys
void Alpha_Keys(char *buffer)
{
  char c;
  int input_type = 0;
  int text_start = 0;
  int char_start = 0;
  int char_i;
  int tmp_mode = 0;
  int tmp_selection = 0;
  int i = 0;

  c = buffer[0];

  switch (c)
  {
  case '\n':
  case '\r':
    if ((current_sel >= 0) && (current_sel < 4))
    {
      // simple I think
      Inc_Selection();
    } // end of teh if

    if (current_sel == 4)
    {

      // process the selection
      if (current_mode == SEL_CLIENT_MODE)
      {
        Finalize_Selection();

        // connect to server, the wheels are in motion now
        Start_Service(final_str, SEL_CLIENT_MODE);
      }
      else if (current_mode == SEL_SERVER_MODE)
      {
        Finalize_Selection();

        // start the server, watch the messages at bottom screen
        Start_Service(final_str, SEL_SERVER_MODE);
      } // end of if-else

    } // end of if selection = 4

    break;

  default:
    char_i = 0;

    // the mode effects how to handle input characters
    if (current_mode == SEL_CLIENT_MODE)
    {
      input_type = client_sel[current_sel];

      if ((current_sel >= 0) && (current_sel < 4))
      {
        text_start = client_start[current_sel];
        char_i = current_sel;
        tmp_selection = client_str[current_sel];
      } // end o if

      tmp_mode = 0;
    }
    else
    {

      input_type = server_sel[current_sel];
      if ((current_sel >= 0) && (current_sel < 4))
      {
        text_start = server_start[current_sel];
        char_i = current_sel;
        tmp_selection = server_str[current_sel];
      } // end of if

      tmp_mode = 1;

    } // end of the if - else

    switch (input_type)
    {
    case SEL_TYPE_TEXT:

      if (((c >= 'a') && (c <= 'z')) ||
          ((c >= 'A') && (c <= 'Z')))
      {
        // confusing yet?
        char_start = current_char[char_i][tmp_mode];
        network_str_[tmp_selection][text_start + char_start] = c;

        current_char[char_i][tmp_mode]++;
        if (current_char[char_i][tmp_mode] >= MAX_INPUT_STR)
          current_char[char_i][tmp_mode] = MAX_INPUT_STR;

        // and dont forget to add a null terminator
        network_str_[tmp_selection][text_start + char_start + 1] = '\0';

      } // end of if
      break;

    case SEL_TYPE_IP:

      if (((c >= '0') && (c <= '9')) || (c == '.'))
      {
        // confusing yet?
        char_start = current_char[char_i][tmp_mode];
        network_str_[tmp_selection][text_start + char_start] = c;

        current_char[char_i][tmp_mode]++;
        if (current_char[char_i][tmp_mode] >= MAX_INPUT_STR)
          current_char[char_i][tmp_mode] = MAX_INPUT_STR;

        // and dont forget to add a null terminator
        network_str_[tmp_selection][text_start + char_start + 1] = '\0';

      } // end of if
      break;

      break;

    case SEL_TYPE_NOCHANGE:

      break;

    case SEL_TYPE_BOOL:

      // clear everything that is already there

      if (((c >= 'a') && (c <= 'z')) ||
          ((c >= 'A') && (c <= 'Z')))
      {
        // this is easy
        // we have only two values
        // rotate them
        // the letters used are random

        char_start = 0;

        Super_Printf("%c", network_str_[tmp_selection][text_start + char_start]);

        if (network_str_[tmp_selection][text_start + char_start] == 'f')
        {

          for (i = 0; i < 10; i++)
            network_str_[tmp_selection][text_start + i] = '\0';

          network_str_[tmp_selection][text_start + char_start + 0] = 't';
          network_str_[tmp_selection][text_start + char_start + 1] = 'r';
          network_str_[tmp_selection][text_start + char_start + 2] = 'u';
          network_str_[tmp_selection][text_start + char_start + 3] = 'e';
          network_str_[tmp_selection][text_start + char_start + 4] = '\0';
        }
        else
        {

          for (i = 0; i < 10; i++)
            network_str_[tmp_selection][text_start + i] = '\0';

          network_str_[tmp_selection][text_start + char_start] = 'f';
          network_str_[tmp_selection][text_start + char_start + 1] = 'a';
          network_str_[tmp_selection][text_start + char_start + 2] = 'l';
          network_str_[tmp_selection][text_start + char_start + 3] = 's';
          network_str_[tmp_selection][text_start + char_start + 4] = 'e';
          network_str_[tmp_selection][text_start + char_start + 5] = '\0';
        } // end of if else

      } // end of teh if

      break;

    case SEL_TYPE_NUMERIC:

      if ((c >= '0') && (c <= '9'))
      {
        // confusing yet?
        char_start = current_char[char_i][tmp_mode];
        network_str_[tmp_selection][text_start + char_start] = c;

        current_char[char_i][tmp_mode]++;
        if (current_char[char_i][tmp_mode] >= MAX_INPUT_STR)
          current_char[char_i][tmp_mode] = MAX_INPUT_STR;

        // and dont forget to add a null terminator
        network_str_[tmp_selection][text_start + char_start + 1] = '\0';

      } // end of if

      break;

    case SEL_TYPE_APPLY:
      break;

    default:
      break;
    };

    break;
  };

} // end of teh functino

//
// Display_NetSel
// - display the selected network text
//
void Display_NetSel(char str[26][80])
{
  int i;
  // reset back to the normal setting
  // reset the other client strings
  for (i = 0; i < 4; i++)
  {
    str[client_str[i]][0] = ':';
    str[client_str[i]][1] = ':';
  } // end of the for

  for (i = 0; i < 4; i++)
  {
    str[server_str[i]][0] = ':';
    str[server_str[i]][1] = ':';

  } // end of the for

  // clear the connect strings
  for (i = 4; i < 6; i++)
  {
    str[client_str[i]][0] = ' ';
    str[client_str[i]][1] = ' ';

    str[server_str[i]][0] = ' ';
    str[server_str[i]][1] = ' ';

  } // end of the for

  if (current_mode == SEL_CLIENT_MODE)
  {

    // change the string to signify selection
    if ((current_sel >= 0) && (current_sel < 4))
    {
      str[client_str[current_sel]][0] = '>';
      str[client_str[current_sel]][1] = '>';
    } // end of the if

    if ((current_sel >= 4) && (current_sel < 6))
    {
      str[client_str[current_sel]][0] = '>';
      str[client_str[current_sel]][1] = '>';
    } // end of the if
  }
  else if (current_mode == SEL_SERVER_MODE)
  {

    if ((current_sel >= 0) && (current_sel < 4))
    {
      str[server_str[current_sel]][0] = '>';
      str[server_str[current_sel]][1] = '>';
    } // end of the if

    if ((current_sel >= 4) && (current_sel < 6))
    {
      str[server_str[current_sel]][0] = '>';
      str[server_str[current_sel]][1] = '>';
    } // end of the if

  } // end of if-else

} // end of the function

//
// Inc_Selection
//
static void Inc_Selection(void)
{
  int max1 = 0;

  Reset_StartChar();

  if (current_mode == SEL_SERVER_MODE)
  {

    max1 = SEL_SERVER_MAX + 1;
  }
  else if (current_mode == SEL_CLIENT_MODE)
  {

    max1 = SEL_CLIENT_MAX + 1;

  } // end of if-else

  current_sel++;
  if (current_sel >= max1)
  {
    current_sel = START_SEL;
  } // end of the if

} // end of the function

//
// Dec_Selection
//
static void Dec_Selection(void)
{
  int max1 = 0;

  Reset_StartChar();

  if (current_mode == SEL_SERVER_MODE)
  {

    max1 = SEL_SERVER_MAX;
  }
  else if (current_mode == SEL_CLIENT_MODE)
  {

    max1 = SEL_CLIENT_MAX;

  } // end of if-else

  current_sel--;
  if (current_sel < 0)
  {
    current_sel = max1;
  } // end of the if

} // end of the func

//
// Cmd_Keys
//
void Cmd_Keys(KeySym key)
{

  // make sure we are in the correct mode
  if (ant_globals->menu_mode == MENU_SETTINGS_MODE)
  {
    switch (key)
    {
    case XK_Down:

      Inc_Selection();

      break;

    case XK_Up:

      Dec_Selection();

      break;

    case XK_Tab:
      // client mode
      if (current_mode == SEL_SERVER_MODE)
        current_mode = SEL_CLIENT_MODE;
      else
        current_mode = SEL_SERVER_MODE;
      break;

    case XK_Return:
    case XK_space:
      break;

    default:
      break;
    };

  } // end of the if

} // end of the function
