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


gSkinTemplate *gCreateSkinTemplate()
{
   gSkinTemplate *ret = malloc(sizeof(gSkinTemplate));

   ret->clist = gNewList(gFreeTempCursor);
   ret->ilist = gNewList(gFreeTempIcon);
   ret->plist = gNewList(gFreeTempProcess);
   ret->images = gNewList(gFreeTempImage);
   ret->attributes = gNewList(gFreeAttribute);

   return ret;
}


// New functions for attribute lists.
void gFreeAttribute(void *object)
{
   gSkinAttribute *att = (gSkinAttribute *) object;

   if(!att)
      return;

   if(att->name)
      free(att->name);

   if(att->data)
   {
      if(att->type == attFontSpec)
         gFreeFontSpec((gFontSpec *)att->data);
      else
         free(att->data);
   }

   free(att);
}




void gFreeTempImage(void *object)
{
   gTempImage *img = (gTempImage *)object;

   if(img->name)
      free(img->name);
   if(img->surface)
      SDL_FreeSurface(img->surface);

   free(img);
}



void gFreeTempBit(gTempBit *bit)
{
   free(bit);
}


void gFreeTempSpan(gTempSpan *span)
{
   if(span->startbit)
      gFreeTempBit(span->startbit);
   if(span->endbit)
      gFreeTempBit(span->endbit);
   if(span->centerbit)
      gFreeTempBit(span->centerbit);
   free(span);
}



void gFreeTempProcess(void *object)
{
   gTempProcess *p = (gTempProcess *)object;

  
   if(p->name)
      free(p->name);

   if(p->attributes)
      gFreeList(p->attributes);

   if(p->startspan)
      gFreeTempSpan(p->startspan);
   if(p->midspan)
      gFreeTempSpan(p->midspan);
   if(p->endspan)
      gFreeTempSpan(p->endspan);

   free(p);
}



void gFreeTempIcon(void *object)
{
   gTempIcon *i = (gTempIcon *)object;

   if(i->name)
      free(i->name);

   if(i->imgname)
      free(i->imgname);

   free(i);
}




void gFreeSkinCursor(gSkinCursor *object)
{
   gSkinCursor *c = (gSkinCursor *)object;

   if(c->name)
      free(c->name);

   if(c->cursor)
      SDL_FreeCursor(c->cursor);

   free(c);
}



void gFreeTempCursor(void *object)
{
   gTempCursor *c = (gTempCursor *)object;

   if(c->name)
      free(c->name);

   if(c->data)
      free(c->data);

   if(c->mask)
      free(c->mask);

   free(c);
}




void gFreeSkinTemplate(gSkinTemplate *skin)
{
   gFreeList(skin->plist);
   gFreeList(skin->ilist);
   gFreeList(skin->clist);
   gFreeList(skin->images);
   gFreeList(skin->attributes);

   free(skin);
}



gTempProcess *gFindTempProcess(const char *name, gSkinTemplate *skin)
{
   // SoM: skin templates should use hash tables because for processing large 
   // skins, this could become time consuming and costly.
   unsigned i;

   for(i = 0; i < skin->plist->size; i++)
   {
      gTempProcess *proc = (gTempProcess *)gGetListItem(skin->plist, i);
      if(!stricmp(proc->name, name))
         return proc;
   }

   return NULL;
}



static void copyBit(gTempBit **dest, gTempBit *src)
{
   gTempBit *bit;

   if(!src)
   {
      // No span? Do nothing.
      *dest = NULL;
      return;
   }

   bit = *dest = malloc(sizeof(gTempBit));
   memcpy(bit, src, sizeof(gTempBit));
}

static void copySpan(gTempSpan **dest, gTempSpan *src)
{
   gTempSpan *span;

   if(!src)
   {
      // No span? Do nothing.
      *dest = NULL;
      return;
   }

   span = *dest = gCreateTempSpan();
   memcpy(span, src, sizeof(gTempSpan));

   copyBit(&span->startbit, src->startbit);
   copyBit(&span->centerbit, src->centerbit);
   copyBit(&span->endbit, src->endbit);
}



gTempProcess *gAddProcess(const char *name, gTempProcess *parent, gSkinTemplate *skin)
{
   gTempProcess *ret;
   unsigned     i;
   bool         newproc = false;

   if(!(ret = gFindTempProcess(name, skin)))
   {
      ret = malloc(sizeof(gTempProcess));
      memset(ret, 0, sizeof(*ret));
      gAppendListItem(skin->plist, ret);
      ret->name = strdup(name);
      ret->attributes = gNewList(gFreeAttribute);
      ret->orientation = gSpanRows;
      newproc = true;
   }


   if(parent)
   {
      if(!newproc)
         gSetError("Warning: process '%s' can not extend process '%s' after it has already been created.\n", name, parent->name);
      else
      {
         ret->orientation = parent->orientation;

         copySpan(&ret->startspan, parent->startspan);
         copySpan(&ret->midspan, parent->midspan);
         copySpan(&ret->endspan, parent->endspan);

         for(i = 0; i < parent->attributes->size; i++)
         {
            gAppendListItem(ret->attributes, 
               gCopyAttribute((gSkinAttribute *)parent->attributes->list[i]));
         }
      }
   }


   return ret;
}



gTempIcon *gAddIcon(const char *name, gSkinTemplate *skin)
{
   gTempIcon *ret;
   unsigned  i;

   // Check the skin first to make sure the icon doesn't already exist.
   for(i = 0; i < skin->ilist->size; i++)
   {
      ret = (gTempIcon *)gGetListItem(skin->ilist, i);
      if(!stricmp(name, ret->name))
      {
         if(ret->imgname)
         {
            free(ret->imgname);
            ret->imgname = NULL;
         }

         return ret;
      }
   }

   ret = malloc(sizeof(gTempIcon));
   ret->name = strdup(name);
   ret->imgname = NULL;

   gAppendListItem(skin->ilist, ret);

   return ret;
}



gTempCursor *gAddCursor(const char *name, gSkinTemplate *skin)
{
   gTempCursor *ret = malloc(sizeof(gTempCursor));

   ret->name = strdup(name);
   ret->data = ret->mask = NULL;

   gAppendListItem(skin->clist, ret);

   return ret;
}






gSkinAttribute *gNewIntAttribute(const char *name, int value)
{
   int *data = malloc(sizeof(int));
   *data = value;
   return gNewAttribute(name, attInteger, data);
}


gSkinAttribute *gNewStringAttribute(const char *name, const char *value)
{
   char *data = malloc(sizeof(char) * (strlen(value) + 1));
   memcpy(data, value, strlen(value) + 1);
   return gNewAttribute(name, attString, data);
}


gSkinAttribute *gNewFloatAttribute(const char *name, float value)
{
   float *data = malloc(sizeof(float));
   *data = value;
   return gNewAttribute(name, attFloat, data);
}





SDL_Color *newColor(SDL_Color *color, int r, int g, int b)
{
   SDL_Color *data;

   if(!color)
      data = malloc(sizeof(SDL_Color));
   else
      data = color;
   
   if(r < 0)
      data->r = 0;
   else if(r >= 255)
      data->r = 255;
   else
      data->r = (Uint8)r;

   if(g < 0)
      data->g = 0;
   else if(g >= 255)
      data->g = 255;
   else
      data->g = (Uint8)g;


   if(b < 0)
      data->b = 0;
   else if(b >= 255)
      data->b = 255;
   else
      data->b = (Uint8)b;

   data->unused = 0;
   return data;
}



gSkinAttribute *gNewColorAttribute(const char *name, int r, int g, int b)
{
   SDL_Color *data = newColor(NULL, r, g, b);
   return gNewAttribute(name, attColor, data);
}


gSkinAttribute *gNewFontAttribute(const char *name, const char *fontname, int ptsize, int style, int r, int g, int b)
{
   gFontSpec   *data;
   SDL_Color   c;

   newColor(&c, r, g, b);
   data = gNewFontSpec(fontname, ptsize, style, c);

   return gNewAttribute(name, attFontSpec, data);
}


gSkinAttribute *gNewAttribute(const char *name, gAttType_e type, void *data)
{
   gSkinAttribute *ret = (gSkinAttribute *)malloc(sizeof(gSkinAttribute));
   memset(ret, 0, sizeof(*ret));

   ret->name = strdup(name);
   ret->type = type;
   ret->data = data;

   return ret;
}


gSkinAttribute *gCopyAttribute(gSkinAttribute *src)
{
   gSkinAttribute *ret = (gSkinAttribute *)malloc(sizeof(gSkinAttribute));
   gFontSpec      *data, *sdata;

   memset(ret, 0, sizeof(*ret));

   ret->name = strdup(src->name);
   ret->type = src->type;

   switch(src->type)
   {
      case attInteger:
         ret->data = malloc(sizeof(int));
         memcpy(ret->data, src->data, sizeof(int));
         break;
      case attFloat:
         ret->data = malloc(sizeof(float));
         memcpy(ret->data, src->data, sizeof(float));
         break;
      case attColor:
         ret->data = malloc(sizeof(SDL_Color));
         memcpy(ret->data, src->data, sizeof(SDL_Color));
         break;
      case attString:
         ret->data = strdup((char *)src->data);
         break;
      case attFontSpec:
         data = (gFontSpec *)(ret->data = malloc(sizeof(gFontSpec)));
         sdata = (gFontSpec *)src->data;

         data->name = strdup(sdata->name);
         data->ptsize = sdata->ptsize;
         data->style = sdata->style;
         data->fgcolor = sdata->fgcolor;
   }

   return ret;
}




gTempSpan *gCreateTempSpan()
{
   gTempSpan *ret = malloc(sizeof(gTempSpan));

   memset(ret, 0, sizeof(gTempSpan));

   return ret;
}




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
)
{
   int pitch = image->pitch;
   Uint32  fg, bg, tr, xor;
   int i = -1;
   Uint8 *dat, *mask, *p;
   int datasize = (w / 8) * h;

   fg = SDL_MapRGB(image->format, 255, 255, 255);
   bg = SDL_MapRGB(image->format,   0,   0,   0);
   tr = SDL_MapRGB(image->format, transcolor->r, transcolor->g, transcolor->b);
   xor = SDL_MapRGB(image->format, xorcolor->r, xorcolor->g, xorcolor->b);

   if(w % 8)
   {
      gSetError("gMakeCursor: cursor %s has invalid width. Width must be divisible by 8.", cursor->name);
      return false;
   }

   if(x + w > (unsigned int)image->w)
      w = image->w - x;

   if(y + h > (unsigned int)image->h)
      h = image->h - y;

   if(x >= (unsigned int)image->w || y >= (unsigned int)image->h || !w || !h)
      return false;

   cursor->data = dat = malloc(sizeof(Uint8) * datasize);
   cursor->mask = mask = malloc(sizeof(Uint8) * datasize);

   cursor->width = w;
   cursor->height = h;
   cursor->hotx = hotx;
   cursor->hoty = hoty;

   memset(dat, 0, sizeof(Uint8) * datasize);
   memset(mask, 0, sizeof(Uint8) * datasize);

   p = (Uint8 *)image->pixels + (y * image->pitch) + (x * image->format->BytesPerPixel);

   if(image->format->BytesPerPixel == 1)
   {
      for(y = 0; y < h; y++)
      {
         for(x = 0; x < w; x++)
         {
            int modx = x % 8;
            
            if(!modx)
               i++;

            modx = 7 - modx;

            if(p[x] == fg || p[x] == xor)
               dat[i] |= (1 << modx);

            if(p[x] == bg || p[x] == fg)
               mask[i] |= (1 << modx);
         }

         p += image->pitch;
      }

      return true;
   }   
   else if(image->format->BytesPerPixel == 2)
   {
      Uint16 px;
      for(y = 0; y < h; y++)
      {
         for(x = 0; x < w; x++)
         {
            int modx = x % 8;
            
            if(!modx)
               i++;

            modx = 7 - modx;
            px = p[x * 2];

            if(px == fg || px == xor)
               dat[i] |= (1 << modx);

            if(px == bg || px == fg)
               mask[i] |= (1 << modx);
         }

         p += image->pitch;
      }

      return true;
   }
   else if(image->format->BytesPerPixel == 3)
   {
      Uint32 px;
      for(y = 0; y < h; y++)
      {
         for(x = 0; x < w; x++)
         {
            int modx = x % 8;
            
            if(!modx)
               i++;

            modx = 7 - modx;

            px = *(Uint32 *)(p + x * 3) & 0xFFFFFF;

            if(px == bg || px == xor)
               dat[i] |= (1 << modx);

            if(px == bg || px == fg)
               mask[i] |= (1 << modx);
         }

         p += image->pitch;
      }

      return true;
   }
   else if(image->format->BytesPerPixel == 4)
   {
      Uint32 px;
      for(y = 0; y < h; y++)
      {
         for(x = 0; x < w; x++)
         {
            int modx = x % 8;
            
            if(!modx)
               i++;

            modx = 7 - modx;
            px = p[x * 4];

            if(px == fg || px == xor)
               dat[i] |= (1 << modx);

            if(px == bg || px == fg)
               mask[i] |= (1 << modx);
         }

         p += image->pitch;
      }

      return true;
   }


   return false;
}




gFontSpec *gNewFontSpec(const char *filename, int ptsize, int style, SDL_Color fgcolor)
{
   gFontSpec *ret = malloc(sizeof(gFontSpec));

   ret->name = strdup(filename);
   ret->ptsize = ptsize;
   ret->style = style;
   memcpy(&ret->fgcolor, &fgcolor, sizeof(SDL_Color));

   return ret;
}


void gFreeFontSpec(gFontSpec *fs)
{
   if(fs->name)
      free(fs->name);

   free(fs);
}

