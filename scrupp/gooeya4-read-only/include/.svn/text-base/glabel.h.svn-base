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


#ifndef GLABEL_H
#define GLABEL_H

#include "qstring.h"

// ----------------------------------------------------------------------------
// gLabel - Simple widget that displays text. Can be made to respond to events.

#define gLabelBase \
   gWidgetBase\
   void        (*setCaption)(gLabel *self, const char *caption);\
   const char *(*getCaption)(gLabel *self);\
   qstring_t  *caption;\
   gFont      *font;\
   Uint32     color;\


typedef struct gLabel gLabel;
typedef struct gLabel
{
   gLabelBase
};


// ----------------------------------------------------------------------------
// Member functions

// gLabelDelete
// Deletes a gLabel object.
void gLabelDelete(gWidget *self);

// gLabelDraw
void gLabelDraw(gWidget *self);

// gLabelSetFactory
void gLabelSetFactory(gWidget *self, gFactory *f);

// gLabelSetCaption
// Sets the text of the gLabel.
void gLabelSetCaption(gLabel *self, const char *caption);

// gLabelGetCaption
// Gets the text from a gLabel.
const char *gLabelGetCaption(gLabel *self);

// ----------------------------------------------------------------------------
// Non-member functions

// gIsLabel
// Returns true if the given widget can be cast to a gLabel.
bool gIsLabel(gWidget *w);

// gNewLabel
// Creates, initializes and returns a new gLabel object.
gLabel *gNewLabel(int x, int y, int w, int h, const char *caption);

#endif

