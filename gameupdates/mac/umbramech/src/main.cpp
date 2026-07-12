/**
 * Copyright (c) 2006-2026 Berlin Brown.  All Rights Reserved
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
 * Updated: 2026 for Mac, OpenGL
 */

#include <GLUT/glut.h>   // GLUT for window/context
#include <OpenGL/gl.h>   // Core OpenGL functions
#include <OpenGL/glu.h>  // OpenGL Utility Library
#include <math.h>
#include <unistd.h>

#include <cstdlib>
#include <ctime>

#include <stdio.h>
#include <stdlib.h>

#include "bot.hpp"
#include "camera.hpp"
#include "collision.hpp"
#include "fireants.hpp"
#include "gldrawlib.hpp"
#include "globals.hpp"
#include "lights.hpp"
#include "menu.hpp"
#include "objects.hpp"
#include "particles.hpp"
#include "text.hpp"
#include "walls.hpp"
#include "world.hpp"

static int gWindowWidth = SCREEN_WIDTH;
static int gWindowHeight = SCREEN_HEIGHT;
static bool gGameReady = false;

static constexpr int kSpecialKeyBase = 256;
static bool gKeys[512] = {false};

static const int kFrameRateSamples = 60;

extern float framerate;
extern int keyCodes[20];

static int gFrames = 0;
static clock_t gLastTime = 0;

static float newYPos = 0.0f;
static float newXPos = 0.0f;
static float gCamYawDeg = 0.0f;
static float gCamDistance = 120.0f;
static bool gMouseDrag = false;

static void PrintCurrentCameraView(void)
{
  const float camYawRad = gCamYawDeg * ((float)M_PI / 180.0f);
  const int viewMode = GetViewMode();

  if (viewMode == THIRD_PERSON_MODE)
  {
    const float camX = sinf(camYawRad) * gCamDistance;
    const float camZ = cosf(camYawRad) * gCamDistance;
    printf("Camera 1 (World View) pos=(%.2f, %.2f, %.2f)\n", camX, (180.0f + newYPos), camZ);
  }
  else if (viewMode == CLOSE_THIRD_MODE)
  {
    const float mechX = GetBotX();
    const float mechZ = GetBotY();
    const float forwardX = sinf(camYawRad);
    const float forwardZ = cosf(camYawRad);
    const float rightX = cosf(camYawRad);
    const float rightZ = -sinf(camYawRad);
    const float eyeX = mechX - forwardX * 10.0f + rightX * 4.0f;
    const float eyeZ = mechZ - forwardZ * 10.0f + rightZ * 4.0f;
    printf("Camera 2 (Close Third Person) pos=(%.2f, %.2f, %.2f) target=(%.2f, %.2f, %.2f)\n",
           eyeX, (14.0f + newYPos), eyeZ, mechX, 3.0f, mechZ);
  }
  else if (viewMode == FIRST_PERSON_MODE)
  {
    const float mechX = GetBotX();
    const float mechZ = GetBotY();
    const float eyeX = mechX - sinf(camYawRad) * 12.0f;
    const float eyeZ = mechZ - cosf(camYawRad) * 12.0f;
    printf("Camera 3 (First Person) pos=(%.2f, %.2f, %.2f)\n", eyeX, (18.0f + newYPos), eyeZ);
  }
  else if (viewMode == DEMO_MODE)
  {
    printf("Camera 4 (Demo Spin) pos=(%.2f, %.2f, %.2f)\n", CAMERA->position[0], CAMERA->position[1], CAMERA->position[2]);
  }
}
static int gPrevMouseX = 0;
static int gPrevMouseY = 0;

static float ClampFloat(float value, float minv, float maxv)
{
  if (value < minv) return minv;
  if (value > maxv) return maxv;
  return value;
}


static void InitKeyCodes(void)
{
  keyCodes[VK_ESC] = 27;
  keyCodes[VK_F1] = kSpecialKeyBase + GLUT_KEY_F1;
  keyCodes[VK_f] = 'f';
  keyCodes[VK_F] = 'F';
  keyCodes[VK_s] = 's';
  keyCodes[VK_S] = 'S';
  keyCodes[VK_r] = 'r';
  keyCodes[VK_R] = 'R';
  keyCodes[VK_UP] = kSpecialKeyBase + GLUT_KEY_UP;
  keyCodes[VK_DOWN] = kSpecialKeyBase + GLUT_KEY_DOWN;
  keyCodes[VK_LEFT] = kSpecialKeyBase + GLUT_KEY_LEFT;
  keyCodes[VK_RIGHT] = kSpecialKeyBase + GLUT_KEY_RIGHT;
  keyCodes[VK_SPACE] = ' ';
}

static void GetFramesPerSecond(void)
{
  clock_t now;
  float delta;

  GameTick();
  CheckRespawn();

  if (gFrames++ >= kFrameRateSamples)
  {
    now = clock();
    delta = (now - gLastTime) / (float)CLOCKS_PER_SEC;
    AddTime((float)(now - gLastTime));
    AddSeconds(delta);
    gLastTime = now;
    framerate = kFrameRateSamples / delta;
    gFrames = 0;
  }
}

static void DrawHUD(void)
{
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);

  glColor3ub(0, 255, 0);
  PrintGlobals();

  glPopAttrib();

  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  GetFramesPerSecond();
}

static void InitGame(void)
{
  srand((unsigned int)time(NULL));
  InitKeyCodes();
  gLastTime = clock();

  chdir("resources");

  Super_LoadGlobals();
  Load_ConfigDefaultsOnly();
  Super_LoadBots();
  Super_FireAnts();

  Create_Col_List();
  Create_Wall_List();

  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);

  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  InitMaterial();

  quadric = gluNewQuadric();
  gluQuadricNormals(quadric, GLU_SMOOTH);
  gluQuadricTexture(quadric, GL_TRUE);

  GenerateBots();
  GenerateFireAnts();

  CreateWorld();
  LoadCameras();

  for (int i = 0; i < MAX_CAMERAS; i++)
  {
    driver_camera[i]->zoom_factor = 6.0f;
  }

  InitObjects();
  GenerateLights();
  InitGlobals();

  nest.generate();
  garden.generate();
  trail_set.generate();

  InitFood();
  Build_ParticleSet();
  Reset_FontID();

  Super_MainText();

  CreateWalls();

  InsertColSegment(world_ptr->x_min, world_ptr->y_max, world_ptr->x_max, world_ptr->y_max);
  InsertColSegment(world_ptr->x_max, world_ptr->y_min, world_ptr->x_max, world_ptr->y_max);
  InsertColSegment(world_ptr->x_min, world_ptr->y_min, world_ptr->x_max, world_ptr->y_min);
  InsertColSegment(world_ptr->x_min, world_ptr->y_min, world_ptr->x_min, world_ptr->y_max);

  ant_globals->paused = 0;
  ant_globals->menu_mode = MENU_RUN_MODE;
  ant_globals->_menu_state = FIRST_TIME_FALSE;
  gGameReady = true;
}

static void AnimateScene(void)
{
  if (ant_globals->paused == 0)
  {
    AnimFireAnts();
  }

  AnimateExplosions();
}

static void DrawGLScene(void)
{
  const float camYawRad = gCamYawDeg * ((float)M_PI / 180.0f);
  const float camX = sinf(camYawRad) * gCamDistance;
  const float camZ = cosf(camYawRad) * gCamDistance;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Document : gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
  //.  GLdouble centerX, GLdouble centerY, GLdouble centerZ, GLdouble upX, GLdouble upY, GLdouble upZ)
  if (ant_globals && ant_globals->paused) {
    // Paused: spin the camera slowly around the world
    Paused_Camera();
  } else if (GetViewMode() == CLOSE_THIRD_MODE) {
    // Close third-person: raised slightly and centered on the mech body
    const float mechX = GetBotX();
    const float mechZ = GetBotY();
    const float forwardX = sinf(camYawRad);
    const float forwardZ = cosf(camYawRad);
    const float rightX = cosf(camYawRad);
    const float rightZ = -sinf(camYawRad);
    const float eyeX = mechX - forwardX * 10.0f + rightX * 4.0f;
    const float eyeZ = mechZ - forwardZ * 10.0f + rightZ * 4.0f;
    gluLookAt(eyeX, 14.0f + newYPos, eyeZ,
              mechX, 3.0f, mechZ,  0.0f, 1.0f, 0.0f);
  } else if (GetViewMode() == FIRST_PERSON_MODE) {
    // First-person: camera sits just behind and above the mech, looking forward
    const float mechX = GetBotX();
    const float mechZ = GetBotY();
    const float behindX = mechX - sinf(camYawRad) * 12.0f;
    const float behindZ = mechZ - cosf(camYawRad) * 12.0f;
    const float lookX   = mechX + sinf(camYawRad) * 30.0f;
    const float lookZ   = mechZ + cosf(camYawRad) * 30.0f;
    gluLookAt(behindX, 18.0f + newYPos, behindZ,
              lookX,  8.0f,             lookZ,  0.0f, 1.0f, 0.0f);
  } else if (GetViewMode() == DEMO_MODE) {
    // Demo/attract mode: spinning camera, entities still animate
    Paused_Camera();
  } else {
    // World view: high orbit around the whole map (default)
    gluLookAt(camX, (180.0f + newYPos), camZ,
              0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
  }

  BEGIN_BOT;

  RenderGrid();
  RenderWalls();
  RenderPlane();

  SetLights();
  driver_objects[STARS_OBJECT]->render();

  Draw_Wall_List();
  DrawExplosions();
  DrawFireAnts();
  DrawBots();
  nest.drawall();
  garden.drawall();
  trail_set.drawall();

  END_BOT;

  Super_DrawText();
  Draw_Title();
  DrawHUD();
}

static void DisplayGL(void)
{
  if (!gGameReady)
  {
    InitGame();
  }

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  newXPos = ClampFloat(newXPos, -25.0f, 55.0f);
  gluPerspective((45.0f + newXPos), (GLfloat)gWindowWidth / (GLfloat)gWindowHeight, 0.1f,
                 PERSPECTIVE_Z);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  HandleCameraKeys(gKeys);
  AnimateScene();
  DrawGLScene();

  glutSwapBuffers();
}

static void ResizeGL(int width, int height)
{
  if (height <= 0) height = 1;

  gWindowWidth = width;
  gWindowHeight = height;

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, PERSPECTIVE_Z);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

static void HandleEsc(void)
{
  if (ant_globals->menu_mode == MENU_TITLE_MODE)
  {
    if (ant_globals->_menu_state == FIRST_TIME_TRUE)
    {
      std::exit(0);
    }
    else
    {
      ant_globals->paused = 0;
      ant_globals->menu_mode = MENU_RUN_MODE;
    }
  }
  else if (ant_globals->menu_mode == MENU_HELP_MODE)
  {
    ant_globals->paused = 1;
    ant_globals->menu_mode = MENU_TITLE_MODE;
  }
  else
  {
    ant_globals->paused = 1;
    ant_globals->menu_mode = MENU_TITLE_MODE;
  }
}

static void KeyDown(unsigned char key, int, int)
{
  gKeys[(unsigned char)key] = true;

  switch (key)
  {
    case '\t':
      ToggleViewMode();
      PrintCurrentCameraView();
      break;

    case '\r':
    case ' ':
      if (Set_MenuMode())
      {
        std::exit(0);
      }
      break;

    case 27:
      HandleEsc();
      break;

    case 'p':
    case 'P':
      if (ant_globals->menu_mode == MENU_RUN_MODE)
      {
        TogglePaused();
      }
      break;

    case 'q':
    case 'Q':
      std::exit(0);
      break;

    case 'o':
    case 'O':
      newYPos += 8.0f;
      break;

    case 'l':
    case 'L':
      newYPos -= 8.0f;
      break;

    case 'k':
    case 'K':
      newXPos += 4.0f;
      break;

    case 'i':
    case 'I':
      newXPos -= 4.0f;
      break;

    case 'a':
    case 'A':
      gCamYawDeg -= 5.0f;
      break;

    case 'd':
    case 'D':
      gCamYawDeg += 5.0f;
      break;

    case '=':
    case '+':
      gCamDistance = ClampFloat(gCamDistance - 8.0f, 35.0f, 360.0f);
      break;

    case '-':
    case '_':
      gCamDistance = ClampFloat(gCamDistance + 8.0f, 35.0f, 360.0f);
      break;

    default:
      break;
  }
}

static void KeyUp(unsigned char key, int, int) { gKeys[(unsigned char)key] = false; }

static void SpecialKeyDown(int key, int, int)
{
  gKeys[kSpecialKeyBase + key] = true;

  if (ant_globals && ant_globals->menu_mode == MENU_RUN_MODE)
  {
    return;
  }

  switch (key)
  {
    case GLUT_KEY_UP:
      Toggle_MenuItems(-1);
      break;

    case GLUT_KEY_DOWN:
      Toggle_MenuItems(1);
      break;

    default:
      break;
  }
}

static void SpecialKeyUp(int key, int, int) { gKeys[kSpecialKeyBase + key] = false; }

static void MouseButton(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON)
  {
    gMouseDrag = (state == GLUT_DOWN);
    gPrevMouseX = x;
    gPrevMouseY = y;
    return;
  }

  // Mouse wheel: 3 up, 4 down in GLUT
  if ((state == GLUT_DOWN) && (button == 3 || button == 4))
  {
    const float delta = (button == 3) ? -6.0f : 6.0f;
    gCamDistance = ClampFloat(gCamDistance + delta, 35.0f, 360.0f);
  }
}

static void MouseMove(int x, int y)
{
  SetMousePosition(x, y);

  if (!gMouseDrag) return;

  const int dx = x - gPrevMouseX;
  const int dy = y - gPrevMouseY;

  gPrevMouseX = x;
  gPrevMouseY = y;

  gCamYawDeg += dx * 0.35f;
  newYPos -= dy * 0.45f;
  newYPos = ClampFloat(newYPos, -140.0f, 220.0f);
}

static void IdleGL(void) { glutPostRedisplay(); }

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(gWindowWidth, gWindowHeight);
  glutCreateWindow("UmbraMech");

  glutDisplayFunc(DisplayGL);
  glutReshapeFunc(ResizeGL);
  glutKeyboardFunc(KeyDown);
  glutKeyboardUpFunc(KeyUp);
  glutSpecialFunc(SpecialKeyDown);
  glutSpecialUpFunc(SpecialKeyUp);
  glutMouseFunc(MouseButton);
  glutPassiveMotionFunc(MouseMove);
  glutMotionFunc(MouseMove);
  glutIdleFunc(IdleGL);

  glutMainLoop();

  return 0;
}