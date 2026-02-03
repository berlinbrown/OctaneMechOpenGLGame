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
#ifndef _DEFAULTS_H_
#define _DEFAULTS_H_

#define D_LINE_OF_SIGHT 14.0f
#define D_ATTACK_RADIUS 4.0f;
#define D_BULLET_DAMAGE 280.0f  // 200=750 pretty good
#define D_MIN_BULLET_SPEED 0.30f;
#define D_USE_ANT_ENGINE 0
#define D_MAX_FIRE_ANTS 4  // default: 10
#define D_MAX_BOTS 120
#define D_USE_GARDEN_AREA 1
#define D_MAX_TRAIL_STACK 300
#define D_DYING_STATE 390
#define D_MAX_PHEROMONES 200
#define D_PHEROMONE_LIFE 1000
#define D_PHEROMONE_DROP 40
#define D_MAX_BULLETS 20
#define D_MAX_FIRE_SPEED 10
#define D_MAX_GARDENS 35  // 48 default
#define D_BOT_SPEED 0.09f
#define D_BOT_MAX_SPEED 0.14f
#define D_MIN_TURN_SPEED 1.1f;  // default:0.8
#define D_CHECK_RESPAWN 600
#define D_GARD_RESPAWN_RATE 15  // default: 32
#define D_MIN_STRAIGHT_STEPS 60

#define D_MAX_STRAIGHT_STEPS 200
#define D_MIN_STRAIGHT_STEPS_2 150
#define D_MAX_STRAIGHT_STEPS_2 360
#define D_INITIAL_ANT_FOOD 1000
#define D_INITIAL_GARD_FOOD 7000  // default: 3000-8000
#define D_FOOD_WIDTH 0.3f
#define D_INIT_FOOD_RATE 480  // realistically this should be low
#define D_MAX_FOOD_RATE 850   // but 400-1000 is ok
#define D_MOVE_FOOD_RATE 1.7f
#define D_FOOD_RATE 0.4f  // 0.35-0.8

#endif
