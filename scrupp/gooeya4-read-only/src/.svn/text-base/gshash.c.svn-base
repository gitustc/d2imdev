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

#include "gshash.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ----------------------------------------------------------------------------
// gStringHash
// Generalized hash table which associates an object with a unique string
// identifier. 


// SoM: This is only ever used in this file
// This is the object type that's actually stored in the lists
typedef struct gHashItem
{
   // This is the key associated with the object
   char *key;

   // This is the hash calculated from the key
   unsigned hash;

   // This is the chain the item is stored in
   unsigned chain;

   // This is the object 
   void *object;

   // This is the function that should be used to free the object when it is
   // deleted. (This is 'inherited' from the gStringHash object)
   void (*freeFunc)(void *object);

   // Next item in the chain
   struct gHashItem *next, *prev;
} gHashItem;



void hashFreeNOP(void *obj) {}

// Used to free the entry objects.
static void freeHashEntry(gHashItem *item)
{
   if(!item)
      return;

   item->freeFunc(item->object);
   free(item->key);
   free(item);
}



static void freeHashKey(void *k)
{
   gStringKey *key = (gStringKey *)k;

   if(!key)
      return;

   if(key->string)
      free(key->string);

   free(key);
}



unsigned shHashCaseKey(const char *string)
{
   const char *c = string;
   unsigned h = 0;

   if(!string)
      return 0;

   while(*c)
   {
      h = 5 * h + toupper(*c);
      ++c;
   }

   return h;
}



unsigned shHashKey(const char *string)
{
   const char *c = string;
   unsigned h = 0;

   if(!string)
      return 0;

   while(*c)
   {
      h = 5 * h + *c;
      ++c;
   }

   return h;
}


static gHashItem *makeChainHead()
{
   gHashItem *it = malloc(sizeof(gHashItem));
   memset(it, 0, sizeof(*it));
   it->next = it->prev = it;
   return it;
}




// shNew
// Creates a new gStringHash object. If freeFunc is null, the code assumes the
// objects in the table should not be freed when they are removed or the table 
// is freed.
gStringHash *shNew(unsigned numChains, void (*freeFunc)(void *), bool ignoreCase)
{
   gStringHash *ret;
   unsigned    i;

   if(!numChains)
      return NULL;

   ret = malloc(sizeof(gStringHash));
   memset(ret, 0, sizeof(*ret));

   ret->chains = gNewList(free);
   ret->keys = gNewList(freeHashKey);

   if(freeFunc == NULL)
      ret->freeFunc = hashFreeNOP;
   else
      ret->freeFunc = freeFunc;

   for(i = 0; i < numChains; i++)
      gAppendListItem(ret->chains, makeChainHead());

   if(ignoreCase)
   {
      ret->hashFunc = shHashCaseKey;
      ret->compFunc = _stricmp;
   }
   else
   {
      ret->hashFunc = shHashKey;
      ret->compFunc = strcmp;
   }

   return ret;
}


// shFree
// Frees the given hash table, deleteing all the bins which free all contained
// objects.
void shFree(gStringHash *table)
{
   unsigned i;
   gHashItem *head, *it, *next;

   if(!table)
      return;


   for(i = 0; i < table->chains->size; i++)
   {
      head = (gHashItem *)gGetListItem(table->chains, i);

      for(it = head->next; it != head; it = next)
      {
         next = it->next;
         freeHashEntry(it);
         it = next; 
      }
   }

   gFreeList(table->chains);
   gFreeList(table->keys);
   free(table);
}



// shRehash
// Resizes and re-hashes the given table to the specified number of bins
bool shRehash(gStringHash *table, unsigned int numChains)
{
   unsigned int i;
   gHashItem    *list = NULL, *it, *next;
   gList        *chains;

   // check everything out
   if(!table || !numChains || !table->chains->size || 
      numChains == table->chains->size)
      return false;

   chains = table->chains;

   // Gather all the hash items
   for(i = 0; i < chains->size; i++)
   {
      gHashItem *head = (gHashItem *)gGetListItem(chains, i);
      gHashItem *first, *last;

      // If there are items in the chain, add it to the gathering chain.
      if(head->next != head)
      {
         first = head->next;
         last = head->prev;

         head->next = head->prev = head;
         first->prev = last->next = NULL;
      }
      else
         first = last = NULL;

      if(!list && first)
         list = first;
      else if(first && last)
      {
         (last->next = list)->prev = last;
         list = first;
      }
   }

   // Expand the table if the new size is larger.
   while(chains->size < numChains)
      gAppendListItem(chains, makeChainHead());

   // Delete extra chains if the new size is smaller.
   if(chains->size > numChains)      
      gDeleteListRange(chains, numChains, chains->size - 1);

   // Now, run through the collector chain and hash items it contains.
   for(it = list; it; it = next)
   {
      unsigned int index = it->chain = it->hash % chains->size;
      gHashItem *head;
      
      next = it->next;
      head = (gHashItem *)gGetListItem(chains, index);
      if(!head)
      {
         // Error out?
         continue;
      }

      (it->next = head->next)->prev = it;
      (it->prev = head)->next = it;
   }

   return true;
}




static gHashItem *findHashItem(gStringHash *table, unsigned hash, const char *key)
{
   gHashItem *item, *head;
   unsigned  chain = hash % table->chains->size;

   if(!table || !key)
      return NULL;

   head = (gHashItem *)gGetListItem(table->chains, chain);

   for(item = head->next; item != head; item = item->next)
   {
      if(item->hash != hash)
         continue;
      if(!table->compFunc(item->key, key))
         return item;
   }

   return NULL;
}



// shStore
// Attempts to add the given object to the hash table. If the given key already exists
// the function will return false. Otherwise returns true. The string passed will be
// copied. The gStringHash will not free the string you pass to this function.
bool shStore(gStringHash *table, const char *key, void *item)
{
   gHashItem *it, *head;
   unsigned  hash;

   if(!table || !key || !item)
      return false;

   hash = table->hashFunc(key);

   if(findHashItem(table, hash, key))
      return false;

   it = malloc(sizeof(gHashItem));
   memset(it, 0, sizeof(*it));

   it->freeFunc = table->freeFunc;
   it->key = _strdup(key);
   it->hash = hash;
   it->chain = hash % table->chains->size;;
   it->object = item;

   head = (gHashItem *)gGetListItem(table->chains, it->chain);
   (it->next = head->next)->prev = it;
   (it->prev = head)->next = it;

   table->itemCount++;

   return true;
}


// shFind
// Searches the hash table for the given key and returns a pointer to the associated
// object (returns NULL on fail)
void *shFind(gStringHash *table, const char *key)
{
   unsigned hash;
   gHashItem *item;

   if(!table || !key)
      return NULL;

   hash = table->hashFunc(key);
   item = findHashItem(table, hash, key);

   return item ? item->object : NULL;
}



static bool removeItem(gStringHash *table, gHashItem *it)
{
   if(!it || !table)
      return false;

   (it->next->prev = it->prev)->next = it->next;

   freeHashEntry(it);
   table->itemCount--;
   return true;
}

// shRemove
// Searches the hash table for the given key and removes it from the chain. If 
// the table is set to free objects, it will be freed. Returns true if the
// key was found, or false if it was not.
bool shRemove(gStringHash *table, const char *key)
{
   unsigned  hash;

   if(!table || !key)
      return false;

   hash = table->hashFunc(key);

   return removeItem(table, findHashItem(table, hash, key));
}




// shGetKey
// Returns a new gStringKey object with the pre-calculated key value. The 
// gStringKey objects are stored within the table and are automatically 
// recalculated when the table rehashes. A table's keys are also freed when
// the table is freed.
gStringKey *shGetKey(gStringHash *table, const char *string)
{
   gStringKey *key;

   unsigned i, hash;

   if(!string || !table)
      return NULL;

   hash = table->hashFunc(string);
   
   for(i = 0; i < table->keys->size; i++)
   {
      key = (gStringKey *)gGetListItem(table->keys, i);

      if(key->hash != hash)
         continue;

      if(!table->compFunc(key->string, string))
         return key;
   }

   key = malloc(sizeof(gStringKey));
   key->string = strdup(string);
   key->hash = hash;

   return key;
}



// shFindKey
// Searches the hash table for the given key and returns a pointer to the associated
// object (returns NULL on fail)
void *shFindKey(gStringHash *table, gStringKey *key)
{
   gHashItem *item;

   if(!key)
      return NULL;

   item = findHashItem(table, key->hash, key->string);
   return item ? item->object : NULL;
}


// shRemoveKey
// Searches the hash table for the given key and removes it from the chain. If 
// the table is set to free objects, it will be freed. Returns true if the
// key was found, or false if it was not.
bool shRemoveKey(gStringHash *table, gStringKey *key)
{
   if(!key)
      return false;

   return removeItem(table, findHashItem(table, key->hash, key->string));
}