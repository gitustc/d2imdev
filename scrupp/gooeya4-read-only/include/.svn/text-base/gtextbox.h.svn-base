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


#ifndef GTEXTBOX_H
#define GTEXTBOX_H


#define gTextBoxBase \
   gWidgetBase\
   gCallback      textcall;\
   gSkinProcess   *proc, *fproc, *sproc;\
   gSkinIcon      *bar;\
   gFont          *font;\
   Uint32         color, scolor;\
   qstring_t      *text;\
   gSkinCursor    *cursor;\
   char           *validchars;\
   unsigned int   maxlen;\
   bool           multiline;\
   int            xoffset, yoffset;\
   int            padding, borderw;\
   unsigned int   cpos;\
   unsigned int   selectstart, selectend, selectbase;\
   void           (*setText)(gTextBox *self, const char *text);\
   void           (*setValidchars)(gTextBox *self, const char *validchars);\
   void           (*setMax)(gTextBox *self, unsigned int max);\


typedef struct gTextBox_s gTextBox;
typedef struct gTextBox_s
{
   gTextBoxBase
};


// ----------------------------------------------------------------------------
// Member function overrides

bool gTextBoxID(gIDFuncParam);
void gTextBoxDraw(gWidget *self);
void gTextBoxEventHandler(gWidget *self, SDL_Event *event, bool *claimed);
gWidget *gTextBoxGetTopWidget(gWidget *self, int x, int y);
gSkinCursor *gTextBoxGetCursor(gWidget *self, int mousex, int mousey);
void gTextBoxSetFactory(gWidget *self, gFactory *f);
void gTextBoxDelete(gWidget *self);


// ----------------------------------------------------------------------------
// Member functions


// gTextBoxSetText
// Sets the text in the box.
void gTextBoxSetText(gTextBox *self, const char *text);


// gTextBoxSetValidchars
// Sets the character validation string of the given text box to a copy of the 
// given string. The string is then considered to be the valid character set
// and a keystroke character must be in the string to be allowed in the text
// box. This function will check the text buffer and strip out any no longer
// valid chars.
void gTextBoxSetValidchars(gTextBox *box, const char *validchars);


// gTextBoxSetMaxLength
// Sets the maximum length of the text (in number of chars).
void gTextBoxSetMaxLength(gTextBox *self, unsigned int max);


// ----------------------------------------------------------------------------
// Non-member functions



// gIsTextBox
// Returns true if the given widget can be safely cast to a gTextBox widget
bool gIsTextBox(gWidget *w);


// gInitTextBox
// Initializes a new gTextBox widget.
void gInitTextBox(gTextBox *tb);

// gNewTextBox
// Creates and initializes a new gTextBox widget.
gTextBox *gNewTextBox(int x, int y, unsigned int w, unsigned int h, const char *text);


#endif // GTEXTBOX_H

