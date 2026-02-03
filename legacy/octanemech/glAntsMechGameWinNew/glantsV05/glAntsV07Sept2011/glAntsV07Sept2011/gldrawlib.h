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
 *
 * https://github.com/berlinbrown/OctaneMechOpenGLGame
 */

#ifndef _GLDRAWLIB_H_
#define _GLDRAWLIB_H_

#define ENABLE_CONSOLE 0
#define USE_SMALL_SCREEN 1

#define MAX_TEXTURES 20

#ifndef ABS
#define ABS(x) (((x) >= 0) ? (x) : (-(x)))
#endif

#define M_PI 3.14159265358f
#define RAD_TO_DEG (180.0f / M_PI)

// dont ask, change the order of normal
#define N_0 CalcNormal(v[0], v[1], v[2], n)
#define N_1 CalcNormal(v[1], v[0], v[2], n)
#define N_2 CalcNormal(v[2], v[0], v[1], n)

#define N_3 CalcNormal(v[0], v[2], v[1], n)

// make sure to call this macro
#define GET_NORMAL glNormal3fv(n)

#if USE_SMALL_SCREEN
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#else
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#endif

#define MED_YELLOW glColor3f(0.8f, 0.8f, 0.0f)
#define MED_RED glColor3f(0.8f, 0.0f, 0.0f)
#define MED_BLUE glColor3f(0.0f, 0.0f, 0.8f)
#define MED_GREEN glColor3f(0.0f, 0.8f, 0.0f)
#define MED_CYAN glColor3f(0.0f, 0.7f, 0.8f)
#define MED_PURPLE glColor3f(0.7f, 0.0f, 0.7f)

#define CLR_0 glColor3f(0.23f, 0.67f, 0.79f)
#define CLR_1 glColor3f(0.755f, 0.234f, 0.237f)
#define CLR_2 glColor3f(0.5646f, 0.3453f, 0.753f)
#define CLR_3 glColor3f(0.9646f, 0.9453f, 0.353f)

DWORD getclock(void);

//
// Bitmap
// title functions
//
void Load_Titles(void);
void Draw_Title(void);

void Normalize(float p[3]);
void CalcNormal(float p[3], float p1[3], float p2[3], float n[3]);

void Calc_Normal(float v[3][3], float out[3]);

void PrintText(const char* fmt, ...);

void SetFunkyTexture(void);
int GetFunkyTexture(void);
void LoadTexture(char* filename);
void NextTexture(void);

void DrawLineOfSight(float x1, float y1, float x2, float y2);

#endif