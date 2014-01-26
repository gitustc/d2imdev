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


#ifndef GLIST_H
#define GLIST_H

/** \file glist.h
    \brief Generalized expandable array.

    gList allows items to be stored in an expandable array. gList handles all
    the allocation and can even be used for memory management. If the list is
    given a free function, any item removed from the list (including when the 
    list is freed) will be sent to that free function.
*/

#ifdef __cplusplus
extern "C"
{
#endif

#include "gbool.h"

// ----------------------------------------------------------------------------
// gList
// Resizable array of object pointers. 


/**
 * \struct gList
 * \brief Structure which holds the list and all related data.
 *
 * Structure which holds the list and all related data.
 *
 * \see  glist.h::gNewList, glist.h::gInitList, glist.h::gFreeList,
 *       glist.h::gClearList, glist.h::gAppendListItem,
 *       glist.h::gInsertListItem, glist.h::gMoveListItem,
 *       glist.h::gGetListSize, glist.h::gGetListItem,
 *       glist.h::gDeleteListItem, glist.h::gDeleteListRange,
 *       glist.h::gTrimUnusedList, glist.::gMoveListItems
*/
typedef struct
{
   void     **list; //!< List of pointers to items
   unsigned max;    //!< Total number of pointers allocated
   unsigned size;   //!< Total number of items in the list (occupied pointers)

   void     (*freeFunc)(void *); //!< Function called with an item is removed.
   bool     freestruct;          //!< If true, gFreeList will free this struct.
} gList;



/**
 * \fn gList *gNewList(void (*freeFunc)(void *))
 * \brief Allocates and inits new gList object.
 *
 * Allocates, initializes and returns a new gList object. freeFunc should point
 * to the function that should free an array object when it is deleted. If 
 * freeFunc is NULL, a NOP is used.
 *
 * @param[in] freeFunc Function caled when an item is removed from the list. Can be NULL.
 * @returns Newly allocated gList object.
*/
gList *gNewList(void (*freeFunc)(void *));


/**
 * \fn void gInitList(gList *list, void (*freeFunc)(void *))
 * \brief Initializes a gList
 *
 * Initializes the given list structure. This is called by newlist on the allocated
 * list structure. This function can be used to initialize a static gList.
 *
 * @param[in] list The list object to initialize
 * @param[in] freeFunc The function called when a list item is removed.
*/
void gInitList(gList *list, void (*freeFunc)(void *));


/** 
 * \fn void gFreeList(gList *l)
 * \brief Frees a list.
 *
 * Frees all substructures and data contained in a gList. If the freestruct
 * flag is set in the given list struct, the struct itself is freed.
 *
 * @param[in] l The list to be emptied and freed.
*/
void gFreeList(gList *l);


/**
 * \fn void gClearList(gList *l)
 * \brief Clears list items.
 *
 * Frees all items contained in a gList, but keeps the list structures 
 * allocated.
 *
 * @param[in] l List to be emptied.
*/
void gClearList(gList *l);


/**
 * \fn void gAppendListItem(gList *l, void *object)
 * \brief Adds an item to the end if a list.
 *
 * Adds the given object to the end of the given list, expanding the list if 
 * necessary to accommodate the new entry.
 *
 * @param[in] l List to add an item to
 * @param[in] object Item to be added to the list.
*/
void gAppendListItem(gList *l, void *object);


/**
 * \fn void gInsertListItem(gList *l, void *object, unsigned int toindex)
 * \brief Inserts a list item at a specified index
 *
 * Inserts the given object into the given list at toindex, moving any 
 * objects >= toindex to make room for the new item. This function will also
 * expand the list if needed. toindex is clipped to the size of the list.
 * so an object won't be placed a index 127129 if the list is 2 items long.
 *
 * @param[in] l List to add the item to
 * @param[in] object Item to be added to the list
 * @param[in] toindex Index to place the item at (0 to l->size)
*/
void gInsertListItem(gList *l, void *object, unsigned int toindex);


/**
 * \fn void gMoveListItem(gList *l, unsigned int index, unsigned int newindex)
 * \brief Moves an item in a list.
 *
 * Moves an item within the array from index to newindex. If index is outside 
 * the bounds of the array, the function will abort. If newindex is outside the
 * bounds of the array, the value is clipped to remain within the array.
 * This function will shift all items between index and newindex to accomidate
 * the move.
 *
 * @param[in] l List the item should be moved in.
 * @param[in] index Index of the item to move.
 * @param[in] newindex Index where the item should move.
*/
void gMoveListItem(gList *l, unsigned int index, unsigned int newindex);


/**
 * \fn int gGetListSize(gList *l)
 * \brief Returns the number of items in a list.
 * 
 * Returns the number of items in a list.
 *
 * @param l List to examine.
 * @return Number if items in l. Equiv. to l->size.
*/
int gGetListSize(gList *l);


/**
 * \fn void *gGetListItem(gList *l, unsigned int index)
 * \brief Get an item in a list.
 *
 * Returns the a pointer at index from strlist.
 *
 * @param[in] l List to get the item from
 * @param[in] index Index of the item to return
 * @return The item at index or NULL if the index is >= l->size
*/
void *gGetListItem(gList *l, unsigned int index);


/**
 * \fn void gDeleteListItem(gList *l, unsigned int index)
 * \brief Removes an item from a list.
 *
 * Removes an item from the list, and calls l->freeFunc with the item.
 * If the item index is out of bounds, the function simply aborts. If the item 
 * is removed, all items past index are shifted down one slot to keep the list 
 * continuous.
 *
 * @param[in] l List to remove the item from
 * @param[in] index Index of the item to remove
*/
void gDeleteListItem(gList *l, unsigned int index);


/**
 * \fn void gDeleteListRange(gList *l, unsigned int first, unsigned int last)
 * \brief Removes a range of items from the list.
 *
 * Deletes a series of items from the list. The indices are clipped to the
 * actual dimensions of the list. If first is greater than last, or if first is
 * out of bounds, the function aborts.
 *
 * @param[in] l List to remove the items from
 * @param[in] first Index of the first item to remove
 * @param[in] last Index of the last item to remove
*/
void gDeleteListRange(gList *l, unsigned int first, unsigned int last);


/**
 * \fn void gTrimUnusedList(gList *l)
 * \brief Trims unused memory.
 *
 * This function will reallocate the internal pointer list of the given list
 * object to the current list size. This is useful if the list got really big
 * and doesn't need to be that big any more.
 *
 * @param[in] l List to trim
*/
void gTrimUnusedList(gList *l);


/**
 * \fn bool gMoveListItems(gList *src, unsigned int srcindex, gList *dest, unsigned int count)
 * \brief Move items from one list to another.
 *
 * Moves items from src and appends them to dest. Note: The objects are MOVED which
 * means they will not be in the source list anymore. Also Note: This function
 * can be dangerous if moving between lists with different free functions.
 *
 * @param[in] src List to copy items from
 * @param[in] srcindex Index in the source list to start pulling items from
 * @param[out] dest List to move items to
 * @param[in] count Number if items to copy
 * @returns true on success, false on error.
*/
bool gMoveListItems(gList *src, unsigned int srcindex, gList *dest, unsigned int count);


#ifdef __cplusplus
}
#endif

#endif

