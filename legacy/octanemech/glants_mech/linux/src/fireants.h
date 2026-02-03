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
// fireants.h
//
// constants that deal with the
// enemy char
//
#ifndef _FIREANTS_H_
#define _FIREANTS_H_

#define PLAYER_0 0

//
// the drawing size
// in x/z
//
#define FIRE_ANT_SIZE 1.5f

// length of the vision
#define PERSPECTIVE_Z 600.0f

// first person mode camera
#define FIRST_PERSON_Z 4.3f
#define FIRST_HEIGHT 2.4f

#define DRAW_LINE_SIGHT 1

#define LIGHT_HEIGHT_0 100.0f
#define LIGHT_HEIGHT_1 180.0f

//
// max distance a bullet can travel
//
#define MAX_BULLET_TRAVEL 600.0f

// bullet height
#define BULLET_H1 1.8f
#define BULLET_H2 2.3f

#define BULLET_LEN 6.0f

//
// For the hex grid
#define HEX_SIZE 12.0f

// the height should be 4 times othe size
#define HEX_HEIGHT 35.0f

#define WORLD_X_MIN -300.0f
#define WORLD_X_MAX 300.0f

#define WORLD_Y_MIN -300.0f
#define WORLD_Y_MAX 300.0f

#define VK_ESC 0
#define VK_F1 1
#define VK_f 3
#define VK_UP 7
#define VK_DOWN 8
#define VK_LEFT 9
#define VK_RIGHT 10
#define VK_s 11
#define VK_S 12
#define VK_r 13
#define VK_R 14
#define VK_F 15
#define VK_SPACE 16

void Reset_NetworkBots(void);

//
void Prepare_DemoMode(void);

void Run_NetworkBots(void);

extern int keyCodes[20];

#endif
