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
#include "gcheckbox.h"

// ----------------------------------------------------------------------------
// gWidget overrides


bool gIsCheckBox(gWidget *self)
{
   return self ? self->id(gCheckBoxID) : false;
}

bool gCheckBoxID(gIDFuncParam func)
{
   if(func == gCheckBoxID)
      return true;
   return gWidgetID(func);
}


void gCheckBoxDraw(gWidget *self)
{
   gCheckBox *cb = (gCheckBox *)self;
   gFactory *f;
   gSkinIcon *box = NULL;
   gScreenRect iconrect, captionrect;

   if(!self->id(gCheckBoxID))
   {
      gSetError("gCheckBoxDraw: self failed id check!\n");
      return;
   }
   if((self->behavior & behaviorInvisible) || !self->factory)
      return;
   
   f = self->factory;

   if(cb->checked)
   {
      if(cb->ch_down && self->state & stateMouseOver && self->state & stateLeftDown)
         box = cb->ch_down;
      else if(cb->ch_over && (self->state & stateMouseOver || 
              self->state & stateLeftDown))
         box = cb->ch_over;
      else if(cb->ch)
         box = cb->ch;
   }
   else
   {
      if(cb->un_down && self->state & stateMouseOver && self->state & stateLeftDown)
         box = cb->un_down;
      else if(cb->un_over && (self->state & stateMouseOver || 
              self->state & stateLeftDown))
         box = cb->un_over;
      else if(cb->un)
         box = cb->un;
   }

   if(box)
   {
      iconrect.x1 = self->screenrect.x1;
      iconrect.y1 = self->screenrect.y1;
      iconrect.y2 = self->screenrect.y2;
      iconrect.x2 = iconrect.x1 + box->graphic->width - 1;
      gDrawIcon(f, box, &iconrect);

      captionrect.x1 = iconrect.x2 + 4;
      captionrect.x2 = self->screenrect.x2;
      captionrect.y1 = iconrect.y1;
      captionrect.y2 = iconrect.y2;
   }
   else
   {
      captionrect = self->screenrect;
   }

   // FIXME: Use color from skin
   if(cb->font && cb->caption)
      gRenderTextCentered(f, cb->font, &captionrect, cb->caption, cb->fontc, false, true);
}



void gCheckBoxEventHandler(gWidget *self, SDL_Event *event, bool *claimed)
{
   gCheckBox *cb = (gCheckBox *)self;
   int prestate = self->state;
   bool flag;

   if(!self->id(gCheckBoxID))
   {
      gSetError("gCheckBoxEventHandler: self failed id check!\n");
      return;
   }
   if(self->behavior & behaviorNoEvent)
      return;

   flag = *claimed;
   gWidgetEventHandler(self, event, claimed);

   
   if(event->type == SDL_MOUSEBUTTONUP && event->button.button == SDL_BUTTON_LEFT)
   {
      if(prestate & stateLeftDown && !(self->state & stateLeftDown) &&
         self->state & stateMouseOver && !flag)
      {
         // Button click Callback
         cb->checked = !cb->checked;
         gCall(&cb->call, self);
         *claimed = true;
      }
   }
   if(event->type == SDL_MOUSEMOTION && self->state & stateLeftDown)
      *claimed = true;
}


void gCheckBoxSetFactory(gWidget *self, gFactory *f)
{
   gCheckBox *cb = (gCheckBox *)self;
   int newbehavior = self->behavior;

   self->factory = f;

   if(!self->id(gCheckBoxID))
   {
      gSetError("gCheckBoxSetFactory: self failed id check!\n");
      return;
   }

   if(f)
   {
      gSkinAttribute *att;

      cb->un = gFindIcon(f, "uncheckedbox");
      cb->un_down = gFindIcon(f, "uncheckedbox_down");
      cb->un_over = gFindIcon(f, "uncheckedbox_over");

      cb->ch = gFindIcon(f, "checkedbox");
      cb->ch_down = gFindIcon(f, "checkedbox_down");
      cb->ch_over = gFindIcon(f, "checkedbox_over");

      if(cb->un_down || cb->ch_down)
         newbehavior |= behaviorUpdateMouseOn;

      if(cb->un_over || cb->un_over)
         newbehavior |= behaviorUpdateMouseOver;

      cb->setBehavior((gWidget *)cb, newbehavior);

      if((att = gFindAttribute(f, NULL, "font", attFontSpec)) != NULL)
      {
         gFontSpec *fs = (gFontSpec *)att->data; 
         cb->font = gFindFontSpec(f, fs);
         cb->fontc = gMapColor(f, &fs->fgcolor);
      }
   }
   else
   {
      cb->un = cb->un_down = cb->un_over = NULL;
      cb->ch = cb->ch_down = cb->ch_over = NULL;
      cb->font = NULL;
      newbehavior &= ~(behaviorUpdateMouseOn | behaviorUpdateMouseOver);
      cb->setBehavior((gWidget *)cb, newbehavior);
   }
}


void gCheckBoxDelete(gWidget *self)
{
   gCheckBox *cb = (gCheckBox *)self;

   if(!self->id(gCheckBoxID))
   {
      gSetError("gCheckBoxDelete: self failed id check! Using gWidgetDelete\n");
      gWidgetDelete(self);
      return;
   }

   if(cb->caption)
      free(cb->caption);

   gWidgetDelete(self);
}


gWidget *gCheckBoxGetTopWidget(gWidget *self, int x, int y)
{
   if(self->state & stateLeftDown)
      return self;

   return gWidgetGetTopWidget(self, x, y);
}


// ----------------------------------------------------------------------------
// Specific member functions

// gCheckBoxGetCallback
// returns a pointer to the button's callback object. The object is called when
// the button registers a valid mouse click.
gCallback *gCheckBoxGetCallback(gCheckBox *self)
{
   return &self->call;
}


// gCheckBoxSetCaption
// Sets the caption text for the button.
void gCheckBoxSetCaption(gCheckBox *self, const char *caption)
{
   if(self->caption)
      free(self->caption);

   if(caption)
      self->caption = strdup(caption);
   else
      self->caption = NULL;


   self->update((gWidget *)self);
}


// gCheckBoxGetState
// Gets the current state of the checkbox (true for checked, false for unchecked)
bool gCheckBoxGetState(gCheckBox *self)
{
   return self->checked;
}



// gCheckBoxSetState
// Sets the current state of the checkbox (true for checked, false for unchecked)
void gCheckBoxSetState(gCheckBox *self, bool checked)
{
   if(self->checked != checked)
   {
      self->checked = checked;
      self->update((gWidget *)self);
   }
}





const char *gCheckBoxGetCaption(gCheckBox *self)
{
   return self->caption;
}




// gCheckBoxSetIcon
// Sets the skin icon used to represent the box. For the sake of this function
// not taking 7 parameters, un is the prefix for the unchecked icons 
// (un, un + "_over", un + "_down") and ch is the prefix for the checked states
// (ch, ch + "_over", ch + "_down")
void gCheckBoxSetIcon(gCheckBox *self, const char *un, const char *ch)
{
}



// ----------------------------------------------------------------------------
// Non-member functions


void gInitCheckBox(gCheckBox *b)
{
   memset(b, 0, sizeof(*b));
   gInitWidget((gWidget *)b);

   b->id = gCheckBoxID;
   b->eventHandler = gCheckBoxEventHandler;
   b->drawWidget = gCheckBoxDraw;
   b->setFactory = gCheckBoxSetFactory;
   b->getTopWidget = gCheckBoxGetTopWidget;
   b->deleteWidget = gCheckBoxDelete;

   b->setCaption = gCheckBoxSetCaption;
   b->setState = gCheckBoxSetState;
   b->setIcon = gCheckBoxSetIcon;
}

// gNewCheckBox
// Creates and initializes a new check box widget.
gCheckBox *gNewCheckBox(int x, int y, unsigned int w, unsigned int h, const char *caption)
{
   gCheckBox *ret = malloc(sizeof(gCheckBox));
   gInitCheckBox(ret);
   ret->setRect((gWidget *)ret, x, y, w, h);
   ret->setCaption(ret, caption);
   return ret;
}
