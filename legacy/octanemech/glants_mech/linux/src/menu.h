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
// menu.h
//
#ifndef _MENU_H_
#define _MENU_H_

#include "bot.h"

// menu mode
#define MENU_TITLE_MODE 1
#define MENU_RUN_MODE 2
#define MENU_HELP_MODE 3
#define MENU_SETTINGS_MODE 4

#define NEW_GAME_ID 0
#define EXIT_ID 1
#define HELP_ID 2
#define SETTINGS_ID 3
#define DEMO_ID 4

// These are used with _menu_state
#define MENU_DEAD_MODE 4
#define FIRST_TIME_TRUE 1
#define FIRST_TIME_FALSE 2

void Toggle_MenuItems(int dir);
bool Set_MenuMode(void);
void Reset_DeadText(void);

#endif
