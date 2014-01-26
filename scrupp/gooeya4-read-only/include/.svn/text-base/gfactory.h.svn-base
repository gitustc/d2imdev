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

#ifndef GFACTORY_H
#define GFACTORY_H

// Graphical representation of widgets
// gskin.c, gfactory.c, gskinparse.c, gskinspan.c


typedef struct gFactory_s gFactory;


#include "gskin.h"
#include "skinrender.h"
#include "gfont.h"
#include "gpane.h"



// ----------------------------------------------------------------------------
// gFactory

// Todo: Hash processies and attribute key-vals
struct gFactory_s
{
   gList             *plist;
   gList             *ilist;
   gList             *fcache;

   // Going to try switch these over one at a time
   gList             *attributes;

   // Custom cursor support
   gList             *clist;
   gSkinCursor       *activecursor, *defaultcursor;

   bool              updateflag;
   gScreenRect       updaterect;

   int stacklevel, stacksize;
   SDL_Rect *clipstack;

   gSurface          *dest;
   gScreenRect       screenrect;
   gScreenRect       cliprect;

   // Rendering stuff
   int       cliplistsize;
   gClipSpan *cliplist; // List of clipping spans for each row of pixels
   gClipSpan *spanbank;

   // Blended stuff
   int         blendsize, blendmax;
   gBlendedRow *blendlist;
};



#define gBitSurfaceFlags 0





// ----------------------------------------------------------------------------
// gfactory.c


// gCreateFactory
// Creates a gFactory object from the skin template provided. The data from 
// the template is copied to the factory and formatted to the pixel format 
// contained in the screen surface.
gFactory *gCreateFactory(gSkinTemplate *skin, gSurface *screen);

// gFreeFactory
// Frees the given gFactory object, freeing all owned surfaces and data.
void gFreeFactory(gFactory *f);

// gFindFactoryAttribute
// Searches the factory list of attributes for an attribute of 'name' and
// returns the attribute or NULL if it was not found.
gSkinAttribute  *gFindFactoryAttribute(gFactory *f, const char *name, gAttType_e type);

// gFindAttribute
// Searches the given process for attribute of 'name' and returns the attribute
// If the attribute was not found, it will call gFindFactoryAttribute and return
// the result of the function call.
gSkinAttribute *gFindAttribute(gFactory *f, gSkinProcess *p, const char *name, gAttType_e type);


// gMapColor
// Maps a color based on the format of the primary surface of the factory.
Uint32 gMapColor(gFactory *f, SDL_Color *color);


// gFindCursor
// Searches the list of cursors in the given factory for a custom cursor of 'name'
// and returns the cursor or NULL if the cursor was not found
gSkinCursor *gFindCursor(gFactory *f, const char *name);


// gSetCursor
// Calls SDL_SetCursor with the give custom cursor and sets the active cursor 
// variable in the given factory object. If the given cursor is already the
// active cursor, this function does nothing. If cursor is NULL, the factory's
// default cursor is used.
void gSetCursor(gFactory *f, gSkinCursor *cursor);


// gAddUpdateRect
// Flags a region of screenspace to be updated.
void gAddUpdateRect(gFactory *f, gScreenRect *r);

// gPerformUpdate
// Sets the clipping, redraws the background and calls the draw method of the 
// supplied widget.
void gPerformUpdate(gFactory *f, gWidget *w);

// gPushUpdateRect
// Pushes the factorys update rect into the clipping stack. If there is already
// anything in the stack this function will report and error and return false.
bool gPushUpdateRect(gFactory *f);

// gPushClippingStackInside
// Creates a new entry in the clipping stack with the given rect, but
// the rect is clipped to be within the rect of the previous entry on the stack
// returns false if no portion of r lies within the previous entry.
// If the function returns false, the push failed and does not require 
// and additional pop.
// when updateonly is true, r is placed within the factory update rect.
bool gPushClippingStackInside(gFactory *f, gScreenRect *r, bool updateonly);


// gPopClippingStack
// Removes the top item on the clipping stack (returns the stack to its
// previous state)
void gPopClippingStack(gFactory *f);


// gSetClipRect*
// Sets the clipping rect in the destination surface and the cliprect
// struct in the factory. It is reccomended that this function be used to
// set the clipping rect for the factory and the surface not be acted
// upon independantly.
void gSetClipRect(gFactory *f, SDL_Rect *r);
void gSetClipRectScreen(gFactory *f, gScreenRect *r);


// gFindIcon
gSkinIcon *gFindIcon(gFactory *f, const char *name);

// gDrawIcon
void gDrawIcon(gFactory *f, gSkinIcon *icon, gScreenRect *screenrect);

// gFindProcess
gSkinProcess *gFindProcess(gFactory *f, const char *name);


// gRenderProcess
void gRenderProcess(gFactory *f, gRect *rect, const char *name);


void gDrawProcess1Row(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h);
void gDrawProcess2RowsA(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h);
void gDrawProcess2RowsB(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h);
void gDrawProcess2RowsC(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h);
void gDrawProcess3Rows(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h);

void gDrawProcess1Column(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h);
void gDrawProcess2ColumnsA(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h);
void gDrawProcess2ColumnsB(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h);
void gDrawProcess2ColumnsC(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h);
void gDrawProcess3Columns(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h);

void gDrawProcessNOP(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h);



void gDrawSpanNOP(gFactory *f, gSkinSpan *span, int x, int y, unsigned int w, unsigned int h);

// Complete: span contains startbit, endbit, and (potentially) a center piece.
// IncompleteA: span contains startbit and a center piece
// IncompleteB: span contains endbit and a center piece
// 1Bit: Span contains one bit or center piece.
void gDrawRowComplete(gFactory *f, gSkinSpan *span, int x, int y, unsigned int w, unsigned int h);
void gDrawRowIncompleteA(gFactory *f, gSkinSpan *span, int x, int y, unsigned int w, unsigned int h);
void gDrawRowIncompleteB(gFactory *f, gSkinSpan *span, int x, int y, unsigned int w, unsigned int h);
void gDrawRow1Bit(gFactory *f, gSkinSpan *span, int x, int y, unsigned int w, unsigned int h);

void gDrawColumnComplete(gFactory *f, gSkinSpan *span, int x, int y, unsigned int w, unsigned int h);
void gDrawColumnIncompleteA(gFactory *f, gSkinSpan *span, int x, int y, unsigned int w, unsigned int h);
void gDrawColumnIncompleteB(gFactory *f, gSkinSpan *span, int x, int y, unsigned int w, unsigned int h);
void gDrawColumn1Bit(gFactory *f, gSkinSpan *span, int x, int y, unsigned int w, unsigned int h);


#endif


