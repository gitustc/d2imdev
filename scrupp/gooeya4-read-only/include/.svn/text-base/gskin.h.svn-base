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


#ifndef GSKIN_H
#define GSKIN_H


#include <SDL_ttf.h>

// Temporary stuff used to store skins prior to them being in a factory
typedef SDL_Color gColor;


// ----------------------------------------------------------------------------
// General




typedef struct gTempImage_s
{
   char     *name;
   gSurface *surface;
} gTempImage;



typedef enum
{
   gNoSpanFlags,

   // Use this for the last span of a process. It will cause the bottom/right
   // of the bits to stay fixed and the top will be clipped when spans
   // overlap.
   gSpanEndJustified = 0x1,
} gSpanFlag_e;



typedef struct gTempSpan_s gTempSpan;


typedef enum
{
   gBitOpaque    = 0x0,
   gBitColorKey  = 0x1,
   gBitAlpha     = 0x2,
   gBitAdditive  = 0x4,
} gBitFlag;


typedef struct gTempBit_s
{
   gRect    rect;

   int      flags;
} gTempBit;




typedef enum
{
   gNoCenterFlag,       // Do Nothing
   gBitRepeat,          // Repeat (tile) the center bit to fill the gap
   gColorFill,          // Fill the empty space with a given color
} gCenterFlag;


struct gTempSpan_s
{
   unsigned int   thickness;
   int            flags;

   gTempBit    *startbit;
   
   // The centerflag indicates how the center of a span should be filled
   gCenterFlag centerflag;
   gTempBit    *centerbit;
   gColor      centercolor;
   Uint32      ucolor;
   
   gTempBit    *endbit;
};



typedef struct gTempProcess_s
{
   char      *name, *imgname;

   enum 
   {
      gSpanRows,
      gSpanColumns,
   } orientation;

   gTempSpan *startspan;
   gTempSpan *midspan;
   gTempSpan *endspan;

   gList     *attributes;
} gTempProcess;




typedef struct gTempIcon_s
{
   char     *name, *imgname;
   gRect    rect;
} gTempIcon;




typedef struct gTempCursor_s
{
   char        *name;
   Uint8       *data, *mask;

   unsigned int width, height;
   int          hotx, hoty;
} gTempCursor;


typedef enum
{
   gStylePlain      = 0x00,
   gStyleBold       = 0x01,
   gStyleItalic     = 0x02,
   gStyleUnderline  = 0x04,
} gFontStyle_e;


typedef struct gFontSpec_s
{
   char      *name;

   int       ptsize;
   int       style;

   SDL_Color fgcolor;
} gFontSpec;



typedef enum
{
   attInteger,
   attString,
   attFloat,
   attColor,
   simpleFreeLevel = attColor,

   attFontSpec,
} gAttType_e;



typedef struct gSkinAttribute_s
{
   char        *name;

   gAttType_e  type;
   void        *data;
} gSkinAttribute;


typedef struct
{
   gList             *plist;
   gList             *ilist;
   gList             *clist;

   gList             *images;

   // Going to try switch these over one at a time
   gList             *attributes;
} gSkinTemplate;




// ----------------------------------------------------------------------------
// gskin.c


// gFreeAttribute
// Frees the given object (assumes it is a valud gSkinAttribute)
void gFreeAttribute(void *object);


// gCreateSkinTemplate
// Creates an empty skin template structure which the parsing functions can
// load raw skin data into.
gSkinTemplate *gCreateSkinTemplate();


// gFreeTempImage
// Frees a gTempImage struct and all data contained in it.
void gFreeTempImage(void *object);


// gFreeTempBit
void gFreeTempBit(gTempBit *bit);


// gFreeTempSpan
// Frees the temp span including all bits. This function isn't used in any gLists 
// so it doesn't take a void parm.
void gFreeTempSpan(gTempSpan *span);


// gFreeTempProcess
// Frees the temp process including all spans.
void gFreeTempProcess(void *object);

// gFreeTempIcon
// Unlinks and frees the given icon.
void gFreeTempIcon(void *object);


// gFreeTempCursor
void gFreeTempCursor(void *object);

// gFreeSkinTemplate
// Frees the template object, including all owned surfaces and data.
void gFreeSkinTemplate(gSkinTemplate *skin);


// gFindTempProcess
// Locates a process of the given name in the given template and returns a 
// pointer to it or NULL if no matching processes could be found.
gTempProcess *gFindTempProcess(const char *name, gSkinTemplate *skin);

// gAddProcess
// Adds a process of the given name to the skin template. The new process
// is the returned. If the process already exists in the template, the existing
// process is returned. If parent is not NULL, the information inside parent is
// copied into the result.
gTempProcess *gAddProcess(const char *name, gTempProcess *parent, gSkinTemplate *skin);

// gAddIcon
// Adds an icon of the given name to the skin template. The new icon is
// returned and is empty. If the icon already exists in the skin, the existing
// structure is cleared and returned.
gTempIcon *gAddIcon(const char *name, gSkinTemplate *skin);

// gAddCursor
// Adds a cursor of the given name to the skin template. The new cursor is
// returned and is empty.
gTempCursor *gAddCursor(const char *name, gSkinTemplate *skin);


// gNewIntAttribute
// Wrapper function for gNewAttribute that creates a new string attribute.
gSkinAttribute *gNewIntAttribute(const char *name, int value);

// gNewStringAttribute
// Wrapper function for gNewAttribute that creates a new integer attribute.
gSkinAttribute *gNewStringAttribute(const char *name, const char *value);

// gNewFloatAttribute
// Wrapper function for gNewAttribute that creates a new float attribute.
gSkinAttribute *gNewFloatAttribute(const char *name, float value);

// gNewColorAttribute
// Wrapper function for gNewAttribute that creates a new color attribute.
gSkinAttribute *gNewColorAttribute(const char *name, int r, int g, int b);

// gNewFontAttribute
// Wrapper function for gNewAttribute that creates a new font attribute.
gSkinAttribute *gNewFontAttribute(const char *name, const char *fontname, int ptsize, int style, int r, int g, int b);


// gNewAttribute
// Allocates a new skin attribute structure with the given data
gSkinAttribute *gNewAttribute(const char *name, gAttType_e type, void *data);

// gCopyAttribute
// Allocates a new skin attribute structure with the given data
gSkinAttribute *gCopyAttribute(gSkinAttribute *src);

// gCreateSpan
// Allocates a new empty span structure.
gTempSpan *gCreateTempSpan();


// gMakeCursor
// Creates SDL cursor data from the supplied surface and color information. the
// cursor is stored in the passed cursor struct
bool gMakeCursor
(
   SDL_Surface *image, 
   SDL_Color *transcolor, 
   SDL_Color *xorcolor, 
   unsigned int x, 
   unsigned int y, 
   unsigned int w, 
   unsigned int h, 
   int hotx, 
   int hoty,
   gTempCursor *cursor
);



// gNewFontSpec
// Allocates a new gFontSpec struct and fills it with the given info
gFontSpec *gNewFontSpec(const char *filename, int ptsize, int style, SDL_Color fgcolor);


// gFreeFontSpec
// Frees the given gFontSpec struct.
void gFreeFontSpec(gFontSpec *fs);




// ----------------------------------------------------------------------------
// gskinparse.c


// gReadSkinArchive
// Reads a Gooey skin from the given archive. Returns true on success and false
// on failure. If the function fails, gGetError can be used to obtain an error
// message.
bool gReadSkinArchive(const char *archivename, gSkinTemplate *skin);




// ----------------------------------------------------------------------------
// gskinspan.c

// Blit functions: gBlit, gTileVert, and gTileHoriz
void gBlit(gSurface *dest, int dx, int dy, 
           gSurface *src, int sx, int sy, int w, int h);

/*void gTileVert(gSurface *dest, int dx, int dy, gSurface *src, int sx, int sy, 
               int w, int h);

void gTileHoriz(gSurface *dest, int dx, int dy, gSurface *src, int sx, int sy, 
                int w, int h);*/

#endif

