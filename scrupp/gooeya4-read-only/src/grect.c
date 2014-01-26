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
// Helper functions


// gMouseOnRect
// returns true if a point is within a gScreenRect
bool gMouseOnRect(int x, int y, gScreenRect *sr)
{
   if(x < sr->x1 || x > sr->x2 || y < sr->y1 || y > sr->y2)
      return false;

   return true;
}




void gClipScreenRect(gScreenRect *r1, const gScreenRect *r2)
{
   if(r1->x1 < r2->x1)
      r1->x1 = r2->x1;

   if(r1->x2 > r2->x2)
      r1->x2 = r2->x2;

   if(r1->y1 < r2->y1)
      r1->y1 = r2->y1;

   if(r1->y2 > r2->y2)
      r1->y2 = r2->y2;
}
