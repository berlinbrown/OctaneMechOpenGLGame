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
 * 
 * https://github.com/berlinbrown/OctaneMechOpenGLGame
 */


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "tree.h"

#define TEST_VALUES		10

static unsigned long tree_searches = 0;
static unsigned long list_searches = 0;
static int stack[20]={0};
static int ctr = 0;


bool TestFuncA(int val, TreeNode *current)
{
	printf(" *%d/%d* ", val, current->data);

	return (val < current->data);
} // end of the function 

#if 0
bool TestFuncB(int val, int data)
{
	printf("[%d<%d]",val, data); 
	return (val < data);
} // end of the function 
#endif

#define TestFuncB(val, data)	((val < data) ? 1 : 0)

//
// InsertNode
//
void InsertTree(TreeNode **node_ptr, TreeNode *current)
{

	if (*node_ptr == NULL) {

		*node_ptr = current;

	} // end of the if
	else {
		if (current->data < (*node_ptr)->data) {

			// recursive call to above
			InsertTree(&((*node_ptr)->left), current);

		} else {

			if (current->data > (*node_ptr)->data)
				InsertTree(&((*node_ptr)->right), current);
				
		} // end of if-else

	} // end of the if - else

} // end of the function 

//
// Test Similar functionality
//
void InsertTest(TreeNode **node_ptr, TreeNode *current)
{

	if (*node_ptr == NULL) {

		*node_ptr = current;

	} // end of the if
	else {
		

		if (TestFuncB(current->data, current->data)) {

			// recursive call to above
			InsertTest(&((*node_ptr)->left), current);
		} else {

			if (!TestFuncB(current->data, current->data))
				InsertTest(&((*node_ptr)->right), current);
				
		} // end of if-else

	} // end of the if - else

} // end of the function 

//
// SearchNode
//
bool SearchTree(TreeNode **node_ptr, TreeNode *current)
{

	tree_searches++;		// keep a tally

	if (*node_ptr == NULL) {

		return false;		// is empty

	} // end of the if
	else {

		//printf(" (%d) ", (*node_ptr)->data);

		// check if we found the data
		if (current->data == (*node_ptr)->data)
			return true;

		if (current->data < (*node_ptr)->data) {

			// recursive call to above
			SearchTree(&((*node_ptr)->left), current);
		} else {

			if (current->data > (*node_ptr)->data)
				SearchTree(&((*node_ptr)->right), current);
				
		} // end of if-else

	} // end of the if - else


	return true;

} // end of the function 


//
// SearchNode
//
int SearchTest(TreeNode **node_ptr, TreeNode *current)
{

	tree_searches++;		// keep a tally

	if (*node_ptr == NULL) {

		return 0;		// is empty

	} // end of the if
	else {

		// check if we found the data
		if (current->data == (*node_ptr)->data)
			return current->data;


		if (TestFuncB(current->data, current->data)) {

				
		//printf(" [%d] ", (*node_ptr)->data);

			// recursive call to above
			SearchTest(&((*node_ptr)->left), current);
		} else {

			//printf(" [%d] ", (*node_ptr)->data);
				
			if (!TestFuncB(current->data, current->data))
				SearchTest(&((*node_ptr)->right), current);
				
		} // end of if-else

	} // end of the if - else


	return current->data;

} // end of the function 



//
// PreOrder
//
void PreOrder(TreeNode *ptr)
{
	if (ptr != 0)
	{
		printf(" +%d+ ", ptr->data);
		PreOrder(ptr->left);
		PreOrder(ptr->right);

	} // end of the if 

} // end of the function 

int removed =0;
//
// DeleteTree
// - danger written all over this one
// when the nodes in the tree are malloc-ed
//
void DeleteTree(Tree *tree)
{
  TreeNode *node;
  TreeNode *next;			// 

  node = tree->root;	// start at beginning

  while (node != NULL) {

		if (node->left == NULL)
		{
			next = node->right;

			removed++;

			free (node);

		} else {

			next = node->left;
			node->left = next->right;
			next->right = node;
		} // end of the if

		node = next;

	} // end of the while 


	free(tree);
} // end of the function

//
// CreateTree
//
Tree *CreateTree(void)
{
		
	Tree *tree;

	tree = (Tree *)malloc(sizeof(Tree));

	tree->root = NULL;

	return tree;
} // end of the function

//
// CreateTreeNode
//
TreeNode *CreateTreeNode(int data)
{
	TreeNode *current;

	current = (TreeNode *)malloc(sizeof(TreeNode));
	current->left = NULL; current->right = NULL;

	current->data = data;

	return current;

} // end of the function

//
// TreeTest
//
void TreeFuncTest(void)
{
	Tree *tree;
	TreeNode *current;
	TreeNode *search;
	int i;
	int values[10];
	int targets;
	int ret;

	values[0] = 47;
	values[1] = 25;
	values[2] = 77;
	values[3] = 11;
	values[4] = 43;
	values[5] = 65;
	values[6] = 93;
	values[7] = 7;
	values[8] = 17; 
	values[9] = 31;
	values[10] = 44;
	values[11] = 68;

	targets = 7;

	tree = CreateTree();

	for (i = 0; i < 12; i++)
	{
		current = CreateTreeNode(values[i]);

		InsertTest(&tree->root, current);

	} // end of the for

	search = CreateTreeNode(targets);
		
	if (ret = SearchTest(&tree->root, search))
		printf("\nNode found in: %d -- %d\n", tree_searches, ret);
	else
		printf("\nCould not find node in: %d %d\n", tree_searches, ret);
	
	free(search);

	printf("\n\n");
	PreOrder(tree->root);

	DeleteTree(tree);
		
} // end of the function 



//
// TreeTest
// - test a linked list versus
// a binary tree
// which one will, I cant wait to see what happens
//
void TreeTest(void)
{
	Tree *tree;
	TreeNode *current;
	TreeNode *search;
	int i;
	int *values = NULL;
	int target;

	values = (int *)malloc(TEST_VALUES * sizeof(int));

	// load the values with random values
	// pick the middle one as the target
	for (i = 0; i < TEST_VALUES; i++)
		values[i] = rand()%TEST_VALUES;


	target = values[(int)(TEST_VALUES/2)];


	tree = CreateTree();

	for (i = 0; i < TEST_VALUES; i++)
	{
		current = CreateTreeNode(values[i]);

		InsertTree(&tree->root, current);

	} // end of the for

	search = CreateTreeNode(target);
		
	if (SearchTree(&tree->root, search))
		printf("Node found in: %d\n", tree_searches);
	else
		printf("Could not find node in: %d\n", tree_searches);
	
	free(search);

	//PreOrder(tree->root);

	DeleteTree(tree);
	free(values);

		
} // end of the function 
