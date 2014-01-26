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

#ifndef GBUTTON_H
#define GBUTTON_H



// gButton inherits from gWidget
#define gButtonBase \
   gWidgetBase\
   gCallback       click;\
   gSkinProcess    *p1, *pdown, *pover;\
   char            *pname, *downname, *overname;\
   gFont           *font;\
   Uint32          fontcolor;\
   char            *iconname, *caption;\
   gSkinIcon       *icon;\
   bool            down;\
   void            (*setCaption)(gButton *self, const char *caption);\
   void            (*setIcon)(gButton *self, const char *iconname);\
   void            (*setProcess)(gButton *self, const char *p, const char *over, const char *down);\


typedef struct gButton_s gButton;
typedef struct gButton_s
{
   gButtonBase
};


// ----------------------------------------------------------------------------
// Member functions

bool gButtonID(gIDFuncParam);
void gButtonDraw(gWidget *self);
void gButtonSetFactory(gWidget *self, gFactory *f);
void gButtonDelete(gWidget *self);

// gButtonSetIcon
// Sets the name of the icon (optional) to be displayed in the button
void gButtonSetIcon(gButton *self, const char *iconname);

// gButtonSetProcessName
// Sets the names of the skin processes used by the buttons. p is the name of
// the normal state process, over is the name of the mouse-over state process,
// and down is the name of the mouse down state process.
void gButtonSetProcessName(gButton *self, const char *p, const char *over, const char *down);

// gButtonGetCallback
// returns a pointer to the button's callback object. The object is called when
// the button registers a valid mouse click.
gCallback *gButtonGetCallback(gButton *button);


// gButtonSetCaption
// Sets the caption text for the button.
void gButtonSetCaption(gButton *self, const char *caption);


// ----------------------------------------------------------------------------
// Non-member functions

// gIsButton
// Returns true if the given widget can safely be case to gButton
bool gIsButton(gWidget *self);

// gInitButton
// Initializes a new button widget.
void gInitButton(gButton *b);

// gNewButton
// Creates and initializes a new button widget.
gButton *gNewButton(int x, int y, unsigned int w, unsigned int h, const char *caption);

#endif // GBUTTON_H

