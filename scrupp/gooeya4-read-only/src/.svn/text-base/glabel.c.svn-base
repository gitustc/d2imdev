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
#include "glabel.h"

// ----------------------------------------------------------------------------
// gLabel - Simple widget that displays text. Can be made to respond to events.


bool gLabelID(gIDFuncParam func)
{
   if(func == gLabelID)
      return true;
   return gWidgetID(func);
}


bool gIsLabel(gWidget *w)
{
   return w->id(gLabelID);
}




void gInitLabel(gLabel *lbl)
{
   memset(lbl, 0, sizeof(*lbl));

   gInitWidget((gWidget *)lbl);

   lbl->id = gLabelID;
   lbl->deleteWidget = gLabelDelete;
   lbl->drawWidget = gLabelDraw;
   lbl->setFactory = gLabelSetFactory;

   lbl->setCaption = gLabelSetCaption;
   lbl->getCaption = gLabelGetCaption;

   lbl->caption = malloc(sizeof(qstring_t));
   M_QStrInitCreate(lbl->caption);

}  


// gNewLabel
// Creates, initializes and returns a new gLabel object.
gLabel *gNewLabel(int x, int y, int w, int h, const char *caption)
{
   gLabel *ret = malloc(sizeof(gLabel));
   gInitLabel(ret);

   ret->setRect((gWidget *)ret, x, y, w, h);
   ret->setCaption(ret, caption);

   return ret;
}


// gLabelDelete
// Deletes a gLabel object.
void gLabelDelete(gWidget *self)
{
   gLabel *lbl = (gLabel *)self;

   if(!self->id(gLabelID))
   {
      gSetError("gLabelDelete: self failed id check!\n Using gWidgetDelete\n");
      gWidgetDelete(self);
      return;
   }

   if(lbl->caption)
      M_QStrFree(lbl->caption);

   free(lbl);
}


// gLabelDraw
void gLabelDraw(gWidget *self)
{
   gLabel   *lbl = (gLabel *)self;
   gFactory *f = self->factory;

   if(!self->id(gLabelID))
   {
      gSetError("gLabelDraw: self failed id check!\n");
      return;
   }
   if((self->behavior & behaviorInvisible) || !self->factory)
      return;

   if(*lbl->caption->buffer && lbl->font &&
      gPushClippingStackInside(f, &self->screenrect, false))
   {
      gScreenRect r;

      r.x1 = self->screenrect.x1;
      r.x2 = r.x1 + self->rect.w - 1;
      r.y1 = self->screenrect.y1;
      r.y2 = r.y1 + self->rect.h - 1;

      gRenderTextWordWrap(f, lbl->font, &r, lbl->caption->buffer, lbl->color);
      gPopClippingStack(f);
   }
}



// gLabelSetFactory
void gLabelSetFactory(gWidget *self, gFactory *f)
{
   gLabel *lbl = (gLabel *)self;

   if(!self->id(gLabelID))
   {
      gSetError("gLabelDraw: self failed id check!\n");
      return;
   }

   gWidgetSetFactory(self, f);

   lbl->font = NULL;

   if(f)
   {
      gSkinAttribute *att;

      if((att = gFindAttribute(f, NULL, "font", attFontSpec)) != NULL)
      {
         gFontSpec *fs = (gFontSpec *)att->data; 
         lbl->font = gFindFontSpec(f, fs);
         lbl->color = gMapColor(f, &fs->fgcolor);
      }
   }
}


// gLabelSetCaption
// Sets the text of the gLabel.
void gLabelSetCaption(gLabel *self, const char *caption)
{
   M_QStrSet(self->caption, caption);
   gSendSignal((gWidget *)self, "textChanged", (void *)self->caption->buffer);

   if(self->factory)
      self->update((gWidget *)self);
}


// gLabelGetCaption
// Gets the text from a gLabel.
const char *gLabelGetCaption(gLabel *self)
{
   return self->caption->buffer;
}

