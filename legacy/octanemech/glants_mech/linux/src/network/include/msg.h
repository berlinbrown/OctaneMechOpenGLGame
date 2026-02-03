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
// msg.h
//
#ifndef _MSG_H_
#define _MSG_H_

// timeout at 5 seconds
// 1 is really too long but we will be nice
#define ALARM_TIMEOUT 10

// you may need a lot, you may need a little
#define MAX_MESSAGES 200

#define RELEASE_OBJ(x) \
  if (x != NULL) free(x)

enum MsgType
{
  MSG_MOVE = 0,
  MSG_CHAT = 1,
  MSG_LOGIN = 2,
  MSG_REPLY = 3,
  MSG_LOAD = 4
};

enum MoveType
{
  MOVE_FORWARD = 8,
  MOVE_BACKWARD = 16,
  MOVE_TURNLEFT = 32,
  MOVE_TURNRIGHT = 64,
  MOVE_FIRE = 128,
  MOVE_NOMOVE = 256,
  MOVE_COLLIDE = 512,
};

//

// have to be careful, dont want
// too many variables
//
// most messages will revolve around
// movement
typedef struct tagMsg
{
  int msg_type;
  int move_type;
  int msg_id;     // the index
  int object_id;  // the client connect id

  // move
  float pos_x;
  float pos_y;
  float heading;

  // misc messages
  char msg[42];

  struct tagMsg* next;
} Msg, *MsgPtr;

//
// MsgList
typedef struct tagMsgList
{
  Msg* front;
  int objects;
} MsgList;

Msg* CreateMsgObj(void);
void Create_Msg_List(void);
void Delete_Msg_List(void);
void Print_Msg_List(void);

void Msg_Test(void);
void Msg_AddQueue(int msg_type, int move_type, int obj_id, float px, float py, float h);
void* Convert_MsgList(void);
Msg* GetMsgCluster(void);
void Print_MsgTest(void);

void Clear_FirstMsg(void);

//
// stats.c
void Start_Net_Time(void);
void End_Net_Time(void);
void Print_Net_Time(void);
void Send_Net_Packet(int sock, int serv_sock);
void Recv_Net_Packet(int sock, int serv_sock);

MsgPtr Get_FirstMsg(void);

int Get_MessageCount(void);

void Set_MsgIndex(int val);
void Set_MsgId(int val);
Msg* Get_Messages(int* msg_count, int* msg_id);
void* Finalize_Messages(int* res_count);
void Reset_Message(void);

#endif
