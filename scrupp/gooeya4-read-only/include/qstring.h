// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// Copyright(C) 2005 James Haley
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


#ifndef M_QSTR_H__
#define M_QSTR_H__

typedef struct qstring_s
{
   char *buffer;
   unsigned int index;
   unsigned int size;
} qstring_t;


//
// M_QStrInitCreate
//
// Initializes a qstring struct to all zeroes, then calls
// M_QStrCreate. This is for safety the first time a qstring
// is created (if qstr->buffer is uninitialized, realloc will
// crash).
//
qstring_t *M_QStrInitCreate(qstring_t *qstr);


//
// M_QStrCreateSize
//
// Creates a qstring with a given initial size, which helps prevent
// unnecessary initial reallocations. Resets insertion point to zero.
// This is safe to call on an existing qstring to reinitialize it.
//
qstring_t *M_QStrCreateSize(qstring_t *qstr, unsigned int size);


//
// M_QStrCreate
//
// Gives the qstring a buffer of the default size and initializes
// it to zero. Resets insertion point to zero. This is safe to call
// on an existing qstring to reinitialize it.
//
qstring_t *M_QStrCreate(qstring_t *qstr);


//
// M_QStrLen
//
// Calls strlen on the internal buffer. Convenience method.
//
unsigned int M_QStrLen(qstring_t *qstr);


//
// M_QStrSize
//
// Returns the amount of size allocated for this qstring
// (will be >= strlen). You are allowed to index into the
// qstring up to size - 1, although any bytes beyond the
// strlen will be zero.
//
unsigned int M_QStrSize(qstring_t *qstr);


//
// M_QStrBuffer
//
// Retrieves a pointer to the internal buffer. This pointer
// shouldn't be cached, and is not meant for writing into
// (although it is safe to do so, it circumvents the
// encapsulation and security of this structure).
//
char *M_QStrBuffer(qstring_t *qstr);


//
// M_QStrGrow
//
// Grows the qstring's buffer by the indicated amount.
// This is automatically called by other qstring methods,
// so there is generally no need to call it yourself.
//
qstring_t *M_QStrGrow(qstring_t *qstr, unsigned int len);


//
// M_QStrClear
//
// Sets the entire qstring buffer to zero, and resets the
// insertion index. Does not reallocate the buffer.
//
qstring_t *M_QStrClear(qstring_t *qstr);


//
// M_QStrFree
//
// Frees the qstring object. It should not be used after this,
// unless M_QStrCreate is called on it. You don't have to free
// a qstring before recreating it, however, since it uses realloc.
//
void M_QStrFree(qstring_t *qstr);


//
// M_QStrCharAt
//
// Indexing function to access a character in a qstring.
// This is slower but more secure than using M_QStrBuffer
// with array indexing.
//
char M_QStrCharAt(qstring_t *qstr, unsigned int idx);


//
// M_QStrPutc
//
// Adds a character to the end of the qstring, reallocating
// via buffer doubling if necessary.
//
qstring_t *M_QStrPutc(qstring_t *qstr, char ch);



//
// M_QStrInsertc
//
// Adds a character to the qstring at the given index, reallocating
// via buffer doubling if necessary.
//
qstring_t *M_QStrInsertc(qstring_t *qstr, char ch, unsigned int index);


//
// M_QStrDeletec
//
// Deletes a character from the qstring at the given index
//
qstring_t *M_QStrDeletec(qstring_t *qstr, unsigned int index);


//
// M_QStrDeleteRange
//
// Deletes a group of characters from the qstring starting at the given index
//
qstring_t *M_QStrDeleteRange(qstring_t *qstr, unsigned int index, unsigned int len);



//
// M_QStrCat
//
// Concatenates a C string onto the end of a qstring, expanding
// the buffer if necessary.
//
qstring_t *M_QStrCat(qstring_t *qstr, const char *str);


//
// M_QStrUpr
//
// Converts the string to uppercase.
//
qstring_t *M_QStrUpr(qstring_t *qstr);


//
// M_QStrLwr
//
// Converts the string to lowercase.
//
qstring_t *M_QStrLwr(qstring_t *qstr);


// 
// M_QStrSet
// 
// SoM: Why wasn't this here before? Clears and sets a qstring
qstring_t *M_QStrSet(qstring_t *qstr, const char *str);


#endif

// EOF

