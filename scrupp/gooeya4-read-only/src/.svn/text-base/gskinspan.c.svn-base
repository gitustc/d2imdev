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


void gBlit(gSurface *dest, int dx, int dy, gSurface *src, int sx, int sy, 
           int w, int h)
{
   static SDL_Rect srect, drect;

   if(w <= 0 || h <= 0)
      return;

   srect.x = sx; srect.y = sy;
   drect.x = dx; drect.y = dy;
   srect.w = drect.w = (Uint16)w;
   srect.h = drect.h = (Uint16)h;

   SDL_BlitSurface(src, &srect, dest, &drect);
}




void gDrawSpanNOP(gFactory *f, gSkinSpan *span, int x, int y, 
                                    unsigned int w, unsigned int h)
{}




// ----------------------------------------------------------------------------
// Row Drawers
void gDrawRowComplete(gFactory *f, gSkinSpan *span, int x, int y, 
                      unsigned int w, unsigned int h)
{
   // Complete span containing a start bit, an end, and possibly a center bit
   bool           endjustify = !!(span->flags & gSpanEndJustified);
   gSkinGraphic   *startbit = span->startbit;
   gSkinGraphic   *endbit = span->endbit;
   unsigned int   startbitw = startbit->width, endbitw = endbit->width;


   if(startbitw + endbitw >= w)
   {
      // There is no middle because the span is not rendered wide enough
      // so split the space between the start and end bits.
      unsigned int mid, w1, w2;

      mid = w >> 1;
      w1 = mid > startbitw ? startbitw : 
           mid > endbitw ? w - endbitw : mid;
      w2 = w >= w1 ? w - w1 : 0;

      gRenderGraphic(f, startbit, 0, endjustify ? startbit->height - h : 0, x, y, w1, h, 0);

      gRenderGraphic(f, endbit, w2 >= endbitw ? 0 : endbitw - w2, endjustify ? endbit->height - h : 0, x + w1, y, w2, h, 0);
      return;
   }

   // Enough room.

   gRenderGraphic(f, startbit, 0, endjustify ? startbit->height - h : 0, x, y, startbitw, h, 0);
   gRenderGraphic(f, endbit, 0, endjustify ? endbit->height - h : 0, x + w - endbitw, y, endbitw, h, 0);


   if(span->centerflag == gBitRepeat)
   {
      gSkinGraphic *centerbit = span->centerbit;
      int rx, mw, jy = endjustify ? centerbit->height - h : 0;

      mw = w - (startbitw + endbitw);
      rx = x + startbitw;

      gRenderGraphic(f, centerbit, 0, jy, rx, y, mw, h, 0);
   }
   else if(span->centerflag == gColorFill)
   {
      gRenderBox(f, x + startbitw, y, w - (startbitw + endbitw), h, span->ucolor);
   }
}



void gDrawRowIncompleteA(gFactory *f, gSkinSpan *span, int x, int y, 
                         unsigned int w, unsigned int h)
{
   // Span contains a start bit, and a middle
   bool           endjustify = !!(span->flags & gSpanEndJustified);
   gSkinGraphic   *startbit = span->startbit;
   gSkinGraphic   *centerbit = span->centerbit;
   unsigned int   startbitw = startbit->width;


   if(startbitw >= w)
   {
      // There is no middle because the span is not rendered wide enough
      gRenderGraphic(f, startbit, 0, endjustify ? startbit->height - h : 0, x, y, w, h, 0);
      return;
   }

   // Enough room.
   gRenderGraphic(f, startbit, 0, endjustify ? startbit->height - h : 0, x, y, startbitw, h, 0);

   if(span->centerflag == gBitRepeat)
   {
      int rx, mw, jy = endjustify ? centerbit->height - h : 0;

      mw = w - (startbitw);
      rx = x + startbitw;
      gRenderGraphic(f, centerbit, 0, jy, rx, y, mw, h, 0);
   }
   else if(span->centerflag == gColorFill)
   {
      gRenderBox(f, x + startbitw, y, w - startbitw, h, span->ucolor);
   }
}




void gDrawRowIncompleteB(gFactory *f, gSkinSpan *span, int x, int y, 
                         unsigned int w, unsigned int h)
{
   // Complete span containing an end, and a center bit
   bool           endjustify = !!(span->flags & gSpanEndJustified);
   gSkinGraphic   *endbit = span->endbit;
   unsigned int   endbitw = endbit->width;


   if(endbitw >= w)
   {
      // There is no middle because the span is not rendered wide enough
      gRenderGraphic(f, endbit, w - endbitw, endjustify ? endbit->height - h : 0, x, y, w, h, 0);
      return;
   }

   // Enough room.
   gRenderGraphic(f, endbit, 0, endjustify ? endbit->height - h : 0, x + w - endbitw, y, endbitw, h, 0);

   if(span->centerflag == gBitRepeat)
   {
      gSkinGraphic *centerbit = span->centerbit;
      int rx, mw, jy = endjustify ? centerbit->height - h : 0;

      mw = w - endbitw;
      rx = x;

      gRenderGraphic(f, centerbit, 0, jy, rx, y, mw, h, 0);
   }
   else if(span->centerflag == gColorFill)
   {
      gRenderBox(f, x, y, w - endbitw, h, span->ucolor);
   }
}





void gDrawRow1Bit(gFactory *f, gSkinSpan *span, int x, int y, 
                  unsigned int w, unsigned int h)
{
   // Only one bit
   bool           endjustify = !!(span->flags & gSpanEndJustified);

   if(span->startbit)
      gRenderGraphic(f, span->startbit, 0, endjustify ? h - span->startbit->height : 0, x, y, w, h, 0);
   else if(span->endbit)
   {
      unsigned int xoff = span->endbit->width < (int)w ? 
                          span->endbit->width - (w % span->endbit->width) : 
                          span->endbit->width - w;

      gRenderGraphic(f, span->endbit, xoff, endjustify ? h - span->startbit->height : 0, x, y, w, h, 0);
   }
   else if(span->centerflag == gBitRepeat)
   {
      int jy = endjustify ? span->centerbit->height - h : 0;
      gRenderGraphic(f, span->centerbit, 0, jy, x, y, w, h, 0);
   }
   else if(span->centerflag == gColorFill)
   {
      gRenderBox(f, x, y, w, h, span->ucolor);
   }
}





// ----------------------------------------------------------------------------
// Column Drawers
void gDrawColumnComplete(gFactory *f, gSkinSpan *span, int x, int y, 
                      unsigned int w, unsigned int h)
{
   // Complete span containing a start bit, an end, and possibly a center bit
   bool           endjustify = !!(span->flags & gSpanEndJustified);
   gSkinGraphic   *startbit = span->startbit;
   gSkinGraphic   *endbit = span->endbit;
   unsigned int   startbith = startbit->height, endbith = endbit->height;

   if(startbith + endbith >= h)
   {
      // There is no middle because the span is not rendered tall enough
      // so split the space between the start and end bits.
      unsigned int mid, h1, h2;

      mid = h >> 1;
      h1 = mid > startbith ? startbith : 
           mid > endbith ? h - endbith : mid;
      h2 = h - h1;

      gRenderGraphic(f, startbit, endjustify ? startbit->width - w : 0, 0, x, y, w, h1, 0);
      gRenderGraphic(f, endbit, endjustify ? endbit->width - w : 0, h2 >= endbith ? 0 : endbith - h2, x, y + h1, w, h2, 0);
      return;
   }

   // Enough room.
   gRenderGraphic(f, startbit, endjustify ? startbit->width - w : 0, 0, x, y, w, startbith, 0);
   gRenderGraphic(f, endbit, endjustify ? endbit->width - w : 0, 0, x, y + h - endbith, w, endbith, 0);


   if(span->centerflag == gBitRepeat)
   {
      gSkinGraphic *centerbit = span->centerbit;
      int ry, mh, jx = endjustify ? centerbit->width - w : 0;

      mh = h - (startbith + endbith);
      ry = y + startbith;

      gRenderGraphic(f, centerbit, jx, 0, x, ry, w, mh, 0);
   }
   else if(span->centerflag == gColorFill)
   {
      gRenderBox(f, x, y + startbith, w, h - (startbith + endbith), span->ucolor);
   }
}





void gDrawColumnIncompleteA(gFactory *f, gSkinSpan *span, int x, int y, 
                      unsigned int w, unsigned int h)
{
   // Span containing a start bit, and a center bit
   bool           endjustify = !!(span->flags & gSpanEndJustified);
   gSkinGraphic   *startbit = span->startbit;
   unsigned int   startbith = startbit->height;


   if(startbith >= h)
   {
      // There is no middle because the span is not rendered tall enough
      gRenderGraphic(f, startbit, endjustify ? startbit->width - w : 0, 0, x, y, w, h, 0);
      return;
   }

   // Enough room.
   gRenderGraphic(f, startbit, endjustify ? startbit->width - w : 0, 0, x, y, w, startbith, 0);

   if(span->centerflag == gBitRepeat)
   {
      gSkinGraphic *centerbit = span->centerbit;
      int ry, mh, jx = endjustify ? centerbit->width - w : 0;

      mh = h - startbith;
      ry = y + startbith;

      gRenderGraphic(f, centerbit, jx, 0, x, ry, w, mh, 0);
   }
   else if(span->centerflag == gColorFill)
   {
      gRenderBox(f, x, y + startbith, w, h - startbith, span->ucolor);
   }
}



void gDrawColumnIncompleteB(gFactory *f, gSkinSpan *span, int x, int y, 
                      unsigned int w, unsigned int h)
{
   // Complete span containing an end, and possibly a center bit
   bool           endjustify = !!(span->flags & gSpanEndJustified);
   gSkinGraphic   *endbit = span->endbit;
   unsigned int   endbith = endbit->height;


   if(endbith >= h)
   {
      // There is no middle because the span is not rendered tall enough
      // so split the space between the start and end bits.
      gRenderGraphic(f, endbit, endjustify ? endbit->width - w : 0, endbith - h, x, y, w, h, 0);
      return;
   }

   // Enough room.
   gRenderGraphic(f, endbit, endjustify ? endbit->width - w : 0, 0, x, y + h - endbith, w, endbith, 0);

   if(span->centerflag == gBitRepeat)
   {
      gSkinGraphic *centerbit = span->centerbit;
      int mh, jx = endjustify ? centerbit->width - w : 0;

      mh = h - endbith;

      gRenderGraphic(f, centerbit, jx, 0, x, y, w, mh, 0);
   }
   else if(span->centerflag == gColorFill)
   {
      gRenderBox(f, x, y, w, h - endbith, span->ucolor);
   }
}




void gDrawColumn1Bit(gFactory *f, gSkinSpan *span, int x, int y, 
                      unsigned int w, unsigned int h)
{
   // 1 bit... figure it out
   bool           endjustify = !!(span->flags & gSpanEndJustified);


   if(span->startbit)
   {
      gRenderGraphic(f, span->startbit, endjustify ? span->startbit->width - w : 0, 0, x, y, w, h, 0);
   }
   else if(span->endbit)
   {
      int yoff = span->endbit->height >= (int)h ? span->endbit->height - h :
                 span->endbit->height - (h % span->endbit->height);
      gRenderGraphic(f, span->endbit, endjustify ? span->endbit->width - w : 0, yoff, x, y, w, h, 0);
   }
   else if(span->centerflag == gBitRepeat)
   {
      gRenderGraphic(f, span->centerbit, endjustify ? span->centerbit->width - w : 0, 0, x, y, w, h, 0);
   }
   else if(span->centerflag == gColorFill)
   {
      gRenderBox(f, x, y, w, h, span->ucolor);
   }
}


