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
// Clip span management
void gDepositSpan(gFactory *f, gClipSpan *span)
{
   // Unlink span
   (span->prev->next = span->next)->prev = span->prev;

   // The bank is a single linked list
   span->prev = NULL;
   span->next = f->spanbank;
   f->spanbank = span;
}



gClipSpan *gGetClipSpan(gFactory *f)
{
   gClipSpan *ret;

   if(f->spanbank)
   {
      ret = f->spanbank;
      f->spanbank = ret->next;
   }
   else
   {
      ret = malloc(sizeof(gClipSpan));
      memset(ret, 0, sizeof(gClipSpan));
   }

   return ret;
}



void gClearClippingList(gFactory *f)
{
   int i;
   gClipSpan *head, *first, *last;

   if(f->cliplistsize != f->dest->h)
   {
      // If the dest surface hight has changed, clearing the clipping list
      // hast to be done in two passes to avoid memory leaks.
      for(i = 0; i < f->cliplistsize; i++)
      {
         head = &f->cliplist[i];

         while(head->next != head)
            gDepositSpan(f, head->next);
      }

      f->cliplistsize = f->dest->h;
      f->cliplist = realloc(f->cliplist, sizeof(gClipSpan) * f->cliplistsize);

      for(i = 0; i < f->cliplistsize; i++)
      {
         head = &f->cliplist[i];

         first = gGetClipSpan(f);
         last = gGetClipSpan(f);

         head->next = last->prev = first;
         head->prev = first->next = last;
         first->prev = last->next = head;

         first->first = -65536;
         first->last = 0;

         last->first = f->dest->w - 1;
         last->last = 65536;

         first->y = last->y = head->y = i;
      }

      return;
   }
   else
   {
      // Clear out the entire list and re-add the left and right spans.
      for(i = 0; i < f->cliplistsize; i++)
      {
         head = &f->cliplist[i];

         while(head->next != head)
            gDepositSpan(f, head->next);

         first = gGetClipSpan(f);
         last = gGetClipSpan(f);

         head->next = last->prev = first;
         head->prev = first->next = last;
         first->prev = last->next = head;

         first->first = -65536;
         first->last = 0;

         last->first = f->dest->w - 1;
         last->last = 65536;

         first->y = last->y = head->y = i;
      }

      return;
   }
}



void gFreeClipSpans(gFactory *f)
{
   int i;
   gClipSpan *head, *rover;

   if(!f->cliplist)
      return;

   for(i = 0; i < f->cliplistsize; i++)
   {
      head = &f->cliplist[i];

      while(head->next != head)
         gDepositSpan(f, head->next);
   }

   while(!!(rover = f->spanbank))
   {
      f->spanbank = f->spanbank->next;
      free(rover);
   }

   free(f->cliplist);
}



void gFillSpan(gFactory *f, int y, int x1, int x2, Uint32 c)
{
   gSurface *screen = f->dest;
   int count = x2 - x1 + 1;
   Uint8 *p;

   if(count <= 0)
      return;

   p = (Uint8 *)screen->pixels + (y * screen->pitch) + (x1 * screen->format->BytesPerPixel);

   switch(screen->format->BytesPerPixel)
   {
      case 1:
      {
         Uint8 color = (Uint8)c;

         while(count--)
         {
            *p = color;
            p++;
         }

         break;
      }
      case 2:
      {
         Uint16 *dest = (Uint16 *)p, color = (Uint16)c;

         while(count--)
         {
            *dest = color;
            dest++;
         }

         break;
      }
      case 3:
      {
         while(count--)
         {
            *(Uint32 *)p &=     0xFF000000; // Clear the actual pixel
            *(Uint32 *)p |= c & 0x00FFFFFF; // Add the new color
            p += 3;
         }

         break;
      }
      case 4:
      {
         Uint32 *dest = (Uint32*)p;

         while(count--)
         {
            *dest = c;
            dest++;
         }
      }
   }
}






void gAddClipSpan(gFactory *f, int y, int x1, int x2)
{
   gClipSpan *n, *r, *head = &f->cliplist[y];

   r = head->next;

   while(r->last < x1)
      r = r->next;

   if(r->first > x2)
   {
      // Entirely new span
      n = gGetClipSpan(f);
      
      // Link
      (n->prev = r->prev)->next = n;
      (n->next = r)->prev = n;

      n->first = x1 - 1;
      n->last = x2 + 1;
      return;
   }

   if(r->first >= x1)
   {
      // Extend the span to the left, and check for merge
      r->first = x1 - 1;

      while(r->prev != head && r->prev->last > r->first)
      {
         r->prev->last = r->last;
         r = r->prev;

         gDepositSpan(f, r->next);
      }
   }

   if(r->last <= x2)
   {
      // Extend the span to the right, and check for merge
      r->last = x2 + 1;

      while(r->next != head && r->next->first < r->last)
      {
         r->last = r->last > r->next->last ? r->last : r->next->last;

         gDepositSpan(f, r->next);
      }
   }
}





void gRenderGraphic(gFactory *f, gSkinGraphic *g, unsigned int xoffset, unsigned int yoffset, int x, int y, unsigned int w, unsigned int h, Uint32 fgcolor)
{
   int x1 = x;
   int x2 = x + w - 1;
   int y1 = y;
   int y2 = y + h - 1;
   int i, srcoff;

   int rownext, rowstart, rowend;
   gPixelRow *row, *rrover;

   if(y1 < f->cliprect.y1)
      y1 = f->cliprect.y1;
   if(y2 > f->cliprect.y2)
      y2 = f->cliprect.y2;

   for(i = y1; i <= y2; i++)
   {
      row = rrover = &g->spans[(i - y + yoffset) % g->height];

      if(row->func == gRowNOP)
         continue;

      rownext = x1 - xoffset;

      while(rownext <= x2)
      {
         rowstart = rownext + rrover->offset;
         rowend = (rowstart + rrover->len - 1) > x2 ? x2 : (rowstart + rrover->len - 1);

         rownext = rowend + 1;

         if(rowstart < x1)
         {
            srcoff = x1 - rowstart;
            rowstart = x1;
         }
         else
            srcoff = 0;

         if(rowstart > rowend || rrover->func == gRowNOP)
            goto nextrow;

         rrover->clipfunc(f, rrover, srcoff, i, rowstart, rowend, fgcolor);

         nextrow:
         rrover = rrover->next == NULL ? row : rrover->next;
      }
   }
}




void gRenderSurface(gFactory *f, gSurface *s, int x, int y, unsigned int w, unsigned int h)
{
   int x1 = x;
   int x2 = x + w - 1;
   int y1 = y;
   int y2 = y + h - 1;
   int i, next, end;
   bool unlock = false;

   gPixelRow row;
   gSurface *conv = NULL;

   if(y1 < f->cliprect.y1)
      y1 = f->cliprect.y1;
   if(y2 > f->cliprect.y2)
      y2 = f->cliprect.y2;

   if(y2 < y1)
      return;

   // Check the surface format before proceeding
   if(s->format->BytesPerPixel != f->dest->format->BytesPerPixel
      || s->format->Rmask != f->dest->format->Rmask 
      || s->format->Gmask != f->dest->format->Gmask 
      || s->format->Bmask != f->dest->format->Bmask)
   {
      s = conv = SDL_ConvertSurface(s, f->dest->format, 0);
   }

   if(SDL_MUSTLOCK(s) && !s->locked)
   {
      SDL_LockSurface(s);
      unlock = true;
   }

   row.func = gDrawRowOpaque;
   row.clipfunc = gClipRowOpaque;
   row.alphamap = NULL;
   row.offset = 0;
   row.len = s->w;
      
   for(i = y1; i <= y2; i++)
   {
      row.data = (Uint8 *)s->pixels + ((i - y) % s->h) * s->pitch;

      next = x1;

      while(next < x2)
      {
         end = next + row.len - 1;
         gClipRowOpaque(f, &row, 0, i, next, end > x2 ? x2 : end, 0);
         next += row.len;
      }
   }

   if(unlock)
      SDL_UnlockSurface(s);

   if(conv)
   {
      SDL_FreeSurface(conv);
      s = NULL;
   }
}




void gRenderBox(gFactory *f, int x, int y, unsigned int w, unsigned int h, Uint32 c)
{
   int x1 = x;
   int x2 = x + w - 1;
   int y1 = y;
   int y2 = y + h - 1;
   int i;

   gPixelRow row;

   if(y1 < f->cliprect.y1)
      y1 = f->cliprect.y1;
   if(y2 > f->cliprect.y2)
      y2 = f->cliprect.y2;

   row.func = gFillRowOpaque;
   row.clipfunc = gClipRowOpaque;
   row.data = row.alphamap = NULL;
   row.offset = 0;
   row.len = x2 - x1 + 1;

   for(i = y1; i <= y2; i++)
   {
      gClipRowOpaque(f, &row, 0, i, x1, x2, c);
   }
}





// ----------------------------------------------------------------------------
// Span rendering functions
void gClipRowOpaque(gFactory *f, gPixelRow *row, int offset, int y, int x1, int x2, Uint32 fgcolor)
{
   gClipSpan *head, *span, *next, *n;
   Uint8 *dest;
   int psize = f->dest->format->BytesPerPixel;

   head = &f->cliplist[y];
   dest = (Uint8 *)f->dest->pixels + (f->dest->pitch * y);
   

   if(x1 < f->cliprect.x1)
   {
      offset += (unsigned int)(f->cliprect.x1 - x1);
      x1 = f->cliprect.x1;
   }

   if(x2 > f->cliprect.x2)
      x2 = f->cliprect.x2;

   if(x2 < x1)
      return;

   span = head->next;
      
   // Skip ahead in the list until a span is encountered that might intersect
   // the span to be rendered.
   while(span->last < x1 && span != head)
      span = span->next;

   // assert: span->last >= x1
   // assert: if there is a previous span, it isn't touching the row
   if(span->first >= x1)
   {
      // All or part of the row is visible to the left of the clip span
      if(span->first > x2)
      {
         // Entire thing is visible, new span
         row->func(dest + (x1 * psize), row, offset, x2 - x1 + 1, psize, fgcolor);

         n = gGetClipSpan(f);
         
         (n->prev = span->prev)->next = n;
         (n->next = span)->prev = n;

         n->first = x1 - 1;
         n->last = x2 + 1;
         return;
      }
      else if(span->first == x2)
      {
         // Entire row is visible, adjoining span
         row->func(dest + (x1 * psize), row, offset, x2 - x1 + 1, psize, fgcolor);

         span->first = x1 - 1;
         return;
      }
      else // span->first < x2
      {
         // Row is partially visible on the left side
         row->func(dest + (x1 * psize), row, offset, span->first - x1 + 1, psize, fgcolor);
         span->first = x1 - 1;
      }
   }

   //left-of-span case has been checked, now check between
   while(span->last <= x2)
   {
      next = span->next;
   
      if(next->first > x2)
      {
         // Row is either touching or partially obscured by the span
         // to the left. Extend the span and we're done
         row->func(dest + span->last * psize, row, offset + span->last - x1, x2 - span->last + 1, psize, fgcolor);

         span->last = x2 + 1;
         return;
      }
      
      // Row is at least touching both
      row->func(dest + span->last * psize, row, offset + span->last - x1, next->first - span->last + 1, psize, fgcolor);

      span->last = next->last;
      gDepositSpan(f, next);
   }
   
   // All left-of-span cases have been checked... right?
}





static void gAddBlendedRow(gFactory *f, Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   gBlendedRow *blendrow;

   if(f->blendsize >= f->blendmax)
   {
      f->blendmax = f->blendsize + 10;
      f->blendlist = realloc(f->blendlist, sizeof(gBlendedRow) * f->blendmax);
   }

   blendrow = f->blendlist + f->blendsize;

   blendrow->dest = dest;
   blendrow->offset = offset;
   blendrow->len = count;
   blendrow->func = src->func;
   blendrow->row = src;
   blendrow->psize = psize;
   blendrow->fgcolor = fgcolor;

   f->blendsize++;
}




void gFlushBlendedRows(gFactory *f)
{
   int i = f->blendsize - 1;
   gBlendedRow *blendrow;

   while(i >= 0)
   {
      blendrow = f->blendlist + i;
      blendrow->func(blendrow->dest, blendrow->row, blendrow->offset, blendrow->len, blendrow->psize, blendrow->fgcolor);
      i--;
   }

   f->blendsize = 0;
}





void gClipRowBlended(gFactory *f, gPixelRow *row, int offset, int y, int x1, int x2, Uint32 fgcolor)
{
   gClipSpan *head, *span, *next;
   Uint8 *dest;
   int psize = f->dest->format->BytesPerPixel;

   head = &f->cliplist[y];
   dest = (Uint8 *)f->dest->pixels + (f->dest->pitch * y);
   
   if(x1 < f->cliprect.x1)
   {
      offset += (unsigned int)(f->cliprect.x1 - x1);
      x1 = f->cliprect.x1;
   }
   if(x2 > f->cliprect.x2)
      x2 = f->cliprect.x2;

   if(x2 < x1)
      return;

   span = head->next;
      
   // Skip ahead in the list until a span is encountered that might intersect
   // the span to be rendered.
   while(span->last < x1 && span != head)
      span = span->next;

   // assert: span->last >= x1
   // assert: if there is a previous span, it isn't touching the row
   if(span->first >= x1)
   {
      // All or part of the row is visible to the left of the clip span
      if(span->first >= x2)
      {
         // Entire thing is visible, new span
         gAddBlendedRow(f, dest + (x1 * psize), row, offset, x2 - x1 + 1, psize, fgcolor);
         return;
      }
      else // span->first < x2
      {
         // Row is partially visible on the left side
         gAddBlendedRow(f, dest + (x1 * psize), row, offset, span->first - x1 + 1, psize, fgcolor);
      }
   }

   //left-of-span case has been checked, now check between
   while(span->last <= x2)
   {
      next = span->next;
   
      if(next->first > x2)
      {
         // Row is either touching or partially obscured by the span
         // to the left.
         gAddBlendedRow(f, dest + span->last * psize, row, offset + span->last - x1, x2 - span->last + 1, psize, fgcolor);
         return;
      }
      
      // Row is at least touching both
      gAddBlendedRow(f, dest + span->last * psize, row, offset + span->last - x1, next->first - span->last + 1, psize, fgcolor);

      // When marking opaque regions of the screen, gDepositSpan makes this unnecessary
      // but here we don't do that...
      span = span->next;
   }
   
   // All left-of-span cases have been checked... right?
}




void gRowNOP(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{}



void gFillRowOpaque(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   Uint32 c = fgcolor;

   if(count <= 0)
      return;


   switch(psize)
   {
      case 1:
      {
         Uint8 color = (Uint8)c;

         while(count--)
         {
            *dest = color;
            dest++;
         }

         break;
      }
      case 2:
      {
         Uint16 *pdest = (Uint16 *)dest, color = (Uint16)c;

         while(count--)
         {
            *pdest = color;
            pdest++;
         }

         break;
      }
      case 3:
      {
         while(count--)
         {
            *(Uint32 *)dest &=     0xFF000000; // Clear the actual pixel
            *(Uint32 *)dest |= c & 0x00FFFFFF; // Add the new color
            dest += 3;
         }

         break;
      }
      case 4:
      {
         Uint32 *pdest = (Uint32*)dest;

         while(count--)
         {
            *pdest = c;
            pdest++;
         }
      }
   }
}




void gFillRowAlpha_16(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   Uint16 color = (Uint16)fgcolor, *d;
   Uint8  *alphamap;
   Uint32 temp;

   if(psize != 2)
      return; // ERROR?

   d = (Uint16 *)dest;
   alphamap = src->alphamap + offset * 2;

   while(count--)
   {
      temp = (((*d & 0xF800) * alphamap[1] + (color & 0xF800) * alphamap[0]) >> 8) & 0xF800;
      temp |= (((*d & 0x7E0) * alphamap[1] + (color & 0x7E0) * alphamap[0]) >> 8) & 0x7E0;
      temp |= (((*d & 0x01F) * alphamap[1] + (color & 0x01F) * alphamap[0]) >> 8) & 0x01F;
      *d = (Uint16)temp;

      d++;
      alphamap += 2;
   }
}



void gFillRowAlpha_24(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   Uint32 c1 = fgcolor & 0xFF00FF;
   Uint32 c2 = fgcolor & 0x00FF00;
   Uint8 *alphamap;
   Uint32 temp;
   Uint32 *d;

   if(psize != 3)
      return; // ERROR?

   alphamap = src->alphamap + offset * 2;

   while(count--)
   {
      d = (Uint32 *)dest;
      temp = (((*d & 0xFF00FF) * alphamap[1] + c1 * alphamap[0]) >> 8) & 0xFF00FF;
      temp |= (((*d & 0x00FF00) * alphamap[1] + c2 * alphamap[0]) >> 8) & 0x00FF00;
      *d = (*d & 0xFF000000) | temp;

      dest += 3;
      alphamap += 2;
   }
}


void gFillRowAlpha_32(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   Uint32 c1 = fgcolor & 0xFF00FF;
   Uint32 c2 = fgcolor & 0x00FF00;
   Uint32 *d = (Uint32 *)dest;
   Uint8  *alphamap;
   Uint32 temp;

   if(psize != 4)
      return; // ERROR?

   alphamap = src->alphamap + offset * 2;

   while(count--)
   {
      temp = (((*d & 0xFF00FF) * alphamap[1] + c1 * alphamap[0]) >> 8) & 0xFF00FF;
      temp |= (((*d & 0x00FF00) * alphamap[1] + c2 * alphamap[0]) >> 8) & 0x00FF00;
      *d = temp;

      d++;
      alphamap += 2;
   }
}






void gDrawRowOpaque(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   memcpy(dest, src->data + offset * psize, count * psize);
}



void gDrawRowAlpha_16(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   Uint16 *srcpixels, *d;
   Uint8  *alphamap;
   Uint32 temp;

   if(psize != 2)
      return; // ERROR?

   srcpixels = ((Uint16 *)src->data) + offset;
   d = (Uint16 *)dest;
   alphamap = src->alphamap + offset * 2;

   while(count--)
   {
      temp = (((*d & 0xF800) * alphamap[1] + (*srcpixels & 0xF800) * alphamap[0]) >> 8) & 0xF800;
      temp |= (((*d & 0x7E0) * alphamap[1] + (*srcpixels & 0x7E0) * alphamap[0]) >> 8) & 0x7E0;
      temp |= (((*d & 0x01F) * alphamap[1] + (*srcpixels & 0x01F) * alphamap[0]) >> 8) & 0x01F;
      *d = (Uint16)temp;

      d++;
      srcpixels++;
      alphamap += 2;
   }
}



void gDrawRowAlpha_24(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   Uint8 *srcpixels;
   Uint8 *alphamap;
   Uint32 temp;
   Uint32 *d;

   if(psize != 3)
      return; // ERROR?

   srcpixels = src->data + offset * 3;
   alphamap = src->alphamap + offset * 2;

   while(count--)
   {
      d = (Uint32 *)dest;
      temp = (((*d & 0xFF00FF) * alphamap[1] + (*(Uint32 *)srcpixels & 0xFF00FF) * alphamap[0]) >> 8) & 0xFF00FF;
      temp |= (((*d & 0x00FF00) * alphamap[1] + (*(Uint32 *)srcpixels & 0x00FF00) * alphamap[0]) >> 8) & 0x00FF00;
      *d = (*d & 0xFF000000) | temp;

      dest += 3;
      srcpixels += 3;
      alphamap += 2;
   }
}


void gDrawRowAlpha_32(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   Uint32 *srcpixels, *d = (Uint32 *)dest;
   Uint8  *alphamap;
   Uint32 temp;

   if(psize != 4)
      return; // ERROR?

   srcpixels = ((Uint32 *)src->data) + offset;
   alphamap = src->alphamap + offset * 2;

   while(count--)
   {
      temp = (((*d & 0xFF00FF) * alphamap[1] + (*srcpixels & 0xFF00FF) * alphamap[0]) >> 8) & 0xFF00FF;
      temp |= (((*d & 0x00FF00) * alphamap[1] + (*srcpixels & 0x00FF00) * alphamap[0]) >> 8) & 0x00FF00;
      *d = temp;

      d++;
      srcpixels++;
      alphamap += 2;
   }
}




void gDrawRowAdditive_16(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   Uint16 *s, *d = (Uint16 *)dest;
   Uint32 c, px;

   if(psize != 2)
      return; // ERROR?

   s = ((Uint16 *)src->data) + offset;

   while(count--)
   {
      c = (*d & 0x1F) + (*s & 0x1F);
      px = c > 0x1F ? 0x1F : c;

      c = (*d & 0x7E0) + (*s & 0x7E0);
      px |= c > 0x7E0 ? 0x7E0 : c;

      c = (*d & 0xF800) + (*s & 0xF800);
      px |= c > 0xF800 ? 0xF800 : c;

      *d = (Uint16)px;
      s++;
      d++;
   }
}

void gDrawRowAdditive_24(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   Uint8 *s, *d = dest;
   Uint32 c;

   if(psize != 3)
      return; // ERROR?

   s = src->data + (offset * 3);

   while(count--)
   {
      c = d[0] + s[0];
      d[0] = c > 255 ? 255 : (Uint8)c;
      c = d[1] + s[1];
      d[1] = c > 255 ? 255 : (Uint8)c;
      c = d[2] + s[2];
      d[2] = c > 255 ? 255 : (Uint8)c;

      s += 3;
      d += 3;
   }
}

void gDrawRowAdditive_32(Uint8 *dest, gPixelRow *src, unsigned int offset, unsigned int count, unsigned int psize, Uint32 fgcolor)
{
   Uint32 *s, *d = (Uint32 *)dest;
   Uint32 px, c;

   if(psize != 4)
      return; // ERROR?

   s = ((Uint32 *)src->data) + offset;

   while(count--)
   {
      c = (*d & 0x00ff00ff) + (*s & 0x00ff00ff);
      px = (c & 0x00ff00ff) | ((c & 0x100) - ((c & 0x100) >> 8)) | ((c & 0x01000000) - ((c & 0x01000000) >> 8));
      c = ((*d & 0xff00ff00) >> 8) + ((*s & 0xff00ff00) >> 8);
      px |= ((c & 0x00ff00ff) | ((c & 0x100) - ((c & 0x100) >> 8)) | ((c & 0x01000000) - ((c & 0x01000000) >> 8))) << 8;

      *d = px;
      d++;
      s++;
   }
}

