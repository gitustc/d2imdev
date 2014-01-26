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
// gPain - Resizable surfaces which are mapped to gSkinGraphics automatically


gPane *gCreateNewPane(gFactory *f, int width, int height)
{
   gPane *ret;
   SDL_PixelFormat *fmt;
   SDL_Surface *surface;
   gPixelRow *row;
   int y;

   if(!f || width <= 0 || height <= 0)
      return NULL;

   fmt = f->dest->format;

   surface = 
      SDL_CreateRGBSurface(0, width, height, fmt->BitsPerPixel, fmt->Rmask, 
                           fmt->Gmask, fmt->Bmask, fmt->Amask);

   if(!surface)
   {
      gSetError("gCreateNewPane: failed to allocate surface.\n");
      return NULL;
   }

   ret = malloc(sizeof(gPane));
   ret->surface = surface;
   ret->width = width;
   ret->height = ret->maxheight = height;
   ret->pitch = surface->pitch;
   ret->graphic = malloc(sizeof(gSkinGraphic));
   
   ret->graphic->width = width;
   ret->graphic->height = height;
   ret->graphic->spans = malloc(sizeof(gPixelRow) * height);
   memset(ret->graphic->spans, 0, sizeof(gPixelRow) * height);

   for(y = 0; y < ret->height; y++)
   {
      row = &ret->graphic->spans[y];

      row->func = gDrawRowOpaque;   
      row->clipfunc = gClipRowOpaque;
      row->len = width;
      row->data = (Uint8 *)surface->pixels + (y * surface->pitch);
   }

   return ret;
}


void gResizePane(gPane *pane, int width, int height)
{
   int maxw, maxh, i;
   gPixelRow *row;

   if(!pane || width <= 0 || height <= 0)
      return;

   if(width > pane->surface->w || height > pane->surface->h)
   {
      SDL_PixelFormat *fmt = pane->surface->format;
      gSurface *news;

      maxw = width > pane->surface->w ? width : pane->surface->w;
      maxh = height > pane->surface->h ? height : pane->surface->h;

      news = 
         SDL_CreateRGBSurface(0, maxw, maxh, fmt->BitsPerPixel, fmt->Rmask, 
                              fmt->Gmask, fmt->Bmask, fmt->Amask);

      SDL_BlitSurface(pane->surface, NULL, news, NULL);
      SDL_FreeSurface(pane->surface);

      pane->pixels = news->pixels;
      pane->pitch = news->pitch;

      pane->surface = news;

      if(maxh > pane->maxheight)
      {
         pane->graphic->spans = realloc(pane->graphic->spans, sizeof(gPixelRow) * maxh);
         memset(pane->graphic->spans, 0, sizeof(gPixelRow) * maxh);
         pane->maxheight = maxh;
      }
   }

   pane->width = pane->graphic->width = width;
   pane->height = pane->graphic->height = height;

   for(i = 0; i < height; i++)
   {
      row = &pane->graphic->spans[i];
      row->func = gDrawRowOpaque;   
      row->clipfunc = gClipRowOpaque;
      row->len = width;
      row->data = (Uint8 *)pane->pixels + (i * pane->pitch);
   }
}




void gFreePane(gPane *pane)
{
   if(!pane)
      return;

   if(pane->surface)
      SDL_FreeSurface(pane->surface);

   free(pane->graphic->spans);
   free(pane->graphic);
   free(pane);
}

