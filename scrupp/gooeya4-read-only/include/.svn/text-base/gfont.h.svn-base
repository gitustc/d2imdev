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

#ifndef GFONT_H
#define GFONT_H


#include <SDL_ttf.h>



// ----------------------------------------------------------------------------
// Actual fonts used by the renderer

typedef struct
{
   gSkinGraphic *graphic;
   int minx, miny, maxy, advance;
} gGlyph;

typedef struct gFont_s
{
   char        *name;
   int         ptsize;
   int         style;

   gGlyph      **glyphs;
   int         ascent, height;
} gFont;



// gCreateFont
// Allocates a font structure with the supplied info. ASCII characters
// 30 to 128 are rendered using SDL_TTF and stored as gSkinGraphics in the
// new font struct. A pointer to the font is returned.
gFont *gCreateFont(gFactory *f, gFontSpec *fontspec);


// gFindFontSpec
// Searches the given factory for a font matching the supplied spec. If no
// matching font is found, the function will attemp to load and cache it.
gFont *gFindFontSpec(gFactory *f, gFontSpec *fontspec);

// gFindFont
// Wraps the given parameters into a gFontSpec struct and calls gFindFontSpec.
gFont *gFindFont(gFactory *f, char *name, int ptsize, int flags);


// gFreeFont
// De-Links and frees the given font structure. This includes all the 
// gSkinGraphic glyph renderings stored within.
void gFreeFont(gFont *font);


// gStringWidth
// Returns the length of the longest line in the given string as rendered with
// the given font. This does not take word wrapping into account.
unsigned int gStringWidth(gFont *font, const char *str);

// gCursorPos
// Calculates the x and y position of the top-left corner of the character in 
// the given string at index cpos.
void gCursorPos(gFont *font, const char *str, unsigned int cpos, int *x, int *y);


// gStringHeight
// Returns the rendered height of the given string
unsigned int gStringHeight(gFont *font, const char *str);

// gStringHeightLen
// Returns the rendered height of the given string (up to len number of chars)
unsigned int gStringHeightLen(gFont *font, const char *str, unsigned int len);


// gGetStringIndex
// Returns the index of the character to the right of the mouse. mousex and mousey
// should be relative to the top-left corner of the rect where the text would be
// printed. This function does not take word wrapping into account.
unsigned int gGetStringIndex(gFont *font, const char *string, int mousex, int mousey);


typedef void (*gTextFunc)(gFactory *, gFont *, gScreenRect *, const char *, Uint32);

// gRenderText
// Renders the given string of text to the given factory inside the given
// screen rect.
void gRenderText(gFactory *f, gFont *font, gScreenRect *r, const char *str, Uint32 color);


// gRenderTextTrunc
// Renders the given string of text to the given factory inside the given
// screen rect. If the string is too long, ... is added to the end
void gRenderTextTrunc(gFactory *f, gFont *font, gScreenRect *r, const char *str, Uint32 color);


// gRenderTextWordWrap
// Renders the given string of text to the given factory inside the given
// screen rect, wrapping the text if the current word crosses the right-edge 
// of the given screen rect.
void gRenderTextWordWrap(gFactory *f, gFont *font, gScreenRect *r, const char *str, Uint32 color);


// gRenderTextCentered
// Reders the given string centered inside the given screen rect
// The text is centered horizontally if horiz == true and vertically
// if vert is true.
void gRenderTextCentered(gFactory *f, gFont *font, gScreenRect *r, const char *str, Uint32 color, bool horiz, bool vert);

#endif //GFONT_H
