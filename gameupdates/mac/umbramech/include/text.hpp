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

#pragma once

#define MAX_STR 4096

#define TEXT_NONE 0x00
#define TEXT_DRAW_BOX 0x04
#define TEXT_DRAW_SPACES 0x02
#define TEXT_WRAP_SPACES 0x08
#define TEXT_DRAW_BACKGROUND 0x01

typedef struct
{
  unsigned char r, g, b, a;
} COLOR;
#define MALLOC malloc

#define SET_COLOR(c, rIn, gIn, bIn, aIn) \
  c.r = rIn;                             \
  c.g = gIn;                             \
  c.b = bIn;                             \
  c.a = aIn

typedef struct tagTextBox
{
  // Bounding Box
  int boxL;
  int boxR;
  int boxT;
  int boxB;

  // Text Buffers
  char* buffer;
  char* drawBufferPtr;

  // Info
  int pageLineCnt;
  int scrollCnt;
  int currX;
  int currY;

  COLOR color;

  unsigned int mode;

} TextBox, *TextBoxPtr;

void Paginate(TextBoxPtr text);
void DrawStr(TextBoxPtr text, int x, int y, int maxFlag, int maxY, char* string);
void DrawBoundingBox(TextBoxPtr text);

void TextBegin(TextBoxPtr text);
void TextEnd(void);
void DrawString(TextBoxPtr text, int x, int y, char* str);

TextBoxPtr InitTextBox(int inL, int inR, int inT, int inB);
void DestroyTextBox(TextBoxPtr text);
void SetTextMode(TextBoxPtr text, unsigned int modeIn);
void DrawText(TextBoxPtr text);
void Printf(TextBoxPtr text, char* fmt, ...);

void FormatStrCat(TextBoxPtr text, char* str);

void SetTextColor(TextBoxPtr text, unsigned char r, unsigned char g, unsigned char b);

void Reset_FontID(void);
