/**
 * Copyright (c) 2006-2011 Berlin Brown.  All Rights Reserved
 *
 * http://www.opensource.org/licenses/bsd-license.php
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of the Botnode.com (Berlin Brown) nor
 * the names of its contributors may be used to endorse or promote
 * products derived from this software without specific prior written
 * permission. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
 * NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
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
// similar to plist.cpp
//
// Note: the direction, trail stack is defined in bot.cpp

#include "plist.hpp"

#include <iostream>

class PtrNode
{
 public:
  void* ptr;
  PtrNode* next;

  explicit PtrNode(void* p) : ptr(p), next(nullptr) {}
};

// Constructor
PtrList::PtrList() : head(nullptr), items(0) {}

// Destructor
PtrList::~PtrList()
{
  PtrNode* current = head;
  while (current != nullptr)
  {
    PtrNode* next = current->next;
    destroyNode(current);
    current = next;
  }
}

bool PtrList::isEmpty() const { return head == nullptr; }

PtrNode* PtrList::createNode(void* data) { return new PtrNode(data); }

void PtrList::destroyNode(PtrNode* node) { delete node; }

void PtrList::insertFront(void* data)
{
  PtrNode* new_node = createNode(data);
  new_node->next = head;
  head = new_node;
  ++items;
}

void* PtrList::removeFront()
{
  if (isEmpty()) return nullptr;

  PtrNode* temp = head;
  void* result = temp->ptr;

  head = head->next;
  destroyNode(temp);
  --items;

  return result;
}

void PtrList::deleteNode(void* val)
{
  PtrNode* current = head;
  PtrNode* previous = nullptr;

  while (current != nullptr)
  {
    if (current->ptr == val)
    {
      if (previous)
        previous->next = current->next;
      else
        head = current->next;

      destroyNode(current);
      --items;
      return;
    }
    previous = current;
    current = current->next;
  }
}

void PtrList::printListAsInt() const
{
  PtrNode* current = head;
  while (current)
  {
    int* x = static_cast<int*>(current->ptr);
    std::cout << "<" << *x << ">\n";
    current = current->next;
  }
}

size_t PtrList::size() const { return items; }
