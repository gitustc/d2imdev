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
#include "gbutton.h"


bool gIsButton(gWidget *self)
{
   return self ? self->id(gButtonID) : false;
}


bool gButtonID(gIDFuncParam func)
{
   if(func == gButtonID)
      return true;
   return gWidgetID(func);
}


void gButtonDraw(gWidget *self)
{
   gButton *b = (gButton *)self;
   gFactory *f;

   if(!self->id(gButtonID))
   {
      gSetError("gButtonDraw: self failed id check!\n");
      return;
   }
   if((b->behavior & behaviorInvisible) || !b->factory)
      return;
   
   f = b->factory;

   if(b->caption && b->font)
   {
      gScreenRect r;
      unsigned int w = gStringWidth(b->font, b->caption);
      unsigned int h = gStringHeight(b->font, b->caption);

      r.x1 = ((b->screenrect.x1 + b->screenrect.x2) / 2) - (w / 2);
      r.x2 = r.x1 + w;
      r.y1 = ((b->screenrect.y1 + b->screenrect.y2) / 2) - (h / 2);
      r.y2 = r.y1 + h;

      gRenderText(f, b->font, &r, b->caption, b->fontcolor);
   }
   else if(b->icon)
      gDrawIcon(f, b->icon, &b->screenrect);

   if(b->pdown && b->state & stateMouseOver && b->state & stateLeftDown)
      b->pdown->func(f, b->pdown, b->screenrect.x1, 
                         b->screenrect.y1, b->rect.w, b->rect.h);
   else if(b->pover && (b->state & stateMouseOver || 
      b->state & stateLeftDown))
      b->pover->func(f, b->pover, b->screenrect.x1, 
                         b->screenrect.y1, b->rect.w, b->rect.h);
   else if(b->p1)
      b->p1->func(f, b->p1, b->screenrect.x1, 
                      b->screenrect.y1, b->rect.w, b->rect.h);
}




void gButtonSetFactory(gWidget *self, gFactory *f)
{
   gButton *b = (gButton *)self;
   int newbehavior = self->behavior;

   self->factory = f;

   if(!self->id(gButtonID))
   {
      gSetError("gButtonSetFactory: self failed id check!\n");
      return;
   }

   if(f)
   {
      gSkinAttribute *att;

      if(!b->pname || !(b->p1 = gFindProcess(f, b->pname)))
         b->p1 = gFindProcess(f, "button");
      if(!b->downname || !(b->pdown = gFindProcess(f, b->downname)))
         b->pdown = gFindProcess(f, "button_down");
      if(!b->overname || !(b->pover = gFindProcess(f, b->overname)))
         b->pover = gFindProcess(f, "button_over");

      if((att = gFindAttribute(f, b->p1, "font", attFontSpec)) != NULL)
      {
         gFontSpec *fs = (gFontSpec *)att->data; 
         b->font = gFindFontSpec(f, fs);
         b->fontcolor = gMapColor(f, &fs->fgcolor);
      }

      if(b->pdown)
         newbehavior |= behaviorUpdateMouseOn;

      if(b->pover)
         newbehavior |= behaviorUpdateMouseOver;

      b->setBehavior((gWidget *)b, newbehavior);

      if(b->iconname)
         b->icon = gFindIcon(f, b->iconname);
   }
   else
   {
      b->p1 = b->pdown = b->pover = NULL;
      b->font = NULL;
      b->icon = NULL;
      newbehavior &= ~(behaviorUpdateMouseOn | behaviorUpdateMouseOver);
      b->setBehavior((gWidget *)b, newbehavior);
   }
}




void gButtonSetProcessName(gButton *self, const char *p, const char *over, const char *down)
{
   gButton *b = (gButton *)self;
   int newbehavior = self->behavior;

   if(b->pname)
      free(b->pname);
   if(p)
      b->pname = strdup(p);
   else
      b->pname = NULL;

   if(b->downname)
      free(b->downname);
   if(down)
      b->downname = strdup(down);
   else
      b->downname = NULL;

   if(b->overname)
      free(b->overname);
   if(over)
      b->overname = strdup(over);
   else
      b->overname = NULL;


   if(self->factory)
   {
      if(!b->pname || !(b->p1 = gFindProcess(self->factory, b->pname)))
         b->p1 = gFindProcess(self->factory, "button");
      if(!b->downname || !(b->pdown = gFindProcess(self->factory, b->downname)))
         b->pdown = gFindProcess(self->factory, "button_down");
      if(!b->overname || !(b->pover = gFindProcess(self->factory, b->overname)))
         b->pover = gFindProcess(self->factory, "button_over");

      if(b->pdown)
         newbehavior |= behaviorUpdateMouseOn;

      if(b->pover)
         newbehavior |= behaviorUpdateMouseOver;

      b->setBehavior((gWidget *)b, newbehavior);
   }
}



gWidget *gButtonGetTopWidget(gWidget *self, int x, int y)
{
   if(self->state & stateLeftDown)
      return self;

   return gWidgetGetTopWidget(self, x, y);
}


void gButtonDelete(gWidget *self)
{
   gButton *b = (gButton *)self;

   if(!self->id(gButtonID))
   {
      gSetError("gButtonDelete: self failed id check!\n Using gWidgetDelete\n");
      gWidgetDelete(self);
      return;
   }

   // Just free the strings saved in the data struct before calling the normal
   // widget delete function.
   if(b->downname)
      free(b->downname);
   if(b->iconname)
      free(b->iconname);
   if(b->overname)
      free(b->overname);
   if(b->pname)
      free(b->pname);
   if(b->caption)
      free(b->caption);

   gWidgetDelete(self);
}




void gInitButton(gButton *b)
{
   memset(b, 0, sizeof(*b));

   gInitWidget((gWidget *)b);

   b->id = gButtonID;
   b->drawWidget = gButtonDraw;
   b->setFactory = gButtonSetFactory;
   b->getTopWidget = gButtonGetTopWidget;
   b->deleteWidget = gButtonDelete;

   b->down = false;
   b->p1 = NULL;
   b->pdown = NULL;
   b->pover = NULL;
   b->iconname = NULL;
   b->icon = NULL;
   b->pname = b->overname = b->downname = NULL;
   b->caption = NULL;

   b->setIcon = gButtonSetIcon;
   b->setCaption = gButtonSetCaption;
   b->setProcess = gButtonSetProcessName;
}




gButton *gNewButton(int x, int y, unsigned int w, unsigned int h, const char *caption)
{
   gButton *ret = malloc(sizeof(gButton));
   gInitButton(ret);

   ret->setRect((gWidget *)ret, x, y, w, h);
   ret->setCaption(ret, caption);

   return ret;
}



void gButtonSetIcon(gButton *self, const char *iconname)
{
   if(self->iconname)
      free(self->iconname);

   if(iconname)
   {
      self->iconname = strdup(iconname);
      self->icon = self->factory ? gFindIcon(self->factory, self->iconname) : NULL;
   }
   else
   {
      self->iconname = NULL;
      self->icon = NULL;
   }

   self->update((gWidget *)self);
}


gCallback *gButtonGetCallback(gButton *self)
{
   return NULL;
}


void gButtonSetCaption(gButton *self, const char *caption)
{
   if(self->caption)
      free(self->caption);
   
   if(caption)
      self->caption = strdup(caption);
   else
      self->caption = NULL;

   self->update((gWidget *)self);
}

