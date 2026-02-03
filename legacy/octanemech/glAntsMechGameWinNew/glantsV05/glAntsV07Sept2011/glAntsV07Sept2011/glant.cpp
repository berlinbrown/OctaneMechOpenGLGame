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
 * Description: Simple OpenGL Mech Game.  The glant.cpp file contains the
 * main entry point for the application.
 *
 * Contact: Berlin Brown <berlin dot brown at gmail.com>
 *
 * https://github.com/berlinbrown/OctaneMechOpenGLGame
 *
 * Build environment, tested : 9/20/2011
 * Visual C++ 6.0 for Win32
 */

#include <float.h>     // used for _control
#include <gl\gl.h>     // Header File For The OpenGL32 Library
#include <gl\glaux.h>  // Header File For The Glaux Library
#include <gl\glu.h>    // Header File For The GLu32 Library
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>  // Header File For Windows

#include "bot.h"
#include "camera.h"
#include "collision.h"
#include "fireants.h"
#include "gldrawlib.h"
#include "globals.h"
#include "lights.h"
#include "list.h"
#include "maps.h"
#include "menu.h"
#include "objects.h"
#include "octree.h"
#include "particles.h"
#include "plist.h"
#include "resource.h"
#include "text.h"
#include "tree.h"
#include "walls.h"
#include "world.h"

//
// Begin in fullscreen - flag
#define FULL_SCREEN_F 0

//
// Try 32/16
#define INIT_BIT_RATE 32

HDC hDC = NULL;       // Private GDI Device Context
HGLRC hRC = NULL;     // Permanent Rendering Context
HWND hWnd = NULL;     // Holds Our Window Handle
HINSTANCE hInstance;  // Holds The Instance Of The Application

bool keys[256];          // Array Used For The Keyboard Routine
bool active = TRUE;      // Window Active Flag Set To TRUE By Default
bool fullscreen = TRUE;  // Fullscreen Flag Set To Fullscreen Mode By Default

GLfloat rtri;   // Angle For The Triangle ( NEW )
GLfloat rquad;  // Angle For The Quad ( NEW )

GLuint base;  // Base Display List For The Font Set

static BOOL done = FALSE;  // Bool Variable To Exit Loop

int mSuper_Loaded = LOADED_FALSE;

//
// frames per second variables
//
#define FRAME_RATE_SAMPLES 60
int frames = 0;
float framerate = 0;
clock_t last_time = 0;

LARGE_INTEGER timerFrequency;
LARGE_INTEGER lastTime;

//
// get frames per second
//
void GetFramesPerSecond(void)
{
  clock_t now;
  float delta;

  GameTick();  // add to global counter
  CheckRespawn();

  if (frames++ >= FRAME_RATE_SAMPLES)
  {
    now = clock();
    delta = (now - last_time) / (float)CLOCKS_PER_SEC;
    AddTime((float)(now - last_time));
    AddSeconds(delta);  // tabulate data
    last_time = now;
    framerate = FRAME_RATE_SAMPLES / delta;
    frames = 0;
  }  // end of the if

}  // end of the function

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);  // Declaration For WndProc

//
// Build FONT
//
GLvoid BuildFont(GLvoid)  // Build Our Bitmap Font
{
  HFONT font;     // Windows Font ID
  HFONT oldfont;  // Used For Good House Keeping

  base = glGenLists(96);  // Storage For 96 Characters

  font = CreateFont(-14,                          // Height Of Font
                    0,                            // Width Of Font
                    0,                            // Angle Of Escapement
                    0,                            // Orientation Angle
                    FW_NORMAL,                    // Font Weight
                    FALSE,                        // Italic
                    FALSE,                        // Underline
                    FALSE,                        // Strikeout
                    ANSI_CHARSET,                 // Character Set Identifier
                    OUT_TT_PRECIS,                // Output Precision
                    CLIP_DEFAULT_PRECIS,          // Clipping Precision
                    ANTIALIASED_QUALITY,          // Output Quality
                    FF_DONTCARE | DEFAULT_PITCH,  // Family And Pitch
                    "Arial");                     // Font Name

  oldfont = (HFONT)SelectObject(hDC, font);  // Selects The Font We Want
  wglUseFontBitmaps(hDC, 32, 96, base);      // Builds 96 Characters Starting At Character 32
  SelectObject(hDC, oldfont);                // Selects The Font We Want
  DeleteObject(font);                        // Delete The Font

}  // end of the function

//
// kill font
//
GLvoid KillFont(GLvoid)  // Delete The Font List
{
  glDeleteLists(base, 96);  // Delete All 96 Characters
}  // end of the function

//
// PrintText --
//
void PrintText(const char* fmt, ...)  // Custom GL "Print" Routine
{
  char text[256];  // Holds Our String
  va_list ap;      // Pointer To List Of Arguments

  if (fmt == NULL)  // If There's No Text
    return;         // Do Nothing

  va_start(ap, fmt);        // Parses The String For Variables
  vsprintf(text, fmt, ap);  // And Converts Symbols To Actual Numbers
  va_end(ap);               // Results Are Stored In Text

  glPushAttrib(GL_LIST_BIT);                          // Pushes The Display List Bits
  glListBase(base - 32);                              // Sets The Base Character to 32
  glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  // Draws The Display List Text
  glPopAttrib();                                      // Pops The Display List Bits

}  // end of the function s

//
// Draw Hud
//
void DrawHUD(void)
{
  glLoadIdentity();

  // change the matrix
  glMatrixMode(GL_PROJECTION);

  // current view
  glPushMatrix();

#if ENABLE_HUD

  glDisable(GL_LIGHTING);

  // setup projectino for text
  glLoadIdentity();
  glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1.0, 1.0);

  // get rid of textured text
  glDisable(GL_TEXTURE_2D);

  // dont need depth test
  glDisable(GL_DEPTH_TEST);

  // draw variables
  glColor3f(1.0, 1.0, 0.0);

  glRasterPos2i(10, 48);
  PrintText("FPS: %0.2f", framerate);

  PrintGlobals();

  glPopMatrix();  // end

  // reset the textures
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);

#endif

  // Begin normal code

  // reset the matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, PERSPECTIVE_Z);

  glMatrixMode(GL_MODELVIEW);
  GetFramesPerSecond();  // get frames

}  // end of the function

//
// Resize gl scene
//
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)  // Resize And Initialize The GL Window
{
  if (height == 0)  // Prevent A Divide By Zero By
  {
    height = 1;  // Making Height Equal One
  }

  glViewport(0, 0, width, height);  // Reset The Current Viewport

  glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
  glLoadIdentity();             // Reset The Projection Matrix

  // Calculate The Aspect Ratio Of The Window
  gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

  glMatrixMode(GL_MODELVIEW);  // Select The Modelview Matrix
  glLoadIdentity();            // Reset The Modelview Matrix
}

//
// LaunchConsole
//
void LaunchConsole(void)
{
  AllocConsole();  // Create Console Window

  freopen("CONIN$", "rb", stdin);  // reopen stdin handle as console window input

  freopen("CONOUT$", "wb", stdout);  // reopen stout handle as console window output

  freopen("CONOUT$", "wb", stderr);  // reopen stderr handle as console window output

}  // end of the function

//
// KillConsole
//
void KillConsole(void)
{
  FreeConsole();  // Free Console Window

}  // end of the function

//
// Init GL
//
int InitGL(GLvoid)  // All Setup For OpenGL Goes Here
{
  // seed rand generator
  srand(getclock());

#if 0
    QueryPerformanceFrequency(&timerFrequency);
    QueryPerformanceCounter(&lastTime);
#endif

  // ShowCursor(FALSE);
  Super_LoadGlobals();

  Load_ConfigFile();

  // Now load main variables
  Super_LoadBots();
  Super_FireAnts();

  Create_Col_List();

  Create_Wall_List();

  // enable all the goodies
  // glEnable(GL_TEXTURE_2D);		   // enable texture mapping

  glShadeModel(GL_SMOOTH);               // Enable Smooth Shading
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f);  // Black Background
  glClearDepth(1.0f);                    // Depth Buffer Setup

  glEnable(GL_NORMALIZE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glShadeModel(GL_SMOOTH);

  glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
  glDepthFunc(GL_LEQUAL);                             // The Type Of Depth Testing To Do
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Really Nice Perspective Calculations

  // have to load the quadrice to be used
  quadric =
      gluNewQuadric();  // Create A Pointer To The Quadric Object (Return 0 If No Memory) (NEW)
  gluQuadricNormals(quadric, GLU_SMOOTH);  // Create Smooth Normals (NEW)
  gluQuadricTexture(quadric, GL_TRUE);     // Create Texture Coords (NEW)

#if ENABLE_LIGHTS
//	InitMaterial();
#endif

  GenerateBots();

  GenerateFireAnts();  // a new breed of warrior

  CreateWorld();

  LoadCameras();

  BuildFont();  // Build The Font

  // Load the objects
  InitObjects();

  GenerateLights();

  InitGlobals();  // for printing data, etc

  // generate the nest objects
  nest.generate();
  garden.generate();

  trail_set.generate();

  // give the ant food
  InitFood();

  Build_ParticleSet();

  Reset_FontID();

  // load the title screen text
  Load_Titles();

  // the text library
  Super_MainText();

  CreateWalls();

  //
  // Load the collision test for the wall
  // very easy test
  // 4 different walls
  //

  // front wall
  InsertColSegment(world_ptr->x_min, world_ptr->y_max, world_ptr->x_max, world_ptr->y_max);

  // right wall
  InsertColSegment(world_ptr->x_max, world_ptr->y_min, world_ptr->x_max, world_ptr->y_max);

  // back wall (top)
  InsertColSegment(world_ptr->x_min, world_ptr->y_min, world_ptr->x_max, world_ptr->y_min);

  // left wall
  InsertColSegment(world_ptr->x_min, world_ptr->y_min, world_ptr->x_min, world_ptr->y_max);

  //
  // The first thing to do is begin in paused mode
  //
  Super_BeginPaused();
  mSuper_Loaded = LOADED_TRUE;
  return TRUE;  // Initialization Went OK

}  // end of the function

//
// AnimateScene
// - do all the processing outside of drawing
//
void AnimateScene(void)
{
  AnimFireAnts();
  AnimateExplosions();

}  // end of the function

//
// Draw The Scene
//
void DrawGLScene(GLvoid)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear Screen And Depth Buffer
  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);                // Black Background

  // BEGIN_CAMERA			// setup viewing camera
  BEGIN_BOT;
  // RenderGrid();
  RenderWalls();
  RenderPlane();

#if ENABLE_LIGHTS
  SetLights();
#endif
  driver_objects[STARS_OBJECT]->render();
  Draw_Wall_List();
  DrawExplosions();  // draw fire explosions
  DrawFireAnts();    // the mother of all enemies

  // END_CAMERA				// end viewing camera
  END_BOT;

  // Draw the heads up display --
  Super_DrawText();
  Draw_Title();
  DrawHUD();

}  // end of the function

//
// Run_Anim
// - combine animation and draw scene
void Run_Anim(void)
{
  if (ant_globals->paused == 0) AnimateScene();  // do all the processing

  DrawGLScene();  // render to window

}  // end of the function

//
// ShutdownGL
// - place all memory releases and stuff here
//
void ShutdownGL(void)
{
#if 1
  KillFont();  // kill the font

  DeleteObjects();

  ShutdownBots();

  ShutdownFireAnts();

  nest.shutdown();

  trail_set.shutdown();
  garden.shutdown();

  ShutdownWorld();  // hope I got all these

  Delete_Wall_List();

  Delete_Col_List();

  // ShutdownLights();

  Destroy_ParticleSet();

  Super_KillText();

  // Try to place code below --
  Super_KillBots();
  Super_KillFires();

  // make sure thispretty close to last
  Super_KillGlobals();
  DestroyGlobals();

#if ENABLE_CONSOLE
  KillConsole();  // kill launched console screen
#endif

#endif

}  // end of the function

//
// KILLGLWINDOW
//
GLvoid KillGLWindow(GLvoid)  // Properly Kill The Window
{
  if (fullscreen)  // Are We In Fullscreen Mode?
  {
    ChangeDisplaySettings(NULL, 0);  // If So Switch Back To The Desktop
    ShowCursor(TRUE);                // Show Mouse Pointer
  }

  if (hRC)  // Do We Have A Rendering Context?
  {
    if (!wglMakeCurrent(NULL, NULL))  // Are We Able To Release The DC And RC Contexts?
    {
      MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR",
                 MB_OK | MB_ICONINFORMATION);
    }

    if (!wglDeleteContext(hRC))  // Are We Able To Delete The RC?
    {
      MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR",
                 MB_OK | MB_ICONINFORMATION);
    }
    hRC = NULL;  // Set RC To NULL
  }

  if (hDC && !ReleaseDC(hWnd, hDC))  // Are We Able To Release The DC
  {
    MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR",
               MB_OK | MB_ICONINFORMATION);
    hDC = NULL;  // Set DC To NULL
  }

  if (hWnd && !DestroyWindow(hWnd))  // Are We Able To Destroy The Window?
  {
    MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
    hWnd = NULL;  // Set hWnd To NULL
  }

  if (!UnregisterClass("OpenGL", hInstance))  // Are We Able To Unregister Class
  {
    MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
    hInstance = NULL;  // Set hInstance To NULL
  }  // end of the if

  ShutdownGL();

}  // end of the function

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
 *	title			- Title To Appear At The Top Of The Window
 *	* width			- Width Of The GL Window Or Fullscreen Mode
 *	* height			- Height Of The GL Window Or Fullscreen Mode
 *	* bits			- Number Of Bits To Use For Color (8/16/24/32)			*
 *	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
  GLuint PixelFormat;                // Holds The Results After Searching For A Match
  WNDCLASS wc;                       // Windows Class Structure
  DWORD dwExStyle;                   // Window Extended Style
  DWORD dwStyle;                     // Window Style
  RECT WindowRect;                   // Grabs Rectangle Upper Left / Lower Right Values
  WindowRect.left = (long)0;         // Set Left Value To 0
  WindowRect.right = (long)width;    // Set Right Value To Requested Width
  WindowRect.top = (long)0;          // Set Top Value To 0
  WindowRect.bottom = (long)height;  // Set Bottom Value To Requested Height

  fullscreen = fullscreenflag;  // Set The Global Fullscreen Flag

  hInstance = GetModuleHandle(NULL);              // Grab An Instance For Our Window
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Redraw On Size, And Own DC For Window.
  wc.lpfnWndProc = (WNDPROC)WndProc;              // WndProc Handles Messages
  wc.cbClsExtra = 0;                              // No Extra Window Data
  wc.cbWndExtra = 0;                              // No Extra Window Data
  wc.hInstance = hInstance;                       // Set The Instance
  wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);         // Load The Default Icon
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // Load The Arrow Pointer
  wc.hbrBackground = NULL;                        // No Background Required For GL
  wc.lpszMenuName = NULL;                         // We Don't Want A Menu
  wc.lpszClassName = "OpenGL";                    // Set The Class Name

  if (!RegisterClass(&wc))  // Attempt To Register The Window Class
  {
    MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;  // Return FALSE
  }

  //
  // Launch console called
  //
#if ENABLE_CONSOLE
  LaunchConsole();  // console for printing functions
#endif

  if (fullscreen)  // Attempt Fullscreen Mode?
  {
    DEVMODE dmScreenSettings;                                // Device Mode
    memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));  // Makes Sure Memory's Cleared
    dmScreenSettings.dmSize = sizeof(dmScreenSettings);      // Size Of The Devmode Structure
    dmScreenSettings.dmPelsWidth = width;                    // Selected Screen Width
    dmScreenSettings.dmPelsHeight = height;                  // Selected Screen Height
    dmScreenSettings.dmBitsPerPel = bits;                    // Selected Bits Per Pixel
    dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
    if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
    {
      // If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
      if (MessageBox(NULL,
                     "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use "
                     "Windowed Mode Instead?",
                     "NeHe GL", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
      {
        fullscreen = FALSE;  // Windowed Mode Selected.  Fullscreen = FALSE
      }
      else
      {
        // Pop Up A Message Box Letting User Know The Program Is Closing.
        MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
        return FALSE;  // Return FALSE
      }
    }
  }

  if (fullscreen)  // Are We Still In Fullscreen Mode?
  {
    dwExStyle = WS_EX_APPWINDOW;  // Window Extended Style
    dwStyle = WS_POPUP;           // Windows Style
                                  // ShowCursor(FALSE);
    // // Hide Mouse Pointer
  }
  else
  {
    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;  // Window Extended Style
    dwStyle = WS_OVERLAPPEDWINDOW;                   // Windows Style
  }

  AdjustWindowRectEx(&WindowRect, dwStyle, FALSE,
                     dwExStyle);  // Adjust Window To True Requested Size

  // Create The Window
  if (!(hWnd = CreateWindowEx(dwExStyle,                           // Extended Style For The Window
                              "OpenGL",                            // Class Name
                              title,                               // Window Title
                              dwStyle |                            // Defined Window Style
                                  WS_CLIPSIBLINGS |                // Required Window Style
                                  WS_CLIPCHILDREN,                 // Required Window Style
                              0, 0,                                // Window Position
                              WindowRect.right - WindowRect.left,  // Calculate Window Width
                              WindowRect.bottom - WindowRect.top,  // Calculate Window Height
                              NULL,                                // No Parent Window
                              NULL,                                // No Menu
                              hInstance,                           // Instance
                              NULL)))  // Dont Pass Anything To WM_CREATE
  {
    KillGLWindow();  // Reset The Display
    MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;  // Return FALSE
  }

  // Note: for the zbuffer, I have tried 32/16
  // they both work, 32 works the best
  //
  static PIXELFORMATDESCRIPTOR pfd =  // pfd Tells Windows How We Want Things To Be
      {
          sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor
          1,                              // Version Number
          PFD_DRAW_TO_WINDOW |            // Format Must Support Window
              PFD_SUPPORT_OPENGL |        // Format Must Support OpenGL
              PFD_DOUBLEBUFFER,           // Must Support Double Buffering
          PFD_TYPE_RGBA,                  // Request An RGBA Format
          bits,                           // Select Our Color Depth
          0,
          0,
          0,
          0,
          0,
          0,  // Color Bits Ignored
          0,  // No Alpha Buffer
          0,  // Shift Bit Ignored
          0,  // No Accumulation Buffer
          0,
          0,
          0,
          0,               // Accumulation Bits Ignored
          INIT_BIT_RATE,   // 16Bit Z-Buffer (Depth Buffer)
          0,               // No Stencil Buffer
          0,               // No Auxiliary Buffer
          PFD_MAIN_PLANE,  // Main Drawing Layer
          0,               // Reserved
          0,
          0,
          0  // Layer Masks Ignored
      };

  if (!(hDC = GetDC(hWnd)))  // Did We Get A Device Context?
  {
    KillGLWindow();  // Reset The Display
    MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;  // Return FALSE
  }

  if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))  // Did Windows Find A Matching Pixel Format?
  {
    KillGLWindow();  // Reset The Display
    MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;  // Return FALSE
  }

  if (!SetPixelFormat(hDC, PixelFormat, &pfd))  // Are We Able To Set The Pixel Format?
  {
    KillGLWindow();  // Reset The Display
    MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;  // Return FALSE
  }

  if (!(hRC = wglCreateContext(hDC)))  // Are We Able To Get A Rendering Context?
  {
    KillGLWindow();  // Reset The Display
    MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;  // Return FALSE
  }

  if (!wglMakeCurrent(hDC, hRC))  // Try To Activate The Rendering Context
  {
    KillGLWindow();  // Reset The Display
    MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR",
               MB_OK | MB_ICONEXCLAMATION);
    return FALSE;  // Return FALSE
  }

  ShowWindow(hWnd, SW_SHOW);     // Show The Window
  SetForegroundWindow(hWnd);     // Slightly Higher Priority
  SetFocus(hWnd);                // Sets Keyboard Focus To The Window
  ReSizeGLScene(width, height);  // Set Up Our Perspective GL Screen

  if (!InitGL())  // Initialize Our Newly Created GL Window
  {
    KillGLWindow();  // Reset The Display
    MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
    return FALSE;  // Return FALSE
  }

  return TRUE;  // Success
}

LRESULT CALLBACK WndProc(HWND hWnd,      // Handle For This Window
                         UINT uMsg,      // Message For This Window
                         WPARAM wParam,  // Additional Message Information
                         LPARAM lParam)  // Additional Message Information
{
  switch (uMsg)  // Check For Windows Messages
  {
    case WM_ACTIVATE:  // Watch For Window Activate Message
    {
      if (!HIWORD(wParam))  // Check Minimization State
      {
        active = TRUE;  // Program Is Active
      }
      else
      {
        active = FALSE;  // Program Is No Longer Active
      }

      return 0;  // Return To The Message Loop
    }

    case WM_SYSCOMMAND:  // Intercept System Commands
    {
      switch (wParam)  // Check System Calls
      {
        case SC_MAXIMIZE:
          break;

        case SC_RESTORE:
          break;

        case SC_MINIMIZE:
          break;

        case SC_SCREENSAVE:    // Screensaver Trying To Start?
        case SC_MONITORPOWER:  // Monitor Trying To Enter Powersave?
          return 0;            // Prevent From Happening
      }
      break;  // Exit
    }

    case WM_CLOSE:  // Did We Receive A Close Message?
    {
      PostQuitMessage(0);  // Send A Quit Message
      return 0;            // Jump Back
    }

    case WM_KEYDOWN:  // Is A Key Being Held Down?
    {
      // auxillary handle keyboard
      // use the keys interface instead
      // this is for special functions
      switch (wParam)
      {
        case VK_TAB:
          ToggleViewMode();
          break;

        case VK_DOWN:
          Toggle_MenuItems(1);
          break;

        case VK_UP:
          Toggle_MenuItems(-1);
          break;

        case VK_RETURN:
        case VK_SPACE:

          // check for the current
          if (Set_MenuMode())
          {
            ShowCursor(TRUE);
            done = true;
            return 0;
          }  // end of the if

          if (ant_globals->menu_mode == MENU_HELP_MODE)
          {
            ShowCursor(TRUE);
          }  // end of the if
          else if (ant_globals->menu_mode == MENU_RUN_MODE)
          {
            // ShowCursor(FALSE);
          }  // end of if - else

          break;

        case VK_ESCAPE:

          // Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
          if (active)  // Active?  Was There A Quit Received?
          {
            if (ant_globals->menu_mode == MENU_TITLE_MODE)
            {
              if (ant_globals->_menu_state == FIRST_TIME_TRUE)
                done = TRUE;
              else
              {
                // put back in run mode
                ant_globals->paused = 0;
                ant_globals->menu_mode = MENU_RUN_MODE;
                // ShowCursor(FALSE);

              }  // end of if
            }
            else if (ant_globals->menu_mode == MENU_HELP_MODE)
            {
              ant_globals->paused = 1;
              ant_globals->menu_mode = MENU_TITLE_MODE;

              ShowCursor(TRUE);
            }
            else
            {
              // done=TRUE;							// ESC or
              // DrawGLScene Signalled A Quit
              ant_globals->paused = 1;
              ant_globals->menu_mode = MENU_TITLE_MODE;

              ShowCursor(TRUE);
            }  // end of if-else

          }  // end of the if

          break;

        case 'P':

          if (ant_globals->menu_mode == MENU_RUN_MODE)
          {
            TogglePaused();
          }  // end of the if

          break;

        case 'Q':

          done = TRUE;

          break;

        default:
          break;
      };

      keys[wParam] = TRUE;  // If So, Mark It As TRUE
      return 0;             // Jump Back
    }

    case WM_KEYUP:  // Has A Key Been Released?
    {
      keys[wParam] = FALSE;  // If So, Mark It As FALSE
      return 0;              // Jump Back
    }

    case WM_SIZE:  // Resize The OpenGL Window
    {
      ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
      return 0;                                       // Jump Back
    }
  }

  // Pass All Unhandled Messages To DefWindowProc
  return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance,      // Instance
                   HINSTANCE hPrevInstance,  // Previous Instance
                   LPSTR lpCmdLine,          // Command Line Parameters
                   int nCmdShow)             // Window Show State
{
  MSG msg;  // Windows Message Structure

#if 0
    // Ask The User Which Screen Mode They Prefer
    if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
    {
        fullscreen=FALSE;							// Windowed Mode
    } // end of the if
#endif

  // for the sake of simplicity, start in windowed mode
#if FULL_SCREEN_F
  fullscreen = TRUE;
#else
  fullscreen = FALSE;
#endif

  // Create Our OpenGL Window
  if (!CreateGLWindow("glAnt", SCREEN_WIDTH, SCREEN_HEIGHT, INIT_BIT_RATE, fullscreen))
  {
    return 0;  // Quit If Window Was Not Created
  }

  while (!done)  // Loop That Runs While done=FALSE
  {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  // Is There A Message Waiting?
    {
      if (msg.message == WM_QUIT)  // Have We Received A Quit Message?
      {
        done = TRUE;  // If So done=TRUE
      }
      else  // If Not, Deal With Window Messages
      {
        TranslateMessage(&msg);  // Translate The Message
        DispatchMessage(&msg);   // Dispatch The Message
      }
    }
    else  // If There Are No Messages
    {
      Run_Anim();  // draw and process!!!

      SwapBuffers(hDC);  // Swap Buffers (Double Buffering)

      // process the keyboard some where else
      HandleCameraKeys(keys);

      if (keys[VK_F1])  // Is F1 Being Pressed?
      {
        keys[VK_F1] = FALSE;       // If So Make Key FALSE
        KillGLWindow();            // Kill Our Current Window
        fullscreen = !fullscreen;  // Toggle Fullscreen / Windowed Mode

        // Recreate Our OpenGL Window
        if (!CreateGLWindow("glAnts", 640, 480, INIT_BIT_RATE, fullscreen))
        {
          return 0;  // Quit If Window Was Not Created
        }  // end if Create
      }  // end if f1
    }
  }

  // Shutdown
  KillGLWindow();       // Kill The Window
  return (msg.wParam);  // Exit The Program

}  // end of the function
