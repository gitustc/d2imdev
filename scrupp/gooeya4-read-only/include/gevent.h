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

#ifndef GEVENT_H
#define GEVENT_H


// ----------------------------------------------------------------------------
// Signals

/**
 * \struct sigResize
 * \brief sent as data for 'resized' and 'childResized' signals.
 *
 * This struct is sent along with 'resized' and 'childResized' signals by
 * Gooeys foundational classes. Signals of this type can more easily be sent
 * with gSendResizeSignal.
 *
 * \see gevent.h::gSendResizeSignal
*/
typedef struct sigResize
{
   gWidget     *widget;  //!< Widget moved/resized
   const gRect *oldrect; //!< Rect with previous size/position
} sigResize;

void gSendResizeSignal(gWidget *w, const char *type, const gRect *oldrect, gWidget *moved_obj);
void gSendKeySignal(gWidget *w, const char *type, int sym, int mods, Uint16 unicode);
//void gSendMouseSignal(gWidget *w, int type, int x, int y, int buttons, int dx, int dy);



typedef struct
{
   const char *type;
   unsigned   hash;

   void *data;
} gSignal;


void gSendSignal(gWidget *w, const char *type, void *data);



typedef void (*signalFunc)(gWidget *sender, gSignal *sig);

typedef struct
{
   /** \brief string name of the signal trapped by the func */
   char        *type;

   /** \brief (case-insensitive) hash of type */
   unsigned    hash;

   /** \brief Function called when signal of type is recieved. */
   signalFunc  func;
} gResponder;


// gNewResponder
// Simply a convience function. Just calls malloc and assigns type and func
gResponder *gNewResponder(const char *type, signalFunc func);


// gFreeResponder
// Function that should be used to free responder objects.
void gFreeResponder(void *object);



// ----------------------------------------------------------------------------
// Callbacks
// Callbacks are handled based on the events which trigger them. Some callbacks
// have specific input which needs to be passed along to the callback function.
// Callbacks may be phased out in favor of the signal system...
typedef enum
{
   callNone,
   callSimple,
   callWidget,
   callInt,
   callEvent,
} gCallType_e;


// Simple callbacks are simple events which require no additional 
// information. This is generally for things like a button click.
typedef void (*gSimpleCallFunc)(gWidget *);
typedef void (*gWidgetCallFunc)(gWidget *, gWidget *);
typedef void (*gIntCallFunc)(gWidget *, int *);
typedef void (*gEventCallFunc)(gWidget *, SDL_Event *);



typedef struct
{
   gCallType_e type;

   gSimpleCallFunc func;
} gSimpleCallData;



typedef struct
{
   gCallType_e type;

   gWidgetCallFunc func;
   gWidget *w;
} gWidgetCallData;

typedef struct
{
   gCallType_e type;

   gIntCallFunc func;
   int *i;
} gIntCallData;


typedef struct
{
   gCallType_e type;

   gEventCallFunc func;
} gEventCallData;


typedef union
{
   gCallType_e type;

   union
   {
      gSimpleCallData   simple;
      gWidgetCallData   widget;
      gIntCallData      integer;
      gEventCallData    event;
   } data;
} gCallback;



// gSetSimpleCallback
// Sets the given callback object to a simple callback with the given function.
void gSetSimpleCallback(gCallback *call, gSimpleCallFunc f);

// gSetWidgetCallback
// Sets the given callback object to a widget callback with the given function
// and widget.
void gSetWidgetCallback(gCallback *call, gWidgetCallFunc f, gWidget *w);


// gSetIntCallback
// Sets the given callback object to a widget callback with the given function
// and widget.
void gSetIntCallback(gCallback *call, gIntCallFunc f, int *i);


// gSetEventCallback
// Sets the given callback object to an event callback with the given function.
void gSetEventCallback(gCallback *call, gEventCallFunc f);


// gCall
// Executes a callback with the given call and caller.
void gCall(gCallback *call, gWidget *caller);


// gEventCall
// Executes a callback with the given caller and event. If the object isn't an
// event callback object, gCall is called with caller.
void gEventCall(gCallback *call, gWidget *caller, SDL_Event *event);







// ----------------------------------------------------------------------------
// Timers
// Timers are callbacks that are set to occur on or after a certain number of 
// system ticks. This system assumes there are 1000 ticks in a second.

typedef struct gTimerHook_s
{
   unsigned int installed;
   unsigned int interval;

   unsigned int nexttrigger;

   gCallback    call;

   struct gTimerHook_s *next, *prev;
} gTimerHook;




// gNewTimer
// Allocates and clears a new timer hook
gTimerHook *gNewTimer();

// gFreeTimer
// Frees a timer hook. If the hook is currently linked into the list of
// active hooks, it is removed first, then freed.
void gFreeTimer(gTimerHook *hook);

// gInstallTimer
// Installs the supplied hook into the list of active timers. This function
// links the given timer object into the list, sets the installed time, and the
// nexttrigger time (starttime + interval)
void gInstallTimer(gTimerHook *hook);

// gUninstallTimer
// Removes the timer object from the list of active timer hooks. Does not free.
void gUninstallTimer(gTimerHook *hook);


// gRunTimers
// Called from the loop this function calls SDL_GetTicks and records the time
// at the start of the function call and checks the list of active hooks, 
// calling any callback functions which are due to be called. This function
// then increments the nexttrigger value by interval.
void gRunTimers();

#endif

