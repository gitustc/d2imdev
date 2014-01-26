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
#include "gtokenize.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>


// ----------------------------------------------------------------------------
// Internally used typedefs:

typedef struct
{
   // Index of the current step
   int  stepIndex;

   // Index of the first step this stack entry called, or the most recent fallback
   // point set in the current stack entry.
   int  backIndex;

   // Index of the last step this stack entry called with the sfSetFallback flag
   // This is -1 if there have been none.
   int  fallbackIndex;

   // Current error handling function for this stack entry
   tpErrHook  errHook;
} tpStackEntry;


// ----------------------------------------------------------------------------
// Standard ops:
bool opNoComp(tpHookParms *parms, int steptype)
{
   return true;
}

bool opCompType(tpHookParms *parms, int steptype)
{
   return (parms->t->type == steptype);
}



// ----------------------------------------------------------------------------
// Token bank parsing aids.


tpStackEntry *newStackEntry(int startIndex, tpErrHook efunc)
{
   tpStackEntry *ret = malloc(sizeof(tpStackEntry));
   memset(ret, 0, sizeof(*ret));

   ret->backIndex = ret->stepIndex = startIndex;
   ret->errHook = efunc;
   ret->fallbackIndex = -1;

   return ret;
}



// tpNewPattern
// Creates a new tPattern object for use with the parsing functions.
tPattern *tpNewPattern(tpStep *stepList, gTokenStream *tstream)
{
   int i;
   tPattern *ret = malloc(sizeof(tPattern));
   memset(ret, 0, sizeof(*ret));

   ret->stepList = stepList;
   ret->tstream = tstream;

   ret->stack = gNewStack(free);

   // Build labeltable.
   ret->labelTable = shNew(71, NULL, true);
   for(i = 0; stepList[i].stepOp != NULL; i++)
   {
      if(stepList[i].label)
         shStore(ret->labelTable, stepList[i].label, stepList + i);
   }

   return ret;
}



// tpResetPattern
// Clears all internal data to it's initial state. Allows the same pattern to
// be run multiple times
void tpResetPattern(tPattern *p)
{
   gClearStack(p->stack);
   p->ecount = p->wcount = 0;
   p->i = 0;
   gResetTokenStream(p->tstream);
}


// tpFreePattern
// Frees the given tPattern.
void tpFreePattern(tPattern *p)
{
   if(p->stack)
      gFreeStack(p->stack);

   if(p->tstream)
      gFreeTokenStream(p->tstream);

   if(p->labelTable)
      shFree(p->labelTable);

   free(p);
}



// tpClearInlist
// Clears out the inlist, and clears the cache in the token stream.
void tpClearInlist(tpHookParms *p)
{
   if(!p)
      return;

   gClearList(p->inlist);
   gClearTCache(p->pattern->tstream);
}




int tpIncrementCount(tPattern *p, int code)
{
   if(code == tpError || code == tpFatal)
      p->ecount++;
   else if(code == tpWarning)
      p->wcount++;

   return code;
}




static void setError(tPattern *p, const char *msg, int elevel, gToken *t)
{
   gTokenStream *ts = p->tstream;
   char ebuf[1025];

   if(!elevel)
      return;

   _snprintf(ebuf, 1024, msg, t->token);

   if(elevel == tpWarning)
      ts->parameters->setError("Warning %s(%i, %i): %s\n", ts->name, t->linenum, t->charnum, ebuf);
   if(elevel == tpError)
      ts->parameters->setError("Error %s(%i, %i): %s\n", ts->name, t->linenum, t->charnum, ebuf);
   if(elevel == tpFatal)
      ts->parameters->setError("Fatal Error %s(%i, %i): %s\n", ts->name, t->linenum, t->charnum, ebuf);
}



static tpStackEntry *findCatch(tPattern *p, bool forcepop)
{
   tpStackEntry *ret;
   gStack *s = p->stack;

   if(forcepop)
      ret = (tpStackEntry *)gPopStack(s);
   else
      ret = (tpStackEntry *)gGetStackTop(s);

   while(ret && ret->fallbackIndex == -1)
      ret = (tpStackEntry *)gPopStack(s);
   
   if(ret)
      ret->stepIndex = ret->fallbackIndex;

   return ret;
}



// tpExecutePattern
// Executes the given pattern object and returns the highest error level
// encountered during execution.
int tpExecutePattern(tPattern *p)
{
   tpStackEntry *top;
   gTokenStream *ts;
   tpStep       *step, *substep;
   gToken       *t;
   gList        *tlist = gNewList(NULL);
   int          ret = tpNoError;
   int          hookret;

   tpHookParms  hp;

   p->i = 0;
   p->ecount = p->wcount = 0;

   hp.pattern = p;
   hp.inlist = tlist;

   ts = p->tstream;

   // Make sure the stack is empty and create the first entry
   if(p->stack->top)
      gClearStack(p->stack);

   // Create the first stack entry
   gPushEntry(p->stack, top = newStackEntry(p->i, NULL));

   while((t = gGetToken(p->tstream, p->i)) != NULL)
   {
      bool result;
      int  code, flags;

      hp.step = step = &p->stepList[top->stepIndex];

      // NULL stepOp means it's the end of the pattern list. Abort.
      if(!step->stepOp)
         break;

      // set the error hook before doing anything else.
      if(step->errHook)
         top->errHook = step->errHook;

      hp.t = t;

      // Now: Run the op and handle the output.
      result = step->stepOp(&hp, step->tokenType);

      // Separate out the flags from the code section of the action to be taken.
      flags = result ? step->onTrue : step->onFalse;
      code = flags & scCodeMask;
      flags &= ~scCodeMask;

      if(flags & sfStore)
         gAppendListItem(tlist, t);
      if(flags & sfSetFB)
         top->fallbackIndex = top->backIndex = top->stepIndex;

      if(code != scThrow)
      {
         // No throw, so if result was successful, run the step hook and check
         // the result.
         if(!result)
            hookret = tpNoError;
         else
         {
            hookret = step->hook ? step->hook(&hp) : tpNoError;
            if(hookret > ret)
               ret = hookret;
         }
      }

      // scThrow and hook errors need to be handled first
      if(code == scThrow || hookret != tpNoError)
      {
         int errores;

         if(step->emsg)
            setError(p, step->emsg, step->elevel, t);

         if(step->elevel > ret)
            ret = step->elevel;

         errores = (top->errHook && step->elevel < tpFatal) ? top->errHook(&hp) : step->elevel;

         tpIncrementCount(p, step->elevel);

         if(errores == tpAbort)
            goto finish;
         else if(errores > tpContinue)
         {
            if(!(top = findCatch(p, errores == tpPopFallback ? true : false)))
               goto finish;
         }

         continue;
      }

      // If the code is label related, make sure the label is valid!
      if(code == scPush || code == scGoto)
      {
         char *label = step->subLabel;
         if(!label)
         {
            ret = tpFatal;
            ts->parameters->setError("Pattern error: Step with an opcode requiring a label had no label.\n");
            goto finish;
         }
         else if(!(substep = shFind(p->labelTable, label)))
         {
            ret = tpFatal;
            ts->parameters->setError("Pattern error: Label '%s' not found in pattern.\n", label);
            goto finish;
         }
      }

      switch(code)
      {
         case scEnd:
            gClearStack(p->stack);
            top = NULL;
            goto finish;
            break;
         case scPop:
            if(!(top = gPopStack(p->stack)))
               goto finish;
            break;
         case scNext:
            top->stepIndex++;
            break;
         case scPush:
            if(flags & sfBack)
               top->stepIndex = top->backIndex;
            else
               top->stepIndex ++;

            gPushEntry(p->stack, top = newStackEntry(substep - p->stepList, top->errHook));
            break;
         case scGoto:
            top->stepIndex = substep - p->stepList;
            break;
         default:
            ts->parameters->setError("Internal error: tpRunPattern: step op unhandled/unrecognized.");
            ret = tpFatal;
            p->ecount++;
            goto finish;
            break;
      }

      if(!(flags & sfStay))
         p->i++;
   }

   // Out of tokens but NOT out of the stack?
   if(p->stack->top)
   {
      ts->parameters->setError("%s: Unexpected EOF", ts->name);
      p->ecount++;
      ret = tpFatal;
   }

   finish:

   // Couple of things. First, if there were warnings or errors, set another 
   // error message.
   if(p->wcount > 0 || p->ecount > 0)
      ts->parameters->setError("%s: %i Errors, %i Warnings", ts->name, p->ecount, p->wcount);

   // Clean up
   gFreeList(tlist);

   return ret;
}

