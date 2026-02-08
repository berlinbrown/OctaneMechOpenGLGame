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

// Berlin Brown

#pragma once

#include <cstring>

#define ENABLE_CONSOLE 0
#define USE_SMALL_SCREEN 1

#define MAX_TEXTURES 20

#ifndef M_PI
#define M_PI 3.14159265358f
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#ifndef RAD_TO_DEG
#define RAD_TO_DEG 57.2957795f
#endif

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif

#ifndef ZeroMemory
#define ZeroMemory(ptr, size) std::memset((ptr), 0, (size))
#endif

unsigned long getclock(void);

// Bitmap
// title functions
void Load_Titles(void);
void Draw_Title(void);

void Normalize(float p[3]);
void CalcNormal(float p[3], float p1[3], float p2[3], float n[3]);

void Calc_Normal(float v[3][3], float out[3]);

void PrintText(const char* fmt, ...);

void SetFunkyTexture(void);
int GetFunkyTexture(void);
void LoadTexture(const char* filename);
void NextTexture(void);

void DrawLineOfSight(float x1, float y1, float x2, float y2);
