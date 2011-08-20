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
// msg.c
// - some msg networking code
//
//
// The network message interface is simple at best
// the goal is store messages in a message queue
// and when the server is ready to handle them
// process each message and then send them to the
// clients
// delete the message queue, I love it.
//
// There are a couple of problems
// it can be slow because I have to temporarily
// store the messages in an array and then send off
// I couldnt come up with so more creative, shrug
//
#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "include/msg.h"
#include "network.h"

static MsgList *msg_list;

static Msg msg_cluster[MAX_MESSAGES];
static int msg_index = 0;
static int msg_objectid = 0;

//
// Reset_Messages
//
void Reset_MessageCount(void)
{
  Reset_MessageBytes();
  msg_index = 0;

} // end of thefnction

//
// Set_MsgIndex
//
void Set_MsgIndex(int val)
{
  msg_index = val;

} // end of the function

void Set_MsgId(int val)
{

  msg_objectid = val;

} // end

//
// Message Count
//
int Get_MessageCount(void)
{
  if ( (msg_list->objects >= 0) && 
       (msg_list->objects < 40000))
    {

      return msg_list->objects;

    } // end of the if 

  return 0;
				
} // end of the functino 

//
// For simple checks
// get the first message
// 
MsgPtr Get_FirstMsg(void)
{
  return (MsgPtr)&msg_cluster[0];
} // end of the function

//
// GetMsgCluster
//
Msg *GetMsgCluster(void)
{

  // for you cs students out there
  //return &msg_cluster[0];
  return msg_cluster;

} // end of func

//
// Clear_FirstMsg
// - nothing fancy
//
void Clear_FirstMsg(void)
{
  msg_cluster[0].msg_type = -1;
  msg_cluster[0].move_type = -1;
  msg_cluster[0].msg_id = -1;
  msg_cluster[0].object_id = -1;
  
} // end of the func

//
// Get_Messages
// - similar to get message cluster
// except this time, we include the count
//  - Note: this is a dangerous function
// it can mess the heap and possibly other stuff
// but it is a quick fix
//
// should be called by the fireants func
//
// x = Get_Messages(&y);
//
Msg *Get_Messages(int *msg_count, int *msg_id)
{
  *msg_count = msg_index;
  *msg_id = msg_objectid;

  return msg_cluster;

} // end of the functio 

//
// Print_MsgTest
//
void Print_MsgTest(void)
{
  int i;
  for (i = 0; i < 3; i++)
    {
      printf("%d %0.2f\n", msg_cluster[i].msg_id, msg_cluster[i].pos_x);
    } // end of the for 

} // end of the function 
    

//// CreateMsgList
//
MsgList *CreateMsgList(void) {
  
  MsgList *result = malloc(sizeof(MsgList));

  result->front = NULL;
  result->objects = 0;

  return result;
  
} // end of teh function 

//
// DestroyMsgList
void DestroyMsgList(MsgList *list)
{
  Msg *pos, *next;
  pos = list->front;

  while(pos != NULL) {
    
    next = pos->next;

    // delete msgobj
    RELEASE_OBJ(pos);

    pos = next;

  } // end of the while 
  
  list->objects = 0;

  RELEASE_OBJ(list);

} // end of the function x


//
// Insert
//
void InsertMsgFront(MsgList *list, Msg *obj)
{
  Msg *new_node = NULL;
  
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
void InsertMsg(Msg **ptr)
{
  (*ptr) = CreateMsgObj();  
  (*ptr)->msg_id = msg_list->objects;

  
  if (msg_list->objects < MAX_MESSAGES)
    InsertMsgFront(msg_list, *ptr);

} // end of the function 

//
void Msg_AddQueue(int msg_type, int move_type, int obj_id, float px, float py, float h)
{

  Msg *ptr = NULL;

  InsertMsg(&ptr);

  ptr->msg_type = msg_type;
  ptr->move_type = move_type;
  
  ptr->object_id = obj_id;

  ptr->pos_x = px;
  ptr->pos_y = py;

  ptr->heading = h;

  
} // end of the function 


//
// Print List
// - for testing mainly
//
void PrintMsgList(MsgList *list)
{
  Msg *current_ptr;

  if (list->front == NULL)
    return;

  current_ptr = list->front;

  while (current_ptr != NULL)
    {
      
      printf("id: %d\n", current_ptr->msg_id);
      
      current_ptr = current_ptr->next;

    } // end of the while 


} // end of the function 

//
// CreateMsgObj
Msg *CreateMsgObj(void)
{
  Msg *ptr = NULL;

  ptr = malloc(sizeof(Msg));

  memset(ptr, 0, sizeof(Msg));

  ptr->next = NULL;

  return ptr;
} // end of teh function


//
// MORE WRAPPER FUNCTIONS --
//

void Create_Msg_List(void)
{
  msg_list = CreateMsgList();

} // end of thefunctino

void Delete_Msg_List(void)
{
  DestroyMsgList(msg_list);
} // end of the functino 

//
// Print_Msg_List(void
void Print_Msg_List(void)
{
  PrintMsgList(msg_list);
} // end of the functino 

//
// Main function to the outside world
// - convert the msg list so that it can be sent
// out 
void *Convert_MsgList(void)
{
  MsgList *list = NULL;
  Msg *current_ptr = NULL;
  int index = 0;
  void *buffer = NULL;

  list = msg_list;

  if (list->front == NULL)
    return NULL;

  current_ptr = list->front;

  index = 0;
  while (current_ptr != NULL)
    {
     
      memcpy(&msg_cluster[index], current_ptr,sizeof(Msg));  
      index++;
      
      current_ptr = current_ptr->next;

    } // end of the while 

  buffer = &msg_cluster;

  return buffer;

} // end of the function 



//
// Same as above --
// out 
void *Finalize_Messages(int *res_count)
{
  MsgList *list = NULL;
  Msg *current_ptr = NULL;
  int index = 0;
  void *buffer = NULL;

  list = msg_list;

  if (list->front == NULL)
    return NULL;

  current_ptr = list->front;

  index = 0;
  while (current_ptr != NULL)
    {
     
      memcpy(&msg_cluster[index], current_ptr,sizeof(Msg));  
      index++;
      
      current_ptr = current_ptr->next;

    } // end of the while 


  *res_count = index;
  buffer = &msg_cluster;

  return buffer;

} // end of the function 



//
// Simple test
//
void Msg_Test(void)
{
 
  Create_Msg_List();
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Print_Msg_List();
  Delete_Msg_List();


  // do it multiple times in order to make sure malloc and the 
  // heap are up to the challenge
  Create_Msg_List();
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Print_Msg_List();
  Delete_Msg_List();

  Create_Msg_List();
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Msg_AddQueue(MSG_MOVE, MOVE_FORWARD, 0, 23.4, 24.5, 180.5);
  Print_Msg_List();
  Delete_Msg_List();
  

} // end of teh function 
