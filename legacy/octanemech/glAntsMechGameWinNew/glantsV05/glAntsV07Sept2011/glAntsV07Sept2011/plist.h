//
// list.h
#ifndef _P_LIST_H_
#define _P_LIST_H_

typedef struct tagPtrNode
{
  void* ptr;

  struct tagPtrNode* next;
} PtrNode;

typedef struct tagPtrList
{
  PtrNode* head;
  int items;
} PtrList;

void InsertFront(PtrList* list, void* data);

void PtrLinkTest(void);

PtrList* CreatePtrList();
void DestroyPtrList(PtrList* list);
int isempty(PtrList* list);
void DeletePtrNode(PtrList* list, void* val);
void* RemoveFront(PtrList* list);
void PrintPtrList(PtrList* list);

// STACK MACROS
#define POP_STACK(list) RemoveFront(list)
#define PUSH_STACK(list, data) InsertFront(list, data)
#define CREATE_STACK CreatePtrList()
#define DESTROY_STACK(list) DestroyPtrList(list)

#endif