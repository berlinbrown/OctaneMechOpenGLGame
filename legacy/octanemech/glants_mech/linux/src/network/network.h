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
// network.h
// - mainly client
//
#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

// make sure this matches the server
#if 1
#define UDP_PROTOCOL
#else
#define TCP_PROTOCOL
#endif

#define MAXPENDING 12
#define MAX_SIZE 4096

#define _CONNECTED_ 1
#define _NOT_CONNECTED_ 0

#define _WAITING_ 2
#define _RUNNING_ 4
#define _READY_ 8

#define _HELLO_MSG_ "glAnts Server Hello"

#define _TYPE_SERVER_ 2
#define _TYPE_CLIENT_ 4

#define _WATCHDOG_ON_ 1
#define _WATCHDOG_OFF_ 2

#define CHECK_WATCHDOG (client_obj.watchdog == _WATCHDOG_ON_)
#define TURNOFF_WATCHDOG client_obj.watchdog = _WATCHDOG_OFF_
#define TURNON_WATCHDOG client_obj.watchdog = _WATCHDOG_ON_

#define _NET_SCREEN_X 5
#define _NET_SCREEN_Y1 448
#define _NET_SCREEN_Y2 460

//
// com_obj
//
typedef struct tagComObj
{
  int id;
  int sock;
  char ipaddress[32];
  char local_address[32];

  int msg_size;
  char msg[256];
  int object_id;

  struct sockaddr_in remote_addr;
  unsigned short port;

  int connect_flag;
  int connect_type;  // client/server
  int watchdog;      // on or off

  int activity;  // waiting/running

} ComObj, *ComObjPtr;

void WaitChar(void);
void* WaitMsg(void* args);
void Connect(ComObjPtr com_obj, char* ip, int port);
void RunTest(void);

void Get_LocalAddress(void);
void Create_Client(void);

void Print_ServerRun(void);
void Print_ClientRun(void);

void Set_ServRunning(void);

void Set_ServObjID(int val);
int Get_ServObjID(void);

void Reset_MessageBytes(void);
void Reset_MessageCount(void);

#endif
