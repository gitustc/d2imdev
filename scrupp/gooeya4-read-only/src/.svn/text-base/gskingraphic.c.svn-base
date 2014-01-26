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
// Graphic generation / Management
static void alphaMpaFormat(SDL_PixelFormat *fmt)
{
   // Format the map image. We want a 32-bit image (8 red, 8 green, 8 blue, no alpha)
   fmt->BytesPerPixel = 4;
   fmt->BitsPerPixel = 32;
   fmt->Rloss = fmt->Gloss = fmt->Bloss = 0;
   fmt->Aloss = 0;
   fmt->Rmask = 0xFF000000;
   fmt->Gmask = 0x00FF0000;
   fmt->Bmask = 0x0000FF00;
   fmt->Amask = 0x000000FF;
   fmt->Rshift = 24;
   fmt->Gshift = 16;
   fmt->Bshift = 8;
   fmt->Ashift = 0;
   fmt->colorkey = 0;
   fmt->palette = NULL;
}

// copRowOpaque
// copies a row of opaque pixels to the given gPixelRow
static void copyRowOpaque(gPixelRow *row, Uint8 *src, int width, int pixelsize)
{
   row->clipfunc = gClipRowOpaque;
   row->func = gDrawRowOpaque;
   row->data = (Uint8 *)malloc(pixelsize * width);
   row->len = width;
   memcpy(row->data, src, width * pixelsize);
}



// allocateGraphic
// allocates, initializes, and returns a new graphic structure.
static gSkinGraphic *allocateGraphic(unsigned int w, unsigned int h)
{
   gSkinGraphic *ret = malloc(sizeof(gSkinGraphic));
   int y;

   ret->width = w;
   ret->height = h;

   ret->spans = malloc(sizeof(gPixelRow) * ret->height);
   memset(ret->spans, 0, sizeof(gPixelRow) * ret->height);

   // In case anything goes wrong!
   for(y = 0; y < ret->height; y++)
      ret->spans[y].func = gRowNOP;

   return ret;
}







gSkinGraphic *gCreateGlyphGraphic(gFactory *f, gSurface *s)
{
   gSkinGraphic     *ret;
   int              y, x, pixelsize;
   int              rowstart, rownext;
   gPixelRow        *rrover;
   Uint32           *blendmap;
   int              p1, i;
   gSurface         *msurface;
   SDL_PixelFormat  mapformat;

   if(!f || !s)
      return NULL;

   pixelsize = f->dest->format->BytesPerPixel;

   p1 = s->w;

   alphaMpaFormat(&mapformat);
   if(!(msurface = SDL_ConvertSurface(s, &mapformat, 0)))
      return NULL;

   ret = allocateGraphic(s->w, s->h);

   for(y = 0; y < ret->height; y++)
   {
      blendmap = (Uint32 *)msurface->pixels + y * p1;
      rrover = NULL;

      rownext = 0;

      for(x = 0; x < ret->width;)
      {
         // Skip transparent pixels
         while(x < ret->width && (*blendmap & 0xFF) == 0)
            x++, blendmap++;


         // Transparent pixels have been skipped over
         if(!rrover)
            rrover = ret->spans + y;
         else
         {
            rrover->next = malloc(sizeof(gPixelRow));
            rrover = rrover->next;
            memset(rrover, 0, sizeof(gPixelRow));

            rrover->func = gRowNOP;
         }

         rrover->offset = x - rownext;

         // Reached the end of the row, but there still needs to be a nop
         // span here to hold place for the renderer.
         if(x == ret->width)
            break;

         rowstart = x;
         while(x < ret->width && (blendmap[x - rowstart] & 0xFF) != 0x0)
         {
            x++;
         }

         if(x > rowstart)
         {
            // Blended pixels
            rrover->len = x - rowstart;
            rrover->alphamap = (Uint8 *)malloc(rrover->len * 2);
            rrover->clipfunc = gClipRowBlended;

            switch(pixelsize)
            {
               case 2:
                  rrover->func = gFillRowAlpha_16;
                  break;
               case 3:
                  rrover->func = gFillRowAlpha_24;
                  break;
               case 4:
                  rrover->func = gFillRowAlpha_32;
                  break;
               default:
                  break;
            }

            for(i = 0; i < rrover->len; i++)
            {
               rrover->alphamap[i * 2] = (Uint8)(blendmap[i] & 0xFF);
               rrover->alphamap[i * 2 + 1] = 255 - rrover->alphamap[i * 2];
            }

            blendmap += rrover->len;
            rownext = x;
         }
      }
   }

   SDL_FreeSurface(msurface);
   return ret;
}





gSkinGraphic *gCreateGraphic(gFactory *f, gSurface *s)
{
   gSkinGraphic   *ret;
   int            y, x, pixelsize;
   gSurface       *src;
   int            rowstart, rownext;
   Uint8          *pixels;
   gPixelRow      *rrover;

   if(!f || !s)
      return NULL;

   if(!(src = SDL_ConvertSurface(s, f->dest->format, 0)))
      return NULL;

   ret = allocateGraphic(src->w, src->h);

   pixelsize = f->dest->format->BytesPerPixel;

   if(s->flags & SDL_SRCALPHA)
   {
      Uint32   *blendmap;
      int      p1, p2, i;
      gSurface *msurface;
      SDL_PixelFormat  mapformat;

      p1 = src->pitch;
      p2 = src->w;

      alphaMpaFormat(&mapformat);
      if(!(msurface = SDL_ConvertSurface(s, &mapformat, 0)))
         return NULL;

      for(y = 0; y < ret->height; y++)
      {
         pixels = (Uint8 *)src->pixels + y * p1;
         blendmap = (Uint32 *)msurface->pixels + y * p2;
         rrover = NULL;

         rownext = 0;

         for(x = 0; x < ret->width;)
         {
            // Skip transparent pixels
            while(x < ret->width && (*blendmap & 0xFF) == 0)
               x++, pixels += pixelsize, blendmap++;


            // Transparent pixels have been skipped over
            if(!rrover)
               rrover = ret->spans + y;
            else
            {
               rrover->next = malloc(sizeof(gPixelRow));
               rrover = rrover->next;
               memset(rrover, 0, sizeof(gPixelRow));

               rrover->func = gRowNOP;
            }

            rrover->offset = x - rownext;

            // Reached the end of the row, but there still needs to be a nop
            // span here to hold place for the renderer.
            if(x == ret->width)
               break;

            rowstart = x;
            // check map, if the row is opaque, store them as opaque pixels
            while(x < ret->width && (*blendmap & 0xFF) == 0xFF)
               x++, blendmap++;

            if(x > rowstart)
            {
               copyRowOpaque(rrover, pixels, x - rowstart, pixelsize);

               pixels += rrover->len * pixelsize;
               rownext = x;
               continue;
            }

            while(x < ret->width && (blendmap[x - rowstart] & 0xFF) != 0xFF 
               && !!(blendmap[x - rowstart] & 0xFF))
            {
               x++;
            }

            if(x > rowstart)
            {
               // Blended pixels
               rrover->len = x - rowstart;
               rrover->data = (Uint8 *)malloc(rrover->len * pixelsize);
               rrover->alphamap = (Uint8 *)malloc(rrover->len * 2);
               rrover->clipfunc = gClipRowBlended;

               switch(pixelsize)
               {
                  case 2:
                     rrover->func = gDrawRowAlpha_16;
                     break;
                  case 3:
                     rrover->func = gDrawRowAlpha_24;
                     break;
                  case 4:
                     rrover->func = gDrawRowAlpha_32;
                     break;
                  default:
                     break;
               }

               memcpy(rrover->data, pixels, rrover->len * pixelsize);

               for(i = 0; i < rrover->len; i++)
               {
                  rrover->alphamap[i * 2] = (Uint8)(blendmap[i] & 0xFF);
                  rrover->alphamap[i * 2 + 1] = 255 - rrover->alphamap[i * 2];
               }

               pixels += rrover->len * pixelsize;
               blendmap += rrover->len;
               rownext = x;
            }
         }
      }

      SDL_FreeSurface(msurface);
   }
   else if(src->flags & SDL_SRCCOLORKEY)
   {
      Uint32 colorkey = src->format->colorkey;

      for(y = 0; y < ret->height; y++)
      {
         pixels = (Uint8 *)src->pixels + src->pitch * y;

         rrover = NULL;

         rownext = 0;

         for(x = 0; x < ret->width;)
         {
            // Skip transparent pixels
            while(x < ret->width && !memcmp(pixels, &colorkey, pixelsize))
               x++, pixels += pixelsize;

            // Either an opaque pixel was encountered, or the end of the row
            // in each case, a new row should be allocated to occupy space so
            // the renderer can properly space the rows when rendering.
            if(!rrover)
               rrover = ret->spans + y;
            else
            {
               rrover->next = malloc(sizeof(gPixelRow));
               rrover = rrover->next;
               memset(rrover, 0, sizeof(gPixelRow));
            }

            rrover->offset = x - rownext;

            // Reached the end of the row, but there still needs to be a nop
            // span here to hold place for the renderer.
            if(x == ret->width)
            {
               rrover->func = gRowNOP;
               break;
            }

            rowstart = x;

            while(x < ret->width && memcmp(pixels + (x - rowstart) * pixelsize, &colorkey, pixelsize))
               x++;

            // x now lies one pixel past the end of the opaque row
            copyRowOpaque(rrover, pixels, x - rowstart, pixelsize);

            pixels += pixelsize * rrover->len;
            rownext = x;
         }
      }
   }
   else
   {
      for(y = 0; y < ret->height; y++)
         copyRowOpaque(&ret->spans[y], (Uint8 *)src->pixels + (src->pitch * y), ret->width, pixelsize);
   }

   SDL_FreeSurface(src);
   return ret;
}


gSkinGraphic *gMakeBit(gFactory *f, gSurface *src, gRect *rect)
{
   gSkinGraphic   *ret;
   SDL_Rect       srect, drect;
   gSurface       *stemp;
   int            hide;

   stemp =
      SDL_CreateRGBSurface(gBitSurfaceFlags, rect->w, rect->h, 
      src->format->BitsPerPixel,
      src->format->Rmask, src->format->Gmask, src->format->Bmask, 
      src->format->Amask);

   stemp->flags = 0;

   // CHECK ALPHA AND COLORKEY FLAGS??
   hide = src->flags & (SDL_SRCALPHA | SDL_SRCCOLORKEY);
   src->flags &= ~hide;

   srect.x = rect->x;
   srect.y = rect->y;

   drect.x = 0;
   drect.y = 0;

   srect.w = drect.w = rect->w;
   srect.h = drect.h = rect->h;

   SDL_BlitSurface(src, &srect, stemp, &drect);

   src->flags |= hide;
   stemp->flags |= hide;

   if(hide & SDL_SRCCOLORKEY)
      SDL_SetColorKey(stemp, SDL_SRCCOLORKEY, src->format->colorkey);

   ret = gCreateGraphic(f, stemp);

   SDL_FreeSurface(stemp);

   return ret;
}




void gDestroyGraphic(gSkinGraphic *g)
{
   gPixelRow *r, *next;
   int i;

   for(i = 0; i < g->height; i++)
   {
      r = g->spans + i;

      if(r->data)
         free(r->data);

      if(r->alphamap)
         free(r->alphamap);

      next = r->next;

      while(next)
      {
         r = next;
         if(r->data)
            free(r->data);

         if(r->alphamap)
            free(r->alphamap);

         next = r->next;
         free(r);
      }
   }

   free(g->spans);
   free(g);
}


