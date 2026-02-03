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
// connect.h
//
#ifndef _CONNECT_H_
#define _CONNECT_H_

#define CONNECT_STR "127.0.0.1"
#define CONNECT_PORT 9000

#define SNAP_ELAPSED                          \
  SnapEndTime.tv_sec - SnapStartTime.tv_sec + \
      ((SnapEndTime.tv_usec - SnapStartTime.tv_usec) / 1.0E6);

//
// Snapshot time
//
#define SNAP_SHOT_T 60

//
// Place server object Here

void Start_Service(char final_str[4][80], int mode);
void Get_NetworkMsg(char* buffer);
void Set_NetworkMsg(char* buffer);

// clients.c
void Build_StartMsg(void);

// super functions
//
void Super_InitNetwork(void);
void Super_DeleteNetwork(void);

void Launch_Server(void);
void Kill_Server(void);

//
// call in bitmaps.c or glant.c
//
void Build_StartMsg(void);

//
// should be called in fireants.c
//
void Build_MoveMsg(float x, float y, float heading, int type);

//
// network.c
void Kill_Client(void);
void Connect_Server(char* buffer);
void Print_NetRun(void);

void Snapshot_StartTime(void);
void mPerformSnapshot(void);

// call this outside of the network interface
void Perform_Snapshots(void);

#endif
