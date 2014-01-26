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

#ifndef GSTRLIST_H
#define GSTRLIST_H

#include "glist.h"

/** 
 * \file gshash.h
 * \brief Generalized string-keyed hashtable.
 *
 * Contains definitions and functions for gStringHash, a generalized hash table
 * which archives, and searches objects by a string "key". This can also be 
 * used for memory management: if a free function is given to the table, that
 * function will be called with any item removed from the table, including 
 * when the table is freed.
*/


#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------------------------------------------
// gStringHash
// Generalized hash table which associates an object with a unique string
// identifier. 


/**
 * \struct gStringHash
 * \brief Generalized hash table object.
 *
 * gStringHash archives, and retrieves objects based on a string "key".
 * Only one occurance of each key is allowed in the table, and the table can 
 * optionally ignore case. These tables can also optionally be used for memory
 * management of the contents. It is recommended that these tables be created 
 * with gNewSHash and freed with gFreeSHash.
 * \see gshash.h::shNew, gshash.h::shFree, 
 *      gshash.h::shStore, gshash.h::shFind, 
 *      gshash.h::shRemove, gshash.h::shRehash
*/
typedef struct gStringHash
{
   gList *chains;  //!< List of hash chains in the table.

   /** \brief Function called when objects are removed from the table. */
   void (*freeFunc)(void *object);

   /** \brief Function used to hash the strings passed to the table. */
   unsigned (*hashFunc)(const char *string);

   /** \brief Function used to compare individual elements in a hash chain. */
   int (*compFunc)(const char *s1, const char *s2);

   /** \brief Number of items in the table. */
   unsigned int itemCount;

   /** \brief List of all precalculated keys */
   gList *keys;
} gStringHash;



/** 
 * \fn unsigned shHashCaseKey(const char *string)
 * \brief Hashes the given string (case-insensitive)
 *
 * Calculates a hash value for an upper-case version of the given string.
 *
 * @param[in] string String value to hash
 * @returns calculated hash value (returns 0 if string is NULL)
*/
unsigned shHashCaseKey(const char *string);


/** 
 * \fn unsigned shHashKey(const char *string)
 * \brief Hashes the given string (case-sensitive)
 *
 * Calculates a hash value for the given string.
 *
 * @param[in] string String value to hash
 * @returns calculated hash value (returns 0 if string is NULL)
*/
unsigned shHashKey(const char *string);



/** 
 * \fn gStringHash *shNew(unsigned numChains, void (*freeFunc)(void *), bool ignoreCase)
 * \brief Creates a new hash table.
 *
 * Creates a new gStringHash object with the given number of hash chains.
 * If freeFunc is null, the code assumes the objects in the table should 
 * not be freed when they are removed or the table is freed.
 *
 * @param[in] numChains Initian number of chains in the table.
 * @param[in] freeFunc Function used to free items when they are removed. Can be NULL.
 * @param[in] ignoreCase If true, the hash table will ignore case when storing/finding items.
 * @returns Newly created hash table or NULL on error.
*/
gStringHash *shNew(unsigned numChains, void (*freeFunc)(void *), bool ignoreCase);


/**
 * \fn void shFree(gStringHash *table)
 * \brief Frees a hash table.
 *
 * Frees the given hash table, calling the freeFunc for all objects in the table.
 *
 * @param[in] table Hash table to free.
*/
void shFree(gStringHash *table);


/**
 * \fn bool shStore(gStringHash *table, const char *key, void *item)
 * \brief Adds an item to a hash table.
 *
 * Attempts to add the given item to the hash table. If the given key already exists
 * the function will return false. Otherwise returns true. The string passed will be
 * copied. The gStringHash will not free the string you pass to this function.
 *
 * @param[in] table Table to add the item to.
 * @param[in] key String key to associate with the item
 * @param[in] item Actual item to be stored. 
 * @returns true on success, false if the item already exists.
*/
bool shStore(gStringHash *table, const char *key, void *item);


/**
 * \fn void *shFind(gStringHash *table, const char *key)
 * \brief Searches a hash table for an item with a matching key.
 *
 * Searches the hash table for the given key and returns a pointer to the associated
 * item.
 *
 * @param[in] table Hash table to search
 * @param[in] key Hash key to search for
 * @returns Associated object on success, NULL on fail.
*/
void *shFind(gStringHash *table, const char *key);


/** 
 * \fn bool shRemove(gStringHash *table, const char *key)
 * \brief Removes an item from a hash table.
 *
 * Searches the hash table for the given key and removes it from the bin. If 
 * the table is set to free objects, it will be freed. 
 *
 * @param[in] table Table to remove the item from
 * @param[in] key The key of the item to be removed.
 * @return True if the item was successfully removed, false if it could not be found.
*/
bool shRemove(gStringHash *table, const char *key);


/**
 * \fn bool shRehash(gStringHash *table, unsigned int numChains)
 * \brief Changes the number of hash chains
 *
 * Resizes and re-hashes the given table to the specified number of chains.
 *
 * @param[in] table Hash table to be rehashed
 * @param[in] numChains Number of chains the table should have.
 * @returns true on success, false on error.
*/
bool shRehash(gStringHash *table, unsigned int numChains);




/**
 * \struct gStringKey
 * \brief Pre-calculated hash key object.
 *
 * gStringKey is a pre-calculated hash key object. It stores a copy of the 
 * string key and the hasked key value. When shRehash is called, the key is 
 * automatically re-calculated.
 * \see gshash.h::shGetKey, gshash.h::shFree, 
 *      gshash.h::shRehash
*/
typedef struct gStringKey
{
   char     *string;   //!< String this key represents
   unsigned hash;      //!< The hash of the key
} gStringKey;


/**
 * \fn gStringKey *shGetKey(gStringHash *table, const char *string)
 * \brief Returns a new gStringKey object with the pre-calculated key value
 *
 * Returns a new gStringKey object with the pre-calculated key value. The 
 * gStringKey objects are stored within the table and are automatically 
 * recalculated when the table rehashes. A table's keys are also freed when
 * the table is freed.
 *
 * @param[in] table Hash table to be rehashed
 * @param[in] string String to hash
 * @returns A pointer to the pre-calculated key.
*/
gStringKey *shGetKey(gStringHash *table, const char *string);



/**
 * \fn void *shFindKey(gStringHash *table, gStringKey *key)
 * \brief Find an object with a key object
 *
 * Searches a gStringHash table from the given pre-cached key.
 *
 * @param[in] table Hash table to search
 * @param[in] key Pre-calculated key object.
 * @returns object assocated with the key or NULL if no object was found.
*/
void *shFindKey(gStringHash *table, gStringKey *key);


/**
 * \fn bool shRemoveKey(gStringHash *table, gStringKey *key)
 * \brief Removes an object with a pre-calculated key
 *
 * Removes an associated object from a gStringHash table using a pre-calculated
 * key.
 *
 * @param[in] table Hash table to search
 * @param[in] key Pre-calculated key object.
 * @returns true if an object was removed or false on error or object not found
*/
bool shRemoveKey(gStringHash *table, gStringKey *key);





#ifdef __cplusplus
}
#endif

#endif
