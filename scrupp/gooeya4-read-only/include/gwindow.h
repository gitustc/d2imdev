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

#ifndef GWINDOW_H
#define GWINDOW_H

// gwindow.h
// Contains all window variants (window, dialog box, ect)

// ----------------------------------------------------------------------------
// gWindow

// Following widgets are required by this class:
#include "gbutton.h" // Buttons for the titlebar

typedef enum
{
   winDragNone          = 0x0,
   winDragMove          = 0x1,
   winDragResizeTop     = 0x2,
   winDragResizeLeft    = 0x4,
   winDragResizeRight   = 0x8,
   winDragResizeBottom  = 0x10,
} gWindowDragType_e;



#define gWindowBase\
   gContainerBase\
   int tbarh;\
   gSkinProcess *wproc, *topproc;\
   gFont        *tfont, *ttopfont;\
   Uint32       tfontc, ttopfontc;\
   gButton      *minbutton, *maxbutton;\
   gButton      *xbutton;\
   char         *caption;\
   bool         allowresize;\
   int          drag;\
   int          dxoff, dyoff;\
   int          dwoff, dhoff;\
   int          minwidth, minheight;\
   gScreenRect  tbarrect, topdragrect, leftdragrect, rightdragrect,\
                bottomdragrect, trdragrect, tldragrect, brdragrect,\
                bldragrect, titlerect;\
   bool         maximized;\
   gRect        maxrect;\
   unsigned int topborder, leftborder, rightborder, bottomborder;\
   unsigned int buttonwidth, buttonheight, buttonindent, buttontopindent;\
   unsigned int captionindent;\
   int          buttonspacing;\
   gSkinCursor  *c_resizeh, *c_resizew, *c_resizepositive, *c_resizenegative;\
   void         (*setButtons)(gWindow *self, bool min, bool max, bool x);\
   void         (*setCaption)(gWindow *self, const char *caption);\
   void         (*allowResize)(gWindow *self, bool allow);\
   gCallback*   (*xCallback)(gWindow *self);\
   gCallback*   (*maxCallback)(gWindow *self);\
   gCallback*   (*minCallback)(gWindow *self);\
   void         (*setMaximized)(gWindow *self, bool maximized);\

typedef struct gWindow_s gWindow;
typedef struct gWindow_s
{
   gWindowBase
};


// ----------------------------------------------------------------------------
// Member function overrides

bool gWindowID(gIDFuncParam);
void gWindowDraw(gWidget *self);
void gWindowEventHandler(gWidget *self, SDL_Event *event, bool *claimed);
void gWindowSetFactory(gWidget *self, gFactory *f);
gSkinCursor *gWindowGetCursor(gWidget *self, int mousex, int mousey);
gWidget *gWindowGetTopWidget(gWidget *self, int x, int y);
void gWindowDelete(gWidget *self);


// ----------------------------------------------------------------------------
// Member functions
gCallback *gWindowGetXCallback(gWindow *self);
gCallback *gWindowGetMaxCallback(gWindow *self);
gCallback *gWindowGetMinCallback(gWindow *self);

void gWindowSetButtons(gWindow *self, bool min, bool max, bool x);
void gWindowSetCaption(gWindow *self, const char *caption);
void gWindowAllowResize(gWindow *self, bool allowresize);
void gWindowSetMaximized(gWindow *self, bool maximized);

// ----------------------------------------------------------------------------
// Non-member functions

bool gIsWindow(gWidget *w);
void gInitWindow(gWindow *w);
gWindow *gNewWindow(int x, int y, unsigned int w, unsigned int h, const char *caption);
void gWindowSetRects(gWindow *self);

// ----------------------------------------------------------------------------
// Default callbacks
void gWindowDefaultMaxCallback(gWidget *caller, gWidget *other);


#endif//GWINDOW_H

