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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <globals.hpp>
#include <defaults.hpp>
#include <gldrawlib.hpp>
#include <legacy_stubs.hpp>
#include <camera.hpp>

AntGlobals* ant_globals;

void Super_LoadGlobals(void)
{
  ant_globals = (AntGlobals*)malloc(sizeof(AntGlobals));
  ZeroMemory((AntGlobals*)ant_globals, sizeof(AntGlobals));
}

void InitGlobals(void)
{
  ant_globals->alive_ants  = MAX_FIRE_ANTS;
  ant_globals->seconds     = 0;
  ant_globals->time_t      = 0;
  ant_globals->nest_food   = 0;
  ant_globals->garden      = 0;
  ant_globals->player_health = 0;
  ant_globals->paused      = 0;
  ant_globals->alive_bots  = MAX_FIRE_ANTS - 1;

  ant_globals->menu_mode   = MENU_TITLE_MODE;
  ant_globals->_menu_state = FIRST_TIME_TRUE;

  SET_NET_OFF;

  ant_globals->score_obj = (ScoreObj*)malloc(MAX_FIRE_ANTS * sizeof(ScoreObj));
  ZeroMemory(ant_globals->score_obj, MAX_FIRE_ANTS * sizeof(ScoreObj));
}

void SubtractAnts(int ants)
{
  if (ant_globals->alive_ants <= 0)
  {
    ant_globals->alive_ants = 0;
    return;
  }
  ant_globals->alive_ants -= ants;
}

void AddAnts(int ants)    { ant_globals->alive_ants += ants; }
int  GetAnts(void)        { return ant_globals->alive_ants; }
void SetNestFood(float d) { ant_globals->nest_food = d; }
void SetGardenSize(int v) { ant_globals->garden += v; }
int  GetGardenSize(void)  { return ant_globals->garden; }

