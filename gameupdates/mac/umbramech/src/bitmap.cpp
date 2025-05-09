/**
 * Copyright (c) 2006-2011 Berlin Brown.  All Rights Reserved
 *
 * http://www.opensource.org/licenses/bsd-license.php
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
modification,
 * are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of the Botnode.com (Berlin Brown) nor
 * the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written
permission.
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
// - Sept 18, 2002
*/

#include <GLUT/glut.h>   // GLUT for window/context
#include <OpenGL/gl.h>   // Core OpenGL functions
#include <OpenGL/glu.h>  // OpenGL Utility Library
#include <float.h>       // used for _control
#include <math.h>        // math libraries
#include <stdio.h>
#include <stdlib.h>
#include <time.h>  // used for randomizing

#include <cstdio>
#include <cstdlib>

static float mTextHeight = 36.0f;

// Note: the terrain should be about 1/2 what the
// perspective z view angle is
// terrain = 10000 then perspective z = 2000
#define TERRAINVIEW 24.0f  // distance from database

static float m_size_z = 0.01f;

//---------------------------------------------------------
// Main globals
//=========================================================
static unsigned int texture[MAX_TEXTURES];
static int textureindex = 0;  // counter of what textures are available
static int funky_texture = 0;
static unsigned int titlesID = 5;

//
// cursor_heights
//
#define MAX_MENU_ITEMS 5
#define NEW_GAME_H 140
#define EXIT_H 171
#define HELP_H 205
#define SETTINGS_H 239
#define DEMO_H 273

static int cursor_heights[MAX_MENU_ITEMS] = {NEW_GAME_H, EXIT_H, HELP_H,
                                             SETTINGS_H, DEMO_H};
static int cursor_index = NEW_GAME_ID;

//
// Texture Image
//
typedef struct {
  int width;
  int height;
  unsigned char *data;

} textureImage;

//
// Reset_DeadText
//
void Reset_DeadText(void) { m_size_z = 0.01f; }

//
// SetFunkyTexture
// - must be placed right after loadTexture
// but I didnt want to put it in the actual function
//
void SetFunkyTexture(void) {
  LoadTexture("data/tile.bmp");
  funky_texture = textureindex - 1;
}

//
// GetFunkyTexture
//
int GetFunkyTexture(void) { return funky_texture; }

//
// NewTexure
//
void NextTexture(void) {
  textureindex++;  // up the index
  if (textureindex > MAX_TEXTURES) {
    textureindex = MAX_TEXTURES;
  }
}

//=========================================================
// loadbmp
// - load a bitmap using aux library
// removed AUX bitmap stuff
//=========================================================
void *LoadBitmap(char *filename) { return NULL; }

//
// GetTexture
//
unsigned int GetTexture(int index) { return texture[index]; }

//
//
// LoadBitmap for linux
//
int LoadBitmap_Lin(char *filename, textureImage *texture) {
  FILE *file;

  unsigned short int bfType;
  long int bfOffBits;
  short int biPlanes;
  short int biBitCount;
  long int biSizeImage;
  int i;
  unsigned char temp;

  /* make sure the file is there and open it read-only (binary) */
  if ((file = fopen(filename, "rb")) == NULL) {
    printf("File not found : %s\n", filename);
    return 0;
  }
  if (!fread(&bfType, sizeof(short int), 1, file)) {
    printf("Error reading file!\n");
    return 0;
  }
  /* check if file is a bitmap */
  if (bfType != 19778) {
    printf("Not a Bitmap-File!\n");
    return 0;
  }
  /* get the file size */
  /* skip file size and reserved fields of bitmap file header */
  fseek(file, 8, SEEK_CUR);
  /* get the position of the actual bitmap data */
  if (!fread(&bfOffBits, sizeof(long int), 1, file)) {
    printf("Error reading file!\n");
    return 0;
  }
  // printf("Data at Offset: %ld\n", bfOffBits);

  /* skip size of bitmap info header */
  fseek(file, 4, SEEK_CUR);
  /* get the width of the bitmap */
  fread(&texture->width, sizeof(int), 1, file);
  // printf("Width of Bitmap: %d\n", texture->width);

  /* get the height of the bitmap */
  fread(&texture->height, sizeof(int), 1, file);
  // printf("Height of Bitmap: %d\n", texture->height);

  /* get the number of planes (must be set to 1) */
  fread(&biPlanes, sizeof(short int), 1, file);
  if (biPlanes != 1) {
    printf("Error: number of Planes not 1!\n");
    return 0;
  }
  /* get the number of bits per pixel */
  if (!fread(&biBitCount, sizeof(short int), 1, file)) {
    printf("Error reading file!\n");
    return 0;
  }

  // printf("Bits per Pixel: %d\n", biBitCount);
  if (biBitCount != 24) {
    printf("Bits per Pixel not 24\n");
    return 0;
  }
  /* calculate the size of the image in bytes */
  biSizeImage = texture->width * texture->height * 3;
  // printf("Size of the image data: %ld\n", biSizeImage);
  texture->data = malloc(biSizeImage);
  /* seek to the actual data */
  fseek(file, bfOffBits, SEEK_SET);
  if (!fread(texture->data, biSizeImage, 1, file)) {
    printf("Error loading file!\n");
    return 0;
  }
  /* swap red and blue (bgr -> rgb) */
  for (i = 0; i < biSizeImage; i += 3) {
    temp = texture->data[i];
    texture->data[i] = texture->data[i + 2];
    texture->data[i + 2] = temp;
  }
  return 1;

}  // end of teh function

//=========================================================
// loadtexture
// - load a texture based on glaux load bitmap
//---------------------------------------------------------
void LoadTexture(char *filename) {
  textureImage *texture_image;
  texture_image = malloc(sizeof(textureImage));

  // load the bitmap, from the file
  if (LoadBitmap_Lin(filename, texture_image)) {
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, GetTexture(0));
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_image->width,
                 texture_image->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 texture_image->data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  }  // end of the if

  // free the memory we just allocated, it is probably going in vram
  if (texture_image) {
    if (texture_image->data) free(texture_image->data);

    free(texture_image);
  }  // end of the if

  NextTexture();
}

//
// LoadTitleBitmap
//
void Load_Titles(void) {
  textureImage *texture_image;
  texture_image = malloc(sizeof(textureImage));

  // load the bitmap, from the file
  if (LoadBitmap_Lin("data/title1.ilf", texture_image)) {
    // glGenTextures(1, &titlesID);
    titlesID = 8;
    glBindTexture(GL_TEXTURE_2D, titlesID);

    // GL_LUMINANCE_ALPHA

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_image->width,
                 texture_image->height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 texture_image->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  }  // end of the if

  // free the memory we just allocated, it is probably going in vram
  if (texture_image) {
    if (texture_image->data) free(texture_image->data);

    free(texture_image);
  }  // end of the if
}

//
// Title_Begin
//
static void Title_Begin(void) {
  // Push the neccessary Matrices on the stack
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();
  glLoadIdentity();

  // Push the neccessary Attributes on the stack
  glPushAttrib(GL_TEXTURE_BIT | GL_ENABLE_BIT);

  glBindTexture(GL_TEXTURE_2D, titlesID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  // Always Draw in Front
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glEnable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

//
// TitleEnd
//
static void Title_End(void) {
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();

  glDisable(GL_TEXTURE_2D);
  glEnable(GL_LIGHTING);
}

//
// Render_Name
// - the title of the game
//
void Render_BText(int val, float x, float y, float Yoffset) {
  float t_offset;
  float t_height;
  float tex_y_1, tex_y_2;

  t_offset = 256.0f - 244.0f;
  t_offset = t_offset / 256.0f;

  // get f height
  t_height = mTextHeight / 256.0f;

  tex_y_2 = (1.0f - t_offset) - (((float)val * t_height) + 0.0f) + Yoffset;
  tex_y_1 = (1.0f - t_offset) - (((float)val * t_height) + t_height) + Yoffset;

  glBegin(GL_QUADS);

  glTexCoord2f(0, -tex_y_1);  // Texture Coord (Bottom Left)
  glVertex3i(x + 0, y, 0);    // Vertex Coord (Bottom Left)

  glTexCoord2f(1, -tex_y_1);  // Texture Coord (Bottom Right)
  glVertex3i(x + 256, y, 0);  // Vertex Coord (Bottom Right)

  glTexCoord2f(1, -tex_y_2);                // Texture Coord (Top Right)
  glVertex3i(x + 256, y + mTextHeight, 0);  // Vertex Coord (Top Right)

  glTexCoord2f(0, -tex_y_2);              // Texture Coord (Top Left)
  glVertex3i(x + 0, y + mTextHeight, 0);  // Vertex Coord (Top Left)

  glEnd();
}

//**
// Render _ Trick
// - the title of the game !!!!
//**
void Render_BTrick(int val, float perc, float x, float y, float Yoffset) {
  float t_offset;
  float t_height;
  float tex_y_1, tex_y_2;

  float t_size = 0.0f;

  t_offset = 256.0f - 244.0f;
  t_offset = t_offset / 256.0f;

  // get f height
  t_height = mTextHeight / 256.0f;

  tex_y_2 = (1.0f - t_offset) - (((float)val * t_height) + 0.0f) + Yoffset;
  tex_y_1 = (1.0f - t_offset) - (((float)val * t_height) + t_height) + Yoffset;

  t_size = 256.0f * 1.0f;

  glBegin(GL_QUADS);

  glTexCoord2f(0, -tex_y_1);  // Texture Coord (Bottom Left)
  glVertex3i(x + 0, y, 0);    // Vertex Coord (Bottom Left)

  glTexCoord2f(perc, -tex_y_1);  // Texture Coord (Bottom Right)
  glVertex3i(x + t_size, y, 0);  // Vertex Coord (Bottom Right)

  glTexCoord2f(perc, -tex_y_2);                // Texture Coord (Top Right)
  glVertex3i(x + t_size, y + mTextHeight, 0);  // Vertex Coord (Top Right)

  glTexCoord2f(0, -tex_y_2);              // Texture Coord (Top Left)
  glVertex3i(x + 0, y + mTextHeight, 0);  // Vertex Coord (Top Left)

  glEnd();

  glDisable(GL_TEXTURE_2D);
  glColor3ub(255, 255, 255);
  glBegin(GL_LINE_LOOP);

  glVertex3i(x, y, 0);
  glVertex3i(x, y + mTextHeight, 0);

  glEnd();

  glBegin(GL_LINE_LOOP);

  glVertex3i(x + t_size, y, 0);
  glVertex3i(x + t_size, y + mTextHeight, 0);

  glEnd();

  // Bottom
  glBegin(GL_LINE_LOOP);

  glVertex3i(x, y + mTextHeight, 0);
  glVertex3i(x + t_size, y + mTextHeight, 0);

  glEnd();

  // Top
  glBegin(GL_LINE_LOOP);

  glVertex3i(x, y, 0);
  glVertex3i(x + t_size, y, 0);

  glEnd();
}

//
// Draw_Shadow
//
void Draw_Shadow(void) {
  int dx = 2;
  int box_top = (SCREEN_HEIGHT / 2) + 100;
  int box_bottom = (SCREEN_HEIGHT / 2) + 200;

  glDisable(GL_TEXTURE_2D);

  // Draw the shadow box
  glColor4ub(205, 205, 205, 200);
  glBegin(GL_QUADS);
  glVertex3i(172 + dx, 100 + dx, 0);  // Vertex Coord (Bottom Left)
  glVertex3i(468 - dx, 100 + dx, 0);  // Vertex Coord (Bottom Right)
  glVertex3i(468 - dx, 310 - dx, 0);  // Vertex Coord (Top Right)
  glVertex3i(172 + dx, 310 - dx, 0);  // Vertex Coord (Top Left)
  glEnd();

  glEnable(GL_TEXTURE_2D);
}

//
// Draw_Cursor
//
void Draw_Cursor(int y) {
  int left_far;
  int left_mid;
  int left_inside;
  int height_1;
  int height_2;
  int height_3;

  // now the other side
  int right_far;
  int right_mid;
  int right_inside;

  // right side
  right_far = 600;
  right_mid = 500;
  right_inside = 360;

  // left
  left_far = 30;
  left_mid = 140;
  left_inside = 200;

  height_1 = y;
  height_2 = y - 8;
  height_3 = y + 8;

  glDisable(GL_TEXTURE_2D);

  glColor4ub(255, 255, 255, 205);
  glBegin(GL_LINE_LOOP);

  glVertex3i(left_far, height_1, 0);
  glVertex3i(left_mid, height_1, 0);

  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3i(left_mid, height_2, 0);
  glVertex3i(left_mid, height_3, 0);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3i(left_mid, height_2, 0);
  glVertex3i(left_inside, height_2, 0);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3i(left_mid, height_3, 0);
  glVertex3i(left_inside, height_3, 0);
  glEnd();

  //
  // handle right side
  glBegin(GL_LINE_LOOP);

  glVertex3i(right_far, height_1, 0);
  glVertex3i(right_mid, height_1, 0);

  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3i(right_mid, height_2, 0);
  glVertex3i(right_mid, height_3, 0);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3i(right_mid, height_2, 0);
  glVertex3i(right_inside, height_2, 0);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3i(right_mid, height_3, 0);
  glVertex3i(right_inside, height_3, 0);
  glEnd();

  // For pure effect
  // draw lines connect to the end of the screen
  glBegin(GL_LINE_LOOP);
  glVertex3i(right_far, 0, 0);
  glVertex3i(right_far, height_1, 0);
  glEnd();

  glBegin(GL_LINE_LOOP);
  glVertex3i(left_far, SCREEN_HEIGHT, 0);
  glVertex3i(left_far, height_1, 0);
  glEnd();

  //
  // draw a bounding box
  //
  glBegin(GL_LINE_LOOP);
  glVertex3i(192, 120, 0);
  glVertex3i(448, 120, 0);

  glVertex3i(448, 120, 0);
  glVertex3i(448, 294, 0);

  glVertex3i(192, 294, 0);
  glVertex3i(192, 294, 0);
  glEnd();

  glEnable(GL_TEXTURE_2D);
}

//
// Draw_GameOver
//
void Draw_GameOver(void) {
  float offset = 34.0f;
  float begin = 120.0f;
  float t;

  // based on the frame rate
  if (framerate <= 8.0f) framerate = 30.0f;

  t = 0.007f / framerate;
  t = framerate * t;

  m_size_z += t;
  if (m_size_z >= 1.0f) m_size_z = 1.0f;

  Title_Begin();

  glColor3ub(255, 255, 0);
  Render_BTrick(2, m_size_z, 192.0f, 220.0f, 0.034f);

  Title_End();
}

//
// Draw_Title
//
void Draw_Title(void) {
  float offset = 34.0f;
  float begin = 120.0f;

  // Also draw game over
  // if it is turned on
  if (ant_globals->_menu_state == MENU_DEAD_MODE) {
    Draw_GameOver();
  }  // end of the  if

  switch (ant_globals->menu_mode) {
    case MENU_HELP_MODE:

      glLineWidth(2.0f);

      Title_Begin();

      Draw_HelpScreen();

      glDisable(GL_LIGHTING);

      Draw_IntroScreen();

      glEnable(GL_LIGHTING);

      Title_End();
      glLineWidth(1.0f);

      break;

    case MENU_SETTINGS_MODE:

      glLineWidth(2.0f);

      Title_Begin();
      Draw_NetworkScreen();
      Title_End();

      glLineWidth(1.0f);

      break;

    case MENU_TITLE_MODE:
      glLineWidth(2.0f);

      Title_Begin();

      Draw_Shadow();

      glColor4ub(255, 255, 255, 255);

      // title, newgame, exit, demo
      if (cursor_index == 0) glColor4ub(255, 255, 0, 255);
      Render_BText(6, 192.0f, begin + (0 * offset), 0.03f);

      glColor4ub(255, 255, 255, 255);
      if (cursor_index == 1) glColor4ub(255, 255, 0, 255);
      Render_BText(5, 192.0f, begin + (1 * offset), 0.016f);

      glColor4ub(255, 255, 255, 255);
      if (cursor_index == 2) glColor4ub(255, 255, 0, 255);
      Render_BText(4, 192.0f, begin + (2 * offset), -0.015f);

      glColor4ub(255, 255, 255, 255);
      if (cursor_index == 3) glColor4ub(255, 255, 0, 255);
      Render_BText(3, 192.0f, begin + (3 * offset), -0.041f);

      glColor4ub(255, 255, 255, 255);
      if (cursor_index == 4) glColor4ub(255, 255, 0, 255);
      Render_BText(1, 192.0f, begin + (4 * offset), 0.01f);

      // draw the selection tool --
      Draw_Cursor(cursor_heights[cursor_index]);

      // Draw_HelpScreen();
      Draw_IntroScreen();

      Title_End();

      glLineWidth(1.0f);

      break;

    default:
      break;
  };
}

//
// Toggle_MenuItems
//
void Toggle_MenuItems(int dir) {
  if (ant_globals->menu_mode == MENU_TITLE_MODE) {
    if (dir == 1) {
      cursor_index++;
      if (cursor_index >= MAX_MENU_ITEMS) cursor_index = 0;
    } else if (dir == -1) {
      cursor_index--;
      if (cursor_index < 0) cursor_index = MAX_MENU_ITEMS - 1;

    }  // end of if-else

  }  // end of main if

}  // end of the functoin

//
// Set_MenuMode
//
bool Set_MenuMode(void) {
  if (ant_globals->menu_mode == MENU_TITLE_MODE) {
    switch (cursor_index) {
      case NEW_GAME_ID:

        if (CHECK_NET_SERVER) {
          if (ant_globals->_menu_state == FIRST_TIME_TRUE)
            cursor_index = NEW_GAME_ID;
          else
            cursor_index = HELP_ID;

          // redundant code
          cursor_index = HELP_ID;

          Build_StartMsg();

          ant_globals->paused = 0;
          ant_globals->menu_mode = MENU_RUN_MODE;

          // NOTE NOTE NOTE NOTE!
          // reset the bots here
          // this may take a while
          Reset_NetworkBots();

          return false;
        } else {
          if (ant_globals->_menu_state == FIRST_TIME_TRUE)
            cursor_index = NEW_GAME_ID;
          else
            cursor_index = HELP_ID;

          // redundant code
          cursor_index = HELP_ID;

          ant_globals->paused = 0;

          ant_globals->menu_mode = MENU_RUN_MODE;

          // Not the first time any more
          ant_globals->_menu_state = FIRST_TIME_FALSE;

          // NOTE NOTE NOTE NOTE!
          // reset the bots here
          // this may take a while
          Reset_FireAnts();

          return false;

        }  // end of if-else

        break;

      case SETTINGS_ID:

        if (ant_globals->_menu_state == FIRST_TIME_TRUE)
          cursor_index = NEW_GAME_ID;
        else
          cursor_index = HELP_ID;

        ant_globals->paused = 1;
        ant_globals->menu_mode = MENU_SETTINGS_MODE;

        break;

      case HELP_ID:

        if (ant_globals->_menu_state == FIRST_TIME_TRUE)
          cursor_index = NEW_GAME_ID;
        else
          cursor_index = HELP_ID;

        ant_globals->paused = 1;
        ant_globals->menu_mode = MENU_HELP_MODE;
        break;

      case DEMO_ID:

        if (ant_globals->_menu_state == FIRST_TIME_TRUE)
          cursor_index = NEW_GAME_ID;
        else
          cursor_index = HELP_ID;

        // redundant code
        cursor_index = HELP_ID;

        ant_globals->paused = 0;

        ant_globals->menu_mode = MENU_RUN_MODE;

        // Not the first time any more
        ant_globals->_menu_state = FIRST_TIME_FALSE;

        // NOTE NOTE NOTE NOTE!
        // reset the bots here
        // this may take a while
        Prepare_DemoMode();

        return false;

        break;

      case EXIT_ID:

        return true;  // we are done here
        break;

      default:
        break;
    };

  }  // end of the if --

  return false;
}
