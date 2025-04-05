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
// commands.cpp
//
// The command interface
// to have a LaunchCommand
//

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <GL/gl.h>			// Header File For The OpenGL32 Library
#include <GL/glu.h>			// Header File For The GLu32 Library

#include "bot.h"


//
// GetStartCommand
//
int GetStartState(int cmd)
{
	switch(cmd)
	{
		case ATTACK_COMMAND:
			return RECHECK_STATE;
		break;

		case WANDER_COMMAND:
			return MOVE_STATE;
		break;

		case MOVE_COMMAND:
			return GENERATE_STATE;
		break;

		default: break;
	};

	return MOVE_STATE;
} // end of the function 
 

//
// GenerateCommand
// 
//
void Generate_Command(DriverBotPtr bot, int cmd)
{

	switch(cmd)
	{
		case ATTACK_COMMAND:
			bot->run = Attack_Command;
		break;

		case WANDER_COMMAND:
			bot->run = Wander_Command;
		break;

		case MOVE_COMMAND:
			bot->run = Move_Command;
		break;

		default: break;
	};

	bot->state = GetStartState(cmd);

	bot->last_command = bot->command;
	bot->command = cmd;

} // end of the function 
