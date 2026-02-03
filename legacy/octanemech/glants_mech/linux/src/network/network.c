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
// - client code
//
// network.c
// - c network routines
//
// linux version gcc 2.96/ mandrake 8.2
//
// Note: run /etc/services for empty ports
//
#include "network.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>  // posix threads
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include "../bot.h"
#include "../fireants.h"
#include "../globals.h"
#include "include/connect.h"
#include "include/msg.h"

#define SNIFFER_OFF 0
#define SNIFFER_ON 1

extern char network_str_[22][80];

//
// main client object
static ComObj client_obj;

static pthread_t thread_id[2];

struct timeval now;
struct timespec timeout;

static unsigned long _msg_bytes = 0;

//
// sniffer variables
pthread_mutex_t snifferMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t snifferCond = PTHREAD_COND_INITIALIZER;
pthread_attr_t attr;

static int sniffer_flag = SNIFFER_OFF;
static int sniffer_state = SNIFFER_OFF;

//
// Reset_MessageCount
//
void Reset_MessageBytes(void) { _msg_bytes = 0; }  // end of thef unction

//
// GetMessageCount
//
int GetMessageCount(void)
{
  int z;

  z = (_msg_bytes / sizeof(Msg));

  return z;

}  // end of func

//
// Set_NetAddr
//
void Set_RemoteAddr(ComObj* ptr, struct sockaddr_in* c_addr)
{
  memcpy(&ptr->remote_addr, c_addr, sizeof(struct sockaddr_in));

}  // end of the if

//
// Create_Mutex()
void Create_Mutex(void)
{
  pthread_mutex_init(&snifferMutex, NULL);

  // also create condition
  pthread_cond_init(&snifferCond, NULL);

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

}  // end of the function

//
// Destroy Mutex
void Destroy_Mutex(void)
{
  pthread_mutex_destroy(&snifferMutex);
  pthread_cond_destroy(&snifferCond);

}  // end of the function

//
// WaitThread
//
void WaitThread(void)
{
  int status;
  int i;
  int res;

  pthread_attr_destroy(&attr);

  for (i = 0; i < 2; i++)
  {
    // blocked call
    res = pthread_join(thread_id[i], (void**)&status);  // Parent waits for

    pthread_cancel(thread_id[1]);
    printf("join complete, killing thread 2\n");
    break;

  }  // end of the for

}  // end of the function

//
// Lock_Sniffer()
//
static void Lock_Sniffer(void) { sniffer_flag = SNIFFER_ON; }  // end of func

//
// unlock
static void Unlock_Sniffer(void) { sniffer_flag = SNIFFER_OFF; }  // end of func

//
// another thread function
//
void* SnifferThread(void* args)
{
  int res;
  res = 0;

  // main while loop
  while (1)
  {
    if (sniffer_state == SNIFFER_OFF)
    {
      if (sniffer_flag == SNIFFER_ON)
      {
        // flag is set restart the timer
        sniffer_state = SNIFFER_ON;

        // reset collection flag --
        sniffer_flag = SNIFFER_OFF;

        // collect a new timeout
        gettimeofday(&now, NULL);
        timeout.tv_sec = now.tv_sec + ALARM_TIMEOUT;
        timeout.tv_nsec = now.tv_usec * 1000;

      }  // end of the if
    }
    else
    {
      // sniffer state is going

      pthread_mutex_lock(&snifferMutex);
      while (1)
      {
        // note, this is another blocked call in addition to recvfrom

        // res returns zero is based on condition
        res = pthread_cond_timedwait(&snifferCond, &snifferMutex, &timeout);
        if (res == ETIMEDOUT)
        {
          // kill the other thread
          sniffer_state = SNIFFER_OFF;
          pthread_cancel(thread_id[0]);
          printf("Timer was released, killing thread\n");

          client_obj.connect_flag = _NOT_CONNECTED_;
          close(client_obj.sock);
          Set_NetworkMsg("Connection Timed Out");

          // unlock the watchdog
          Unlock_Sniffer();

          break;
        }  // end of the if

      }  // end of while
      pthread_mutex_unlock(&snifferMutex);

    }  // end of if - else

    // continue with sniffer agent

  }  // endof while --

}  // end of the function

//
// waitmsg
//
void* WaitMsg(void* args)
{
  int bytes_recv;
  int sock;
  struct sockaddr_in from_addr;
  unsigned int addr_size;

  int no_bots;

  void* buf = NULL;
  MsgPtr msg = NULL;

  // get the socket data
  sock = ((ComObjPtr)args)->sock;
  bytes_recv = 0;

  // main thread loop, get messages
  for (;;)
  {
    //
    Clear_FirstMsg();

    buf = GetMsgCluster();
    addr_size = sizeof(from_addr);

    if (CHECK_WATCHDOG) Lock_Sniffer();

    bytes_recv = recvfrom(sock, buf, MAX_SIZE, 0, (struct sockaddr*)&from_addr, &addr_size);

    if (CHECK_WATCHDOG) Unlock_Sniffer();

    // we will keep it simple
    // - if it is a reply message
    // process just that one
    msg = Get_FirstMsg();
    if (msg->msg_type == MSG_REPLY)
    {
      // copy the message to the display string
      Set_NetworkMsg(msg->msg);

      // make sure we are connet
      client_obj.connect_flag = _CONNECTED_;

      // no more watchdog, run client
      // till the server/client quits
      TURNOFF_WATCHDOG;

      // and more yet,
      // close the network screen
      // once we have a server intro
      // message
      // close screen and wait for
      // user to press new game
      // and display message

      client_obj.activity = _READY_;

      SET_NET_CLIENT;  // turn network interface on

      // get your current id
      client_obj.object_id = msg->object_id;

      Set_MsgId(msg->object_id);

      // Note that the client
      // cannot start the game at
      // this point
      // once the server sends a start
      // game, the game begins
      Mode_TitleScreen();

    }  // end of the
    else
    {
      // the first messagse is not a
      // reply type, so that means
      // it is a game position
      _msg_bytes = bytes_recv;

      //
      // not perfect, but it will speed things up
      no_bots = _msg_bytes / sizeof(Msg);

      // check first message
      switch (msg->msg_type)
      {
        case MSG_LOAD:

          // change into running mode
          // start the network game!
          Mode_SetRunning();
          client_obj.activity = _RUNNING_;

          MAX_NETWORK_BOTS = no_bots;
          Set_MsgIndex(no_bots);

          break;

        default:
          break;

      };  // end of switch

      // after the blocking call recvfrom
      // process the message que

      // this msgindex is flaky....!
      Set_MsgIndex(no_bots);
      Run_NetworkBots();

    }  // end of if

  }  // end of the for

  return NULL;
}  // end of the function

//
// Get_LocalAddress
// - utility function
// not must substance
// - you can call this without loading objects
//
void Get_LocalAddress(void)
{
  int res;
  int i;
  char* buf = NULL;
  int text_start = 0;
  int sock;
  unsigned short port;
  struct sockaddr_in serv_addr;
  struct sockaddr_in remote_addr;

  port = CONNECT_PORT;

#if defined(UDP_PROTOCOL)
  sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
#else
  // create a socket for incoming connection
  sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#endif

  if (sock < 0)
  {
    printf("socket() failed\n");
    exit(1);
  }  // end of the if

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);

  // bind to the address
  res = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  if (res < 0)
  {
    printf("bind failed\n");
    exit(1);
  }  // end of if

#if 1
  remote_addr.sin_port = htons(port);
  remote_addr.sin_family = AF_INET;

  // get local address
  //
  res = sizeof(remote_addr);
  if (connect(sock, (struct sockaddr*)&remote_addr, res) < 0)
  {
    printf("connect() failed\n");
  }
#endif

  //
  res = sizeof(serv_addr);
  if (getsockname(sock, (struct sockaddr*)&serv_addr, &res) < 0)
  {
    printf("getsockname() failed\n");
  }  // end of the if

  // copy the address
  // only for the server
  buf = inet_ntoa(serv_addr.sin_addr);
  res = strlen(buf);
  text_start = 14;

  for (i = 0; i < res; i++)
  {
    network_str_[SERVER_NET_MENU + 1][text_start + i] = *buf++;

  }  // end of the for

  // null-terminate
  network_str_[SERVER_NET_MENU + 1][text_start + i] = '\0';

  // now close the connection
  close(sock);

}  // end of the function

//
// Build_LoginMsg
//
void Build_LoginMsg(MsgPtr ptr)
{
  // see include/msg.h
  char buffer[42];
  char buf[34];
  int text_start;
  int res;
  char c;
  int i;

  ptr->msg_type = MSG_LOGIN;

  text_start = 15;
  res = 32;

  for (i = 0; i < res; i++)
  {
    // this may work, but so will final_str
    // which is safer!
    c = network_str_[CLIENT_NET_MENU + 0][text_start + i];
    buf[i] = c;

    if ((c == '\0') || (c == '\n'))
    {
      break;
    }  // end of if

  }  // end of the for

  // null-terminate
  buf[i] = '\0';

  // build the message
  sprintf(buffer, "%s?%d?%s", buf, __VERSION__, __OS__);
  strcpy(ptr->msg, buffer);

}  // end of the function

//
// Build_MoveMsg
// - actually use sendto and send the message to the server
//
void Build_MoveMsg(float x, float y, float heading, int type)
{
  int res;
  void* buf = NULL;
  int id;
  int serv_id;
  Msg msg;

  id = client_obj.object_id;

  msg.pos_x = x;
  msg.pos_y = y;
  msg.heading = heading;
  msg.object_id = id;
  msg.msg_type = type;

  buf = (void*)&msg;
  res = sizeof(Msg);

  if (CHECK_NET_CLIENT)
  {
    res = sendto(client_obj.sock, (MsgPtr)&msg, res, 0, (struct sockaddr*)&client_obj.remote_addr,
                 sizeof(client_obj.remote_addr));
  }
  else if (CHECK_NET_SERVER)
  {
    // this is server code, so the id will be based on the server obj
    // skip the sendto and just add to the message queue
    // to be sent out
    serv_id = Get_ServObjID();
    Msg_AddQueue(MSG_MOVE, type, serv_id, x, y, heading);

  }  // end of if-else

}  // end of the function

//
// Connect
//
void Connect(ComObjPtr com_obj, char* ip, int port)
{
  int res;
  struct sockaddr_in serv_addr;
  Msg msg;

  strcpy(com_obj->ipaddress, ip);
  com_obj->port = port;

  com_obj->sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (com_obj->sock < 0)
  {
    fprintf(stderr, "socket() failed\n");
  }  // end of the if

  // construct server address
  memset(&serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(com_obj->ipaddress);
  serv_addr.sin_port = htons(com_obj->port);

  // build the message and send it off,
  res = sizeof(Msg);

  Build_LoginMsg((MsgPtr)&msg);

  res =
      sendto(com_obj->sock, (MsgPtr)&msg, res, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

  // set remote address --
  Set_RemoteAddr(com_obj, (struct sockaddr_in*)&serv_addr);

  // timeout variables and init functions
  Create_Mutex();

  // for simplicity, turn on connected, this may need refinement
  com_obj->connect_flag = _CONNECTED_;
  com_obj->connect_type = _TYPE_CLIENT_;
  com_obj->watchdog = _WATCHDOG_ON_;

  // create a sniffer agent thread also
  pthread_create(&thread_id[1], &attr, SnifferThread, NULL);

  // also recv a message
  // create a thread to wait for messages
  pthread_create(&thread_id[0], &attr, WaitMsg, (void*)com_obj);

}  // end of the function

//
// RunTest
//
void Connect_Server(char* buffer)
{
  client_obj.activity = _WAITING_;
  Connect((ComObjPtr)&client_obj, buffer, CONNECT_PORT);

}  // end of the function

//
// Create_Client
// - called at main
//
void Create_Client(void)
{
  client_obj.connect_flag = _NOT_CONNECTED_;
  TURNON_WATCHDOG;

  client_obj.activity = _WAITING_;

}  // end of thefunction

//
// Kill_Client
//
void Kill_Client(void)
{
  if (client_obj.connect_flag == _CONNECTED_)
  {
    Destroy_Mutex();
    close(client_obj.sock);

  }  // end of the if

}  // end of the function

//
//
void Print_ClientRun(void)
{
  char buffer[80];

  if ((client_obj.activity == _RUNNING_) || (client_obj.activity == _READY_))
  {
    sprintf(buffer, "Messages x%d recvd\n", GetMessageCount());

    if (client_obj.activity == _RUNNING_)
      Draw_TString(_NET_SCREEN_X, _NET_SCREEN_Y1, "Client Running");
    else
      Draw_TString(_NET_SCREEN_X, _NET_SCREEN_Y1, "Client Waiting");

    Draw_TString(_NET_SCREEN_X, _NET_SCREEN_Y2, buffer);

  }  // end of the if

}  // end of the function
