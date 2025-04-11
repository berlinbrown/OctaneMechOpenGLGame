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
// Berlin Brown - berlin _dot_ brown __at __ gmail 
//
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "bot.h"

#define MAX_ZOOM			45.2f

#define OFFSET_ROTATION		8.0f

#define PI_1 3.141592654f

#define  PI		 (3.14159265358f)
#define  PI_180	         (PI_1 / 180.0f)

#define CAMERA		driver_camera[current_camera]

typedef float Vector[3];

#define MAX_CAMERAS		4	

#define BEGIN_CAMERA 	glPushMatrix(); SetCamera();		
#define END_CAMERA		glPopMatrix();

#define SELECT_CAMERA(cam_id)	current_camera = cam_id

#define CAMERA_STATIC			0
#define CAMERA_WALKING			1

//----------------------------------
// define a camera struct
//..................................
typedef struct tagCamera 
{

	float position[3];			// current location
	float angle[3];				// angle camera is pointing
	float rotation[3];			// rotation around the world
	float centerx;				// center axis
	float centery;
	float centerz;				// center axis x, y, z

	float Yaw;
	float Pitch;
	float Roll;

	float	zoom_factor;
	float	old_zoom;			// save zoom

	int		id;					// id number for camera
	int		type;				// camera TYPE
	
} DriverCamera;


void Vector_Normalize(Vector a, Vector res);

void HandleCameraKeys(bool *keys);


float GetBotX(void);
float GetBotY(void);

void LoadCameras(void);
void ToggleCamera(void);
void SetCamera(void);

void TranslateCamera(float x, float y, float z);
void AngleCamera(float x, float y, float z);
void TurnCamera(float x, float y, float z);

void PosCamera(float x, float y, float z);

void SyncCamera(void);

void GetCameraBot(DriverBotPtr bot);

void SpringDamp(	
		Vector currPos,
		Vector trgPos,     // Target Position
		Vector prevTrgPos, // Previous Target Position
		Vector result,

		float springConst,  // Hooke's Constant
		float dampConst,    // Damp Constant
		float springLen);

extern DriverCamera *driver_camera[MAX_CAMERAS];
extern int	current_camera;

void ToggleViewMode(void);


#endif

