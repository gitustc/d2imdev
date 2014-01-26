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

#ifndef GCHECKBOX_H
#define GCHECKBOX_H



#define gCheckBoxBase \
   gWidgetBase \
   gSkinIcon *un, *un_over, *un_down;\
   gSkinIcon *ch, *ch_over, *ch_down;\
   gFont     *font;\
   Uint32    fontc;\
   gCallback call;\
   bool checked;\
   char    *caption;\
   void    (*setCaption)(gCheckBox *self, const char *caption);\
   void    (*setState)(gCheckBox *self, bool checked);\
   void    (*setIcon)(gCheckBox *self, const char *un, const char *ch);\



typedef struct gCheckBox_s gCheckBox;
typedef struct gCheckBox_s
{
   gCheckBoxBase
};


// ----------------------------------------------------------------------------
// Member functions

bool gCheckBoxID(gIDFuncParam);
void gCheckBoxDraw(gWidget *self);
void gCheckBoxEventHandler(gWidget *self, SDL_Event *event, bool *claimed);
void gCheckBoxSetFactory(gWidget *self, gFactory *f);
void gCheckBoxDelete(gWidget *self);

// gCheckBoxGetCallback
// returns a pointer to the button's callback object. The object is called when
// the button registers a valid mouse click.
gCallback *gCheckBoxGetCallback(gCheckBox *self);

// gCheckBoxSetCaption
// Sets the caption text for the button.
void gCheckBoxSetCaption(gCheckBox *self, const char *caption);

// gCheckBoxGetState
// Gets the current state of the checkbox (true for checked, false for unchecked)
bool gCheckBoxGetState(gCheckBox *self);


// gCheckBoxSetState
// Sets the current state of the checkbox (true for checked, false for unchecked)
void gCheckBoxSetState(gCheckBox *self, bool checked);


// gCheckBoxGetCaption
// Returns the caption (if there is one)
const char *gCheckBoxGetCaption(gCheckBox *self);

// gCheckBoxSetIcon
// Sets the skin icon used to represent the box. For the sake of this function
// not taking 7 parameters, un is the prefix for the unchecked icons 
// (un, un + "_over", un + "_down") and ch is the prefix for the checked states
// (ch, ch + "_over", ch + "_down")
void gCheckBoxSetIcon(gCheckBox *self, const char *un, const char *ch);

// ----------------------------------------------------------------------------
// Non-member functions

// gIsCheckBox
// Returns true of the given widget can safely be casted to a gCheckBox
bool gIsCheckBox(gWidget *self);

// gInitCheckBox
// Initializes a new check box widget.
void gInitCheckBox(gCheckBox *b);

// gNewCheckBox
// Creates and initializes a new check box widget.
gCheckBox *gNewCheckBox(int x, int y, unsigned int w, unsigned int h, const char *caption);


#endif