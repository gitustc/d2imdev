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



#ifndef GMAINFRAME_H
#define GMAINFRAME_H


// ----------------------------------------------------------------------------
// Helpers for gMainFrame
gSurface *gLoadImage(const char *name);


// ----------------------------------------------------------------------------
// gMainFrame
// the main window manager element which optionally handles backgrounds.
typedef enum
{
   imageCentered,
   imageTiled,
   imageScaled,
} gMainFrameFlag_e;




#define gMainFrameBase\
   gContainerBase\
   bool updatebg;\
   gColor bgrgb;\
   Uint32 bgcolor;\
   gMainFrameFlag_e  flag;\
   gPane             *bgpane;\
   gSurface          *bgsurface;\
   void              (*setBGColor)(gMainFrame *self, Uint8 r, Uint8 g, Uint8 b);\
   void              (*setBGImage)(gMainFrame *self, gSurface *image, gMainFrameFlag_e flag);\
   void              (*noBG)(gMainFrame *self);\


typedef struct gMainFrame_s gMainFrame;
typedef struct gMainFrame_s
{
   gMainFrameBase
};

// ----------------------------------------------------------------------------
// Member overrides
bool gMainFrameID(gIDFuncParam);
void gMainFrameDraw(gWidget *self);
void gMainFrameSetFactory(gWidget *self, gFactory *f);
void gMainFrameSetRect(gWidget *self, int x, int y, unsigned int w, unsigned int h);
void gMainFrameDelete(gWidget *self);


// ----------------------------------------------------------------------------
// Member functions
void gMainFrameSetBGColor(gMainFrame *self, Uint8 r, Uint8 g, Uint8 b);
void gMainFrameSetBGImage(gMainFrame *self, gSurface *image, gMainFrameFlag_e flag);
void gMainFrameNoBG(gMainFrame *self);


// ----------------------------------------------------------------------------
// Non member functions

// gIsMainFrame
// Returns true if the given gWidget can be safely cast to gMainFrame
bool gIsMainFrame(gWidget *w);

// gInitMainFrame
// Initializes the given gMainFrame object
void gInitMainFrame(gMainFrame *mf);

// gNewMainFrame
// Allocates and initializes a new gMainFrame widget.
gMainFrame *gNewMainFrame(int x, int y, unsigned int w, unsigned int h);


#endif //GMAINFRAME_H