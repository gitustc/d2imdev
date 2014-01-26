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
// gWidget functions

bool gWidgetID(gIDFuncParam func)
{
   if(func == gWidgetID)
      return true;
   return false;
}


void gWidgetDraw(gWidget *self)
{
}


void gWidgetEventHandler(gWidget *self, SDL_Event *event, bool *claimed)
{
   if(self->behavior & behaviorNoEvent)
      return;

   if(event->type == SDL_MOUSEMOTION)
   {
      SDL_MouseMotionEvent *motion = &event->motion;

      if(gMouseOnRect(motion->x, motion->y, &self->visiblerect) && !*claimed)
      {
         if(!(self->state & stateMouseOver))
         {
            self->state |= stateMouseOver;

            if(self->behavior & behaviorUpdateMouseOver && self->factory)
               self->update(self);

            gSendSignal(self, "mouseEnter", motion);
         }
         else
            gSendSignal(self, "mouseMove", motion);

         *claimed = true;
      }
      else
      {
         if(self->state & stateMouseOver)
         {
            self->state &= ~stateMouseOver;

            if(self->behavior & behaviorUpdateMouseOver && self->factory)
               self->update(self);

            gSendSignal(self, "mouseLeave", motion);
         }

         if(self->state & stateAnyDown)
            *claimed = true;
      }
   }
   else if(event->type == SDL_MOUSEBUTTONDOWN)
   {
      int x = event->button.x;
      int y = event->button.y;
      int button = event->button.button;
      int bflag = button == SDL_BUTTON_RIGHT ? stateRightDown : 
                  button == SDL_BUTTON_MIDDLE ? stateMiddleDown :
                  button == SDL_BUTTON_LEFT ? stateLeftDown : 0;

      bool over = gMouseOnRect(x, y, &self->visiblerect);

      if(over && !*claimed)
      {
         self->state |= bflag;
         if(self->behavior & behaviorUpdateMouseOn && self->factory)
            self->update(self);

         gSendSignal(self, "mouseDown", &event->button);
         *claimed = true;
      }
      else if(!*claimed && self->state & stateAnyDown)
      {
         self->state &= ~stateAnyDown;
         if(self->behavior & behaviorUpdateMouseOn && self->factory)
            self->update(self);

         gSendSignal(self, "mouseUp", &event->button);
         //*claimed = true;
      }
   }
   else if(event->type == SDL_MOUSEBUTTONUP)
   {
      int x = event->button.x;
      int y = event->button.y;
      int button = event->button.button;
      int bflag = button == SDL_BUTTON_RIGHT ? stateRightDown : 
                  button == SDL_BUTTON_MIDDLE ? stateMiddleDown :
                  button == SDL_BUTTON_LEFT ? stateLeftDown : 0;

      bool over = gMouseOnRect(x, y, &self->visiblerect);

      if(self->state & bflag)
      {
         self->state &= ~bflag;
         if(self->behavior & behaviorUpdateMouseOn && self->factory)
            self->update(self);

         gSendSignal(self, "mouseUp", &event->button);

         if(!*claimed && over)
         {
            gSendSignal(self, "mouseClick", &event->button);
            *claimed = true;
         }
      }
      else if(over && !*claimed)
      {
         self->state |= stateMouseOver;

         if(self->behavior & behaviorUpdateMouseOver && self->factory)
            self->update(self);

         // SoM: I don't even know if this is possible
         /*gSendMouseSignal(self, "mouseEnter", x, y, 
                          SDL_GetMouseState(NULL, NULL), 0, 0);*/
      }
   }
}




void gAddResponder(gWidget *self, char *type, signalFunc func)
{
   if(!func || !type || !self)
      return;

   gAppendListItem(self->responders, gNewResponder(type, func));
}


void gRemoveResponder(gWidget *self, signalFunc func)
{
   unsigned i = 0;

   while(i < self->responders->size)
   {
      gResponder *r = self->responders->list[i];

      if(r->func == func)
         gDeleteListItem(self->responders, i);
      else
         i++;
   }
}


// This basically just runs through the signal list and calls user-supplied
// responders.
void gWidgetSignalHandler(gWidget *self, gSignal *sig)
{
   gResponder *r;
   unsigned   i;

   for(i = 0; i < self->responders->size; i++)
   {
      r = (gResponder *)self->responders->list[i];

      // Quickly reject the responders
      if(r->hash != sig->hash)
         continue;

      if(!stricmp(r->type, sig->type))
         r->func(self, sig);
   }
}





void gWidgetSetRect(gWidget *self, int x, int y, unsigned int w, unsigned int h)
{
   int   mx, my, mbuttons;
   bool  mouseover;
   gRect oldrect = self->rect;

   self->update(self);

   self->rect.x = x;
   self->rect.y = y;
   self->rect.w = w;
   self->rect.h = h;

   self->calcScreen(self);

   if(self->prect)
   {
      self->fromx2 = self->prect->x2 - self->screenrect.x2;
      self->fromy2 = self->prect->y2 - self->screenrect.y2;
   }

   // Get the mouse state and update the flags accordingly
   mbuttons = SDL_GetMouseState(&mx, &my);
   mouseover = gMouseOnRect(mx, my, &self->screenrect);

   if(mouseover && !(self->state & stateMouseOver))
      self->state |= stateMouseOver;
   else if(!mouseover && self->state & stateMouseOver)
      self->state &= ~stateMouseOver;

   if(self->state & stateLeftDown && !(mbuttons & SDL_BUTTON(1)))
      self->state &= ~stateLeftDown;

   gSendResizeSignal(self, "resized", &oldrect, self);

   if(self->parent)
      gSendResizeSignal((gWidget *)self->parent, "childResized", &oldrect, self);

   self->update(self);
}



void gWidgetUpdateSize(gWidget *self)
{
   int newx = self->rect.x, newy = self->rect.y, neww = self->rect.w;
   int newh = self->rect.h;

   if(!self->prect)
      return;

   if(self->behavior & behaviorExpandW)
      neww = self->prect->x2 - self->fromx2 - self->screenrect.x1 + 1;
   if(self->behavior & behaviorExpandH)
      newh = self->prect->y2 - self->fromy2 - self->screenrect.y1 + 1;

   if(neww != self->rect.w || newh != self->rect.h)
      self->setRect(self, newx, newy, neww, newh);
}


void gWidgetCalcScreen(gWidget *self)
{
   if(self->prect)
   {
      if(self->behavior & behaviorFromRight)
      {
         self->screenrect.x1 = self->prect->x2 - self->rect.w - self->rect.x + 1;
         self->screenrect.x2 = self->screenrect.x1 + self->rect.w - 1;
      }
      else
      {
         self->screenrect.x2 = (self->screenrect.x1 = self->rect.x + 
                                self->prect->x1) + self->rect.w - 1;
      }


      if(self->behavior & behaviorFromBottom)
      {
         self->screenrect.y1 = self->prect->y2 - self->rect.h - self->rect.y + 1;
         self->screenrect.y2 = self->screenrect.y1 + self->rect.h - 1;
      }
      else
      {
         self->screenrect.y2 = (self->screenrect.y1 = self->rect.y +
                                self->prect->y1) + self->rect.h - 1;
      }

      // Setup mouse rectangle
      self->visiblerect = self->screenrect;

      if(!(self->behavior & behaviorNoClip))
      {
         if(self->parent)
            gClipScreenRect(&self->visiblerect, &self->parent->visiblerect);

         if(self->prect)
            gClipScreenRect(&self->visiblerect, self->prect);
      }
   }
   else
   {
      self->screenrect.x2 = (self->screenrect.x1 = self->rect.x) +
                            self->rect.w - 1;
      self->screenrect.y2 = (self->screenrect.y1 = self->rect.y) +
                            self->rect.h - 1;

      // Setup mouse rectangle
      self->visiblerect = self->screenrect;
   }

}




void gWidgetSetFactory(gWidget *self, gFactory *f)
{
   self->factory = f;

   self->update(self);
   // Many widgets will need to read attributes out of the skin.
}


void gWidgetSetParentRect(gWidget *self, gScreenRect *prect)
{
   self->prect = prect;

   // Take care of any change in screen position caused by behavior flags and 
   // being inside a parent.
   gWidgetSetRect(self, self->rect.x, self->rect.y, self->rect.w, self->rect.h);
}


void gWidgetDelete(gWidget *self)
{
   gContainer *parent = self->parent;

   if(parent)
      parent->removeChild(parent, self);

   gFreeList(self->responders);
   free(self);
}



gWidget *gNewWidget(void)
{
   gWidget *ret = malloc(sizeof(gWidget));

   gInitWidget(ret);
   gSendSignal(ret, "created", NULL);

   return ret;
}




gWidget *gWidgetGetTopWidget(gWidget *self, int mousex, int mousey)
{
   return gMouseOnRect(mousex, mousey, &self->visiblerect) ? self : NULL;
}



gSkinCursor *gWidgetGetCursor(gWidget *self, int mousex, int mousey)
{
   return NULL;
}


void gWidgetSetInFocus(gWidget *self, bool kfocus)
{
   if(!kfocus != !(self->state & stateHasFocus))
   {
      self->state ^= stateHasFocus;

      if((self->behavior & behaviorUpdateFocus) && self->factory)
         self->update(self);

      gSendSignal(self, self->state & stateHasFocus ? "gainedFocus" : "lostFocus", NULL);
   }
}




void gWidgetUpdate(gWidget *self)
{
   if(!self->factory)
      return;

   gAddUpdateRect(self->factory, &self->visiblerect);
}



void gWidgetSetBehavior(gWidget *self, int behavior)
{
   int change = self->behavior ^ behavior;

   if(change & (behaviorFromRight|behaviorFromBottom))
      self->calcScreen(self);
   if(change & (behaviorNoClip|behaviorInvisible|behaviorDisabled))
      self->update(self);

   self->behavior = behavior;
}



void gInitWidget(gWidget *w)
{
   w->id = gWidgetID;
   w->eventHandler = gWidgetEventHandler;
   w->drawWidget = gWidgetDraw;
   w->setRect = gWidgetSetRect;
   w->calcScreen = gWidgetCalcScreen;
   w->setFactory = gWidgetSetFactory;
   w->deleteWidget = gWidgetDelete;
   w->setParentRect = gWidgetSetParentRect; 
   w->updateSize = gWidgetUpdateSize;
   w->getTopWidget = gWidgetGetTopWidget;
   w->getCursor = gWidgetGetCursor;
   w->setInFocus = gWidgetSetInFocus;
   w->update = gWidgetUpdate;
   w->setBehavior = gWidgetSetBehavior;
   w->signalHandler = gWidgetSignalHandler;

   w->rect.x = w->rect.y = 0;
   w->rect.w = w->rect.h = 1;
   memset(&w->screenrect, 0, sizeof(gScreenRect));

   w->parent = NULL;
   w->prect = NULL;
   w->factory = NULL;
   w->next = w->prev = w;
   w->state = w->behavior = 0;

   w->responders = gNewList(gFreeResponder);
}

