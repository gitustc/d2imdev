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


#ifndef SKINRENDER_H
#define SKINRENDER_H


typedef struct gSkinProcess_s gSkinProcess;
typedef struct gSkinSpan_s gSkinSpan;
typedef struct gPixelRow_s gPixelRow;

// ----------------------------------------------------------------------------
// Rendering 

typedef struct gClipSpan_s
{
   int y;
   int first, last;

   struct gClipSpan_s *next, *prev;
} gClipSpan;


typedef void (*gRowClipFunc)(gFactory *f, gPixelRow *row, int offset, int y, int x1, int x2, Uint32 fgcolor);
typedef void (*gRowDrawFunc)(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);


struct gPixelRow_s
{
   int offset, len;
   Uint8 *data, *alphamap;

   // Rendering function goes here
   gRowClipFunc    clipfunc;
   gRowDrawFunc    func;

   struct gPixelRow_s *next;
};



typedef struct gBlendedRow_s
{
   unsigned int len, offset, psize;
   Uint8 *dest;
   gPixelRow *row;

   Uint32 fgcolor;

   gRowDrawFunc    func;
} gBlendedRow;



typedef struct gSkinGraphic_s
{
   int         width, height;
   gPixelRow   *spans;
} gSkinGraphic;





typedef void (*gSpanFunc)(gFactory *, gSkinSpan *, int, int, unsigned int, unsigned int);


struct gSkinSpan_s
{
   unsigned int   thickness;
   int            flags;
   gSpanFunc      spanfunc;

   gSkinGraphic   *startbit;
   
   // The centerflag indicates how the center of a span should be filled
   gCenterFlag    centerflag;

   gSkinGraphic   *centerbit;
   Uint32         ucolor;
   
   gSkinGraphic   *endbit;
};




typedef void (*gSkinProcFunc)(gFactory *, gSkinProcess *, int, int, unsigned int, unsigned int);

struct gSkinProcess_s
{
   char      *name;

   gSkinSpan *startspan;
   gSkinSpan *midspan;
   gSkinSpan *endspan;

   gSkinProcFunc  func;

   // Going to try switch these over one at a time
   // I'm assuming I mean moving attribute lists into hash tables.
   gList             *attributes;
};




typedef struct gSkinIcon_s
{
   char         *name;
   gSkinGraphic *graphic;
} gSkinIcon;




typedef struct gSkinCursor_s
{
   char       *name;
   SDL_Cursor *cursor;
}  gSkinCursor;




// ----------------------------------------------------------------------------
// skinrender.c

// gDepositSpan
// Delinks the given span and stores it in the factory's spanbank
void gDepositSpan(gFactory *f, gClipSpan *span);


// gGetClipSpan
// This function checks the factory spanbank and will return one of the usused
// spans. If the bank is empty, a new span will be allocated and returned.
gClipSpan *gGetClipSpan(gFactory *f);


// gClearClippingList
// Clears the clipping list of the given factory and resets it to its initial
// state. If the size of the desination surface has changed, the array is
// resized after it is cleared and then the new array is initialized.
void gClearClippingList(gFactory *f);

// gFreeClipSpans
// Frees all the allocated clipspans for the factory
void gFreeClipSpans(gFactory *f);


// gAddClipSpan
// This function adds the given span to the list of occupied spans for
// the given row. This function SHOULD NOT be called when the list of clipping
// spans is being traversed because this function will remove span structs from
// the list when two or more spans are merged. 
void gAddClipSpan(gFactory *f, int y, int x1, int x2);

// gFlushBlendedRows
// This should be called at the end of the refresh procedure. It renders all the
// blended pixel rows queued during the widget rendering.
void gFlushBlendedRows(gFactory *f);

// gRenderBox
// Rnders a solid box on the screen and adds clipping spans.
void gRenderBox(gFactory *f, int x, int y, unsigned int w, unsigned int h, Uint32 c);


// gRenderGraphic
// Renders the given graphic using the given factory and screen coordinates.
// Uses the factory's clipping structs.
void gRenderGraphic(gFactory *f, gSkinGraphic *g, unsigned int xoffset, unsigned int yoffset, int x, int y, unsigned int w, unsigned int h, Uint32 fgcolor);


// gCreateGraphic
// This function returns a new gSkinGraphic structure created based on the
// given surface. The surface data is converted to the same format as the
// factory->dest format.
gSkinGraphic *gCreateGraphic(gFactory *f, gSurface *s);


// gMakeBit
// Makes a 'bit' from a given source surface. This function will create a 
// skin graphic and copies the contents of src inside the rect
gSkinGraphic *gMakeBit(gFactory *f, gSurface *src, gRect *rect);


// gCreateGlyphGraphic
// This function returns a new gSkinGraphic structure created based on the
// given glyph surface. The graphic will be able to fill in the solid and
// blended parts of a font glyph with a color, thus facilitating mutliple
// colors from one font.
gSkinGraphic *gCreateGlyphGraphic(gFactory *f, gSurface *s);


// gDestroyGraphic
// Frees all data and structures associated with the given graphic.
void gDestroyGraphic(gSkinGraphic *g);


void gClipRowOpaque(gFactory *f, gPixelRow *row, int offset, int y, int x1, int x2, Uint32 fgcolor);
void gClipRowBlended(gFactory *f, gPixelRow *row, int offset, int y, int x1, int x2, Uint32 fgcolor);


void gRowNOP(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);

void gFillRowOpaque(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);
void gFillRowAlpha_16(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);
void gFillRowAlpha_24(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);
void gFillRowAlpha_32(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);

void gDrawRowOpaque(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);
void gDrawRowAlpha_16(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);
void gDrawRowAlpha_24(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);
void gDrawRowAlpha_32(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);

void gDrawRowAdditive_16(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);
void gDrawRowAdditive_24(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);
void gDrawRowAdditive_32(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor);
#endif

