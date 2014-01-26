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


#ifndef GSTACK_H
#define GSTACK_H

/**
 * \file gstack.h
 * \brief Generalized stack object.
 *
 * gStack provides generalized stack management facilities. The stack 
 * essentially a single linked list where only the "top" entry is immideiately
 * available. Entries can be "pushed" on top of the stack, and the top entry 
 * can be "popped" off (removed) exposing the entry directly under it.
*/
#ifdef __cplusplus
extern "C"
{
#endif

#include "gbool.h"

// ----------------------------------------------------------------------------
// gStack
// Generalized stack container object.


#ifndef DOXYGEN_IGNORE
typedef struct gStackEntry gStackEntry;
#endif

/**
 * \struct gStackEntry
 * \brief Used by gStack to store an entry and a back-link.
*/
typedef struct gStackEntry
{
   void        *entry; //!< Object in this entry
   gStackEntry *prev;  //!< Previous stack entry
};


/**
 * \struct gStack
 * \brief Generalized stack object.
 *
 * gStack provides generalized stack management facilities. gStack can also
 * be used for memory management. The member freeFunc is called whenever the
 * top entry is poped from the stack.
 *
 * \see  gstack.h::gNewStack, gstack.h::gInitStack, gstack.h::gFreeStack,
 *       gstack.h::gClearStack, gstack.h::gPushEntry, gstack::gGetStackTop,
 *       gstack.h::gGetStackSize, gstack.h::gPopStack
*/
typedef struct
{
   gStackEntry *top;    //!< Top entry in the stack.
   gStackEntry *unused; //!< Empty/unused entries kept to reduce the number of malloc calls.

   void     (*freeFunc)(void *); //!< Used to free an entry when the stack is poped.
   bool     freestruct; //!< If set to true, gFreeStack will free this struct.
} gStack;



/**
 * \fn gStack *gNewStack(void (*freeFunc)(void *))
 * \brief Allocates a new gStack
 *
 * Allocates, initializes and returns a new gStack object. freeFunc is called
 * on the top element when the stack is poped. If freeFunc is NULL, a NOP 
 * function is used and gStack won't free any elements.
 *
 * @param freeFunc Function called on the top element when the stack is poped.
*/
gStack *gNewStack(void (*freeFunc)(void *));


/**
 * \fn void gInitStack(gStack *stack, void (*freeFunc)(void *))
 * \brief Initializes a stack object.
 *
 * Initializes the given stack object. This is called by gNewStack on a newly
 * allocated stack structure.
*/
void gInitStack(gStack *stack, void (*freeFunc)(void *));


/**
 * \fn void gFreeStack(gStack *s)
 * \brief Frees a gStack object
 *
 * Frees a gStack and all substructures and data contained in it. If the 
 * freestruct flag is set in the given stack, the struct itself is freed.
 *
 * @param[in] s gStack to be freed.
*/
void gFreeStack(gStack *s);


/**
 * \fn void gClearStack(gStack *s)
 * \brief Clears a gStack
 *
 * Pops all entries contained in a gStack.
 *
 * @param[in] s Stack to clear.
*/
void gClearStack(gStack *s);


/**
 * \fn void gPushEntry(gStack *s, void *object)
 * \brief Pushes a new entry on top of a gStack
 *
 * Pushes the an entry on top of a gStack.
 *
 * @param[in] s Stack to push to
 * @param[in] object Object to push on top of the stack
*/
void gPushEntry(gStack *s, void *object);


/**
 * \fn void *gGetStackTop(gStack *s)
 * \brief Get the top entry from a stack.
 *
 * Returns a void pointer to the current top entry from the stack
 *
 * @param[in] s Stack to get the top from
 * @return Entry at the top of the stack.
*/
void *gGetStackTop(gStack *s);


/**
 * \fn int gGetStackSize(gStack *s)
 * \brief Counts entries in a stack.
 *
 * Polls the stack and returns the number of entries in the stack.
 *
 * @param[in] s Stack to poll
 * @return Number of entries in the stack.
*/
int gGetStackSize(gStack *s);


/**
 * \fn void *gPopStack(gStack *s)
 * \brief Pops the stack.
 *
 * Removes the top of the stack and returns the new top item (returns NULL if
 * the stack is empty). s->freeFunc is called on the old top item.
 *
 * @param[in] s Stack to pop
 * @return New top entry or NULL if stack is empty.
*/
void *gPopStack(gStack *s);


#ifdef __cplusplus
}
#endif

#endif

