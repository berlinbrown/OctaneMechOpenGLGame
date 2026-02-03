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
// objects.h
// driver for object source
//
#ifndef _OBJECTS_H_
#define _OBJECTS_H_

#define CUBE_OBJECT 0
#define GRID_OBJECT 1
#define ANT_OBJECT 2
#define WORLD_OBJECT 3
#define WIREBOX_OBJECT 4
#define NORM_CUBE_OBJECT 5
#define SQUARE_OBJECT 6
#define PLANE_OBJECT 7
#define FIREANT_OBJECT 8
#define PYRAMID_OBJECT 9
#define STARS_OBJECT 10

#define MAX_OBJECTS 11

#define GRID_SIZE 300.0f

extern GLUquadricObj* quadric;  // main Quadric Object

//
// major struct for driver objects
//
typedef struct tagDriverObjects
{
  void (*init)(int list_id);
  void (*compile)(void);
  void (*draw)(void);    // used with compile
  void (*render)(void);  // render object to scenes

  int call_id;  // id used to compile object
  int visible;

} DriverObjects;

void InitObjects(void);
void DeleteObjects(void);
void RenderObjects(void);
void RenderGrid(void);

void RenderWalls(void);

void RenderBounds(float x, float y, float width);

void Bot_Triangle(float x, float y);

//
// set of the objects
//
extern DriverObjects colorcube;
extern DriverObjects grid;
extern DriverObjects ant;
extern DriverObjects walls;
extern DriverObjects wirebox;
extern DriverObjects norm_cube;
extern DriverObjects pheromone;
extern DriverObjects plane;
extern DriverObjects fireant;
extern DriverObjects pyramid;
extern DriverObjects stars;

extern DriverObjects* driver_objects[MAX_OBJECTS];

void RenderPlane(void);
#endif
