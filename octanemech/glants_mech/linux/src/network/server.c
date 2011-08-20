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
// server.c
//
// - run the server thread
//
// - the server code needs to build a queue of clients
// linux version
// gcc under mandrake, 2.96
//
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <pthread.h> // posix threads

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../bot.h"
#include "include/msg.h"
#include "include/clients.h"
#include "include/connect.h"
#include "../globals.h"
#include "network.h"

static pthread_t thread_id;

static ComObjPtr server_obj = NULL;

//
// Set_ServObjID
//
void Set_ServObjID(int val)
{

  server_obj->object_id = val;

} // end of the function 

//
// Get_ServObjID(void)
//
int Get_ServObjID(void)
{
  if (server_obj != NULL)
    return server_obj->object_id;

  return -1;

} // end of the function

//
// Print_ServerRun
//
void Print_ServerRun(void)
{
  char buffer[80];

  if (server_obj != NULL)
    {

      if (server_obj->activity == _RUNNING_)
	{
	  sprintf(buffer, "Messages x%d sent\n", Get_MessageCount()); 

	  Draw_TString(_NET_SCREEN_X, _NET_SCREEN_Y1,"Server Running");
	  Draw_TString(_NET_SCREEN_X, _NET_SCREEN_Y2,buffer);

	} // end of the if 

    } // end of the if 

} // end of the function 


//
// Perform_Snapshots
//
void Perform_Snapshots(void)
{

  if (CHECK_NET_SERVER)
    {
      if (server_obj->activity == _RUNNING_)
	{
	  
	  mPerformSnapshot();
	  
	} // end of theif 

    } // end of the if 

} // end of the function 

//
// Set_ServRunning
//
void Set_ServRunning(void)
{

  server_obj->activity = _RUNNING_;
  
} // end of the functino 

//
// Server_ReadMsg
// - process msg
//
int Server_ReadMsg(MsgPtr msg, struct sockaddr_in *client_addr, int sock)
{
  Msg reply_msg;
  int type;
  int id;
  
  int res;
  int n;
  char buffer[80];
  void *buf = NULL;

  type = msg->msg_type;

  switch(type)
    {

    case MSG_LOGIN:		
      // on login, get the name
      // and the ip address
      // send a message back
      
      // login means we get to queue the addresses
      id = Set_ClientID();
      Client_AddQueue(sock, client_addr, id, msg->msg);

      if (server_obj != NULL)
	{
      
	  // dont waste any time, send off the 
	  // ok message
	  reply_msg.msg_type = MSG_REPLY;
	  sprintf(buffer, "*%s?%d?%d",_HELLO_MSG_, __VERSION__,  server_obj->activity);
	  strcpy(reply_msg.msg, buffer);

	  reply_msg.object_id = id;

	  res = sizeof(reply_msg);
	  buf = (void *)&reply_msg;

	  n = sendto(sock, buf, res,  0,
		       (struct sockaddr *)client_addr, 
		     sizeof(struct sockaddr_in));

	  // thats it, the message is sent, 
	  // the client will wait for snapshots

	} // end of the if 


      // after this message is sent, thats it, the client
      // waits for a start, game message
      break;
      
    default:

      Msg_AddQueue(msg->msg_type,msg->move_type,msg->object_id,
		   msg->pos_x, msg->pos_y, msg->heading);
	
      break;
    };


  return -1;

} // end of the fucntion 

//
// Server recvfrom thread
// - run accept in order to collect new connections
//
// Every message that comes into this thread 
// will be a message type including logins
//
// the server has to distinguish what to do with the 
// message
//
void *Server_Recvfrom(void *args)
{
  int res;
  int len;
  struct sockaddr_in client_addr;

  Msg msg;                     // temporary variable
  void *buf = NULL;
  
  // main server network event loop
  for(;;)
    {
      len = sizeof(client_addr);

      buf = (void *)&msg;  

      // blocking call, watch out
      res = recvfrom(((ComObjPtr)args)->sock, buf, MAX_SIZE, 0,
		     (struct sockaddr *)&client_addr, &len);

      // unblocked, now get the address add to queue
      Server_ReadMsg(&msg, &client_addr, ((ComObjPtr)args)->sock);

      // read the message
  
      if (res < 0) {
	fprintf(stderr, "recvfrom() failed\n");
      } // end of the if 
      
    } // end of the for (main-loop)

  
  return NULL;
  
} // end of the function 


//
// 

//
// - Wait for connections
// - launch the thread
void Start_Server(ComObjPtr com_obj, int port)
{

  int res;
  struct sockaddr_in serv_addr;  	
  
  com_obj->port = (unsigned short)port;
  com_obj->sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (com_obj->sock < 0)
    {
      printf("socket() failed\n");
    } // end of the if 

  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  //serv_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
  serv_addr.sin_port = htons(com_obj->port);

  // bind to the address
  res = bind(com_obj->sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  if (res < 0)
    {
      printf("bind failed\n");
    } // end of if 

  server_obj->connect_flag = _CONNECTED_;

  // create the wait thread
  pthread_create(&thread_id, NULL, 
		 Server_Recvfrom, (void *)com_obj);

} // end of the function 

//
// PrintConnections
//
void printConnections(void)
{

  if (server_obj == NULL)
    return;

  if (server_obj->connect_flag == _CONNECTED_)
    {
      Print_Connections();
    } // end of the if 

} // end of the function

//
// Launch_Server
// - same as start_server
// wrapper for start-server
void Launch_Server(void)
{
  server_obj = malloc(sizeof(ComObj));

  server_obj->connect_flag = _NOT_CONNECTED_;
  server_obj->activity = _WAITING_;


  SET_NET_SERVER;

  Start_Server(server_obj, CONNECT_PORT);

} // end of the function 

//
// Kill_Server
//
void Kill_Server(void)
{

  if (server_obj != NULL)
    {
      // kill the main thread
      pthread_cancel(thread_id);

      if (server_obj->connect_flag == _CONNECTED_)
	close(server_obj->sock);
      
      if (server_obj != NULL)
	free(server_obj);

    } // end of if 


} // end of the function




