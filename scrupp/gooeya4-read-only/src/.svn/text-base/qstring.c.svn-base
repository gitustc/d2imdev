// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// Copyright(C) 2004 James Haley
//
// This program is free software: you can redistribute it and/or modify
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
//--------------------------------------------------------------------------
//
// Reallocating string structure
//
// What this "class" guarantees:
// * The string will always be null-terminated
// * Indexing functions always check array bounds
// * Insertion functions always reallocate when needed
//
// Of course, using M_QStrBuffer can negate these, so avoid it
// except for passing a char * to read op functions.
//
// By James Haley
//
//-----------------------------------------------------------------------------
//
// This file was changed to LGPL with specific permission from 
// James Haley


#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "qstring.h"

// 32 bytes is the minimum block size currently used by the zone 
// allocator, so it makes sense to use it as the base default 
// string size too.

#define QSTR_BASESIZE 32


qstring_t *M_QStrInitCreate(qstring_t *qstr)
{
   memset(qstr, 0, sizeof(*qstr));

   return M_QStrCreate(qstr);
}


qstring_t *M_QStrCreateSize(qstring_t *qstr, unsigned int size)
{
   qstr->buffer = realloc(qstr->buffer, size);
   qstr->size   = size;
   qstr->index  = 0;
   memset(qstr->buffer, 0, size);

   return qstr;
}


qstring_t *M_QStrCreate(qstring_t *qstr)
{
   return M_QStrCreateSize(qstr, QSTR_BASESIZE);
}


unsigned int M_QStrLen(qstring_t *qstr)
{
   return strlen(qstr->buffer);
}


unsigned int M_QStrSize(qstring_t *qstr)
{
   return qstr->size;
}


char *M_QStrBuffer(qstring_t *qstr)
{
   return qstr->buffer;
}


qstring_t *M_QStrGrow(qstring_t *qstr, unsigned int len)
{   
   int newsize = qstr->size + len;

   qstr->buffer = realloc(qstr->buffer, newsize);
   memset(qstr->buffer + qstr->size, 0, len);
   qstr->size += len;
   
   return qstr;
}


qstring_t *M_QStrClear(qstring_t *qstr)
{
   memset(qstr->buffer, 0, qstr->size);
   qstr->index = 0;

   return qstr;
}


void M_QStrFree(qstring_t *qstr)
{
   free(qstr->buffer);
   qstr->buffer = NULL;
   qstr->index = qstr->size = 0;
}


char M_QStrCharAt(qstring_t *qstr, unsigned int idx)
{
   if(idx >= qstr->size)
      return 0;

   return qstr->buffer[idx];
}


qstring_t *M_QStrPutc(qstring_t *qstr, char ch)
{
   if(qstr->index >= qstr->size - 1) // leave room for \0
      M_QStrGrow(qstr, qstr->size);  // double buffer size

   qstr->buffer[qstr->index++] = ch;

   return qstr;
}


qstring_t *M_QStrInsertc(qstring_t *qstr, char ch, unsigned int index)
{
   //unsigned int i;
   char         *b;

   if(index >= qstr->index)
      return M_QStrPutc(qstr, ch);

   if(qstr->index >= qstr->size - 1) // leave room for \0
      M_QStrGrow(qstr, qstr->size);  // double buffer size

   b = qstr->buffer + index;

   //for(i = qstr->index; i > index; i--)
   //   qstr->buffer[i] = qstr->buffer[i - 1];
   memmove(b + 1, b, qstr->index - index); 

   *b = ch;

   qstr->index++;

   return qstr;
}


qstring_t *M_QStrDeletec(qstring_t *qstr, unsigned int index)
{
   char *b;

   if(index >= qstr->index - 1)
   {
      qstr->index--;
      qstr->buffer[qstr->index] = '\0';
      return qstr;
   }

   b = qstr->buffer + index;
   memmove(b, b + 1, qstr->index - index);

   qstr->index --;
   return qstr;
}


qstring_t *M_QStrDeleteRange(qstring_t *qstr, unsigned int index, unsigned int len)
{
   char *b = qstr->buffer;

   if(index >= qstr->index)
      return qstr;

   if(index + len > qstr->index)
      len = qstr->index - index;

   memmove(b + index, b + index + len, qstr->index - index - len);
   memset(b + (qstr->index - len), 0, len);

   qstr->index -= len;

   return qstr;
}




qstring_t *M_QStrCat(qstring_t *qstr, const char *str)
{
   unsigned int cursize = qstr->size;
   unsigned int newsize = strlen(qstr->buffer) + strlen(str) + 1;

   if(newsize > cursize)
      M_QStrGrow(qstr, newsize - cursize);

   strcat(qstr->buffer, str);

   qstr->index = newsize - 1;

   return qstr;
}


// -- From m_misc.c in Eternity ---
// haleyjd: portable strupr function
static char *M_Strupr(char *string)
{
   char *s = string;

   while(*s)
   {
      char c = *s;
      *s++ = toupper(c);
   }

   return string;
}

// haleyjd: portable strlwr function
static char *M_Strlwr(char *string)
{
   char *s = string;

   while(*s)
   {
      char c = *s;
      *s++ = tolower(c);
   }

   return string;
}


qstring_t *M_QStrUpr(qstring_t *qstr)
{
   M_Strupr(qstr->buffer);
   return qstr;
}


qstring_t *M_QStrLwr(qstring_t *qstr)
{
   M_Strlwr(qstr->buffer);
   return qstr;
}



qstring_t *M_QStrSet(qstring_t *qstr, const char *str)
{
   M_QStrClear(qstr);
   return M_QStrCat(qstr, str);
}

// EOF
