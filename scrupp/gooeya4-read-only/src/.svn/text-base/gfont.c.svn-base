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
#include "stdarg.h"


// gCreateFont
// Allocates an empty font structure with the supplied name. ASCII characters
// 30 to 128 are rendered using SDL_TTF and stored as gSkinGraphics in the
// new font struct. A pointer to the font is returned.
gFont *gCreateFont(gFactory *f, gFontSpec *fontspec)
{
   gFont       *ret;
   Uint16      i;
   TTF_Font    *ttffont;
   SDL_Color   c;
   SDL_RWops   *fontfile;

   if(!f || !fontspec)
      return NULL;

   // SoM: RWops via PhysicsFS
   if(!PHYSFS_exists(fontspec->name))
      return NULL;
   fontfile = PHYSFSRWOPS_openRead(fontspec->name);

   if(!(ttffont = TTF_OpenFontRW(fontfile, true, fontspec->ptsize)))
      return NULL;

   TTF_SetFontStyle(ttffont, fontspec->style);

   ret = (gFont *)malloc(sizeof(gFont));

   ret->name = strdup(fontspec->name);
   ret->ptsize = fontspec->ptsize;
   ret->style = fontspec->style;

   ret->ascent = TTF_FontAscent(ttffont);
   ret->height = TTF_FontHeight(ttffont);

   ret->glyphs = malloc(sizeof(gGlyph *) * 128);
   memset(ret->glyphs, 0, sizeof(gGlyph *) * 128);

   // Link the new font into the factory
   gAppendListItem(f->fcache, ret);

   c.r = c.g = c.b = 255;

   // Render and cache glyphs
   for(i = 20; i < 128; i++)
   {
      SDL_Surface *s = TTF_RenderGlyph_Blended(ttffont, i, c);
      gGlyph *g;

      if(!s)
         continue;

      ret->glyphs[i] = g = malloc(sizeof(gGlyph));

      g->graphic = gCreateGlyphGraphic(f, s);
      TTF_GlyphMetrics(ttffont, i, &g->minx, NULL, &g->miny, &g->maxy, &g->advance);

      SDL_FreeSurface(s);
   }

   return ret;
}



// gFreeFont
// De-Links and frees the given font structure. This includes all the 
// gSkinGraphic glyph renderings stored within.
void gFreeFont(gFont *font)
{
   int i;

   if(font->name)
      free(font->name);

   if(font->glyphs)
   {
      for(i = 20; i < 128; i++)
      {
         if(font->glyphs[i])
         {
            if(font->glyphs[i]->graphic)
               gDestroyGraphic(font->glyphs[i]->graphic);
            free(font->glyphs[i]);
         }
      }

      free(font->glyphs);
   }
}




unsigned int gStringWidth(gFont *font, const char *str)
{
   int i;
   unsigned int width = 0, longest = 0;

   if(!font || !str)
      return 0;

   for(i = 0; !!str[i]; i++)
   {
      if(str[i] == '\n')
      {
         longest = width > longest ? width : longest;
         width = 0;
         continue;
      }

      width += font->glyphs[str[i]]->advance;
   }

   return width > longest ? width : longest;
}





void gCursorPos(gFont *font, const char *str, unsigned int cpos, int *x, int *y)
{
   unsigned int i;

   if(!font || !str || !x || !y)
      return;

   *x = *y = 0;

   for(i = 0; !!str[i] && i < cpos; i++)
   {
      if(str[i] == '\n')
      {
         *x = 0;
         *y += font->height;
         continue;
      }

      *x += font->glyphs[str[i]]->advance;
   }
}



unsigned int gStringHeight(gFont *font, const char *str)
{
   int i;
   unsigned int height = 0;

   if(!font || !str)
      return 0;

   height = font->height;
   
   for(i = 0; !!str[i]; i++)
   {
      if(str[i] != '\n')
         continue;
      
      height += font->height;
   }

   return height;
}




unsigned int gStringHeightLen(gFont *font, const char *str, unsigned int len)
{
   unsigned int i;
   unsigned int height = 0;

   if(!font || !str)
      return 0;

   height = font->height;
   
   for(i = 0; !!str[i] && i < len; i++)
   {
      if(str[i] != '\n')
         continue;
      
      height += font->height;
   }

   return height;
}





unsigned int gGetStringIndex(gFont *font, const char *string, int mousex, int mousey)
{
   const char     *r;
   int            x, mrow, row, gadvance;
   int            ret = -1;

   mousex = mousex < 0 ? 0 : mousex;
   mousey = mousey < 0 ? 0 : mousey;

   mrow = mousey / font->height;
   row = x = 0;

   for(r = string; *r; r++)
   {
      if(*r == '\n')
      {
         if(row == mrow)
            return (r - string);

         row++;
         x = 0;
         continue;
      }

      if(*r < 20 || *r >= 128)
         continue;

      gadvance = font->glyphs[*r]->advance;

      if(x <= mousex && x + gadvance >= mousex)
      {
         if(row == mrow)
            return (r - string);
         else
            ret = r - string;
      }

      x += gadvance;
   }

   return ret == -1 ? r - string : ret;
}




//static char buf[10240];

// gRenderText
// Renders the given string of text to the given factory inside the given
// screen rect.
void gRenderText(gFactory *f, gFont *font, gScreenRect *r, const char *str, Uint32 color)
{
   int         i, x, y, len;
   gGlyph      *g; 
   const char  *sptr;

   if(!f || !font || !r || !str)
      return;

   if(!gPushClippingStackInside(f, r, false))
      return; // Stack push failed because no window was left open.

   // begin printing.
   len = strlen(str);
   x = r->x1;
   y = r->y1;

   for(i = 0, sptr = str; i < len; i++, sptr++)
   {
      if(*sptr == '\n')
      {
         x = r->x1;
         y += font->height;
         continue;
      }

      g = font->glyphs[*sptr];
      if(!g)
         continue;

      gRenderGraphic(f, g->graphic, 0, 0, x + g->minx, y + font->ascent - g->maxy, g->graphic->width, g->graphic->height, color);
      x += g->advance;
   }
   

   gPopClippingStack(f);
}



// gRenderTextTrunc
// Renders the given string of text to the given factory inside the given
// screen rect. If the string is too long, ... is added to the end
void gRenderTextTrunc(gFactory *f, gFont *font, gScreenRect *r, const char *str, Uint32 color)
{
   int         i, x, y, len;
   gGlyph      *g; 
   const char  *sptr;
   int         width, widthcount, truncsize;

   if(!f || !font || !r || !str)
      return;

   if(!gPushClippingStackInside(f, r, false))
      return; // Stack push failed because no window was left open.

   // begin printing.
   if(font->glyphs['.'])
      truncsize = font->glyphs['.']->advance * 3;
   else
      truncsize = 0;

   width = r->x2 - r->x1 + 1;
   if((int)gStringWidth(font, str) > width)
      width -= truncsize;

   len = strlen(str);
   x = r->x1;
   y = r->y1;
   widthcount = width;

   for(i = 0, sptr = str; i < len; i++, sptr++)
   {
      if(*sptr == '\n')
      {
         x = r->x1;
         y += font->height;
         continue;
      }

      g = font->glyphs[*sptr];
      if(!g)
         continue;

      if(widthcount < g->advance && font->glyphs['.'])
      {
         g = font->glyphs['.'];

         gRenderGraphic(f, g->graphic, 0, 0, x + g->minx, y + font->ascent - g->maxy, g->graphic->width, g->graphic->height, color);
         x += g->advance;
         gRenderGraphic(f, g->graphic, 0, 0, x + g->minx, y + font->ascent - g->maxy, g->graphic->width, g->graphic->height, color);
         x += g->advance;
         gRenderGraphic(f, g->graphic, 0, 0, x + g->minx, y + font->ascent - g->maxy, g->graphic->width, g->graphic->height, color);

         // Go until there is a next line or string is run
         while(*sptr != '\n' && i < len)
         {
            sptr++; i++;
         }

         if(*sptr == '\n')
         {
            x = r->x1;
            y += font->height;
            widthcount = width;
         }

         continue;
      }

      gRenderGraphic(f, g->graphic, 0, 0, x + g->minx, y + font->ascent - g->maxy, g->graphic->width, g->graphic->height, color);
      x += g->advance;
      widthcount -= g->advance;
   }
   

   gPopClippingStack(f);
}



// gRenderTextWordWrap
// Renders the given string of text to the given factory inside the given
// screen rect, wrapping the text if the current word crosses the right-edge 
// of the given screen rect.
void gRenderTextWordWrap(gFactory *f, gFont *font, gScreenRect *r, const char *str, Uint32 color)
{
   int         i, x, y, len;
   gGlyph      *g; 
   const char  *sptr;
   int         wstart, wwidth;

   if(!gPushClippingStackInside(f, r, false))
      return; // Stack push failed because no window was left open.

   // begin printing.
   len = strlen(str);
   x = r->x1;
   y = r->y1;

   // Setup word-based printing loop.
   i = 0;
   sptr = str;

   while(i < len)
   {
      wstart = i;
      wwidth = 0;

      while(i < len && *sptr != ' ' && *sptr != '\n' 
            && font->glyphs[*sptr] && font->glyphs[*sptr]->graphic)
      {
         wwidth += font->glyphs[*sptr]->graphic->width;

         // Things like punctuation are special cases that should be printed as
         // part of the word but should also cause a word wrap event
         if((*sptr >= '!' && *sptr <= '&') || (*sptr >= '(' && *sptr <= '/')
            || (*sptr >= ':' && *sptr <= '?') || (*sptr >= '[' && *sptr <='`') 
            || (*sptr >= '{' && *sptr <= '~'))
         {
            i++; sptr++;
            break;
         }

         i++; sptr++;
      }

      if(wstart < i)
      {
         // Print the word
         if(x + wwidth > r->x2 && x > r->x1)
         {
            x = r->x1;
            y += font->height;
         }

         for(; wstart < i; wstart++)
         {
            g = font->glyphs[str[wstart]];
            if(!g)
               continue;

            gRenderGraphic(f, g->graphic, 0, 0, x + g->minx, y + font->ascent - g->maxy, g->graphic->width, g->graphic->height, color);
            x += g->advance;
         }

         continue;
      }

      if(*sptr == '\n')
      {
         x = r->x1;
         y += font->height;

         sptr++; i++;
         continue;         
      }

      if(!font->glyphs[*sptr] || !font->glyphs[*sptr]->graphic)
      {
         sptr++; i++;
         continue;
      }

      x += font->glyphs[*sptr]->advance;
      sptr++; i++;
   }
   

   gPopClippingStack(f);
}



void gRenderTextCentered(gFactory *f, gFont *font, gScreenRect *r, const char *str, Uint32 color, bool horiz, bool vert)
{
   int width, height;
   gScreenRect newr;

   if(!f || !font || !r || !str)
      return;

   width = gStringWidth(font, str);
   height = gStringHeight(font, str);

   if(horiz)
   {
      newr.x1 = ((r->x1 + r->x2) / 2) - (width / 2);
      newr.x2 = newr.x1 + width;
   }
   else
   {
      newr.x1 = r->x1;
      newr.x2 = r->x2;
   }

   if(vert)
   {
      newr.y1 = ((r->y1 + r->y2) / 2) - (height / 2);
      newr.y2 = newr.y1 + height;
   }
   else
   {
      newr.y1 = r->y1;
      newr.y2 = r->y2;
   }

   gRenderText(f, font, &newr, str, color);
}