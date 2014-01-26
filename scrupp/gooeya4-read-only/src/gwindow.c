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
#include "gwindow.h"
#include "gbutton.h"

// gwindow.c
// Contains all window variants (window, dialog box, ect)

// ----------------------------------------------------------------------------
// gWindow


bool gIsWindow(gWidget *w)
{
   return w ? w->id(gWindowID) : false;
}


bool gWindowID(gIDFuncParam func)
{
   if(func == gWindowID)
      return true;
   return gContainerID(func);
}



void gWindowDraw(gWidget *self)
{
   gWindow  *w = (gWindow *)self;
   gFactory *f;
   gSkinProcess *proc;
   gFont    *font;
   Uint32   fontc;

   if(!self->id(gWindowID))
   {
      gSetError("gContainerGetTopWidget: self failed id check!\n");
      return;
   }
   if((self->behavior & behaviorInvisible) || !self->factory)
      return;
   
   f = self->factory;

   gContainerDraw(self);

   if(self->state & stateHasFocus)
   {
      proc = !!(w->topproc) ? w->topproc : !!(w->wproc) ? w->wproc : NULL;
      font = !!(w->ttopfont) ? w->ttopfont : !!(w->tfont) ? w->tfont : NULL;
      fontc = !!(w->ttopfont) ? w->ttopfontc : !!(w->tfont) ? w->tfontc : 0;
   }
   else
   {
      proc = !!(w->wproc) ? w->wproc : !!(w->topproc) ? w->topproc : NULL;
      font = !!(w->tfont) ? w->tfont : !!(w->ttopfont) ? w->ttopfont : NULL;
      fontc = !!(w->tfont) ? w->tfontc : !!(w->ttopfont) ? w->ttopfontc : 0;
   }

   if(font && w->caption)
   {
      gScreenRect r;
      
      r.x1 = w->titlerect.x1 + w->captionindent;
      r.x2 = w->titlerect.x2;
      r.y1 = w->titlerect.y1 + (w->tbarh / 2) - (font->ascent / 2);
      r.y2 = r.y1 + font->height;

      gRenderTextTrunc(self->factory, font, &r, w->caption, fontc);
   }

   if(proc)
      proc->func(f, proc, self->screenrect.x1, 
                 self->screenrect.y1, self->rect.w, self->rect.h);
}



void gWindowEventHandler(gWidget *self, SDL_Event *event, bool *claimed)
{
   gWindow *w = (gWindow *)self;
   int     oldstate = self->state;
   bool    wasclaimed;
   bool    over;
   
   wasclaimed = *claimed;

   if(!self->id(gWindowID))
   {
      gSetError("gWindowEventHandler: self failed id check!\n");
      return;
   }

   if(w->drag && (event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEMOTION))
      *claimed = true;

   gContainerEventHandler(self, event, claimed);
   
   if(event->type == SDL_MOUSEBUTTONDOWN && self->state & stateLeftDown 
      && !(oldstate & stateLeftDown) && !w->maximized 
      && !w->drag && !wasclaimed)
   {
      over = gMouseOnRect(event->button.x, event->button.y, &self->visiblerect);
      if(over && gMouseOnRect(event->button.x, event->button.y, &w->tbarrect))
      {
         w->drag |= winDragMove;
         w->dxoff = event->button.x - self->rect.x;
         w->dyoff = event->button.y - self->rect.y;  
      }
      else if(w->allowresize)
      {
         if(over && gMouseOnRect(event->button.x, event->button.y, &w->bottomdragrect))
         {
            w->drag |= winDragResizeBottom;
            w->dhoff = event->button.y - (int)self->rect.h;
         }
         if((gMouseOnRect(event->button.x, event->button.y, &w->rightdragrect) ||
            gMouseOnRect(event->button.x, event->button.y, &w->trdragrect) ||
            gMouseOnRect(event->button.x, event->button.y, &w->brdragrect)) && over)
         {
            w->drag |= winDragResizeRight;
            w->dwoff = event->button.x - (int)self->rect.w;
         }
         if(over && gMouseOnRect(event->button.x, event->button.y, &w->topdragrect))
         {
            w->drag |= winDragResizeTop;
            w->dhoff = event->button.y - self->rect.y;
            w->dyoff = event->button.y - self->rect.y;
         }
         if((gMouseOnRect(event->button.x, event->button.y, &w->leftdragrect) ||
            gMouseOnRect(event->button.x, event->button.y, &w->tldragrect) ||
            gMouseOnRect(event->button.x, event->button.y, &w->bldragrect)) && over)
         {
            w->drag |= winDragResizeLeft;
            w->dwoff = event->button.x - self->rect.x;
            w->dxoff = event->button.x - self->rect.x;
         }
      }
   }

   if(w->drag && !(self->state & stateLeftDown))
      w->drag = 0;

   if(w->drag && event->type == SDL_MOUSEMOTION && !wasclaimed)
   {
      int x = self->rect.x, y = self->rect.y;
      int width = (int)self->rect.w, height = (int)self->rect.h;

      if(w->drag & winDragMove)
      {
         x = event->motion.x - w->dxoff;
         y = event->motion.y - w->dyoff;
      }

      if(w->drag & winDragResizeBottom)
         height = event->motion.y - w->dhoff >= w->minheight ? 
                  event->motion.y - w->dhoff : w->minheight;

      if(w->drag & winDragResizeTop)
      {
         height = self->screenrect.y2 - event->motion.y + w->dhoff + 1;
         if(height < w->minheight) height = w->minheight;
         y = self->screenrect.y2 - height + 1;
      }

      if(w->drag & winDragResizeLeft)
      {
         width = self->screenrect.x2 - event->motion.x + w->dwoff + 1;
         if(width < w->minwidth) width = w->minwidth;
         x = self->screenrect.x2 - width + 1;
      }

      if(w->drag & winDragResizeRight)
         width = event->motion.x - w->dwoff >= w->minwidth ? 
                 event->motion.x - w->dwoff : w->minwidth;

      self->setRect(self, x, y, width, height);
   }

}


void gWindowSetRects(gWindow *w)
{
   gScreenRect *r;

   if(!w->id(gWindowID))
   {
      gSetError("gWindowSetRects: self failed id check!\n");
      return;
   }

   // Mostly just set up the rects
   r = &w->tbarrect;
   r->x1 = w->screenrect.x1 + 4;
   r->y1 = w->screenrect.y1 + 4;
   r->x2 = w->screenrect.x2 - 4;
   r->y2 = r->y1 + w->tbarh - 4;

   r = &w->titlerect;
   r->x1 = w->screenrect.x1;
   r->y1 = w->screenrect.y1;
   r->x2 = w->screenrect.x2;
   r->y2 = r->y1 + w->tbarh;

   if(w->minbutton)
      r->x2 = w->minbutton->screenrect.x1 - 4;
   else if(w->maxbutton)
      r->x2 = w->maxbutton->screenrect.x1 - 4;
   else if(w->xbutton)
      r->x2 = w->xbutton->screenrect.x1 - 4;


   r = &w->topdragrect;
   r->x1 = w->screenrect.x1;
   r->x2 = w->screenrect.x2;
   r->y1 = w->screenrect.y1;
   r->y2 = r->y1 + 3;

   r = &w->bottomdragrect;
   r->x1 = w->screenrect.x1;
   r->x2 = w->screenrect.x2;
   r->y2 = w->screenrect.y2;
   r->y1 = r->y2 - 3;

   r = &w->rightdragrect;
   r->x2 = w->screenrect.x2;
   r->x1 = r->x2 - 3;
   r->y1 = w->screenrect.y1;
   r->y2 = w->screenrect.y2;

   r = &w->leftdragrect;
   r->x1 = w->screenrect.x1;
   r->x2 = r->x1 + 3;
   r->y1 = w->screenrect.y1;
   r->y2 = w->screenrect.y2;

   r = &w->tldragrect;
   r->x1 = w->screenrect.x1;
   r->x2 = r->x1 + 15;
   r->y1 = w->screenrect.y1;
   r->y2 = w->screenrect.y1 + 3;

   r = &w->trdragrect;
   r->x1 = w->screenrect.x2 - 15;
   r->x2 = w->screenrect.x2;
   r->y1 = w->screenrect.y1;
   r->y2 = w->screenrect.y1 + 3;


   r = &w->bldragrect;
   r->x1 = w->screenrect.x1;
   r->x2 = r->x1 + 15;
   r->y1 = w->screenrect.y2 - 3;
   r->y2 = w->screenrect.y2;

   r = &w->brdragrect;
   r->x1 = w->screenrect.x2 - 15;
   r->x2 = w->screenrect.x2;
   r->y1 = w->screenrect.y2 - 3;
   r->y2 = w->screenrect.y2;
}






void gWindowCalcScreen(gWidget *self)
{
   gContainerCalcScreen(self);
   gWindowSetRects((gWindow *)self);
}





static void arrangeButtons(gWindow *w)
{
   gButton *b;
   int x, y;
   unsigned int width, height;

   if(!w->id(gWindowID))
   {
      gSetError("arrangeButtons: self failed id check!\n");
      return;
   }

   width = w->buttonwidth;
   height = w->buttonheight;
   y = w->buttontopindent;
   x = w->buttonindent;

   if(b = w->xbutton)
   {
      b->setRect((gWidget *)b, x, y, width, height);
      x += width + w->buttonspacing;

      if(w->maxbutton || w->minbutton)
         b->setProcess(b, "buttonright", "buttonright_over", "buttonright_down");
      else
         b->setProcess(b, NULL, NULL, NULL);
   }

   if(b = w->maxbutton)
   {
      b->setRect((gWidget *)b, x, y, width, height);
      x += width + w->buttonspacing;

      if(w->xbutton && w->minbutton)
         b->setProcess(b, "buttonmid", "buttonmid_over", "buttonmid_down");
      else if(w->xbutton)
         b->setProcess(b, "buttonleft", "buttonleft_over", "buttonleft_down");
      else if(w->minbutton)
         b->setProcess(b, "buttonright", "buttonright_over", "buttonright_down");
      else
         b->setProcess(b, NULL, NULL, NULL);
   }

   if(b = w->minbutton)
   {
      b->setRect((gWidget *)b, x, y, width, height);
      x += width + w->buttonspacing;

      if(w->maxbutton || w->xbutton)
         b->setProcess(b, "buttonleft", "buttonleft_over", "buttonleft_down");
      else
         b->setProcess(b, NULL, NULL, NULL);
   }

   // Calculate the available space in the title bar for the title here
   gWindowSetRects(w);
}







void gWindowSetFactory(gWidget *self, gFactory *f)
{
   gWindow *w = (gWindow *)self;
   gSkinAttribute *att;

   if(!self->id(gWindowID))
   {
      gSetError("gWindowSetFactory: self failed id check!\n");
      return;
   }

   gContainerSetFactory(self, f);

   w->wproc = NULL;

   w->c_resizeh = w->c_resizenegative = w->c_resizepositive = 
   w->c_resizew = NULL;

   w->leftbuffer = w->rightbuffer = w->topbuffer = 
   w->bottombuffer = 0;
   w->tbarh = 0;

   w->topborder = w->bottomborder = w->leftborder = 
   w->rightborder = w->topbuffer = w->leftbuffer = 
   w->rightbuffer = w->bottombuffer = w->buttonindent = 0;

   w->buttonwidth = w->buttonheight = 18;
   w->buttonspacing = 0;

   if(f)
   {
      w->wproc = gFindProcess(f, "window");
      w->topproc = gFindProcess(f, "window_focus");

      if((att = gFindAttribute(f, w->wproc, "font", attFontSpec)) != NULL)
      {
         gFontSpec *fs = (gFontSpec *)att->data; 
         w->tfont = gFindFontSpec(f, fs);
         w->tfontc = gMapColor(f, &fs->fgcolor);
      }

      if((att = gFindAttribute(f, w->topproc, "focus_font", attFontSpec)) != NULL)
      {
         gFontSpec *fs = (gFontSpec *)att->data; 
         w->ttopfont = gFindFontSpec(f, fs);
         w->ttopfontc = gMapColor(f, &fs->fgcolor);
      }

      w->c_resizew = gFindCursor(f, "resize_width");
      w->c_resizeh = gFindCursor(f, "resize_height");
      w->c_resizenegative = gFindCursor(f, "resize_negative");
      w->c_resizepositive = gFindCursor(f, "resize_positive");
      

      if((att = gFindAttribute(f, w->wproc, "titlebar_height", attInteger)) != NULL)
         w->tbarh = abs(*((int *)att->data));

      if((att = gFindAttribute(f, w->wproc, "title_indent", attInteger)) != NULL)
         w->captionindent = abs(*(int *)att->data);

      if((att = gFindAttribute(f, w->wproc, "button_width", attInteger)) != NULL)
         w->buttonwidth = abs(*(int *)att->data);

      if((att = gFindAttribute(f, w->wproc, "button_height", attInteger)) != NULL)
         w->buttonheight = abs(*(int *)att->data);

      if((att = gFindAttribute(f, w->wproc, "button_indent", attInteger)) != NULL)
         w->buttonindent = abs(*(int *)att->data);

      if((att = gFindAttribute(f, w->wproc, "button_top_indent", attInteger)) != NULL)
         w->buttontopindent = abs(*(int *)att->data);

      if((att = gFindAttribute(f, w->wproc, "button_spacing", attInteger)) != NULL)
         w->buttonspacing = *(int *)att->data;


      if((att = gFindAttribute(f, w->wproc, "padding", attInteger)) != NULL)
         w->leftbuffer = w->rightbuffer = w->topbuffer = 
         w->bottombuffer = abs(*(int *)att->data);

      if((att = gFindAttribute(f, w->wproc, "top_padding", attInteger)) != NULL)
         w->topbuffer = abs(*(int *)att->data); 

      if((att = gFindAttribute(f, w->wproc, "bottom_padding", attInteger)) != NULL)
         w->bottombuffer = abs(*(int *)att->data); 

      if((att = gFindAttribute(f, w->wproc, "left_padding", attInteger)) != NULL)
         w->leftbuffer = abs(*(int *)att->data); 

      if((att = gFindAttribute(f, w->wproc, "right_padding", attInteger)) != NULL)
         w->rightbuffer = abs(*(int *)att->data); 


      if((att = gFindAttribute(f, w->wproc, "border", attInteger)) != NULL)
         w->leftborder = w->rightborder = w->topborder = 
         w->bottomborder = abs(*(int *)att->data); 

      if((att = gFindAttribute(f, w->wproc, "top_border", attInteger)) != NULL)
         w->topborder = abs(*(int *)att->data); 

      if((att = gFindAttribute(f, w->wproc, "bottom_border", attInteger)) != NULL)
         w->bottomborder = abs(*(int *)att->data); 

      if((att = gFindAttribute(f, w->wproc, "left_border", attInteger)) != NULL)
         w->leftborder = abs(*(int *)att->data); 

      if((att = gFindAttribute(f, w->wproc, "right_border", attInteger)) != NULL)
         w->rightborder = abs(*(int *)att->data); 

      w->topbuffer += w->tbarh;
   }


   gWindowSetRects(w);
   self->calcScreen(self);
   arrangeButtons(w);
}




gSkinCursor *gWindowGetCursor(gWidget *self, int mousex, int mousey)
{
   gWindow *w = (gWindow *)self;

   if(!self->id(gWindowID))
   {
      gSetError("gWindowGetCursor: self failed id check!\n");
      return NULL;
   }
   if(!self->factory)
      return NULL;

   if(w->maximized || !w->allowresize)
      return NULL;

   if(gMouseOnRect(mousex, mousey, &w->tldragrect) ||
      gMouseOnRect(mousex, mousey, &w->brdragrect) ||
      (w->drag & winDragResizeTop && w->drag & winDragResizeLeft) ||
      (w->drag & winDragResizeBottom && w->drag & winDragResizeRight))
   {
      return w->c_resizenegative;
   }
   else if(gMouseOnRect(mousex, mousey, &w->trdragrect) ||
           gMouseOnRect(mousex, mousey, &w->bldragrect) ||
           (w->drag & winDragResizeTop && w->drag & winDragResizeRight) ||
           (w->drag & winDragResizeBottom && w->drag & winDragResizeLeft))
   {
      return w->c_resizepositive;
   }
   else if(gMouseOnRect(mousex, mousey, &w->topdragrect) || 
           gMouseOnRect(mousex, mousey, &w->bottomdragrect) ||
          (w->drag & winDragResizeTop) ||
          (w->drag & winDragResizeBottom))
   {
      return w->c_resizeh;
   }
   else if(gMouseOnRect(mousex, mousey, &w->leftdragrect) ||
           gMouseOnRect(mousex, mousey, &w->rightdragrect) ||
          (w->drag & winDragResizeRight) ||
          (w->drag & winDragResizeLeft))
   {
      return w->c_resizew;
   }

   return NULL;
}



gWidget *gWindowGetTopWidget(gWidget *self, int x, int y)
{
   if(self->state & stateLeftDown)
      return self;

   return gContainerGetTopWidget(self, x, y);
}




void gInitWindow(gWindow *w)
{
   gInitContainer((gContainer *)w);

   w->id = gWindowID;
   w->drawWidget = gWindowDraw;
   w->eventHandler = gWindowEventHandler;
   w->setFactory = gWindowSetFactory;
   w->calcScreen = gWindowCalcScreen;
   w->getCursor = gWindowGetCursor;
   w->getTopWidget = gWindowGetTopWidget;
   w->deleteWidget = gWindowDelete;

   w->setBehavior((gWidget *)w, w->behavior | behaviorUpdateFocus);

   // Defaults
   w->buttonwidth = w->buttonheight = 18;
   w->buttontopindent = 4;
   w->minwidth = w->minheight = 128;
   w->allowresize = true;

   w->setButtons = gWindowSetButtons;
   w->allowResize = gWindowAllowResize;
   w->setCaption = gWindowSetCaption;
   w->xCallback = gWindowGetXCallback;
   w->maxCallback = gWindowGetMaxCallback;
   w->minCallback = gWindowGetMinCallback;
   w->setMaximized = gWindowSetMaximized;
}


gWindow *gNewWindow(int x, int y, unsigned int w, unsigned int h, const char *caption)
{
   gWindow *ret = malloc(sizeof(gWindow));
   memset(ret, 0, sizeof(gWindow));

   gInitWindow(ret);
   ret->setRect((gWidget *)ret, x, y, w, h);
   ret->setCaption(ret, caption);

   return ret;
}



void gWindowDelete(gWidget *self)
{
   gWindow *w = (gWindow *)self;

   if(!self->id(gWindowID))
   {
      gSetError("gWindowDelete: self failed id check!\n Using gWidgetDelete\n");
      gWidgetDelete(self);
      return;
   }

   // Just free the strings saved in the data struct before calling the normal
   // widget delete function.
   if(w->caption)
      free(w->caption);

   gContainerDelete(self);
}




void gWindowDefaultMaxCallback(gWidget *caller, gWidget *other)
{
   gWindow *w = (gWindow *)other;
   gButton *b = NULL;
   if(!gIsWindow(other))
      return;

   if(gIsButton(caller))
      b = (gButton *)caller;

   if(w->maximized)
   {
      other->setRect(other, w->maxrect.x, w->maxrect.y, w->maxrect.w, w->maxrect.h);
      w->maximized = false;

      if(b)
         b->setIcon(b, "window_max");
   }
   else
   {
      w->maximized = true;
      memcpy(&w->maxrect, &w->rect, sizeof(gRect));

      // SoM: TODO: Figure out the border width for the window
      if(other->prect)
      {
         other->setRect(other, -(int)w->leftborder, -(int)w->topborder, 
                        other->prect->x2 + 1 + w->leftborder + w->rightborder, 
                        other->prect->y2 + 1 + w->topborder + w->bottomborder);
      }

      if(b)
         b->setIcon(b, "window_win");
   }
}




gCallback *gWindowGetXCallback(gWindow *self)
{
   return self->xbutton ? &self->xbutton->click : NULL;
}




gCallback *gWindowGetMaxCallback(gWindow *self)
{
   return self->maxbutton ? &self->maxbutton->click : NULL;
}



gCallback *gWindowGetMinCallback(gWindow *self)
{
   return self->minbutton ? &self->minbutton->click : NULL;
}



void gWindowSetButtons(gWindow *self, bool min, bool max, bool x)
{
   if(min && !self->minbutton)
   {
      gButton *b = self->minbutton = gNewButton(0, 0, 0, 0, NULL);
      self->implantWidget((gContainer *)self, (gWidget *)b);
      b->setBehavior((gWidget *)b, b->behavior|behaviorFromRight);
      b->setIcon(b, "window_min");
   }


   if(max && !self->maxbutton)
   {
      gButton *b = self->maxbutton = gNewButton(0, 0, 0, 0, NULL);
      self->implantWidget((gContainer *)self, (gWidget *)b);
      b->setBehavior((gWidget *)b, b->behavior|behaviorFromRight);
      b->setIcon(b, "window_max");

      gSetWidgetCallback(&b->click, gWindowDefaultMaxCallback, (gWidget *)self);

      if(!self->allowresize)
         b->setBehavior((gWidget *)b, b->behavior | behaviorDisabled);
      else
         b->setBehavior((gWidget *)b, b->behavior & ~behaviorDisabled);
   }

   if(x && !self->xbutton)
   {
      gButton *b = self->xbutton = gNewButton(0, 0, 0, 0, NULL);
      self->implantWidget((gContainer *)self, (gWidget *)b);
      b->setBehavior((gWidget *)b, b->behavior | behaviorFromRight);
      b->setIcon(b, "window_x");
   }

   arrangeButtons(self);
}



void gWindowSetCaption(gWindow *self, const char *caption)
{
   if(self->caption)
      free(self->caption);

   if(caption)
      self->caption = strdup(caption);
   else
      self->caption = NULL;

   if(self->factory)
      gAddUpdateRect(self->factory, &self->tbarrect);
}




void gWindowAllowResize(gWindow *self, bool allowresize)
{
   gButton *b;

   self->allowresize = allowresize;

   b = self->maxbutton;

   if(!allowresize && b)
      b->setBehavior((gWidget*)b, b->behavior | behaviorDisabled);
   if(allowresize && b)
      b->setBehavior((gWidget*)b, b->behavior & ~behaviorDisabled);
}



void gWindowSetMaximized(gWindow *self, bool maximized)
{
   gCallback *call;

   if(!self->allowresize || maximized == self->maximized)
      return;

   if(!(call = self->maxCallback(self)))
      gWindowDefaultMaxCallback((gWidget *)self->maxbutton, (gWidget *)self);
   else
      gCall(call, (gWidget *)self->maxbutton);
}

