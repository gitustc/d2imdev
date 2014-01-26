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
#include "gmainframe.h"



gSurface *gLoadImage(const char *name)
{
   gSurface    *ret;
   SDL_RWops   *ops;

   if(!PHYSFS_exists(name))
   {
      gSetError("gLoadImage was unable to load the image %s", name);
      return NULL;
   }

   ops = PHYSFSRWOPS_openRead(name);
   if(!ops)
   {
      gSetError("gLoadImage was unable to load the image %s: failed _openRead", name);
      return NULL;
   }

   if(!(ret = IMG_Load_RW(ops, true)))
   {
      gSetError("gLoadImage was unable to load the image %s: %s", name, SDL_GetError());
      return NULL;
   }

   return ret;
}



// ----------------------------------------------------------------------------
// gMainFrame
// the main window manager element which optionally handles backgrounds.


bool gIsMainFrame(gWidget *w)
{
   return w ? w->id(gMainFrameID) : false;
}


bool gMainFrameID(gIDFuncParam func)
{
   if(func == gMainFrameID)
      return true;

   return gContainerID(func);
}


void gMainFrameDraw(gWidget *self)
{
   gMainFrame *mf = (gMainFrame *)self;

   if(!self->id(gMainFrameID))
   {
      gSetError("gMainFrameDraw: *self failed id check!\n");
      return;
   }
   if((self->behavior & behaviorInvisible) || !self->factory)
      return;

   gContainerDraw(self);

   // Update background here
   if(mf->updatebg)
   {
      if(mf->bgpane)
      {
         if(mf->flag == imageCentered)
         {
            int x, y, xoffset, yoffset, w, h;

            x = (self->rect.w / 2) - (mf->bgpane->width / 2);
            xoffset = x < 0 ? -x : 0;

            y = (self->rect.h / 2) - (mf->bgpane->height / 2);
            yoffset = y < 0 ? -y : 0;

            w = self->rect.w < mf->bgpane->width ? self->rect.w : mf->bgpane->width;
            h = self->rect.h < mf->bgpane->height ? self->rect.h : mf->bgpane->height;

            gRenderGraphic(self->factory, mf->bgpane->graphic, xoffset, yoffset, x > 0 ? x : 0, y > 0 ? y : 0, w, h, 0);
            gRenderBox(self->factory, self->screenrect.x1, self->screenrect.y1, self->rect.w, self->rect.h, mf->bgcolor);
         }
         else if(mf->flag == imageTiled)
         {
         }
      }
      else
         gRenderBox(self->factory, self->screenrect.x1, self->screenrect.y1, self->rect.w, self->rect.h, mf->bgcolor);
   }
}




void gMainFrameSetFactory(gWidget *self, gFactory *f)
{
   gMainFrame *mf = (gMainFrame *)self;
   gSkinAttribute *att;

   if(!self->id(gMainFrameID))
   {
      gSetError("gMainFrameSetFactory: *self failed id check!\n");
      return;
   }

   gContainerSetFactory(self, f);

   if(!f)
      return;

   if((att = gFindAttribute(f, NULL, "bgcolor", attColor)) != NULL)
   {
      SDL_Color *c = (SDL_Color *)att->data;

      mf->bgrgb.r = c->r;
      mf->bgrgb.g = c->g;
      mf->bgrgb.b = c->b;
   }
   if((att = gFindAttribute(f, NULL, "bgimage", attString)) != NULL && !mf->bgsurface)
   {
      mf->bgsurface = gLoadImage((char *)att->data);
   }

   mf->bgcolor = SDL_MapRGB(f->dest->format, mf->bgrgb.r, mf->bgrgb.g, mf->bgrgb.b);

   if(mf->bgsurface)
   {
      gSurface *newbgimg = SDL_ConvertSurface(mf->bgsurface, f->dest->format, 0);

      SDL_FreeSurface(mf->bgsurface);
      mf->bgsurface = newbgimg;

      if(mf->bgpane)
         gFreePane(mf->bgpane);

      mf->bgpane = gCreateNewPane(f, mf->bgsurface->w, mf->bgsurface->h);
      SDL_BlitSurface(mf->bgsurface, NULL, mf->bgpane->surface, NULL);
   }

   self->update(self);
}




void gMainFrameSetRect(gWidget *self, int x, int y, unsigned int w, unsigned int h)
{
   bool shrink;
   gWidget *rover, *head;
   gContainer *cself;

   if(!self->id(gMainFrameID))
   {
      gSetError("gMainFrameSetRect: *self failed id check!\n");
      return;
   }

   shrink = w < self->rect.w || h < self->rect.h ? true : false;

   gContainerSetRect(self, x, y, w, h);

   // Check the child widgets to make sure none of them have been left outside the window.
   if(!shrink)
      return;

   cself = (gContainer *)self;
   head = cself->head;

   for(rover = head->next; rover != head; rover = rover->next)
   {
      int newx = rover->rect.x, newy = rover->rect.y;
      bool update = false;

      if(rover->screenrect.x2 < self->screenrect.x1)
         newx = 0, update = true;
      else if(rover->screenrect.x1 > self->screenrect.x2)
         newx = self->rect.w - rover->rect.w, update = true;

      if(rover->screenrect.y2 < self->screenrect.y2)
         newy = 0, update = true;
      else if(rover->screenrect.y1 > self->screenrect.y2)
         newy = self->rect.w - rover->rect.w, update = true;

      if(update)
         rover->setRect(rover, newx, newy, rover->rect.w, rover->rect.h);
   }
}





void gInitMainFrame(gMainFrame *mf)
{
   memset(mf, 0, sizeof(gMainFrame));
   gInitContainer((gContainer *)mf);

   mf->id = gMainFrameID;
   mf->drawWidget = gMainFrameDraw;
   mf->setFactory = gMainFrameSetFactory;
   mf->setRect = gMainFrameSetRect;
   mf->sortchildren = true;

   mf->setBGColor = gMainFrameSetBGColor;
   mf->setBGImage = gMainFrameSetBGImage;
   mf->noBG = gMainFrameNoBG;
}



gMainFrame *gNewMainFrame(int x, int y, unsigned int w, unsigned int h)
{
   gMainFrame *ret = malloc(sizeof(gMainFrame));
   gInitMainFrame(ret);
   ret->setRect((gWidget *)ret, x, y, w, h);
   return ret;
}




void gMainFrameDelete(gWidget *self)
{
   gMainFrame *mf = (gMainFrame *)self;

   if(!self->id(gMainFrameID))
   {
      gSetError("gMainFrameDelete: *self failed id check! Using gWidgetDelete.\n");
      gWidgetDelete(self);
      return;
   }

   if(mf->bgsurface)
      SDL_FreeSurface(mf->bgsurface);

   if(mf->bgpane)
      gFreePane(mf->bgpane);

   gWidgetDelete(self);
}




void gMainFrameSetBGColor(gMainFrame *self, Uint8 r, Uint8 g, Uint8 b)
{
   self->updatebg = true;
   self->bgrgb.r = r;
   self->bgrgb.g = g;
   self->bgrgb.b = b;

   if(self->factory)
   {
      self->bgcolor = SDL_MapRGB(self->factory->dest->format, r, g, b);
      self->update((gWidget *)self);
   }
}




void gMainFrameSetBGImage(gMainFrame *self, gSurface *image, gMainFrameFlag_e flag)
{
   self->updatebg = true;
   self->flag = flag;

   if(self->bgpane)
   {
      gFreePane(self->bgpane);
      self->bgpane = NULL;
   }
   if(self->bgsurface)
   {
      SDL_FreeSurface(self->bgsurface);
      self->bgsurface = NULL;
   }

   if(self->factory)
   {
      self->bgsurface = SDL_ConvertSurface(image, self->factory->dest->format, 0);
      SDL_FreeSurface(image);
      SDL_BlitSurface(self->bgsurface, NULL, self->bgpane->surface, NULL);

      self->update((gWidget *)self);
   }
   else
      self->bgsurface = image;
}



void gMainFrameNoBG(gMainFrame *self)
{
   self->updatebg = false;

   if(self->bgpane)
   {
      gFreePane(self->bgpane);
      self->bgpane = NULL;
   }
   if(self->bgsurface)
   {
      SDL_FreeSurface(self->bgsurface);
      self->bgsurface = NULL;
   }
}