//
// Copyright (C) Dante Treglia II, 2000.
//
#ifndef _TEXT_H
#define _TEXT_H

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

#endif
