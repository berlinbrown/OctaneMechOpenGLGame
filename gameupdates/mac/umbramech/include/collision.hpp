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
// collision.h
//

#pragma once

#define MOVING_COL_TYPE 3
#define PLANE_COL_TYPE 1  // i.e a wall
#define RAY_COL_TYPE 2    // i.e a bullet shot

#define INWARD_TYPE 1   // wall type
#define OUTWARD_TYPE 2  // box or something

#define LINE_NO_INTERSECTION 1
#define LINE_INTERSECTION 2

//
// max number of line segments for testing
//
#define MAX_DIST_STACK 250

//
// The bullet has to stop somewhere, so
// create some insane value for the desination
#define MAX_BULLET_DEST 2000.0f

//
// collisionobj
// - for now only handle
// objects with a width and position
//
typedef struct tagCollisionObj
{
  int id;

  // used for a line segment
  //
  float pos_0[2];
  float pos_1[2];

  //
  // collision x,y
  // the predicted positions
  float collision_x;
  float collision_y;
  float dist;  // distance of collision

  //
  //
  int movement_type;

  //
  // drawing plane object
  float box_x;
  float box_y;
  float size[3];

  // object can be a driverbot or staticbot
  StaticBotPtr static_ptr;

  DriverBotPtr bot_ptr;

  // we need to stop using static arrays
  struct tagCollisionObj* next;

} CollisionObj, *CollisionPtr;

void Build_DistStack(void);

//
// CollisionList
//
typedef struct tagCollisionList
{
  CollisionObj* front;
  int objects;
} CollisionList;

//
// Library functions
//
CollisionPtr CreateCollisionObj(void);
void DeleteCollisionObj(CollisionObj* ptr);

CollisionList* CreateCollisionList(void);
void DestroyColList(CollisionList* list);
void PrintCollisionList(CollisionList* list);
void InsertColFront(CollisionList* list, CollisionObj* col_obj);

//
// NOTE: in order to use this collision object
// only use the following functions
//

// for outward objects
void InsertColSegment(float x_1, float y_1, float x_2, float y_2);

// call in main.cpp/glAnt.cpp, wherever main is
void Create_Col_List(void);
void Delete_Col_List(void);
void Print_Col_List(void);

// used to perform the check
CollisionPtr CheckCollisionList(void* test_obj, int type);

// another member function for collision test
bool CheckCollisionBot(DriverBotPtr test_obj);

//
// For moving objects here is the library
//
CollisionPtr CheckCollisionMoving(StaticBotPtr test_obj);
void Insert_MovingObj(DriverBotPtr bot);
