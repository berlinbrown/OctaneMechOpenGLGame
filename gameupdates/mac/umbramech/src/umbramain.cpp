/**
 * Copyright (c) 2006-2011 Berlin Brown.  All Rights Reserved
 *
 * http://www.opensource.org/licenses/bsd-license.php
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of the Botnode.com (Berlin Brown) nor
 * the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written
 * permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
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
// Berlin Brown
// berlin _dot__ brown  __at_ g_mail _ dot_ com
//
// glAnt.cpp
//
// http://glants.sourceforge.net
//
// originally designed for Windows
// look for linux version soon
//
// - GL init calls based on nehe's opengl library
//		http://nehe.gamedev.net/
//
// my asus probe temperature monitor has my CPU
// running at 52C, during heavy rendering
// this should mean nothing you, however
//
// - Initial version: (sat)Aug 3, 2002
//

#include <GLUT/glut.h>   // GLUT for window/context
#include <OpenGL/gl.h>   // Core OpenGL functions
#include <OpenGL/glu.h>  // OpenGL Utility Library
#include <float.h>       // used for _control
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void Cmd_Keys(KeySym key);

// use False or True here	--
#define FULLSCREEN_MODE False

//
// Begin in fullscreen - flag
#define FULL_SCREEN_F 0

//
// Try 32/16
#define INIT_BIT_RATE 32

static GLvoid KillGLWindow(GLvoid);
static bool CreateGLWindow(char *title, int width, int height, int bits,
                           bool fullscreenflag);

bool keys[256];  // Array Used For The Keyboard Routine
int keyCodes[20];

bool active = true;      // Window Active Flag Set To TRUE By Default
bool fullscreen = true;  // Fullscreen Flag Set To Fullscreen Mode By Default

GLfloat rtri;   // Angle For The Triangle ( NEW )
GLfloat rquad;  // Angle For The Quad ( NEW )

GLuint base;  // Base Display List For The Font Set

static bool done = false;  // Bool Variable To Exit Loop

int mSuper_Loaded = LOADED_FALSE;

//
// frames per second variables
//
#define FRAME_RATE_SAMPLES 60
int frames = 0;
float framerate = 0;
clock_t last_time = 0;

void LoadKeyCodes(void);

//
//=====================================
// GLWINDOW struct
//
typedef struct {
  Display *dpy;
  int screen;
  Window win;
  GLXContext ctx;
  XSetWindowAttributes attr;
  bool fs;
  XF86VidModeModeInfo deskMode;
  int x, y;
  unsigned int width, height;
  unsigned int depth;
} GLWindow;

static int attrListSgl[] = {GLX_RGBA, GLX_RED_SIZE,  4, GLX_GREEN_SIZE,
                            4,        GLX_BLUE_SIZE, 4, GLX_DEPTH_SIZE,
                            16,       None};

static int attrListDbl[] = {GLX_RGBA,
                            GLX_DOUBLEBUFFER,
                            GLX_GREEN_SIZE,
                            4,
                            GLX_BLUE_SIZE,
                            4,
                            GLX_DEPTH_SIZE,
                            16,
                            None};

GLWindow GLWin;

//
// END LINUX GRAPHICS ROUTINES
//

//
// get frames per second
//
void GetFramesPerSecond(void) {
  clock_t now;
  float delta;

  if (frames++ >= FRAME_RATE_SAMPLES) {
    now = clock();

    delta = (now - last_time) / (float)CLOCKS_PER_SEC;
    // delta = (now - last_time) / (float)CLK_TCK;

    last_time = now;

    framerate = FRAME_RATE_SAMPLES / delta;

    frames = 0;
  }  // end of the if

}  // end of the functino

//
// PrintText --
//
void PrintText(const char *fmt, ...)  // Custom GL "Print" Routine
{
  char text[256];  // Holds Our String
  va_list ap;      // Pointer To List Of Arguments

  if (fmt == NULL)  // If There's No Text
    return;         // Do Nothing

  va_start(ap, fmt);          // Parses The String For Variables
  vsprintf(text, fmt, ap);    // And Converts Symbols To Actual Numbers
  va_end(ap);                 // Results Are Stored In Text
  glPushAttrib(GL_LIST_BIT);  // Pushes The Display List Bits
  glListBase(base - 32);      // Sets The Base Character to 32
  glCallLists(strlen(text), GL_UNSIGNED_BYTE,
              text);  // Draws The Display List Text
  glPopAttrib();      // Pops The Display List Bits
}
s

    //
    // Handle_Esc
    //
    static void
    Handle_Esc(void) {
  if (active) {
    if (ant_globals->menu_mode == MENU_TITLE_MODE) {
      if (ant_globals->_menu_state == FIRST_TIME_TRUE)
        done = true;
      else {
        // put back in run mode
        ant_globals->paused = 0;
        ant_globals->menu_mode = MENU_RUN_MODE;

        // SHOW CURSOR FALSE

      }  // end if -else

    }  // end of theif
    else if (ant_globals->menu_mode == MENU_HELP_MODE) {
      ant_globals->paused = 1;
      ant_globals->menu_mode = MENU_TITLE_MODE;

      // show cursor tru
    } else if (ant_globals->menu_mode == MENU_SETTINGS_MODE) {
      ant_globals->paused = 1;
      ant_globals->menu_mode = MENU_TITLE_MODE;
    } else {
      // done = true;

      ant_globals->paused = 1;
      ant_globals->menu_mode = MENU_TITLE_MODE;

      // showcursor
    }  // end of if-else

  }  // end of the if

}  // end of thefunction

//
// keyPressed
//
void keyPressed(KeySym key) {
  switch (key) {
    case XK_Tab:
      ToggleViewMode();
      break;

    case XK_Down:

      Toggle_MenuItems(1);

      break;

    case XK_Up:
      Toggle_MenuItems(-1);

      break;

    case XK_Return:
    case XK_space:

      if (Set_MenuMode()) {
        done = true;

        return;
      }  // end of the if

      if (ant_globals->menu_mode == MENU_HELP_MODE) {
        // ShowCursor
      }  // end of the if
      else if (ant_globals->menu_mode == MENU_RUN_MODE) {
        // Show Cursor
      }  // end of if -else

      break;

    case XK_Escape:
      // done = true;

      // draw the scene
      Handle_Esc();
      break;

    case XK_P:
    case XK_p:

      if (ant_globals->menu_mode == MENU_RUN_MODE) {
        TogglePaused();
      }  // end of the if

      break;

    case XK_Q:
    case XK_q:
      done = true;
      break;

    case XK_F1:
      KillGLWindow();
      GLWin.fs = !GLWin.fs;
      CreateGLWindow("glAnts", SCREEN_WIDTH, SCREEN_HEIGHT, 24, GLWin.fs);
      break;
  };
}

//
// Cmd_KeyPress
//
static void Cmd_KeyPress(KeySym key) {
  switch (key) {
    case XK_Escape:
      // done = true;

      // draw the scene
      Handle_Esc();
      break;

    case XK_Q:
    case XK_q:
      done = true;
      break;

    case XK_F1:
      KillGLWindow();
      GLWin.fs = !GLWin.fs;
      CreateGLWindow("glAnts", SCREEN_WIDTH, SCREEN_HEIGHT, 24, GLWin.fs);
      break;
  };

}  // end of teh function

//
// LoadKeyCodes
//
void LoadKeyCodes(void) {
  keyCodes[0] = XKeysymToKeycode(GLWin.dpy, XK_Escape);
  keyCodes[1] = XKeysymToKeycode(GLWin.dpy, XK_F1);
  keyCodes[2] = XKeysymToKeycode(GLWin.dpy, XK_l);

  keyCodes[3] = XKeysymToKeycode(GLWin.dpy, XK_f);
  keyCodes[15] = XKeysymToKeycode(GLWin.dpy, XK_F);

  keyCodes[4] = XKeysymToKeycode(GLWin.dpy, XK_b);
  keyCodes[5] = XKeysymToKeycode(GLWin.dpy, XK_Page_Up);
  keyCodes[6] = XKeysymToKeycode(GLWin.dpy, XK_Page_Down);
  keyCodes[7] = XKeysymToKeycode(GLWin.dpy, XK_Up);
  keyCodes[8] = XKeysymToKeycode(GLWin.dpy, XK_Down);
  keyCodes[9] = XKeysymToKeycode(GLWin.dpy, XK_Left);
  keyCodes[10] = XKeysymToKeycode(GLWin.dpy, XK_Right);

  // newest chars
  keyCodes[11] = XKeysymToKeycode(GLWin.dpy, XK_s);
  keyCodes[12] = XKeysymToKeycode(GLWin.dpy, XK_S);

  keyCodes[13] = XKeysymToKeycode(GLWin.dpy, XK_r);
  keyCodes[14] = XKeysymToKeycode(GLWin.dpy, XK_R);
  // Note: 15 used above
  keyCodes[16] = XKeysymToKeycode(GLWin.dpy, XK_space);
}

//
// Draw Hud
//
void DrawHUD(void) {
  glLoadIdentity();

  // change the matrix
  glMatrixMode(GL_PROJECTION);

  // current view
  glPushMatrix();

  // Begin normal code

  // reset the matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(45.0f, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f,
                 PERSPECTIVE_Z);

  glMatrixMode(GL_MODELVIEW);

  GetFramesPerSecond();  // get frames
}

//
// Resize gl scene
//
GLvoid ResizeGLScene(GLsizei width,
                     GLsizei height)  // Resize And Initialize The GL Window
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
// Init GL
//
int InitGL(GLvoid)  // All Setup For OpenGL Goes Here
{
  // seed rand generator
  srand((unsigned int)time(NULL));

  // Load the key codes --
  LoadKeyCodes();

  // enable all the goodies
  Super_LoadGlobals();

  Load_ConfigFile();
  Super_FireAnts();

  Create_Col_List();
  Create_Wall_List();

  //
  // loading sound library
  printf("Loading sound library, make sure 'music apps' are closed\n");
  Load_Audio();

  glShadeModel(GL_SMOOTH);               // Enable Smooth Shading
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  // Black Background
  glClearDepth(1.0f);                    // Depth Buffer Setup

  glEnable(GL_NORMALIZE);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glShadeModel(GL_SMOOTH);

  glEnable(GL_DEPTH_TEST);  // Enables Depth Testing
  glDepthFunc(GL_LEQUAL);   // The Type Of Depth Testing To Do
  glHint(GL_PERSPECTIVE_CORRECTION_HINT,
         GL_NICEST);  // Really Nice Perspective Calculations

  // have to load the quadrice to be used
  quadric = gluNewQuadric();  // Create A Pointer To The Quadric Object (NEW)
  gluQuadricNormals(quadric, GLU_SMOOTH);  // Create Smooth Normals (NEW)
  gluQuadricTexture(quadric, GL_TRUE);     // Create Texture Coords (NEW)

  GenerateFireAnts();

  CreateWorld();

  LoadCameras();

  // Load the objects
  InitObjects();
  GenerateLights();

  InitGlobals();

  Build_ParticleSet();
  Reset_FontID();

  // Load the title screen text
  Load_Titles();

  // load the text library
  Super_MainText();

  CreateWalls();

  //
  // Load the collsion test for the walll
  //

  // front wall
  InsertColSegment(world_ptr->x_min, world_ptr->y_max, world_ptr->x_max,
                   world_ptr->y_max);

  // right wall
  InsertColSegment(world_ptr->x_max, world_ptr->y_min, world_ptr->x_max,
                   world_ptr->y_max);

  // back wall
  InsertColSegment(world_ptr->x_min, world_ptr->y_min, world_ptr->x_max,
                   world_ptr->y_min);

  // left wall
  InsertColSegment(world_ptr->x_min, world_ptr->y_min, world_ptr->x_min,
                   world_ptr->y_max);

  //
  // for the network save
  // the number of bots incase it is changed
  MAX_SAVED_BOTS = MAX_FIRE_ANTS;

  // end of insertion

  Super_InitNetwork();

  //
  // begin in paused mode
  //
  Super_BeginPaused();

  mSuper_Loaded = LOADED_TRUE;

  return true;  // Initialization Went OK
}

//
// AnimateScene
// - do all the processing outside of drawing
//
void AnimateScene(void) {
  if (CHECK_NET_CLIENT) {
    AnimNetworkBots();
  } else if (CHECK_NET_SERVER) {
    AnimNetworkBots();
  } else {
    AnimFireAnts();

  }  // endo if else

  AnimateExplosions();
}

//
// Draw The Scene
//
void DrawGLScene(GLvoid)  // Here's Where We Do All The Drawing
{
  glClear(GL_COLOR_BUFFER_BIT |
          GL_DEPTH_BUFFER_BIT);          // Clear Screen And Depth Buffer
  glClearColor(0.0f, 0.0f, 0.3f, 0.0f);  // Black Background

  // begin scene ---
  BEGIN_BOT;

  RenderWalls();
  RenderPlane();

  SetLights();
  driver_objects[STARS_OBJECT]->render();

  Draw_Wall_List();

  DrawExplosions();

  DrawFireAnts();

  END_BOT;

  // Draw the multiple sets of Heads up displays
  Super_DrawText();

  Draw_Title();

  DrawHUD();
}

//
// Run_Anim
// - combine animation and draw scene
void Run_Anim(void) {
  if (ant_globals->paused == 0) {
    AnimateScene();

    // play music
    // Play_Music();

  }  // end of the fi

  DrawGLScene();  // render to window

  // Swap buffers
  glXSwapBuffers(GLWin.dpy, GLWin.win);
}

//
// ShutdownGL
// - place all memory releases and stuff here
//
void ShutdownGL(void) {
  DeleteObjects();
  ShutdownLights();

  DeleteObjects();

  ShutdownFireAnts();

  ShutdownWorld();

  Delete_Wall_List();
  Delete_Col_List();

  // destroy particle set
  Destroy_ParticleSet();

  Super_KillText();

  // Call after shutdown fireants
  Super_KillFires();

  Super_DeleteNetwork();

  // This should probably be close to last
  Super_KillGlobals();

  DestroyGlobals();
}

//
// KILLGLWINDOW
//
static GLvoid KillGLWindow(GLvoid)  // Properly Kill The Window
{
  if (GLWin.ctx) {
    if (!glXMakeCurrent(GLWin.dpy, None, NULL)) {
      printf("released failed\n");
    }  // end of teh if

    glXDestroyContext(GLWin.dpy, GLWin.ctx);
    GLWin.ctx = NULL;

  }  // end of the if

  // switch back to original resolution
  if (GLWin.fs) {
    XF86VidModeSwitchToMode(GLWin.dpy, GLWin.screen, &GLWin.deskMode);
    XF86VidModeSetViewPort(GLWin.dpy, GLWin.screen, 0, 0);
  }  // end of the if

  XCloseDisplay(GLWin.dpy);

  ShutdownGL();
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:
 ** title			- Title To Appear At The Top Of The Window
 ** width			- Width Of The GL Window Or Fullscreen Mode
 ** height			- Height Of The GL Window Or Fullscreen Mode
 ** bits			- Number Of Bits To Use For Color (8/16/24/32)
 ** fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)
 */
bool CreateGLWindow(char *title, int width, int height, int bits,
                    bool fullscreenflag) {
  XVisualInfo *vi;
  Colormap cmap;
  int dpyWidth, dpyHeight;
  int i;
  int glxMajorVersion, glxMinorVersion;
  int vidModeMajorVersion, vidModeMinorVersion;
  XF86VidModeModeInfo **modes;
  int modeNum;
  int bestMode;
  Atom wmDelete;
  Window winDummy;
  unsigned int borderDummy;

  GLWin.fs = fullscreenflag;
  /* set best mode to current */
  bestMode = 0;
  /* get a connection */
  GLWin.dpy = XOpenDisplay(0);
  GLWin.screen = DefaultScreen(GLWin.dpy);
  XF86VidModeQueryVersion(GLWin.dpy, &vidModeMajorVersion,
                          &vidModeMinorVersion);
  printf("XF86VidModeExtension-Version %d.%d\n", vidModeMajorVersion,
         vidModeMinorVersion);
  XF86VidModeGetAllModeLines(GLWin.dpy, GLWin.screen, &modeNum, &modes);
  /* save desktop-resolution before switching modes */
  GLWin.deskMode = *modes[0];
  /* look for mode with requested resolution */
  for (i = 0; i < modeNum; i++) {
    if ((modes[i]->hdisplay == width) && (modes[i]->vdisplay == height)) {
      bestMode = i;
    }
  }
  /* get an appropriate visual */
  vi = glXChooseVisual(GLWin.dpy, GLWin.screen, attrListDbl);
  if (vi == NULL) {
    vi = glXChooseVisual(GLWin.dpy, GLWin.screen, attrListSgl);
    printf("Only Singlebuffered Visual!\n");
  } else {
    printf("Doublebuffered Visual:		[ OK ]\n");
  }
  glXQueryVersion(GLWin.dpy, &glxMajorVersion, &glxMinorVersion);
  printf("glX-Version %d.%d\n", glxMajorVersion, glxMinorVersion);
  /* create a GLX context */
  GLWin.ctx = glXCreateContext(GLWin.dpy, vi, 0, GL_TRUE);
  /* create a color map */
  cmap = XCreateColormap(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen),
                         vi->visual, AllocNone);
  GLWin.attr.colormap = cmap;
  GLWin.attr.border_pixel = 0;

  if (GLWin.fs) {
    XF86VidModeSwitchToMode(GLWin.dpy, GLWin.screen, modes[bestMode]);
    XF86VidModeSetViewPort(GLWin.dpy, GLWin.screen, 0, 0);
    dpyWidth = modes[bestMode]->hdisplay;
    dpyHeight = modes[bestMode]->vdisplay;
    printf("Resolution %dx%d\n", dpyWidth, dpyHeight);
    XFree(modes);

    /* create a fullscreen window */
    GLWin.attr.override_redirect = True;
    GLWin.attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                            ButtonPressMask | StructureNotifyMask;
    GLWin.win = XCreateWindow(
        GLWin.dpy, RootWindow(GLWin.dpy, vi->screen), 0, 0, dpyWidth, dpyHeight,
        0, vi->depth, InputOutput, vi->visual,
        CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
        &GLWin.attr);
    XWarpPointer(GLWin.dpy, None, GLWin.win, 0, 0, 0, 0, 0, 0);
    XMapRaised(GLWin.dpy, GLWin.win);
    XGrabKeyboard(GLWin.dpy, GLWin.win, True, GrabModeAsync, GrabModeAsync,
                  CurrentTime);
    XGrabPointer(GLWin.dpy, GLWin.win, True, ButtonPressMask, GrabModeAsync,
                 GrabModeAsync, GLWin.win, None, CurrentTime);
  } else {
    /* create a window in window mode*/
    GLWin.attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask |
                            ButtonPressMask | StructureNotifyMask;

    GLWin.win =
        XCreateWindow(GLWin.dpy, RootWindow(GLWin.dpy, vi->screen), 0, 0, width,
                      height, 0, vi->depth, InputOutput, vi->visual,
                      CWBorderPixel | CWColormap | CWEventMask, &GLWin.attr);

    /* only set window title and handle wm_delete_events if in windowed mode */
    wmDelete = XInternAtom(GLWin.dpy, "WM_DELETE_WINDOW", True);
    XSetWMProtocols(GLWin.dpy, GLWin.win, &wmDelete, 1);
    XSetStandardProperties(GLWin.dpy, GLWin.win, title, title, None, NULL, 0,
                           NULL);
    XMapRaised(GLWin.dpy, GLWin.win);
  }

  /* connect the glx-context to the window */
  glXMakeCurrent(GLWin.dpy, GLWin.win, GLWin.ctx);
  XGetGeometry(GLWin.dpy, GLWin.win, &winDummy, &GLWin.x, &GLWin.y,
               &GLWin.width, &GLWin.height, &borderDummy, &GLWin.depth);
  printf("Depth %d\n", GLWin.depth);
  if (glXIsDirect(GLWin.dpy, GLWin.ctx))
    printf("Direct Rendering Enabled	[ OK ]\n");
  else
    printf("Error: No Direct Rendering available\n");

  InitGL();

  return True;
}

//
// main
//
int main(int argc, char **argv) {
  int i;

  char buffer[80];

  XEvent event;
  KeySym key;

  done = False;

  // default to windowed mode
  GLWin.fs = FULLSCREEN_MODE;

  CreateGLWindow("glAnts", SCREEN_WIDTH, SCREEN_HEIGHT, 24, GLWin.fs);

  /* wait for events*/
  while (!done) {
    /* handle the events in the queue */
    while (XPending(GLWin.dpy) > 0) {
      XNextEvent(GLWin.dpy, &event);
      switch (event.type) {
        case Expose:
          if (event.xexpose.count != 0) break;

          Run_Anim();

          break;

        case ConfigureNotify:
          /* call resizeGLScene only if our window-size changed */
          if ((event.xconfigure.width != GLWin.width) ||
              (event.xconfigure.height != GLWin.height)) {
            GLWin.width = event.xconfigure.width;
            GLWin.height = event.xconfigure.height;

            printf("Resize event\n");

            ResizeGLScene(event.xconfigure.width, event.xconfigure.height);
          }
          break;

        case ButtonPress:

          // exit on mouse press

          // done = True;
          break;

        case KeyPress:

          if (ant_globals->menu_mode == MENU_SETTINGS_MODE) {
            key = XLookupKeysym(&event.xkey, 0);

            Cmd_KeyPress(key);

            // handle fullscreen press differently
            if (key == XK_F1) break;

            // process the key
            Cmd_Keys(key);

            XLookupString(&event, buffer, 10, &key, 0);
            Alpha_Keys(buffer);
          } else {
            key = XLookupKeysym(&event.xkey, 0);
            keyPressed(key);
            keys[event.xkey.keycode] = true;

          }  // end of the if

          break;

        case KeyRelease:

          if (ant_globals->menu_mode == MENU_SETTINGS_MODE) {
          } else {
            keys[event.xkey.keycode] = false;

          }  // end of the - if

          break;

        case ClientMessage:
          if (*XGetAtomName(GLWin.dpy, event.xclient.message_type) ==
              *"WM_PROTOCOLS") {
            printf("Exiting glants...\n");
            done = True;
          }
          break;
        default:
          break;
      }
    }

    // Check for key pressed in the camera
    HandleCameraKeys(keys);

    // run the gambit------
    Run_Anim();
  }
  KillGLWindow();

  return 0;

}  // end of main //
