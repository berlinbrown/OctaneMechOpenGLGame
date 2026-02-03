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

//
// Berlin Brown
// bigbinc@hotmail.com
//
// camera.cpp
//
#include "camera.h"

#include <gl\gl.h>     // Header File For The OpenGL32 Library
#include <gl\glaux.h>  // Header File For The Glaux Library
#include <gl\glu.h>    // Header File For The GLu32 Library
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "bot.h"
#include "fireants.h"
#include "gldrawlib.h"
#include "globals.h"
#include "menu.h"

static int mouse_x = SCREEN_WIDTH / 2;
static int mouse_y = SCREEN_HEIGHT / 2;

static float mCameraAng = 0.0f;

//
// Follow this object
static DriverBotPtr camera_bot;

void GetCameraBot(DriverBotPtr bot) { camera_bot = bot; }  // end of the functino

//
// GetBotX
//
float GetBotX(void) { return camera_bot->x; }

float GetBotY(void) { return camera_bot->y; }

// with
static float camera_speed = 0.6f;
static float turning_speed = 1.6f;

// this is another of keeping track of the camera position
// a little simpler than a driver objects

int current_camera = 0;

static DriverCamera camera1;
static DriverCamera camera2;
static DriverCamera camera3;
static DriverCamera camera4;
static DriverCamera camera5;
static DriverCamera camera6;
static DriverCamera camera7;
static DriverCamera camera8;
static DriverCamera camera9;
static DriverCamera camera0;

extern LARGE_INTEGER lastTime;
extern LARGE_INTEGER timerFrequency;

static LARGE_INTEGER currentTime;
static float fTime;

//
// Array of camera objects
//
DriverCamera* driver_camera[MAX_CAMERAS] = {&camera0, &camera1, &camera2, &camera3, &camera4,
                                            &camera5, &camera6, &camera7, &camera8, &camera9};

//
// Vector_Minus
//
void Vector_Minus(Vector a, Vector b, Vector res)
{
  res[0] = a[0] - b[0];
  res[1] = a[1] - b[1];
  res[2] = a[2] - b[2];

}  // end of the function

//
// Vector_Add
//
void Vector_Add(Vector a, Vector b, Vector res)
{
  res[0] = a[0] + b[0];
  res[1] = a[1] + b[1];
  res[2] = a[2] + b[2];

}  // end of the function

//
// Vector_Length
//
float Vector_Length(Vector a)
{
  return (float)sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);

}  // end of the function

//
// DotProduct
//
float DotProduct(Vector a, Vector b)
{
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}  // end of the function

//
// Vector_Normalize
//
void Vector_Normalize(Vector a, Vector res)
{
  float len;

  len = Vector_Length(a);

  if (len > 0.0f)
    len = 1.0f / len;
  else
    len = 0.0f;

  res[0] = a[0] * len;
  res[1] = a[1] * len;
  res[2] = a[2] * len;

}  // end of the function

//
// Vector_Multiply
//
void Vector_Multiply(Vector a, Vector res, float b)
{
  res[0] = a[0] * b;
  res[1] = a[1] * b;
  res[2] = a[2] * b;

}  // end of the functiona

//
// A spring dampening function
// for the camera
//
void SpringDamp(Vector currPos,
                Vector trgPos,      // Target Position
                Vector prevTrgPos,  // Previous Target Position
                Vector result,

                float springConst,  // Hooke's Constant
                float dampConst,    // Damp Constant
                float springLen)
{
  Vector disp;      // Displacement
  Vector velocity;  // Velocity
  Vector mx;
  Vector z;

  float len;

  float dot;
  float forceMag;  // Force Magnitude

  // Calculate Spring Force
  Vector_Minus(currPos, trgPos, disp);

  Vector_Minus(prevTrgPos, trgPos, velocity);

  len = Vector_Length(disp);

  // get dot product
  dot = DotProduct(disp, velocity);

  forceMag = springConst * (springLen - len) + dampConst * (dot / len);

  Vector_Normalize(disp, z);

  // disp *= forceMag;
  Vector_Multiply(z, mx, forceMag);

  printf("%0.2f %0.2f\n", mx[0], currPos[0]);

  Vector_Add(currPos, mx, result);

  // result[0] = currPos[0];
  // result[1] = currPos[1];
  // result[2] = currPos[2];

}  // end of the function

//
// For each specific camera
// place in a particular position
//
static void SetupCamera(void)
{
  // start at zero
  SELECT_CAMERA(0);
  PosCamera(0.0f, 2.0f, -10.0f);

  CAMERA->type = CAMERA_WALKING;

  CAMERA->rotation[1] = OFFSET_ROTATION;

  // make sure to reset when done
  SELECT_CAMERA(0);

}  // end of the if

//
// Mouse_Movement
// - handle mouse movement
//
void Mouse_Movement(void)
{
  POINT mouse_pos;
  float ang_z = 0.0f;

  mouse_x = SCREEN_WIDTH / 2;
  mouse_y = SCREEN_HEIGHT / 2;

  // Get the mouse's current X,Y position
  GetCursorPos(&mouse_pos);

  // If our cursor is still in the middle, we never moved... so don't update the screen
  if ((mouse_pos.x == mouse_x) && (mouse_pos.y == mouse_y)) return;

  // Set the mouse position to the middle of our window
  SetCursorPos(mouse_x, mouse_y);

  ang_z = (float)((mouse_y - mouse_pos.y)) / 50.0f;

  // The Angle camera is a little flaky
  // AngleCamera(0.0f, ang_y, 0.0f);

  // increase the zoom also
  CAMERA->zoom_factor -= ang_z;

  if (CAMERA->zoom_factor < 3.0) CAMERA->zoom_factor = 3.0f;

}  // end of the if

//
// Load Cameras
// - basically a constructor for
// the camera objects
// reset variables all to zero, etc
//
void LoadCameras(void)
{
  int index = 0;
  int i = 0;
  current_camera = 0;

  for (index = 0; index < MAX_CAMERAS; index++)
  {
    for (i = 0; i < 3; i++)
    {
      driver_camera[index]->angle[i] = 0.0f;
      driver_camera[index]->position[i] = 0.0f;
      driver_camera[index]->rotation[i] = 0.0f;
    }  // end of the for

    driver_camera[index]->id = index;
    driver_camera[index]->type = CAMERA_STATIC;

    driver_camera[index]->centerx = 0.0f;
    driver_camera[index]->centery = 0.0f;
    driver_camera[index]->centerz = 0.0f;

    driver_camera[index]->zoom_factor = 3.2f;

  }  // end of the functino

  // perform camera specific operations
  SetupCamera();

}  // end of the function

//
// Toggle Camera
// - switch to a different camera using
// a simple iterator, basically cycle through
// the cameras
//
void ToggleCamera(void)
{
  current_camera++;

  if (current_camera >= MAX_CAMERAS) current_camera = 0;

}  // end of the function

//=========================================================
// positioncamera
// - setup the camera
// this should be called before all drawing code
// NOTE: there is a load id, that is important
//  - this is the main matrix, everything it must be
// restored so everything else should have a push or pop
//=========================================================
void SetCamera(void)
{
  glRotatef(CAMERA->angle[1], 0.0f, 1.0f, 0.0f);
  glRotatef(CAMERA->angle[0], 1.0f, 0.0f, 0.0f);
  glRotatef(CAMERA->angle[2], 0.0f, 0.0f, 1.0f);

  glTranslatef(CAMERA->position[0], CAMERA->position[1], CAMERA->position[2]);

  // move camera to according position
  glRotatef(CAMERA->rotation[1], 0.0f, 1.0f, 0.0f);
  glRotatef(CAMERA->rotation[0], 1.0f, 0.0f, 0.0f);
  glRotatef(CAMERA->rotation[2], 0.0f, 0.0f, 1.0f);

}  // end of the functino

//=========================================================
// movecamera
// - basically translate camera
// Note: this should only be called once dont put in loop
//=========================================================
void TranslateCamera(float x, float y, float z)
{
  // move camera to according position
  CAMERA->position[0] += x;
  CAMERA->position[1] += y;
  CAMERA->position[2] += z;

}  // end of the function

//=========================================================
// check angle
//---------------------------------------------------------
void checkangle(void)
{
  int index = 0;
  //  check the bounds

  for (index = 0; index < 3; index++)
  {
    if (CAMERA->angle[index] < 0)
    {
      CAMERA->angle[index] += 360;
    }  // end of the

    if (CAMERA->angle[index] >= 360)
    {
      CAMERA->angle[index] -= 360;
    }  // end of the

  }  // end of the for

}  // end of the functino

//=========================================================
// check rotation
//---------------------------------------------------------
void checkrotation(void)
{
  int index = 0;
  for (index = 0; index < 3; index++)
  {
    if (CAMERA->rotation[index] < 0)
    {
      CAMERA->rotation[index] += 360;
    }  // end of the

    if (CAMERA->rotation[index] >= 360)
    {
      CAMERA->rotation[index] -= 360;
    }  // end of the

  }  // end of the for

}  // end of the functino

//---------------------------------------------------------
// angle camera
// - aim the camera a given directino
// Note: this should only be called once, dont put in loop
//=========================================================
void AngleCamera(float x, float y, float z)
{
  // this may be a little tricky to understand
  // but rotate around own axis if angle flag of
  CAMERA->rotation[0] += x;
  CAMERA->rotation[1] += y;
  CAMERA->rotation[2] += z;
  checkrotation();  // check bounds 0->359

}  // end of the functino

//=========================================================
// turn camera
// - similar to rotate camera
//=========================================================
void TurnCamera(float x, float y, float z)
{
  CAMERA->angle[0] += x;  // rotation around own axis
  CAMERA->angle[1] -= y;
  CAMERA->angle[2] += z;
  checkangle();  // check bounds 0->359

}  // end of the function

//=========================================================
// put camera
// - give a solid constant position
// might be used at the beginning of the main drawing loop
//=========================================================
void PosCamera(float x, float y, float z)
{
  // move camera to according position
  CAMERA->position[0] = x;
  CAMERA->position[1] = -y;
  CAMERA->position[2] = z;
}  // end of the function

//=========================================================
// put camera
// - give a solid constant position
// might be used at the beginning of the main drawing loop
//=========================================================
void Pos_Camera(float x, float y, float z)
{
  // move camera to according position
  CAMERA->position[0] = x;
  CAMERA->position[1] = y;
  CAMERA->position[2] = z;
}  // end of the function

//=========================================================
// set the rotation angles of the cameras
///========================================================
void SetCameraRot(float x, float y, float z)
{
  // setup the camera rotations
  // The trick is figure out which rotation
  // to use (angle or rotation)
  CAMERA->rotation[0] = x;
  CAMERA->rotation[1] = y;
  CAMERA->rotation[2] = z;

}  // end of the functino

//=========================================================
// set angles of the camera
// interesting
//=========================================================
void SetCameraAngle(float x, float y, float z)
{
  CAMERA->angle[0] = x;
  CAMERA->angle[1] = y;
  CAMERA->angle[2] = z;
}  // end of the funtino

//
// Move Forward
//
void MoveForward(void)
{
  float heading = 0;
  float xpos, zpos;

  float speed = camera_speed;
  float new_speed;

  if (CAMERA->type == CAMERA_WALKING)
  {
    heading = CAMERA->angle[1];

    new_speed = fTime * speed;
    xpos = (float)sin(heading * PI_180) * new_speed;
    zpos = (float)cos(heading * PI_180) * new_speed;

    TranslateCamera(-xpos, 0.0f, zpos);
  }
  else
  {
    TranslateCamera(0.0f, 0.0f, camera_speed);
  }  // end of if

}  // end of the functino

//
// Move Backwards
//
void MoveBackward(void)
{
  float heading = 0;
  float xpos, zpos;

  float speed = camera_speed;
  float new_speed;

  if (CAMERA->type == CAMERA_WALKING)
  {
    heading = CAMERA->angle[1];

    heading = CAMERA->angle[1];
    new_speed = fTime * speed;

    xpos = (float)sin(heading * PI_180) * new_speed;
    zpos = (float)cos(heading * PI_180) * new_speed;

    TranslateCamera(xpos, 0.0f, -zpos);
  }
  else
  {
    TranslateCamera(0.0f, 0.0f, -camera_speed);
  }  // end of if

}  // end of the functino

//
// MoveLeft
//
void MoveLeft(void)
{
  float angle;

  if (CAMERA->type == CAMERA_WALKING)
  {
    angle = fTime * turning_speed;
    TurnCamera(0.0f, angle, 0.0f);
  }
  else
  {
    angle = fTime * camera_speed;
    TranslateCamera(camera_speed, 0.0f, 0.0f);
  }  // end of if

}  // end of the functino

//
// SyncCamera
//
void SyncCamera(void)
{
  // sync the camera with the timing
  QueryPerformanceCounter(&currentTime);
  fTime = (float)(currentTime.QuadPart - lastTime.QuadPart) / (float)timerFrequency.QuadPart;
  lastTime = currentTime;

  // null query performance
  fTime = 1.0f;

}  // end of the function

//
// MoveRight
//
void MoveRight(void)
{
  float angle;

  if (CAMERA->type == CAMERA_WALKING)
  {
    angle = fTime * turning_speed;
    TurnCamera(0.0f, -angle, 0.0f);
  }
  else
  {
    angle = fTime * camera_speed;
    TranslateCamera(-angle, 0.0f, 0.0f);
  }  // end of if

}  // end of the functino

//
// Paused_Camera
// - gives kind of a matrix effect
// - not really, but at least I tried
//
void Paused_Camera(void)
{
  float ang;
  float rad;
  float tmp_heading;
  float tmp_x;
  float tmp_y;

  float lookat_x;
  float lookat_y;
  float cam_x, cam_y;
  float x, y;
  float t;

  //
  // calculate the lookat point
  //
  ang = CAMERA->rotation[1] + camera_bot->heading + mCameraAng;

  // now the next point of the triangle
  // shift by 45 degrees
  tmp_heading = ang + 90.0f;
  if (tmp_heading > 360.0f) tmp_heading -= 360.0f;

  rad = tmp_heading / RAD_TO_DEG;

  // also include the zoom
  tmp_x = LOOKAT_OFFSET * (float)cos(rad) * CAMERA->zoom_factor;
  tmp_y = LOOKAT_OFFSET * (float)sin(rad) * CAMERA->zoom_factor;

  tmp_x = tmp_x + camera_bot->x;
  tmp_y = (-tmp_y) + camera_bot->y;

  lookat_x = tmp_x;
  lookat_y = tmp_y;

  // get the camera position now

  tmp_heading = ang + 270.0f;
  if (tmp_heading > 360.0f) tmp_heading -= 360.0f;

  rad = tmp_heading / RAD_TO_DEG;

  tmp_x = CAMERA_BOT_OFFSET * (float)cos(rad) * CAMERA->zoom_factor;
  tmp_y = CAMERA_BOT_OFFSET * (float)sin(rad) * CAMERA->zoom_factor;

  cam_x = tmp_x + camera_bot->x;
  cam_y = (-tmp_y) + camera_bot->y;

  // must be paused

  x = lookat_x;
  y = lookat_y;

  // Of course you can replace glulookat
  // with your own function
  gluLookAt(cam_x, (CAMERA_HEIGHT * CAMERA->zoom_factor), cam_y, x, 0.0f, y, 0.0f, 1.0f, 0.0f);

  // Rotate the paused camera a bot
  // based on the frame rate
  if (framerate <= 8.0f) framerate = 30.0f;

  t = 0.38f / framerate;
  t = framerate * t;

  mCameraAng += t;
  if (mCameraAng > 360.0f) mCameraAng -= 360.0f;

}  // end of the function

//
// FirstPersonMode
//
static void FirstPersonMode(bool* keys)
{
  float x, y;
  float h;
  float cam_x;
  float cam_y;

  x = camera_bot->look_x;
  y = camera_bot->look_y;
  h = camera_bot->look_h;

  cam_x = camera_bot->cam_x;
  cam_y = camera_bot->cam_y;

  Pos_Camera(cam_x, FIRST_HEIGHT, cam_y);

  // Of course you can replace glulookat
  // with your own function
  gluLookAt(CAMERA->position[0], CAMERA->position[1], CAMERA->position[2], x, h, y, 0.0f, 1.0f,
            0.0f);

  // player control, strange place I know
  Player_Control(keys);

}  // end of the function

//
// ThirdPersonMode
//
static void ThirdPersonMode(bool* keys)
{
  float x, y;
  float cam_x;
  float cam_y;

  // process the mouse moves around
  // our axis
  Mouse_Movement();

  x = camera_bot->look_x;
  y = camera_bot->look_y;

  cam_x = camera_bot->cam_x;
  cam_y = camera_bot->cam_y;

  Pos_Camera(cam_x, (CAMERA_HEIGHT * CAMERA->zoom_factor), cam_y);

  // Of course you can replace glulookat
  // with your own function
  gluLookAt(CAMERA->position[0], CAMERA->position[1], CAMERA->position[2], x, 0.0f, y, 0.0f, 1.0f,
            0.0f);

  // player control, strange place I know
  Player_Control(keys);

}  // end of the function

//
// ToggleViewMode
//
// Note: this should be called in WinProc, Winmain
//
void ToggleViewMode(void)
{
  // cant change mode while paused
  if (ant_globals->paused == 1) return;

  if (camera_bot->id == PLAYER_0)
  {
    if (camera_bot->alive == DEAD_STATE)
    {
      camera_bot->view_mode = THIRD_PERSON_MODE;
      return;
    }  // end of the if

    if (camera_bot->view_mode == THIRD_PERSON_MODE)
      camera_bot->view_mode = FIRST_PERSON_MODE;
    else
      camera_bot->view_mode = THIRD_PERSON_MODE;

  }  // end of the if
  else
  {
    camera_bot->view_mode = THIRD_PERSON_MODE;

  }  // end of the if -else

}  // end of the function

//
// Handle Camera Keys
//
// - also handles the following camera
//
void HandleCameraKeys(bool* keys)
{
  if (ant_globals->paused == 0)
  {
    if (camera_bot->view_mode == THIRD_PERSON_MODE)
    {
      ThirdPersonMode(keys);
    }
    else if (camera_bot->view_mode == FIRST_PERSON_MODE)
    {
      FirstPersonMode(keys);

    }  // end of the if --
  }
  else
  {
    Paused_Camera();

  }  // end of the if else

}  // end of the functino
