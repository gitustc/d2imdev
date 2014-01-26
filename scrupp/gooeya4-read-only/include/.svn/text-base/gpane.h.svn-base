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


#ifndef GPANE_H
#define GPANE_H

// ----------------------------------------------------------------------------
// gPain - Resizable surfaces which are mapped to gSkinGraphics automatically

typedef struct gPane_s
{
   gSurface *surface;
   gSkinGraphic *graphic;

   int width, height, maxheight;
   unsigned int pitch;
   void *pixels;
} gPane;


gPane *gCreateNewPane(gFactory *f, int width, int height);
void gResizePane(gPane *pane, int width, int height);
void gFreePane(gPane *pane);

#endif