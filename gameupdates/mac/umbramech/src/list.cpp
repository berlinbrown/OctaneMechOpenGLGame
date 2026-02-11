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

// list.cpp
#include <stdio.h>
#include <stdlib.h>
#include <list.hpp>
#include <bot.hpp>

static int isempty(List* list)
{
  if (list->head == NULL)
    return 1;  // return 1
  else
    return 0;
}

// CreateNode
Node* CreateNode(int data)
{
  Node* h = (Node*)malloc(sizeof(Node));

  h->data = data;
  h->next = NULL;

  return h;
}

// DestroyNode
void DestroyNode(Node* node) { RELEASE_OBJECT(node); }

// Create List
List* CreateList()
{
  List* result = (List*)malloc(sizeof(List));

  result->head = NULL;

  return result;
}

void DeleteNode(List* list, int val)
{
  Node* current = list->head;
  Node* previous = NULL;

  while (current != NULL)
  {
    if (current->data != val)
    {
      previous = current;
      current = previous->next;
    }
    else
    {
      if (previous != NULL)
      {
        previous->next = current->next;

      }

      // free(current);
      RELEASE_OBJECT(current);
      break;

    }

  }
}

// DestroyList
void DestroyList(List* list)
{
  Node *pos, *next;
  pos = list->head;

  while (pos != NULL)
  {
    next = pos->next;
    // free(pos);
    RELEASE_OBJECT(pos);

    pos = next;
  }

  free(list);
}

// Insert Front
static void InsertFront(List* list, int data)
{
  Node* new_node = NULL;

  new_node = CreateNode(data);

  if (isempty(list))

    list->head = new_node;

  else
  {
    new_node->next = list->head;
    list->head = new_node;

  }  // end if
}

// PrintTest
void PrintList(List* list)
{
  Node* current_ptr;

  if (isempty(list)) return;

  current_ptr = list->head;

  while (current_ptr != NULL)
  {
    printf("<%d>\n", current_ptr->data);
    current_ptr = current_ptr->next;
  }
}

// ListTest
void LinkTest(void)
{
  List* list;

  list = CreateList();

  InsertFront(list, 4);
  InsertFront(list, 5);
  InsertFront(list, 3);
  InsertFront(list, 2);

  PrintList(list);

  DeleteNode(list, 3);
  DeleteNode(list, 5);

  printf("\n\nNew List\n");
  PrintList(list);

  DestroyList(list);
}
