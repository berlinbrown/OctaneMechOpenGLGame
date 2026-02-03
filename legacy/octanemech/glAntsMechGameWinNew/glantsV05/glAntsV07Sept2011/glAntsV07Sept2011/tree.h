//
// tree.h
//
#ifndef _TREE_H_
#define _TREE_H_

// class for binary tree

typedef struct tagTreeNode
{
  int data;

  struct tagTreeNode* left;
  struct tagTreeNode* right;

} TreeNode;

typedef struct tagTree
{
  TreeNode* root;

} Tree;

void TreeTest(void);
void TreeFuncTest(void);

#endif