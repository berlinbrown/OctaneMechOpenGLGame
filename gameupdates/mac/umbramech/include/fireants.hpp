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

// fireants.h
// constants that deal with the
// enemy char
#pragma once

constexpr int PLAYER_0 = 0;

// the drawing size
// in x/z
constexpr float FIRE_ANT_SIZE = 1.5f;

// length of the vision
constexpr float PERSPECTIVE_Z = 600.0f;

// first person mode camera
constexpr float FIRST_PERSON_Z = 4.3f;
constexpr float FIRST_HEIGHT = 2.4f;

constexpr int DRAW_LINE_SIGHT = 1;

constexpr float LIGHT_HEIGHT_0 = 100.0f;
constexpr float LIGHT_HEIGHT_1 = 180.0f;

// max distance a bullet can travel
constexpr float MAX_BULLET_TRAVEL = 600.0f;

// bullet height
constexpr float BULLET_H1 = 1.8f;
constexpr float BULLET_H2 = 2.3f;

constexpr float BULLET_LEN = 6.0f;

// For the hex grid
constexpr float HEX_SIZE = 12.0f;

// the height should be 4 times othe size
constexpr float HEX_HEIGHT = 35.0f;

constexpr float WORLD_X_MIN = -300.0f;
constexpr float WORLD_X_MAX = 300.0f;

constexpr float WORLD_Y_MIN = -300.0f;
constexpr float WORLD_Y_MAX = 300.0f;

constexpr int VK_ESC = 0;
constexpr int VK_F1 = 1;
constexpr int VK_f = 3;
constexpr int VK_UP = 7;
constexpr int VK_DOWN = 8;
constexpr int VK_LEFT = 9;
constexpr int VK_RIGHT = 10;
constexpr int VK_s = 11;
constexpr int VK_S = 12;
constexpr int VK_r = 13;
constexpr int VK_R = 14;
constexpr int VK_F = 15;
constexpr int VK_SPACE = 16;

void Reset_NetworkBots(void);

void Prepare_DemoMode(void);

void Run_NetworkBots(void);

extern int keyCodes[20];