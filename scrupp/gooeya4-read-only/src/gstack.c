// Emacs style mode select -*- C++ -*-
// ----------------------------------------------------------------------------
//
// Copyright(C) 2009 Stephen McGranahan
//
// This file is part of gParse
//
// gParse is free software: you can redistribute it and/or modify
// it under the terms of the GNU Limited General Public License as published 
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// ----------------------------------------------------------------------------
//
// gParse is a text file parsing system which operates with a few basic
// behaviors predefined, and others optionalized. This creates a token 
// generator that is at once very simple to use and works well for most
// situations.
//
// ----------------------------------------------------------------------------

#include "gstack.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>


// ----------------------------------------------------------------------------
// gStack
// Generalized stack container object.

void stackFreeNOP(void *obj) {}

// gNewStack
// Allocates, initializes and returns a new gStack object. freeFunc should point
// to the function that should free an array object when it is deleted. If 
// freeFunc is NULL, a NOP is used. 
gStack *gNewStack(void (*freeFunc)(void *))
{
   gStack *ret = malloc(sizeof(gStack));
   gInitStack(ret, freeFunc);
   ret->freestruct = true;
   return ret;
}


// gInitStack
// Initializes the given list structure. This is called by gNewStack on the 
// allocated stack structure.
void gInitStack(gStack *stack, void (*freeFunc)(void *))
{
   memset(stack, 0, sizeof(*stack));
   stack->freeFunc = freeFunc ? freeFunc : stackFreeNOP;
}


// gFreeStack
// Frees a gStack and all substructures and data contained in it. If the 
// freestructflag is set in the given list struct, the struct itself is freed.
void gFreeStack(gStack *s)
{
   gStackEntry *it, *pr;

   for(it = s->top; it; it = pr)
   {
      pr = it->prev;

      s->freeFunc(it->entry);
      free(it);
   }

   for(it = s->unused; it; it = pr)
   {
      pr = it->prev;
      free(it);
   }

   if(s->freestruct)
      free(s);
   else
      memset(s, 0, sizeof(*s));
}



// gClearStack
// Frees all objects contained in a gStack, but keeps the list
// structures allocated.
void gClearStack(gStack *s)
{
   gStackEntry *it, *pr;

   for(it = s->top; it; it = pr)
   {
      pr = it->prev;

      s->freeFunc(it->entry);

      it->prev = s->unused;
      s->unused = it;
   }

   s->top = NULL;
}


// gPushEntry
// Adds the given object to the end of the given stack.
void gPushEntry(gStack *s, void *object)
{
   gStackEntry *it;

   if(!object)
      return;

   if(s->unused)
   {
      it = s->unused;
      s->unused = it->prev;
   }
   else
      it = malloc(sizeof(gStackEntry));

   it->entry = object;
   it->prev = s->top;
   s->top = it;
}


// gGetStackTop
// Returns the current top item on the stack
void *gGetStackTop(gStack *s)
{
   return s->top ? s->top->entry : NULL;
}


// gGetStackSize
// Polls the stack and returns the number of entries in the stack.
int gGetStackSize(gStack *s)
{
   gStackEntry *it;
   int count = 0;

   for(it = s->top; it; it = it->prev)
      count++;

   return count;
}


// gPopStack
// Deletes the top of the stack and returns the new top item (returns NULL if
// the stack is empty).
void *gPopStack(gStack *s)
{
   gStackEntry *it, *ret;

   if(!(it = s->top))
      return NULL;

   ret = s->top = it->prev;

   s->freeFunc(it->entry);
   it->prev = s->unused;
   s->unused = it;

   return ret->entry;
}


