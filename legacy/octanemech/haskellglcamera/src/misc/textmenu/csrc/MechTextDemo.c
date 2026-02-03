//************************************************
// (c) Copyright 2007 - Newspiritcompany.com. All Rights Reserved.
// Berlin Brown
// 10/20/2007
// Simple OpenGL camera demo in C
// Added text routines.
//************************************************

#include <GL/gl.h>    // Header File For The OpenGL32 Library
#include <GL/glu.h>   // Header File For The GLu32 Library
#include <GL/glut.h>  // Header File For The GLUT Library
#include <assert.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>  // needed to sleep

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define FONT_W 8
#define FONT_H 8
#define FONT_DRAW_W 8
#define FONT_DRAW_H 9
#define FONT_TEX_W 64
#define FONT_TEX_H 128

#define MAX_STR 4096

#define TEXT_NONE 0x00
#define TEXT_DRAW_BOX 0x04
#define TEXT_DRAW_SPACES 0x02
#define TEXT_WRAP_SPACES 0x08
#define TEXT_DRAW_BACKGROUND 0x01

#define SET_COLOR(c, rIn, gIn, bIn, aIn) \
  c.r = rIn;                             \
  c.g = gIn;                             \
  c.b = bIn;                             \
  c.a = aIn;

#define RELEASE_OBJECT(x) \
  if (x != NULL) free(x); \
  x = NULL

typedef struct
{
  unsigned char r, g, b, a;
} COLOR;

//----------------------------------------------------------
// Text Box and Text Data Class
//----------------------------------------------------------
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

static int screenWidth = SCREEN_WIDTH;
static int screenHeight = SCREEN_HEIGHT;

static float angle = 0.0f;

static float posx = 0.0f;
static float posy = 3.2f;
static float posz = -4.5;

// Pointer (instance) of the text object
static TextBoxPtr main_text = NULL;

// Forward definition
extern unsigned char fontData[];

// ASCII code for the escape key.
#define ESCAPE 27

// The number of our GLUT window
int window;

// The text texture only needs to be initialized once
static unsigned int fontID = 0;

static void formatStrCat(TextBoxPtr text, char* str);

void SetTextColor(TextBoxPtr text, unsigned char r, unsigned char g, unsigned char b)
{
  text->color.r = r;
  text->color.g = g;
  text->color.b = b;

}  // end of the function

//----------------------------------------------------------
// Text Render Routines
//----------------------------------------------------------

void glPrintf(TextBoxPtr text, char* fmt, ...)
{
  va_list vlist;
  char buff[MAX_STR];

  // Get output string
  va_start(vlist, fmt);
  vsprintf(buff, fmt, vlist);

  formatStrCat(text, buff);
  va_end(vlist);

}  // end of the function

void setTextMode(TextBoxPtr text, unsigned int modeIn) { text->mode = modeIn; }

/**
 * A constructor for the text box "menu" screen including
 * a placeholder for text content and bounding box.
 */
TextBoxPtr newTextBox(int inL, int inR, int inT, int inB)
{
  TextBoxPtr text = NULL;
  text = (TextBoxPtr)malloc(sizeof(TextBox));

  // Calculate the Text box boundaries
  text->boxL = inL;
  text->boxR = inL + ((inR - inL) - ((inR - inL) % FONT_DRAW_W));
  text->boxT = inT;
  text->boxB = inT + ((inB - inT) - ((inB - inT) % FONT_DRAW_H));

  // Initialize the Buffers and Pointers
  text->buffer = (char*)malloc(MAX_STR);
  text->buffer[0] = '\0';

  text->drawBufferPtr = text->buffer;

  // Calculate the number of lines per Page
  text->pageLineCnt = (text->boxB - text->boxT) / FONT_DRAW_H;
  text->scrollCnt = 0;
  text->currX = text->boxL;

  if (!fontID)
  {
    // Create Texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &fontID);
    glBindTexture(GL_TEXTURE_2D, fontID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, FONT_TEX_W, FONT_TEX_H, 0,
                 GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, (void*)fontData);

  }  // end of the function

  // Set Color
  SET_COLOR(text->color, 255, 255, 255, 255);

  // Set Mode
  text->mode = 0;
  return text;

}  // end of the function

void destroyTextBox(TextBoxPtr text) { RELEASE_OBJECT(text); }  // end of the function

void setTextColor(TextBoxPtr text, unsigned char r, unsigned char g, unsigned char b)
{
  text->color.r = r;
  text->color.g = g;
  text->color.b = b;

}  // end of the function

void pushTextBegin(TextBoxPtr text)
{
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
  glBindTexture(GL_TEXTURE_2D, fontID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(GL_TEXTURE_2D);

  // Always Draw in Front
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);

  glDisable(GL_LIGHTING);

  if (!(text->mode & TEXT_DRAW_BACKGROUND))
  {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  }

}  // end of the function

/**
 * Reset the screen to its previous state before
 * using text rendering functionality.
 * (Textures disabled, Lighting enabled)
 */
void popTextEnd(void)
{
  // Return to previous Matrix and Attribute states.
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();

  glDisable(GL_TEXTURE_2D);
  // glEnable(GL_LIGHTING);

}  // end of the function

void formatStrCat(TextBoxPtr text, char* str)
{
  char buff[MAX_STR];
  char buff2[MAX_STR];
  char* cP;
  char* cP2;

  strcpy(buff, str);
  cP = buff;

  // Format String for caption
  // Parse the String
  while (*cP != '\0')
  {
    // Visible characters
    if (' ' <= *cP && *cP <= '~')
    {
      text->currX += FONT_DRAW_W;
    }
    else if (*cP == '\n')
    {
      text->currX = text->boxL;
    }

    // Wrap
    if (text->currX >= text->boxR)
    {
      cP++;
      // Find the last space
      if (text->mode & TEXT_WRAP_SPACES)
      {
        while (cP > buff && *cP != ' ') cP--;
      }

      strcpy(buff2, cP);
      *cP++ = '\n';
      *cP = '\0';
      // Eliminate Space
      if (text->mode & TEXT_WRAP_SPACES && buff2[0] == ' ')
        cP2 = &buff2[1];
      else
        cP2 = buff2;
      strcat(cP, cP2);
      text->currX = text->boxL;
    }
    else
    {
      cP++;
    }
  }

  // Make sure it fits in the text buffer by removing
  // strings at the top of the buffer.
  cP = text->buffer;
  while (strlen(cP) + strlen(buff) > MAX_STR)
  {
    cP = strchr(cP, '\n');
    if (!cP)
    {
      assert(!"TextBox::FormatStrCat: string too long!!!");
    }
    cP++;
  }
  // If there are strings that need to be removed. Do so.
  if (cP != text->buffer)
  {
    strcpy(buff2, cP);
    strcpy(text->buffer, buff2);
  }
  // Now there is space. So, concatinate.
  strcat(text->buffer, buff);

}  // end of the function

void renderBoundingBox(TextBoxPtr text)
{
  glColor4f(0.0, 0.0, 0.0, 1.0);
  glBegin(GL_QUADS);

  glVertex3s(text->boxL, text->boxT, 0);
  glVertex3s(text->boxR, text->boxT, 0);
  glVertex3s(text->boxR, text->boxB, 0);
  glVertex3s(text->boxL, text->boxB, 0);
  glEnd();

}  // end of the function

void renderString(TextBoxPtr text, int x, int y, int maxFlag, int maxY, char* string)
{
  int cursorX;
  int cursorY;

  char* cP = string;
  int index;
  int s, t;

  cursorX = x;
  cursorY = y;
  glColor4ub(text->color.r, text->color.g, text->color.b, text->color.a);
  glBegin(GL_QUADS);

  // Parse the String
  while (*cP != '\0')
  {
    // Visible characters
    if (' ' == *cP && !(text->mode & TEXT_DRAW_SPACES))
    {
      cursorX += FONT_DRAW_W;
    }
    else if (' ' <= *cP && *cP <= '~')
    {
      index = *cP - ' ';
      s = (index % 8) * FONT_W;
      t = (index / 8) * FONT_H;

      // Actually render to screen --
      glTexCoord2f((float)(s) / FONT_TEX_W, (float)(t) / FONT_TEX_H);
      glVertex3s(cursorX, cursorY, 0);
      glTexCoord2f((float)(s) / FONT_TEX_W, (float)(t + FONT_H + 1) / FONT_TEX_H);
      glVertex3s(cursorX, cursorY + FONT_DRAW_H, 0);
      glTexCoord2f((float)(s + FONT_W) / FONT_TEX_W, (float)(t + FONT_H + 1) / FONT_TEX_H);
      glVertex3s(cursorX + FONT_DRAW_W, cursorY + FONT_DRAW_H, 0);
      glTexCoord2f((float)(s + FONT_W) / FONT_TEX_W, (float)(t) / FONT_TEX_H);
      glVertex3s(cursorX + FONT_DRAW_W, cursorY, 0);

      cursorX += FONT_DRAW_W;
    }

    if (*cP == '\n')
    {
      cursorX = x;
      cursorY += FONT_DRAW_H;
      if (maxFlag && cursorY + FONT_DRAW_H > maxY) break;
    }
    cP++;
  }
  glEnd();

}  // end of the function

void paginateText(TextBoxPtr text)
{
  int newLineCnt = 0;
  int pageNewLineCnt;
  char* cP;

  // Calculate the number of new lines from the bottom we want
  pageNewLineCnt = text->pageLineCnt + text->scrollCnt;

  // Starting at the end of the string count the number of newlines backwards
  cP = strchr(text->buffer, '\0');
  while (cP > text->buffer)
  {
    if (*cP == '\n') newLineCnt++;
    if (newLineCnt == pageNewLineCnt)
    {
      cP++;
      break;
    }
    cP--;
  }
  if (cP == text->buffer && newLineCnt > text->pageLineCnt)
    text->scrollCnt = newLineCnt - text->pageLineCnt + 1;

  text->drawBufferPtr = cP;

}  // end of the function

/**
 * Render static text that has already been initialized during
 * system initializations.
 */
void renderStaticText(TextBoxPtr text)
{
  paginateText(text);
  pushTextBegin(text);
  if (text->mode & TEXT_DRAW_BOX) renderBoundingBox(text);

  renderString(text, text->boxL, text->boxT, 1, text->boxB, text->drawBufferPtr);
  popTextEnd();

}  // end of the function
//----------------------------------------------------------
// Init and Shutdown Routines
//----------------------------------------------------------
void systemInit()
{
  printf("INFO: performing system initialization\n");
  main_text = newTextBox(10, 200, 400, 480);
  setTextMode(main_text, TEXT_NONE);
  SetTextColor(main_text, 0, 255, 0);
  glPrintf(main_text, "==== Mech Text Demo ====\n");
}

void systemShutdown()
{
  printf("INFO: performing shutdown\n");
  destroyTextBox(main_text);
}

//==========================================================
// GL Specific Routines
//==========================================================

void initGL(void)
{
  glClearColor(0, 0, 0, 0);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (float)screenWidth / screenHeight, 1.0f, 100.0f);
  glMatrixMode(GL_MODELVIEW);

  glViewport(0, 0, screenWidth, screenHeight);

  // GL_MODELVIEW is about having different
  // objects being pushed into a "world space".
  glMatrixMode(GL_MODELVIEW);
}

void displayLoop(void)
{
  angle += 0.1f;

  // Clear The Screen And The Depth Buffer
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(posx, posy, posz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

  glPushMatrix();
  // Rotate quad around the y axis
  glRotatef(angle, 0.0f, 1.0f, 0.0f);
  // Set color for quad
  glBegin(GL_QUADS);

  // Top of cube
  glColor3f(0.0f, 1.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);

  // Bottom of cube
  glColor3f(1.0f, 0.5f, 0.0f);
  glVertex3f(1.0f, -1.0f, 1.0f);
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(1.0f, -1.0f, -1.0f);

  // Front of cube
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glVertex3f(-1.0f, -1.0f, 1.0f);
  glVertex3f(1.0f, -1.0f, 1.0f);

  // Back of cube.
  glColor3f(1.0f, 1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);

  // Left of cube
  glColor3f(0.0f, 0.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, 1.0f);
  glVertex3f(-1.0f, 1.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, -1.0f);
  glVertex3f(-1.0f, -1.0f, 1.0f);

  // Right of cube
  glColor3f(1.0f, 0.0f, 1.0f);
  glVertex3f(1.0f, 1.0f, -1.0f);
  glVertex3f(1.0f, 1.0f, 1.0f);
  glVertex3f(1.0f, -1.0f, 1.0f);
  glVertex3f(1.0f, -1.0f, -1.0f);
  glEnd();
  glPopMatrix();

  // Lower the position of the plain and render
  glPushMatrix();
  glTranslatef(0.0f, -1.1f, -0.0f);

  // Bottom of cube
  glBegin(GL_QUADS);
  glColor3f(0.9f, 0.9f, 0.9f);
  glVertex3f(2.0f, 0.0f, 2.0f);
  glVertex3f(-2.0f, 0.0f, 2.0f);
  glVertex3f(-2.0f, 0.0f, -2.0f);
  glVertex3f(2.0f, 0.0f, -2.0f);
  glEnd();
  glPopMatrix();

  // Draw the static text content
  renderStaticText(main_text);

  // Continue with GL, swap the double buffer
  glutSwapBuffers();
  glutPostRedisplay();
}

//
// The function called whenever a key is pressed.
void keyPressed(unsigned char key, int x, int y)
{
  usleep(100);

  // If escape is pressed, kill everything.
  if (key == ESCAPE)
  {
    glutDestroyWindow(window);
    systemShutdown();
    exit(0);
  }
  else if (key == 's')
  {
    // move the camera back when the s character is pressed
    posz -= 0.5f;
  }
  else if (key == 'w')
  {
    posz += 0.5f;
  }
  else if (key == 'd')
  {
    posx += 0.5f;
  }
  else if (key == 'a')
  {
    posx -= 0.5f;
  }  // End of if - else
}

//
// Main entry point into the program
int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize(screenWidth, screenHeight);

  glutInitWindowPosition(0, 0);

  window = glutCreateWindow("Mech Camera Demo");

  glutDisplayFunc(&displayLoop);
  glutIdleFunc(&displayLoop);
  // glutReshapeFunc(&ReSizeGLScene);
  glutKeyboardFunc(&keyPressed);

  // Initialize our GL window before main loop
  systemInit();
  initGL();
  glutMainLoop();

  systemShutdown();

  return 1;
}
