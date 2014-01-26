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

#include "glist.h"
#include <stdlib.h>
#include <memory.h>
#include <string.h>

// ----------------------------------------------------------------------------
// gList
// Generalized dynamically resizing array with full memory management.


void freeNOP(void *object) {}




void gInitList(gList *list, void (*freeFunc)(void *))
{
   memset(list, 0, sizeof(*list));

   if(freeFunc)
      list->freeFunc = freeFunc;
   else
      list->freeFunc = freeNOP;
}


gList *gNewList(void (*freeFunc)(void *))
{
   gList *ret = (gList *)malloc(sizeof(gList));

   gInitList(ret, freeFunc);

   ret->freestruct = true;

   return ret;
}



void gFreeList(gList *l)
{
   unsigned int i;

   if(!l)
      return;
   
   for(i = 0; i < l->size; i++)
   {
      if(l->list[i])
         l->freeFunc(l->list[i]);
   }

   if(l->list)
      free(l->list);

   if(l->freestruct)
      free(l);
   else
      memset(l, 0, sizeof(*l));
}




void gClearList(gList *l)
{
   unsigned int i;

   if(!l)
      return;

   for(i = 0; i < l->size; i++)
   {
      if(l->list[i])
      {
         l->freeFunc(l->list[i]);
         l->list[i] = NULL;
      }
   }

   l->size = 0;
}




static void checkListSize(gList *l, unsigned int newsize)
{
   if(newsize >= l->max)
   {
      unsigned int newalloc = newsize * 2;
      unsigned int i;

      l->list = (void **)realloc(l->list, sizeof(void *) * newalloc);

      for(i = l->size; i < newalloc; i++)
         l->list[i] = NULL;

      l->max = newalloc;
   }
}




void gAppendListItem(gList *l, void *object)
{
   if(!l || !object)
      return;

   checkListSize(l, l->size + 1);

   l->list[l->size] = object;
   l->size++;
}



void gInsertListItem(gList *l, void *object, unsigned int toindex)
{
   unsigned int i;

   if(!l || !object)
      return;

   // Clip index
   if(toindex < 0)
      toindex = 0;
   if(toindex > l->size)
      toindex = l->size; 

   checkListSize(l, l->size + 1);

   // Push strings one item to the right.
   for(i = l->size; i > toindex; i--)
      l->list[i] = l->list[i - 1];

   l->size++;
   l->list[toindex] = object;
}



int gGetListSize(gList *l)
{
   return l->size;
}



void *gGetListItem(gList *l, unsigned int index)
{
   if(index >= l->size)
      return NULL;

   return l->list[index];
}



void gDeleteListItem(gList *l, unsigned int index)
{
   unsigned int i;

   if(index >= l->size)
      return;

   l->freeFunc(l->list[index]);

   for(i = index; (i + 1) < l->size; i++)
      l->list[i] = l->list[i + 1];

   l->list[i] = NULL;
   l->size--;
}



void gDeleteListRange(gList *l, unsigned int first, unsigned int last)
{
   unsigned int i, q, offset;

   if(last >= l->size)
      last = l->size - 1;

   if(first > last)
      return;

   offset = last - first + 1;
   
   for(i = first; i <= last; i++)
   {
      l->freeFunc(l->list[i]);

      q = i + offset;

      if(q >= l->size)
         l->list[i] = NULL;
      else
         l->list[i] = l->list[q];
   }

   l->size -= offset;
}


void gMoveListItem(gList *l, unsigned int index, unsigned int newindex)
{
   unsigned int   i;
   void     *tmp;

   if(!l)
      return;

   if(index >= l->size)
      return;

   // Clip index
   if(newindex >= l->size) newindex = l->size - 1; 

   if(index == newindex)
      return;
   
   tmp = l->list[index];

   if(index < newindex)
   {
      for(i = index; i < newindex; i++)
         l->list[i] = l->list[i + 1];
   }
   else
   {
      for(i = index; i > newindex; i--)
         l->list[i] = l->list[i - 1];
   }

   l->list[newindex] = tmp;
}




void gTrimUnusedList(gList *l)
{
   void *newlist;
   unsigned newsize = l->size + 1;

   if(newsize >= l->max)
      return; // Needs no trimming

   newlist = malloc(sizeof(void *) * newsize);
   memcpy(newlist, l->list, sizeof(void *) * newsize);

   free(l->list);
   l->list = newlist;
   l->max = newsize;
}



bool gMoveListItems(gList *src, unsigned int srcindex, gList *dest, unsigned int count)
{
   unsigned int destindex = dest->size;
   unsigned int newsrcsize, newdestsize;

   if(!dest || !src || !count)
      return false;

   if(srcindex >= src->size)
      return false;

   if(srcindex + count > src->size)
      count = src->size - srcindex;

   newsrcsize = src->size - count;
   newdestsize = dest->size + count;

   checkListSize(dest, newdestsize);

   memcpy(dest->list + destindex, src->list + srcindex, sizeof(void *) * count);

   if(srcindex + count < src->size)
      memmove(src->list + srcindex, src->list + srcindex + count, srcindex + count - src->size);

   memset(src->list + newsrcsize, 0, src->size - newsrcsize);

   src->size -= count;
   dest->size += count;

   return true;
}