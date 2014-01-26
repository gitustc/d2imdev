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
// gFactory 
static void copyDefines(gList *dest, gList *src)
{
   gSkinAttribute   *a;
   unsigned int     i;

   for(i = 0; i < src->size; i++)
   {
      a = (gSkinAttribute *)gGetListItem(src, i);
      gAppendListItem(dest, gCopyAttribute(a));
   }
}



static void copyCursors(gList *destlist, gList *srclist)
{
   gTempCursor    *r;
   gSkinCursor    *n;
   SDL_Cursor     *cursor;
   unsigned int   i;


   for(i = 0; i < srclist->size; i++)
   {
      r = (gTempCursor *)gGetListItem(srclist, i);

      cursor = SDL_CreateCursor(r->data, r->mask, r->width, r->height, r->hotx, r->hoty);

      if(!cursor)
         continue; // ERROR, MAYBE?

      n = malloc(sizeof(gSkinCursor));
      n->name = strdup(r->name);
      n->cursor = cursor;

      gAppendListItem(destlist, n);
   }
}



static void formatSpan(gFactory *f, gSkinSpan *nspan, gTempProcess *proc, gTempSpan *ospan, gSurface *img)
{
   nspan->thickness = ospan->thickness;
   nspan->flags = ospan->flags;
   nspan->centerflag = ospan->centerflag;

   nspan->ucolor = SDL_MapRGB(f->dest->format, ospan->centercolor.r, 
                              ospan->centercolor.g, ospan->centercolor.b);

   nspan->startbit = 
      ospan->startbit ? 
      gMakeBit(f, img, &ospan->startbit->rect) :
      NULL;

   nspan->centerbit = 
      ospan->centerbit ? 
      gMakeBit(f, img, &ospan->centerbit->rect) :
      NULL;

   nspan->endbit = 
      ospan->endbit ? 
      gMakeBit(f, img, &ospan->endbit->rect) :
      NULL;

   nspan->spanfunc = gDrawSpanNOP;

   // Determine span function
   if(proc->orientation == gSpanRows)
   {
      if(nspan->startbit && nspan->endbit)
         nspan->spanfunc = gDrawRowComplete;
      else if(nspan->startbit && nspan->centerflag)
         nspan->spanfunc = gDrawRowIncompleteA;
      else if(nspan->endbit && nspan->centerflag)
         nspan->spanfunc = gDrawRowIncompleteB;
      else
         nspan->spanfunc = gDrawRow1Bit;
   }
   else
   {
      if(nspan->startbit && nspan->endbit)
         nspan->spanfunc = gDrawColumnComplete;
      else if(nspan->startbit && nspan->centerflag)
         nspan->spanfunc = gDrawColumnIncompleteA;
      else if(nspan->endbit && nspan->centerflag)
         nspan->spanfunc = gDrawColumnIncompleteB;
      else
         nspan->spanfunc = gDrawColumn1Bit;
   }
}



static void formatProcess(gFactory *f, gStringHash *images, gTempProcess *proc)
{
   gSkinProcess *newp;
   gSurface *img;

   img = shFind(images, proc->imgname);

   if(!img)
      return;

   newp = malloc(sizeof(gSkinProcess));
   gAppendListItem(f->plist, newp);

   newp->name = strdup(proc->name);
   newp->startspan = newp->midspan = newp->endspan = NULL;

   if(proc->startspan)
   {
      newp->startspan = malloc(sizeof(gSkinSpan));
      formatSpan(f, newp->startspan, proc, proc->startspan, img);
   }

   if(proc->midspan)
   {
      newp->midspan = malloc(sizeof(gSkinSpan));
      formatSpan(f, newp->midspan, proc, proc->midspan, img);
   }

   if(proc->endspan)
   {
      newp->endspan = malloc(sizeof(gSkinSpan));
      formatSpan(f, newp->endspan, proc, proc->endspan, img);
   }


   newp->attributes = gNewList(gFreeAttribute);
   copyDefines(newp->attributes, proc->attributes);


   if(newp->startspan && newp->midspan && newp->endspan)
   {
      newp->func = (proc->orientation == gSpanColumns) ? 
                      gDrawProcess3Columns :
                      gDrawProcess3Rows;
   }
   else if(newp->startspan && newp->midspan)
   {
      newp->func = proc->orientation == gSpanColumns ? 
                      gDrawProcess2ColumnsA :
                      gDrawProcess2RowsA;
   }
   else if(newp->endspan && newp->midspan)
   {
      newp->func = proc->orientation == gSpanColumns ? 
                      gDrawProcess2ColumnsB :
                      gDrawProcess2RowsB;
   }
   else if(newp->startspan && newp->endspan)
   {
      newp->func = proc->orientation == gSpanColumns ? 
                      gDrawProcess2ColumnsC :
                      gDrawProcess2RowsC;
   }
   else
   {
      newp->func = proc->orientation == gSpanColumns ? 
                      gDrawProcess1Column :
                      gDrawProcess1Row;
   }
}



static void formatIcon(gFactory *f, gStringHash *images, gTempIcon *icon)
{
   gSkinIcon *newi;
   gSurface  *img;

   img = shFind(images, icon->imgname);

   if(!img)
      return;

   newi = malloc(sizeof(gSkinIcon));
   gAppendListItem(f->ilist, newi);

   newi->name = strdup(icon->name);

   newi->graphic = gMakeBit(f, img, &icon->rect);

   return;
}





static void gInitClippingArray(gFactory *f)
{
   int i;

   // Setup clipping array
   f->spanbank = NULL;
   f->cliplistsize = f->dest->h;

   f->cliplist = malloc(sizeof(gClipSpan) * f->cliplistsize);
   for(i = 0; i < f->dest->h; i++)
   {
      gClipSpan *first, *last;

      first = malloc(sizeof(gClipSpan));
      last = malloc(sizeof(gClipSpan));

      f->cliplist[i].next = last->prev = first;
      f->cliplist[i].prev = first->next = last;
      last->next = first->prev = &f->cliplist[i];

      first->first = -65536;
      first->last = 0;

      last->first = f->dest->w - 1;
      last->last = 65536;

      first->y = last->y = f->cliplist[i].y = i;
   }
}



static void gFreeCursor(void *object)
{
   gSkinCursor *c = (gSkinCursor *)object;

   if(c->name)
      free(c->name);

   if(c->cursor)
      SDL_FreeCursor(c->cursor);

   free(c);
}


static void gFreeSkinSpan(gSkinSpan *span)
{
   if(span->startbit)
      gDestroyGraphic(span->startbit);

   if(span->centerbit)
      gDestroyGraphic(span->centerbit);

   if(span->endbit)
      gDestroyGraphic(span->endbit);

   free(span);
}


static void gFreeProcess(void *object)
{
   gSkinProcess *p = (gSkinProcess *)object;

   if(p->name)
      free(p->name);

   if(p->startspan)
      gFreeSkinSpan(p->startspan);

   if(p->midspan)
      gFreeSkinSpan(p->midspan);

   if(p->endspan)
      gFreeSkinSpan(p->endspan);

   free(p);
}


static void gFreeSkinIcon(void *object)
{
   gSkinIcon *i = (gSkinIcon *)object;

   if(i->name)
      free(i->name);

   if(i->graphic)
      gDestroyGraphic(i->graphic);

   free(i);
}




gFactory *gCreateFactory(gSkinTemplate *skin, gSurface *screen)
{
   gStringHash *images;
   gFactory *ret;
   unsigned int i;

   if(!skin || !screen)
      return NULL;
  
   ret = malloc(sizeof(gFactory));

   ret->plist = gNewList(gFreeProcess);
   ret->ilist = gNewList(gFreeSkinIcon);
   ret->fcache = gNewList(gFreeFont);
   ret->clist = gNewList(gFreeCursor);

   ret->clipstack = NULL;
   ret->stacklevel = 0;
   ret->stacksize = 0;

   ret->dest = screen;
   ret->screenrect.x1 = ret->screenrect.y1 = 0;
   ret->screenrect.x2 = screen->w - 1;
   ret->screenrect.y2 = screen->h - 1;

   ret->cliprect.x1 = screen->clip_rect.x;
   ret->cliprect.y1 = screen->clip_rect.y;
   ret->cliprect.x2 = ret->cliprect.x1 + screen->clip_rect.w - 1;
   ret->cliprect.y2 = ret->cliprect.y1 + screen->clip_rect.h - 1;

   ret->updateflag = false;

   // Create the image hash table from the existing list.
   images = shNew(71, NULL, true);

   for(i = 0; i < skin->images->size; i++)
   {
      gTempImage *img = (gTempImage *)gGetListItem(skin->images, i);
      shStore(images, img->name, img->surface);
   }

   for(i = 0; i < skin->plist->size; i++)
      formatProcess(ret, images, (gTempProcess *)gGetListItem(skin->plist, i));

   for(i = 0; i < skin->ilist->size; i++)
      formatIcon(ret, images, (gTempIcon *)gGetListItem(skin->ilist, i));

   copyCursors(ret->clist, skin->clist);

   ret->attributes = gNewList(gFreeAttribute);
   copyDefines(ret->attributes, skin->attributes);


   // Setup cursor stuff
   ret->defaultcursor = gFindCursor(ret, "default");
   
   if(ret->defaultcursor)
      SDL_SetCursor(ret->defaultcursor->cursor);
   else
      SDL_SetCursor(NULL);

   ret->activecursor = ret->defaultcursor;

   gInitClippingArray(ret);

   ret->blendsize = ret->blendmax = 0;
   ret->blendlist = NULL;

   // free image hash
   shFree(images);

   return ret;
}


void gFreeFactory(gFactory *f)
{
   gFreeList(f->plist);
   gFreeList(f->clist);
   gFreeList(f->ilist);
   gFreeList(f->fcache);

   if(f->clipstack)
      free(f->clipstack);

   gFreeList(f->attributes);

   gFreeClipSpans(f);

   if(f->blendlist)
      free(f->blendlist);

   free(f);
}





// ----------------------------------------------------------------------------
// Attribute searching
gSkinAttribute  *gFindFactoryAttribute(gFactory *f, const char *name, gAttType_e type)
{
   unsigned int i;

   if(!name || !f)
      return NULL; // ERROR?

   for(i = 0; i < f->attributes->size; i++)
   {
      gSkinAttribute *r = (gSkinAttribute *)gGetListItem(f->attributes, i);

      if(r->type != type || strcmp(r->name, name))
         continue;

      return r;
   }

   return NULL;
}


gSkinAttribute *gFindAttribute(gFactory *f, gSkinProcess *p, const char *name, gAttType_e type)
{
   gSkinAttribute *r;
   unsigned int   i;

   if(!name)
      return NULL; // ERROR?

   if(!p && f)
      return gFindFactoryAttribute(f, name, type);

   for(i = 0; i < p->attributes->size; i++)
   {
      r = (gSkinAttribute *)gGetListItem(p->attributes, i);

      if(r->type != type || strcmp(r->name, name))
         continue;

      return r;
   }

   if(f)
      return gFindFactoryAttribute(f, name, type);
   else
      return NULL;
}





// ----------------------------------------------------------------------------
// Custom cursor support
gSkinCursor *gFindCursor(gFactory *f, const char *name)
{
   gSkinCursor *r;
   unsigned int i;

   for(i = 0; i < f->clist->size; i++)
   {
      r = (gSkinCursor *)gGetListItem(f->clist, i);

      if(!strcmp(r->name, name))
         return r;
   }

   return NULL;
}




void gSetCursor(gFactory *f, gSkinCursor *cursor)
{
   if(!cursor)
      cursor = f->defaultcursor;

   if(f->activecursor == cursor)
      return;

   if(!cursor)
      SDL_SetCursor(NULL);
   else
      SDL_SetCursor(cursor->cursor);

   f->activecursor = cursor;
}




// ----------------------------------------------------------------------------
// Updates, and clipping stack
void gAddUpdateRect(gFactory *f, gScreenRect *r)
{
   if(!r)
   {
      f->updateflag = true;
      f->updaterect.x1 = f->updaterect.y1 = 0;
      f->updaterect.x2 = f->dest->w - 1;
      f->updaterect.y2 = f->dest->h - 1;
      return;
   }

   if(r->x1 > r->x2 || r->y1 > r->y2)
      return;

   if(!f->updateflag)
   {
      f->updateflag = true;
      memcpy(&f->updaterect, r, sizeof(gScreenRect));
   }
   else
   {
      if(r->x1 < f->updaterect.x1)
         f->updaterect.x1 = r->x1;
      if(r->x2 > f->updaterect.x2)
         f->updaterect.x2 = r->x2;
      if(r->y1 < f->updaterect.y1)
         f->updaterect.y1 = r->y1;
      if(r->y2 > f->updaterect.y2)
         f->updaterect.y2 = r->y2;
   }

   gClipScreenRect(&f->updaterect, &f->screenrect);

   if(f->updaterect.x2 < f->updaterect.x1 || f->updaterect.y1 > f->updaterect.y2)
      f->updateflag = false;
}



//#define TESTCLIP

void gPerformUpdate(gFactory *f, gWidget *w)
{
   SDL_Rect r;
   bool unlock = false;

   if(!f || !w || !f->updateflag)
      return;

   f->updateflag = false;

   r.x = f->updaterect.x1;
   r.y = f->updaterect.y1;
   r.w = (f->updaterect.x2 - r.x + 1);
   r.h = (f->updaterect.y2 - r.y + 1);

   if(f->stacklevel)
      f->stacklevel = 0; // Reset the clipping stack.. Maybe report an error?
   
   // SoM: DEBUG to make sure nothing is drawing outside the correct update rect.
#ifdef TESTCLIP
   {
      SDL_Rect cr;

      cr.x = cr.y = 0;
      cr.w = f->dest->w;
      cr.h = f->dest->h;
      gSetClipRect(f, &cr);
      SDL_FillRect(f->dest, &cr, 0);
      printf("DEBUG: %i, %i, %i, %i\n", r.x, r.y, r.w, r.h);
   }
#endif

   if(!gPushUpdateRect(f))
   {
      gSetError("gPerformUpdate: update flag was set but update rect had negative size\n");
      return;
   }

   gClearClippingList(f);

   if(SDL_MUSTLOCK(f->dest))
   {
      SDL_LockSurface(f->dest);
      unlock = true;
   }

   w->drawWidget(w);


   gFlushBlendedRows(f);

   if(unlock)
      SDL_UnlockSurface(f->dest);

   gPopClippingStack(f);

   if(f->stacklevel)
      printf("Warning: factory clipping stack not back to initial state at end of update\n");

#ifdef TESTCLIP
   SDL_Flip(f->dest);
#else
   SDL_UpdateRect(f->dest, r.x, r.y, r.w, r.h);
#endif
}





static void stackIncrease(gFactory *f)
{
   if(f->stacklevel >= f->stacksize)
   {
      f->stacksize += 5;
      f->clipstack = realloc(f->clipstack, sizeof(SDL_Rect) * f->stacksize);
   }
}


bool gPushUpdateRect(gFactory *f)
{
   gScreenRect *r;

   if(!f)
      return false;

   r = &f->updaterect;
   if(r->x2 < r->x1 || r->y2 < r->y1)
      return false;

   if(f->stacklevel)
   {
      gSetError("gPushUpdateRect: called with stacklevel > 0\n");
      return false;
   }

   stackIncrease(f);

   f->clipstack[f->stacklevel].x = r->x1;
   f->clipstack[f->stacklevel].y = r->y1;
   f->clipstack[f->stacklevel].w = r->x2 - r->x1 + 1;
   f->clipstack[f->stacklevel].h = r->y2 - r->y1 + 1;

   gSetClipRect(f, f->clipstack + f->stacklevel);

   f->stacklevel++;

   return true;
}


bool gPushClippingStackInside(gFactory *f, gScreenRect *r, bool updateonly)
{
   gScreenRect rect;

   if(!f || !r)
      return false;
   if(r->x2 < r->x1 || r->y2 < r->y1)
      return false; // ERROR?

   stackIncrease(f);

   if(!f->stacklevel)
   {
      gSetError("gPushClippingStackInside called without an existing stack member\n");
      return false;
   }

   if(updateonly)
      rect = f->updaterect;
   else
   {
      rect.x1 = f->clipstack[f->stacklevel - 1].x;
      rect.y1 = f->clipstack[f->stacklevel - 1].y;
      rect.x2 = rect.x1 + f->clipstack[f->stacklevel - 1].w - 1;
      rect.y2 = rect.y1 + f->clipstack[f->stacklevel - 1].h - 1;
   }


   gClipScreenRect(&rect, r);

   if(rect.x2 < rect.x1 || rect.y2 < rect.y1)
      return false;

   f->clipstack[f->stacklevel].x = rect.x1;
   f->clipstack[f->stacklevel].y = rect.y1;
   f->clipstack[f->stacklevel].w = rect.x2 - rect.x1 + 1;
   f->clipstack[f->stacklevel].h = rect.y2 - rect.y1 + 1;

   gSetClipRect(f, f->clipstack + f->stacklevel);

   f->stacklevel++;

   return true;
}




void gPopClippingStack(gFactory *f)
{
   if(f->stacklevel <= 1)
   {
      f->stacklevel = 0;

      gSetClipRect(f, NULL);
      return; // Error?
   }

   f->stacklevel--;

   gSetClipRect(f, f->clipstack + f->stacklevel - 1);
}


void gSetClipRect(gFactory *f, SDL_Rect *r)
{
   SDL_SetClipRect(f->dest, r);
   f->cliprect.x1 = f->dest->clip_rect.x;
   f->cliprect.y1 = f->dest->clip_rect.y;
   f->cliprect.x2 = f->cliprect.x1 + f->dest->clip_rect.w - 1;
   f->cliprect.y2 = f->cliprect.y1 + f->dest->clip_rect.h - 1;
}


void gSetClipRectScreen(gFactory *f, gScreenRect *r)
{
   SDL_Rect sdlr;

   sdlr.x = r->x1;
   sdlr.y = r->y1;
   sdlr.w = r->x2 - r->x1 + 1;
   sdlr.h = r->y2 - r->y1 + 1;

   gSetClipRect(f, &sdlr);
}


// ----------------------------------------------------------------------------
// Icon rendering

// gFindIcon
gSkinIcon *gFindIcon(gFactory *f, const char *name)
{
   gSkinIcon *icon;
   unsigned int i;

   for(i = 0; i < f->ilist->size; i++)
   {
      icon = (gSkinIcon *)gGetListItem(f->ilist, i);

      if(strcmp(icon->name, name))
         continue;

      return icon;
   }

   return NULL;
}



void gDrawIcon(gFactory *f, gSkinIcon *icon, gScreenRect *screenrect)
{
   int cx, cy, cw, ch, xoff, yoff;

   if(!icon->graphic)
      return;

   cx = (screenrect->x2 + screenrect->x1 + 1) >> 1;
   cy = (screenrect->y2 + screenrect->y1 + 1) >> 1;

   cw = (int)icon->graphic->width;
   ch = (int)icon->graphic->height;

   cx -= cw >> 1;
   cy -= ch >> 1;

   if(cx < screenrect->x1)
   {
      xoff = screenrect->x1 - cx;
      cx = screenrect->x1;
   }

   if(cx + cw - 1 > screenrect->x2)
   {
      cw = screenrect->x2 - cx + 1;
      if(cw < 0)
         return;
   }



   if(cy < screenrect->y1)
   {
      yoff = screenrect->y1 - cy;
      cy = screenrect->y1;
   }

   if(cy + ch - 1 > screenrect->y2)
   {
      ch = screenrect->y2 - cy + 1;
      if(ch < 0)
         return;
   }

   gRenderGraphic(f, icon->graphic, 0, 0, cx, cy, cw, ch, 0);
}





// ----------------------------------------------------------------------------
// Process rendering
gSkinProcess *gFindProcess(gFactory *f, const char *name)
{
   gSkinProcess *proc;
   unsigned int i;

   for(i = 0; i < f->plist->size; i++)
   {
      proc = (gSkinProcess *)gGetListItem(f->plist, i);

      if(strcmp(proc->name, name))
         continue;

      return proc;
   }

   return NULL;
}


void gRenderProcess(gFactory *f, gRect *rect, const char *name)
{
   gSkinProcess *proc;
   unsigned int i;

   for(i = 0; i < f->ilist->size; i++)
   {
      proc = (gSkinProcess *)gGetListItem(f->plist, i);

      if(strcmp(proc->name, name))
         continue;

      proc->func(f, proc, rect->x, rect->y, rect->w, rect->h);
      return;
   }

   gSetError("gTempProcess can not find process %s\n", name);
}


#define ADJUST_SHADOW(p, x, y, w, h) \
{\
   (x) -= (p)->shadow_left; (w) += ((p)->shadow_left + (p)->shadow_right);\
   (y) -= (p)->shadow_top; (h) += ((p)->shadow_top + (p)->shadow_bottom);\
}


void gDrawProcess1Row(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h)
{
   if(proc->startspan)
      proc->startspan->spanfunc(f, proc->startspan, x, y, w, h);
   else if(proc->midspan)
      proc->midspan->spanfunc(f, proc->midspan, x, y, w, h);
   else if(proc->endspan)
      proc->endspan->spanfunc(f, proc->endspan, x, y, w, h);
}


void gDrawProcess2RowsA(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h)
{
   // Process contains startspan and midspan
   // midspan is flexible, startspan is not 
   if(proc->startspan->thickness > h)
      proc->startspan->spanfunc(f, proc->startspan, x, y, w, h);
   else
   {
      int t = proc->startspan->thickness;

      proc->startspan->spanfunc(f, proc->startspan, x, y, w, t);
      proc->midspan->spanfunc(f, proc->midspan, x, y + t, w, h - t);
   }
}

void gDrawProcess2RowsB(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h)
{
   // Process contains midspan and endspan
   // midspan is flexible, end is not 
   if(proc->endspan->thickness > h)
      proc->endspan->spanfunc(f, proc->endspan, x, y, w, h);
   else
   {
      int t = proc->endspan->thickness, y2 = y + h - 1 - t;

      proc->midspan->spanfunc(f, proc->midspan, x, y, w, h - t);
      proc->endspan->spanfunc(f, proc->endspan, x, y2, w, t);
   }
}



void gDrawProcess2RowsC(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h)
{
   // Two spans, startspan and endspan, neither one is flexible
   unsigned int t = proc->startspan->thickness + proc->endspan->thickness;
   unsigned int h1 = proc->startspan->thickness;
   unsigned int h2 = proc->endspan->thickness;

   if(t <= h)
   {
      proc->startspan->spanfunc(f, proc->startspan, x, y, w, h1);
      proc->endspan->spanfunc(f, proc->endspan, x, y + h - h2, w, h2);
   }
   else
   {
      // 
      t = h >> 1;

      h1 = h1 < t ? h1 : h2 < t ? t - h2 : t;
      h2 = h - h1;

      proc->startspan->spanfunc(f, proc->startspan, x, y, w, h1);
      proc->endspan->spanfunc(f, proc->endspan, x, y + h - h2, w, h2);
   }
}



void gDrawProcess3Rows(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h)
{
   unsigned int t = proc->startspan->thickness + proc->endspan->thickness;
   unsigned int h1 = proc->startspan->thickness;
   unsigned int h2 = proc->endspan->thickness;

   if(t < h)
   {
      proc->startspan->spanfunc(f, proc->startspan, x, y, w, h1);
      proc->midspan->spanfunc(f, proc->midspan, x, y + h1, w, h - h1 - h2);
      proc->endspan->spanfunc(f, proc->endspan, x, y + h - h2, w, h2);
   }
   else if(t == h)
   {
      proc->startspan->spanfunc(f, proc->startspan, x, y, w, h1);
      proc->endspan->spanfunc(f, proc->endspan, x, y + h1, w, h2);
   }
   else
   {
      t = h >> 1;

      h1 = h1 < t ? h1 : h2 < t ? h - h2 : t;
      h2 = h - h1;

      proc->startspan->spanfunc(f, proc->startspan, x, y, w, h1);
      proc->endspan->spanfunc(f, proc->endspan, x, y + h - h2, w, h2);
   }
}








void gDrawProcess1Column(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h)
{
   if(proc->startspan)
      proc->startspan->spanfunc(f, proc->startspan, x, y, w, h);
   else if(proc->midspan)
      proc->midspan->spanfunc(f, proc->midspan, x, y, w, h);
   else if(proc->endspan)
      proc->endspan->spanfunc(f, proc->endspan, x, y, w, h);
}


void gDrawProcess2ColumnsA(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h)
{
   // Process contains startspan and midspan
   // midspan is flexible, startspan is not 
   if(proc->startspan->thickness > w)
      proc->startspan->spanfunc(f, proc->startspan, x, y, w, h);
   else
   {
      int t = proc->startspan->thickness;

      proc->startspan->spanfunc(f, proc->startspan, x, y, t, h);
      proc->midspan->spanfunc(f, proc->midspan, x + t, y, w - t, h);
   }
}

void gDrawProcess2ColumnsB(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h)
{
   // Process contains startspan and endspan
   // midspan is flexible, end is not 
   if(proc->endspan->thickness > w)
      proc->endspan->spanfunc(f, proc->endspan, x, y, w, h);
   else
   {
      int t = proc->endspan->thickness, x2 = x + w - 1 - t;

      proc->midspan->spanfunc(f, proc->midspan, x, y, w - t, h);
      proc->endspan->spanfunc(f, proc->endspan, x2, y, t, h);
   }
}



void gDrawProcess2ColumnsC(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h)
{
   // Two spans, startspan and endspan, neither one is flexible
   unsigned int t = proc->startspan->thickness + proc->endspan->thickness;
   unsigned int w1 = proc->startspan->thickness;
   unsigned int w2 = proc->endspan->thickness;

   if(t <= w)
   {
      proc->startspan->spanfunc(f, proc->startspan, x, y, w1, h);
      proc->endspan->spanfunc(f, proc->endspan, x + w - w2, y, w2, h);
   }
   else
   {
      // 
      t = w >> 1;

      w1 = w1 < t ? w1 : w2 < t ? t - w2 : t;
      w2 = w - w1;

      proc->startspan->spanfunc(f, proc->startspan, x, y, w1, h);
      proc->endspan->spanfunc(f, proc->endspan, x + w - w2, y, w2, h);
   }
}



void gDrawProcess3Columns(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h)
{
   unsigned int t = proc->startspan->thickness + proc->endspan->thickness;
   unsigned int w1 = proc->startspan->thickness;
   unsigned int w2 = proc->endspan->thickness;

   if(t <= w)
   {
      proc->startspan->spanfunc(f, proc->startspan, x, y, w1, h);
      proc->midspan->spanfunc(f, proc->midspan, x + w1, y, w - w1 - w2, h);
      proc->endspan->spanfunc(f, proc->endspan, x + w - w2, y, w2, h);
   }
   else
   {
      t = w >> 1;

      w1 = w1 < t ? w1 : w2 < t ? t - w2 : t;
      w2 = w - w1;

      proc->startspan->spanfunc(f, proc->startspan, x, y, w1, h);
      proc->endspan->spanfunc(f, proc->endspan, x + w - w2, y, w2, h);
   }
}

#undef ADJUST_SHADOW

void gDrawProcessNOP(gFactory *f, gSkinProcess *proc, int x, int y, unsigned int w, unsigned int h)
{
}




// ----------------------------------------------------------------------------
// Fonts
static gFont *getListFont(gList *l, int i)
{
   return (gFont *)gGetListItem(l, i);
}


static bool compFontSpec(gFontSpec *spec, gFont *font)
{
   // Reject based on easy stuff to compare first
   if(spec->ptsize != font->ptsize || spec->style != font->style)
      return false;

   if(strcmp(spec->name, font->name))
      return false;

   return true;
}


gFont *gFindFontSpec(gFactory *f, gFontSpec *fontspec)
{
   gFont        *ret;
   unsigned int i;

   if(!f || !fontspec)
      return NULL;

   for(i = 0; i < f->fcache->size; i++)
   {
      ret = getListFont(f->fcache, i);

      if(!compFontSpec(fontspec, ret))
         continue;

      return ret;
   }

   return gCreateFont(f, fontspec);
}



gFont *gFindFont(gFactory *f, char *name, int ptsize, int style)
{
   gFontSpec s;

   s.name = name;
   s.ptsize = ptsize;
   s.style = style;

   return gFindFontSpec(f, &s);
}



Uint32 gMapColor(gFactory *f, SDL_Color *color)
{
   if(!f || !color)
      return 0;

   return SDL_MapRGB(f->dest->format, color->r, color->g, color->b);
}
