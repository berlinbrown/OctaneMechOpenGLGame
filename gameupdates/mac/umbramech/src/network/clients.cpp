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
// berlin _dot__ brown  __at_ g_mail _ dot_ com
//
// clients.c
// 
// - this is mainly used with the server code--
//
// - dont get confused with client
// this is a list setup for saving  clients
//
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#include "network.h"
#include "../bot.h"                   // release obj
#include "../globals.h"
#include "include/clients.h"
#include "include/msg.h"
#include "include/connect.h"

#define T_COL1                    160
#define T_COL2                    240
#define T_COL3                    300
#define T_COL4                    370
#define T_COL5                    420
#define T_ROW_Y1                  340
#define _HORZ_START               60.0f

//
// the main list
static ClientList *client_list = NULL;


struct timeval SnapEndTime;
struct timeval SnapStartTime;

//
// This one is brutal
// - we need some way of telling the server admin(hehe)
// about the number of connections,
// we use opengl to draw the text
// - I will try to keep opengl type stuff close knit
// 
// this can be called pretty much anywhere, mainly in a iterative function
//
void Print_Connections(void)
{
  int i = 0;
  const int height = 12;
  const int h_0 = T_ROW_Y1;
  Client *current_ptr = NULL;

  if (client_list->front == NULL)
    return;

  current_ptr = client_list->front;
  

  // iterate through the list
  while(current_ptr != NULL)
    {
      
      // draw to network string interface
      Draw_TString(_HORZ_START, h_0+(i*height), current_ptr->ip_address);
      Draw_TString(T_COL1, h_0+(i*height), current_ptr->user_name);
      Draw_TString(T_COL3, h_0+(i*height), current_ptr->os_str);

      i++;
      current_ptr = current_ptr->next;
    } // en dof the while 

} // end of the function

//
// Set_NetGlobals
//
void Set_NetGlobals(void)
{
  
} // end of the function

//
//
// Build_StartMsg
// - the server will set the positions
// of the players
//
// - this will be called from main.c(glant.c)
// - once the new game is selected
//
void Build_StartMsg(void)
{
  int i = 0;
  DriverBots bot;               // tmp bot
  Client *current_ptr = NULL;
  float x=0, y=0;
  float heading;
  void *msg_ptr = NULL;
  int res = 0;
  int sock;
  int obj_id = 0;
    
    
  // create the message list to send to the clients
  Create_Msg_List();
  if (client_list->front == NULL)
    return;

  current_ptr = client_list->front;
  
  i = 0;
  // iterate through the list
  while(current_ptr != NULL)
    {
      
      PositionBot(&bot);
      
      x = bot.x;
      y = bot.y;

      heading = (float)(rand()%360);

      // the object id is a little
      // tricky, we will use the id given by the server
      // for server bots, we will use obj_id++
      obj_id = current_ptr->object_id;
      Msg_AddQueue(MSG_LOAD, MOVE_NOMOVE, obj_id, x, y, heading);
     
      i++;
      current_ptr = current_ptr->next;
    } // en dof the while 

  // we added the clients, now add the servers bots
  // and of-course the users bot id
  // <adding multiple local bots is optional right now>
  
  // add the local server as a user
  PositionBot(&bot);
  
  x = bot.x;
  y = bot.y;
  
  heading = (float)(rand()%360);

  // using the last object id, we will just add 1
  // i and obj_id should match, but you never know
  obj_id = obj_id + 1;
  Msg_AddQueue(MSG_LOAD, MOVE_NOMOVE, obj_id, x, y, heading);

  // and this will be the server bot id
  Set_ServObjID(obj_id);

  i++;

  //
  // This seems like a nice place to send off
  // the position message
  // once this message is sent, then the client will start playing
  // and then waitin     

  msg_ptr = Convert_MsgList(); 

  // loop through again for each client
  //
  // send the udp packets
  res = i * sizeof(Msg);  

  current_ptr = client_list->front;
  // iterate through the list
  while(current_ptr != NULL)
    {

      sock = current_ptr->sock;

      sendto(sock, msg_ptr, res, 0,
	     (struct sockaddr *)&current_ptr->client_addr, 
	     sizeof(current_ptr->client_addr));

      current_ptr = current_ptr->next;

    } // en dof the while      

  // setup where the server is in running mode
  Set_ServRunning();

  // 
  // and finally set the number of network bots
  MAX_NETWORK_BOTS = i;
 
  // now delete the list
  Delete_Msg_List();

  
  // and also reset the message que
  Reset_MessageCount();
  
  //
  // Create another message list for collecting normal operation
  // messages
  Create_Msg_List();
  
} // end of the function 



//
// SNAPSHOT INTERFACE -------------
// - every 50-100ms send a snapshot of the messages the come into the server
//
// - use in main function or other function in mainloop
// - this will be a wrapper for the server.c interface
//

//
// Shapshot_StartTime
//
void Snapshot_StartTime(void)
{
   gettimeofday(&SnapStartTime, NULL);
  
} // end of the function 

//
// Perform
void mPerformSnapshot(void)
{ 
  void *msg_ptr = NULL;
  int res = 0;
  int sock;
  int res_count = 0;
  

  int h;
  Client *current_ptr = NULL;


  float e_time;
  float r_time;
  
  gettimeofday(&SnapEndTime, NULL);

  e_time = SNAP_ELAPSED;
  r_time = e_time * 1000;

  if (r_time > SNAP_SHOT_T) {

    gettimeofday(&SnapStartTime, NULL);

    // perform the snapshot
    //====================

    // send to every client the message queu
    //msg_ptr = Convert_MsgList(); 
    msg_ptr = Finalize_Messages(&res_count);

    // loop through again for each client
    //
    // send the udp packets
    res = res_count * sizeof(Msg);  
    
    current_ptr = client_list->front;
    // iterate through the list
    while(current_ptr != NULL)
      {
	
	sock = current_ptr->sock;
	
	h = sendto(sock, msg_ptr, res, 0,
	       (struct sockaddr *)&current_ptr->client_addr, 
	       sizeof(current_ptr->client_addr));

	current_ptr = current_ptr->next;

      } // en dof the while      

    
    // now delete the list and create another one
    Delete_Msg_List();

    Reset_MessageCount();

    Create_Msg_List();
    
  } // end of if

} // end of the function 


//=============

//
// Server data
// - this func is open to suggestion
// this the id for each client
int Set_ClientID(void)
{
  return client_list->objects;

} // end of function

//// CreateMsgList
//
ClientList *CreateClientList(void) {
  
  ClientList *result = malloc(sizeof(ClientList));

  result->front = NULL;
  result->objects = 0;

  return result;
  
} // end of teh function 

//
// DestroyMsgList
void DestroyClientList(ClientList *list)
{
  Client *pos, *next;
  pos = list->front;

  while(pos != NULL) {
    
    next = pos->next;

    // delete msgobj
    RELEASE_OBJECT(pos);

    pos = next;

  } // end of the while 
  
  RELEASE_OBJECT(list);

} // end of the function x


//
// Insert
//
void InsertClientFront(ClientList *list, Client *obj)
{
  Client *new_node = NULL;
  
  new_node = obj;

  if (list->front == NULL)
    {
      list->front = new_node;
    } else {
      
      new_node->next = list->front;
      
      list->front = new_node;

    } // end of if-else

  list->objects++;
  
} // end of teh func


// WRAPPER FUNCTIONS ===============================

//
// void Setup
//
void InsertClient(Client **ptr)
{
  (*ptr) = CreateClientObj();
  
  (*ptr)->list_id = client_list->objects;

  InsertClientFront(client_list, *ptr);

} // end of the function 

//
// Set_SockAddr
// - uses memcpy
void Set_SockAddr(Client *ptr, struct sockaddr_in *c_addr)
{

  memcpy(&ptr->client_addr, c_addr, sizeof(struct sockaddr_in));

} // end of func

//
// Parse_Msg
// - the cool thing about your own api
// is you make your own rules, here just look for ?
//
void Parse_Msg(char *msg, char *name, char *os, int *vers)
{
  int i;
  int j;
  char *buf=NULL;
  char n[3][32];
 
  j = 0;
  // search for ?
  // we probably should return an error code, maybe later
  for (buf=msg,i=0; *buf != '\0';)
    {      

      if (*buf == '?')
	{
	  // finish this string
	  n[j][i] = '\0';

	  i = 0;
	  j++;
	  ((char *)buf)++;
	  continue;
	} // end of the if 
      
      n[j][i] = *buf;

      i++;
      ((char *)buf)++;

    } // end of the for 
  
  // cap her!!!!
  n[j][i] = '\0';

  // copy strings
  strcpy(name, n[0]);
  strcpy(os, n[2]);

  *vers = atoi(n[1]);

} // end of the function

//
// Client _Add Queue
//
void Client_AddQueue(int sock, struct sockaddr_in *c_addr, int object_id, char *msg)
{
  char *buf = NULL;

  Client *ptr = NULL;

  InsertClient(&ptr);

  ptr->sock = sock;
  ptr->object_id = object_id;

  Set_SockAddr(ptr, c_addr);

  buf = (char *)inet_ntoa(c_addr->sin_addr);
  strcpy(ptr->ip_address, buf);
  Parse_Msg(msg, ptr->user_name, ptr->os_str, (int *)&ptr->vers);

} // end of the function 


//
// CreateMsgObj
Client *CreateClientObj(void)
{
  Client *ptr = NULL;

  ptr = malloc(sizeof(Client));

  memset(ptr, 0, sizeof(Client));

  ptr->next = NULL;

  return ptr;
} // end of teh function


//
// MORE WRAPPER FUNCTIONS --
//

void Create_Client_List(void)
{
  client_list = CreateClientList();
} // end of thefunctino

void Delete_Client_List(void)
{
  DestroyClientList(client_list);
} // end of the functino 

