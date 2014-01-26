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
#include "gtextbox.h"


// ----------------------------------------------------------------------------
// Member functions


bool gIsTextBox(gWidget *w)
{
   return w ? w->id(gTextBoxID) : false;
}


bool gTextBoxID(gIDFuncParam func)
{
   if(func == gTextBoxID)
      return true;
   return gWidgetID(func);
}




static void clearSelection(gTextBox *self)
{
   M_QStrDeleteRange(self->text, self->selectstart, self->selectend - self->selectstart);
   self->cpos = self->selectend = self->selectbase = self->selectstart;
}



static void startSelection(gTextBox *self, unsigned int pos)
{
   self->selectstart = self->selectend = self->selectbase = self->cpos = pos;
}



static void moveSelection(gTextBox *self, unsigned int pos)
{
   self->cpos = pos;

   if(pos < self->selectbase)
   {
      self->selectstart = pos;
      self->selectend = self->selectbase;
   }
   else if(pos > self->selectbase)
   {
      self->selectstart = self->selectbase;
      self->selectend = pos;
   }
   else
      self->selectstart = self->selectend = pos;
}



static void adjustOffset(gTextBox *self)
{
   gFont    *font = self->font;

   if(font)
   {
      int xpos, ypos;

      gCursorPos(font, self->text->buffer, self->cpos, &xpos, &ypos);

      xpos -= self->xoffset;
      ypos -= self->yoffset;

      if(xpos < 0)
         self->xoffset += xpos;
      if(xpos > (self->rect.w - self->padding * 2))
         self->xoffset += xpos - (self->rect.w - self->padding * 2);
   }
}



void gTextBoxDraw(gWidget *self)
{
   gTextBox     *tb = (gTextBox *)self;
   gFactory     *f;
   bool         focus;
   gFont        *font;
   gSkinProcess *proc;
   Uint32       color;
   gScreenRect  iclip;

   if(!self->id(gTextBoxID))
   {
      gSetError("gTextBoxDraw: self failed id check!\n");
      return;
   }
   if((self->behavior & behaviorInvisible) || !self->factory)
      return;

   f = self->factory;
   focus = !!(self->state & stateHasFocus);
   font = tb->font;
   color = tb->color;

   if(focus)
      proc = tb->fproc ? tb->fproc : tb->proc;
   else
      proc = tb->proc ? tb->proc : tb->fproc;

   iclip.x1 = tb->screenrect.x1 + tb->borderw;
   iclip.x2 = tb->screenrect.x2 - tb->borderw;
   iclip.y1 = tb->screenrect.y1 + tb->borderw;
   iclip.y2 = tb->screenrect.y2 - tb->borderw;

   if(gPushClippingStackInside(f, &iclip, false))
   {
      if(tb->text->size > 0 && font)
      {
         gScreenRect r;
         unsigned int w = gStringWidth(font, tb->text->buffer);
         unsigned int h = gStringHeight(font, tb->text->buffer);

         r.x1 = tb->screenrect.x1 + tb->padding - tb->xoffset;
         r.x2 = r.x1 + w;
         r.y1 = ((tb->screenrect.y1 + tb->screenrect.y2) / 2) - (h / 2) + ((font->height - font->ascent) / 2) - tb->yoffset;
         r.y2 = r.y1 + h;

         gRenderText(f, font, &r, tb->text->buffer, color);
      }

      if(font && tb->bar && focus)
      {
         gRect r;
         int xpos, ypos;

         gCursorPos(font, tb->text->buffer, tb->cpos, &xpos, &ypos);

         r.x = tb->screenrect.x1 + tb->padding 
             + xpos - (tb->bar->graphic->width >> 1) - tb->xoffset;

         r.w = tb->bar->graphic->width;

         r.y = ((tb->screenrect.y1 + tb->screenrect.y2 + 1) / 2) - (font->height / 2) - tb->yoffset;
         r.h = font->height;

         gRenderGraphic(f, tb->bar->graphic, 0, 0, r.x, r.y, r.w, r.h, 0);

         if(tb->sproc && tb->selectstart != tb->selectend)
         {
            int xend, yend;

            if(tb->cpos == tb->selectend)
            {
               xend = xpos;
               yend = ypos;

               gCursorPos(font, tb->text->buffer, tb->selectstart, &xpos, &ypos);
            }
            else if(tb->cpos == tb->selectstart)
            {
               gCursorPos(font, tb->text->buffer, tb->selectend, &xend, &yend);
            }
            else
            {
               // Neither matches??
               gCursorPos(font, tb->text->buffer, tb->selectstart, &xpos, &ypos);
               gCursorPos(font, tb->text->buffer, tb->selectend, &xend, &yend);
            }

            r.x = tb->screenrect.x1 + tb->padding + xpos - tb->xoffset;
            r.y = tb->screenrect.y1 + tb->padding + ypos - tb->yoffset;
            r.w = xend - xpos + 1;
            r.h = yend - ypos + 1 + font->height;

            tb->sproc->func(f, tb->sproc, r.x, r.y, r.w, r.h);
         }
      }
      gPopClippingStack(f);
   }

   proc = (focus && tb->fproc) ? tb->fproc : tb->proc;
   if(proc)
      proc->func(f, proc, tb->screenrect.x1, 
                 tb->screenrect.y1, tb->rect.w, tb->rect.h);
}



void gTextBoxEventHandler(gWidget *self, SDL_Event *event, bool *claimed)
{
   gTextBox  *tb = (gTextBox *)self;
   int prestate = self->state;
   bool flag;
   bool update = false;

   if(!self->id(gTextBoxID))
   {
      gSetError("gTextBoxEventHandler: self failed id check!\n");
      return;
   }
   if(self->behavior & behaviorNoEvent)
      return;

   flag = *claimed;
   gWidgetEventHandler(self, event, claimed);

   if(flag)
      return;

   if(event->type == SDL_MOUSEBUTTONDOWN && self->state & stateMouseOver && tb->font)
   {
      int x, y;

      x = event->button.x - self->screenrect.x1 + tb->xoffset;
      y = event->button.y - self->screenrect.y1 + tb->yoffset;

      startSelection(tb, gGetStringIndex(tb->font, tb->text->buffer, x, y));

      update = true;
      *claimed = true;
   }
   if(event->type == SDL_MOUSEMOTION && self->state & stateLeftDown && tb->font)
   {
      int x, y;

      x = event->motion.x - self->screenrect.x1 + tb->xoffset;
      y = event->motion.y - self->screenrect.y1 + tb->yoffset;

      moveSelection(tb, gGetStringIndex(tb->font, tb->text->buffer, x, y));

      update = true;
      *claimed = true;
   }

   if(event->type == SDL_KEYDOWN && self->state & stateHasFocus)
   {
      bool textchange = false;
      bool selection = (tb->selectstart != tb->selectend);

      Uint16 c = event->key.keysym.unicode;

      if(c && strchr(tb->validchars, c))
      {
         if(selection)
            clearSelection(tb);

         if(tb->maxlen > tb->text->index)
         {
            M_QStrInsertc(tb->text, (char)c, tb->cpos);
            tb->cpos++;
         }

         update = textchange = true;
      }
      else
      {
         switch(event->key.keysym.sym)
         {
            case SDLK_BACKSPACE:
               if(selection)
               {
                  clearSelection(tb);
                  update = textchange = true;
               }
               else if(tb->cpos > 0)
               {
                  M_QStrDeletec(tb->text, tb->cpos - 1);
                  tb->cpos--;
                  update = textchange = true;
               }
               break;
            case SDLK_DELETE:
               if(selection)
               {
                  clearSelection(tb);
                  update = textchange = true;
               }
               else if(tb->cpos < tb->text->index)
               {
                  M_QStrDeletec(tb->text, tb->cpos);
                  update = textchange = true;
               }

               break;
            case SDLK_LEFT:
               if(event->key.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL))
               {
                  while(tb->cpos > 0)
                  {
                     tb->cpos --;
                     if(tb->text->buffer[tb->cpos - 1] == ' ')
                        break;
                  }
               }
               else if(tb->cpos > 0)
                  tb->cpos --;

               if(event->key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT))
                  moveSelection(tb, tb->cpos);
               else
                  startSelection(tb, tb->cpos);

               update = true;
               break;
            case SDLK_RIGHT:
               if(event->key.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL))
               {
                  while(tb->cpos < tb->text->index)
                  {
                     if(tb->text->buffer[tb->cpos] != ' ')
                     {
                        tb->cpos++;
                        continue;
                     }

                     tb->cpos++;
                     break;
                  }
               }
               else if(tb->cpos < tb->text->index)
                  tb->cpos++;

               if(event->key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT))
                  moveSelection(tb, tb->cpos);
               else
                  startSelection(tb, tb->cpos);
               update = true;
               break;
            case SDLK_HOME:
               if(event->key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT))
                  moveSelection(tb, 0);
               else
                  startSelection(tb, 0);

               update = true;
               break;
            case SDLK_END:
               if(event->key.keysym.mod & (KMOD_LSHIFT | KMOD_RSHIFT))
                  moveSelection(tb, tb->text->index);
               else
                  startSelection(tb, tb->text->index);
               
               update = true;
               break;
         }
      }

      if(textchange)
      {
         gCall(&tb->textcall, self);
      }
      *claimed = true;
   }

   if(update)
   {
      adjustOffset(tb);
      self->update(self);
   }

}


void gTextBoxSetFactory(gWidget *self, gFactory *f)
{
   gTextBox *tb = (gTextBox *)self;
   self->factory = f;

   if(!self->id(gTextBoxID))
   {
      gSetError("gTextBoxSetFactory: self failed id check!\n");
      return;
   }

   if(f)
   {
      gSkinAttribute *att;

      tb->proc = gFindProcess(f, "textbox");
      tb->fproc = gFindProcess(f, "textbox_focus");

      if((att = gFindAttribute(f, tb->proc, "font", attFontSpec)) != NULL)
      {
         gFontSpec *spec = (gFontSpec *)att->data;

         tb->font = gFindFont(f, spec->name, spec->ptsize, spec->style);
         tb->color = gMapColor(f, &spec->fgcolor);
      }
      else
      {
         tb->font = NULL;
         tb->color = 0;
      }

      if((att = gFindAttribute(f, tb->proc, "selection_color", attColor)) != NULL)
      {
         SDL_Color *c = (SDL_Color *)att->data;
         tb->scolor = gMapColor(f, c);
      }
      else
         tb->scolor = 0;

      if((att = gFindAttribute(f, tb->proc, "padding", attInteger)) != NULL)
         tb->padding = *((int *)att->data);
      else
         tb->padding = 4;

      if((att = gFindAttribute(f, tb->proc, "border_width", attInteger)) != NULL)
         tb->borderw = *((int *)att->data);
      else
         tb->borderw = 1;


      tb->cursor = gFindCursor(f, "ibeam");
      tb->bar = gFindIcon(f, "textbar");
      tb->sproc = gFindProcess(f, "selection");
   }
   else
   {
      tb->proc = tb->fproc = tb->sproc = NULL;
      tb->font = NULL;
      tb->cursor = NULL;
      tb->bar = NULL;
   }
}


gWidget *gTextBoxGetTopWidget(gWidget *self, int x, int y)
{
   if(self->state & stateLeftDown)
      return self;

   return gWidgetGetTopWidget(self, x, y);
}


gSkinCursor *gTextBoxGetCursor(gWidget *self, int mousex, int mousey)
{
   gTextBox *tb = (gTextBox *)self;

   if(!self->id(gTextBoxID))
   {
      gSetError("gTextBoxGetCursor: self failed id check!\n");
      return NULL;
   }

   return tb->cursor;
}


void gTextBoxDelete(gWidget *self)
{
   gTextBox *tb = (gTextBox *)self;

   if(!self->id(gTextBoxID))
   {
      gSetError("gTextBoxDelete: self failed id check!\n Using gWidgetDelete\n");
      gWidgetDelete(self);
      return;
   }

   if(tb->text)
   {
      M_QStrFree(tb->text);
      free(tb->text);
   }

   if(tb->validchars)
      free(tb->validchars);
   
   gWidgetDelete(self);
}


// ----------------------------------------------------------------------------
// Non-member functions

static char *default_validchars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-=\\`~!@#$%^&*()_+|[]{};':\",./<>?";


void gInitTextBox(gTextBox *tb)
{
   gInitWidget((gWidget *)tb);
   tb->id = gTextBoxID;
   tb->eventHandler = gTextBoxEventHandler;
   tb->drawWidget = gTextBoxDraw;
   tb->setFactory = gTextBoxSetFactory;
   tb->getTopWidget = gTextBoxGetTopWidget;
   tb->getCursor = gTextBoxGetCursor;
   tb->deleteWidget = gTextBoxDelete;

   tb->behavior |= behaviorUpdateFocus | behaviorKeepFocus;

   tb->padding = 4;
   tb->maxlen = 100;

   tb->validchars = strdup(default_validchars);
   tb->text = malloc(sizeof(qstring_t));
   M_QStrInitCreate(tb->text);

   tb->setText = gTextBoxSetText;
   tb->setMax = gTextBoxSetMaxLength;
   tb->setValidchars = gTextBoxSetValidchars;
}


// gNewTextBox
// Creates and initializes a new button widget.
gTextBox *gNewTextBox(int x, int y, unsigned int w, unsigned int h, const char *text)
{
   gTextBox *ret = malloc(sizeof(gTextBox));
   memset(ret, 0, sizeof(*ret));
   gInitTextBox(ret);

   ret->setRect((gWidget *)ret, x, y, w, h);
   ret->setText(ret, text);

   return ret;
}


// gTextBoxSetText
// Sets the text in the box.
void gTextBoxSetText(gTextBox *self, const char *text)
{
   gFactory *f = self->factory;

   if(!text)
      M_QStrClear(self->text);
   else
   {
      unsigned int len = strlen(text);

      if(len > self->maxlen)
      {
         if(self->text->size <= self->maxlen)
            M_QStrGrow(self->text, self->maxlen - self->text->size + 1);

         memcpy(self->text->buffer, text, self->maxlen);
      }
      else
         M_QStrSet(self->text, text);
   }

   self->update((gWidget *)self);
}



void gTextBoxSetValidchars(gTextBox *box, const char *validchars)
{
   gFactory     *f;
   int          i;
   qstring_t    *text;

   f = box->factory;

   if(box->validchars)
   {
      free(box->validchars);
      box->validchars = NULL;
   }

   if(validchars)
      box->validchars = strdup(validchars);
   else
      box->validchars = strdup(default_validchars);

   text = box->text;

   for(i = text->index - 1; i >= 0; i--)
   {
      if(!strchr(box->validchars, text->buffer[i]))
         M_QStrDeletec(text, i);
   }

   box->update((gWidget *)box);
}



void gTextBoxSetMaxLength(gTextBox *self, unsigned int max)
{
   self->maxlen = max;

   if(self->text->index >= max)
      M_QStrDeleteRange(self->text, max, self->text->index - max + 1);

   adjustOffset(self);
}