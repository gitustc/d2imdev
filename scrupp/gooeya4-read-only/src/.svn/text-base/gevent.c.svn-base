// Emacs style mode select -*- C++ -*-
// ----------------------------------------------------------------------------
//
// Copyright(C) 2009 Stephen McGranahan
//
// This file is part of Gooey a4
//
// Gooey a4 is free software: you can redistribute it and/or modify
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
// Gooey a4 is a C/SDL-based Graphic User Interface (GUI) library which
// aims to provide a viable cross-platform user interface API which is simple,
// elegant, and enables rapid application development.
//
// ----------------------------------------------------------------------------


#include "gooeya4.h"


// ----------------------------------------------------------------------------
// Signals
gResponder *gNewResponder(const char *type, signalFunc func)
{
   gResponder *ret = malloc(sizeof(gResponder));
   ret->type = strdup(type);
   ret->hash = shHashCaseKey(type);
   ret->func = func;
   return ret;
}


// gFreeResponder
// Function that should be used to free responder objects.
void gFreeResponder(void *object)
{
   gResponder *res = (gResponder *)object;

   if(!res)
      return;

   if(res->type)
      free(res->type);

   free(res);
}

#if 0 
void gSendBasicSignal(gWidget *w, int type)
{
   gSignal sig;

   sig.type = type;
   w->signalHandler(w, &sig);
}


void gSendMouseSignal(gWidget *w, int type, int x, int y, int buttons, int dx, int dy)
{
   gSignal sig;

   sig.type = type;
   sig.mouse.xpos = x;
   sig.mouse.ypos = y;
   sig.mouse.buttons = buttons;
   sig.mouse.xdelta = dx;
   sig.mouse.ydelta = dy;

   w->signalHandler(w, &sig);
}

void gSendChildSignal(gWidget *w, int type, gWidget *child)
{
   gSignal sig;

   sig.type = type;
   sig.child.child = child;

   w->signalHandler(w, &sig);
}

void gSendParentSignal(gWidget *w, int type, gContainer *parent)
{
   gSignal sig;

   sig.type = type;
   sig.parent.parent = parent;
   w->signalHandler(w, &sig);
}

void gSendKeySignal(gWidget *w, int type, int sym, int mods, Uint16 unicode)
{
   gSignal sig;

   sig.type = type;
   sig.key.keysym = sym;
   sig.key.mods = mods;
   sig.key.unicode = unicode;
   w->signalHandler(w, &sig);
}


void gSendDataSignal(gWidget *w, int type, void *data)
{
   gSignal sig;

   sig.type = type;
   sig.data.ptr = data;
   w->signalHandler(w, &sig);
}

#endif


void gSendKeySignal(gWidget *w, const char *type, int sym, int mods, Uint16 unicode)
{

}


void gSendResizeSignal(gWidget *w, const char *type, const gRect *oldrect, gWidget *moved_obj)
{
   sigResize data;

   data.oldrect = oldrect;
   data.widget = moved_obj;

   gSendSignal(w, type, &data);
}



void gSendSignal(gWidget *w, const char *type, void *data)
{
   gSignal sig;

   sig.type = type;
   sig.hash = shHashCaseKey(type);
   sig.data = data;

   w->signalHandler(w, &sig);
}



// ----------------------------------------------------------------------------
// Callbacks....

void gSetSimpleCallback(gCallback *call, gSimpleCallFunc f)
{
   if(!call) return;

   memset(call, 0, sizeof(gCallback));
   call->type = callSimple;
   call->data.simple.func = f;
}


void gSetWidgetCallback(gCallback *call, gWidgetCallFunc f, gWidget *w)
{
   if(!call) return;

   memset(call, 0, sizeof(gCallback));
   call->type = callWidget;
   call->data.widget.func = f;
   call->data.widget.w = w;
}


void gSetIntCallback(gCallback *call, gIntCallFunc f, int *i)
{
   if(!call) return;

   memset(call, 0, sizeof(gCallback));
   call->type = callInt;
   call->data.integer.func = f;
   call->data.integer.i = i;
}


void gSetEventCallback(gCallback *call, gEventCallFunc f)
{
   if(!call) return;

   memset(call, 0, sizeof(gCallback));
   call->type = callEvent;
   call->data.event.func = f;
}




void gCall(gCallback *call, gWidget *caller)
{
   if(!call) return;

   switch(call->type)
   {
      case callSimple:
         call->data.simple.func(caller);
         break;
      case callWidget:
         call->data.widget.func(caller, call->data.widget.w);
         break;
      case callInt:
         call->data.integer.func(caller, call->data.integer.i);
         break;
      case callEvent:
         call->data.event.func(caller, NULL);
         break;
   }
}



void gEventCall(gCallback *call, gWidget *caller, SDL_Event *event)
{
   if(!call) return;

   if(call->type != callEvent)
   {
      gCall(call, caller);
      return;
   }

   call->data.event.func(caller, event);
}






static gTimerHook head = {0, 0, 0, {0}, NULL, NULL};


gTimerHook *gNewTimer()
{
   gTimerHook *ret = malloc(sizeof(gTimerHook));

   memset(ret, 0, sizeof(gTimerHook));

   return ret;
}


void gFreeTimer(gTimerHook *hook)
{
   gUninstallTimer(hook);

   free(hook);
}





void gInstallTimer(gTimerHook *hook)
{
   gTimerHook *rover;

   if(head.next == NULL || head.prev == NULL)
      head.next = head.prev = &head;

   hook->installed = SDL_GetTicks();
   hook->nexttrigger = hook->installed + hook->interval;

   for(rover = head.next; rover != &head; rover = rover->next)
   {
      if(rover == hook)
         return; // Timer already installed
   }

   (head.prev->next = hook)->prev = head.prev;
   (head.prev = hook)->next = &head;

   return;
}



void gUninstallTimer(gTimerHook *hook)
{
   gTimerHook *rover;

   if(head.next == NULL || head.prev == NULL)
   {
      head.next = head.prev = &head;
      return;
   }

   for(rover = head.next; rover != &head; rover = rover->next)
   {
      if(rover == hook)
      {
         (rover->prev->next = rover->next)->prev = rover->prev;
         return;
      }
   }
}





void gRunTimers()
{
   Uint32 ticks = SDL_GetTicks();
   gTimerHook *rover;

   if(head.next == NULL || head.prev == NULL)
      head.next = head.prev = &head;

   for(rover = head.next; rover != &head; rover = rover->next)
   {
      if(rover->nexttrigger >= ticks)
      {
         gCall(&rover->call, NULL);

         while(rover->nexttrigger <= ticks)
            rover->nexttrigger += rover->interval;
      }
   }
}

