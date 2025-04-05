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
// similar to plist.cpp, except uses 
//
// Note: the direction, trail stack is defined in bot.cpp

#include <stdio.h>
#include <stdlib.h>

#include "plist.h"

static int isempty(PtrList *list)
{

 if (list->head == NULL)
	return 1;		/* first pointer null, list is empty */
 else
	return 0;

} /* end of the fcuntion */

//
// CreatePtrNode
//
PtrNode *CreatePtrNode(void *data) {

    PtrNode *h = (PtrNode *) malloc(sizeof(PtrNode));

    h->ptr = data;
    h->next = NULL;

    return h;

} // end of the function 

//
// DestroyPtrNode
//
void DestroyPtrNode(PtrNode *node) {

    free(node);

} // end of the functino 

// 
// Create PtrList
//
PtrList *CreatePtrList() {

    PtrList *result = (PtrList *) malloc(sizeof(PtrList));

    result->head = NULL;
	result->items = 0;

	return result;

} // end of the function 


//
// DestroyPtrList
//
void DestroyPtrList(PtrList *list) {

    PtrNode *pos, *next;
    pos = list->head;

    while(pos != NULL) {
        next = pos->next;

        free(pos);

        pos = next;
    } // end of the while 

    free(list);

} // end of the function 


//
// Delete Ptr
// 
void DeletePtrNode(PtrList *list, void *val)
{
    PtrNode *current = list->head;
    PtrNode *previous = NULL;
 
    while (current != NULL ) {

        if( current->ptr != val) {

            previous = current;
            current = previous->next;

        } else {

            if (previous != NULL ) {

                previous->next = current->next;

            } // end of the if 
			
			list->items--;

            free(current);
            break;

        } // end of the if - else

    } // end of the while 

} // end of the function


//
// Insert Front
void InsertFront(PtrList *list, void *data) {

	PtrNode *new_node = NULL;

	new_node = CreatePtrNode(data);

	if (isempty(list))

  		list->head = new_node;

	else {
 		
		new_node->next = list->head;
		list->head = new_node;

	} // end if 

	list->items++;

} // end of the function

//
// Remove Front
//
static void *RemoveFront(PtrList *list)
{

  PtrNode *temp_ptr = NULL;
  void *res = NULL;

  if (isempty(list))
	return NULL;
  else {
 	temp_ptr = list->head;
	if (list->head->next == NULL)
		list->head = NULL; /* reset */
	else
		list->head = list->head->next;

	res = temp_ptr->ptr;	// remove from list, but keep ptr

	free(temp_ptr);
	list->items--;

	return res;

  } // end of the if-else

 return NULL;		// we should never get here

} // end of the function 


//
// PrintTest
//
void PrintPtrList(PtrList *list)
{
 PtrNode *current_ptr;
 int *x;

 if (isempty(list))
	return;
 
 current_ptr = list->head;
 
 while(current_ptr != NULL)
 {
	// interesting 
	x = (int *)current_ptr->ptr;

	printf("<%d>\n", *x);
 	current_ptr = current_ptr->next;
 } // end of while

} // end of the function 

//
// PrintList
//
void PrintPtrListf(PtrList *list)
{
 PtrNode *current_ptr;
 float *x;

 if (isempty(list))
	return;
 
 current_ptr = list->head;
 
 while(current_ptr != NULL)
 {
	// interesting 
	x = (float *)current_ptr->ptr;

	printf("<%0.2f>\n", *x);
 	current_ptr = current_ptr->next;
 } // end of while

} // end of the function 


//
// PtrListTest
// 
void PtrLinkTest(void)
{
	PtrList *list;
	int *x = (int *)malloc(sizeof(int));
	int *y = (int *)malloc(sizeof(int));
	int *z = (int *)malloc(sizeof(int));
	int *res;

	*x = 9;
	*y = 3;
	*z = 4;

	//list =  CreatePtrList();
	list = CREATE_STACK;
	
	//InsertFront(list, (int *)x);
	PUSH_STACK(list, (int *)x);
	printf("%d PUSHED\n", *x);

	//InsertFront(list, (int *)y);
	PUSH_STACK(list, (int *)y);
	printf("%d PUSHED\n", *y);

	InsertFront(list, (int *)z);
	printf("%d PUSHED\n", *z);

	//PrintPtrList(list);

	// Delete a node pointer
	//DeletePtrNode(list, y);
	//res = (int *)RemoveFront(list);
	res = (int *)POP_STACK(list);
	printf("%d POPPED %d\n", *res, list->items); 

	res = (int *)RemoveFront(list);
	printf("%d POPPED %d\n", *res, list->items); 

	printf("\n\n New List %d\n", list->items);
	PrintPtrList(list);

	InsertFront(list, (int *)y);
	printf("%d PUSHED %d\n", *y, list->items);

	printf("\n\n New with Added\n");
	PrintPtrList(list);

	DestroyPtrList(list);

	free(x);
	free(y);
	free(z);

} // end of the function 





