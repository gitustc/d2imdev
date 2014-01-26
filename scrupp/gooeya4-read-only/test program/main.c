// Emacs style mode select -*- C++ -*-
// ----------------------------------------------------------------------------
//
// Copyright(C) 2009 Stephen McGranahan
//
// This program is free software: you can redistribute it and/or modify
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
// This is a simple test program designed to demonstrate use of the Gooey a4 
// API.
//
// ----------------------------------------------------------------------------

#include "gooeya4.h"
#include "gmainframe.h"
#include "gwindow.h"
#include "glabel.h"
#include "gtextbox.h"

// Widgets the program is going to use
gMainFrame *mf;
gWindow *window;
gLabel *label;
gButton *button;
gTextBox *tbox;


static bool buttonClick = false;


void buttonDown(gWidget *w, gSignal *sig)
{
   if(!buttonClick)
   {
      buttonClick = true;
      label->setCaption(label, "Label frozen\n");
      button->setCaption(button, "Unfreeze Label");
   }
   else
   {
      buttonClick = false;
      label->setCaption(label, "Label unfrozen\n");
      button->setCaption(button, "Freeze Label");
   }
}


void labelHover(gWidget *w, gSignal *sig)
{
   gLabel *lbl = (gLabel *)w;

   if(!gIsLabel(w) || buttonClick)
      return;

   lbl->setCaption(lbl, "Hey, get that thing off me!\n");
   window->setCaption(window, "The mouse is over the gLabel!");
}


void labelNoHover(gWidget *w, gSignal *sig)
{
   gLabel *lbl = (gLabel *)w;

   if(!gIsLabel(w) || buttonClick)
      return;

   lbl->setCaption(lbl, "Phew! That was close.");
   window->setCaption(window, "Gooey a4 Test Program");
}



gWidget *setupWidgets(gFactory *f)
{
   mf = gNewMainFrame(0, 0, f->dest->w, f->dest->h);
   mf->setFactory((gWidget *)mf, f);
   mf->setParentRect((gWidget *)mf, &f->screenrect);
   mf->updatebg = true;

   window = gNewWindow(100, 100, 320, 240, "Gooey a4 Test Program");
   mf->addChild((gContainer *)mf, (gWidget *)window);

   /*label = gNewLabel(0, 0, 250, 20, "Welcome to the Gooey a4 test application!");
   gAddResponder((gWidget *)label, "mouseEnter", labelHover);
   gAddResponder((gWidget *)label, "mouseLeave", labelNoHover);
   window->addChild((gContainer *)window, (gWidget *)label);

   button = gNewButton(0, 0, 100, 24, "Freeze Label");
   button->setBehavior((gWidget *)button, button->behavior | behaviorFromRight | behaviorFromBottom);
   gAddResponder((gWidget *)button, "mouseClick", buttonDown);
   window->addChild((gContainer *)window, (gWidget *)button);

   tbox = gNewTextBox(0, 32, 100, 24, NULL);
   window->addChild((gContainer *)window, (gWidget *)tbox);*/

   return (gWidget *)mf;
}


int main(int argc, char **argv)
{
   gSurface       *screen;
   gSkinTemplate  *stemp;
   gFactory       *factory;
   gWidget        *w;


   gOpenLog(NULL, false, true);

   if(!gInit(SDL_INIT_NOPARACHUTE, SDL_DEFAULT_REPEAT_DELAY, 
             SDL_DEFAULT_REPEAT_INTERVAL, argv[0]))
      return -1;

   // Read the skin template.
   stemp = gCreateSkinTemplate();
   if(!gReadSkinArchive("newskin.zip", stemp))
      return -1;

   if(!(screen = SDL_SetVideoMode(900, 600, 32, 0)))
      return -1;

   // Create the factory which handles rendering the widgets.
   factory = gCreateFactory(stemp, screen);

   // Don't need the skin template anymore.
   gFreeSkinTemplate(stemp);

   // Setup the widgets
   w = setupWidgets(factory);

   // Run the event loop
   gEventLoop(factory, w);
   
   // Program has ended. Clean up.
   w->deleteWidget(w);
   gFreeFactory(factory);

   return 0;
}