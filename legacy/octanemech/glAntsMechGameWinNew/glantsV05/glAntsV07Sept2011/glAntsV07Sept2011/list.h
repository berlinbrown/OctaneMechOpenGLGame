//
// lights.h
//
// list.h
#ifndef _LIST_H_
#define _LIST_H_

typedef struct tagNode
{
  int data;

  struct tagNode* next;
} Node;

typedef struct tagList
{
  Node* head;
} List;

void LinkTest(void);

#endif