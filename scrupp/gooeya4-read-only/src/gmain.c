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


bool gInit (int sdlflags, int repeat_delay, int repeat_interval, const char *argv0)
{
   if(SDL_Init(sdlflags|SDL_INIT_VIDEO|SDL_INIT_TIMER) == -1)
   {
      gLogf("SDL_Init failed: %s\n", SDL_GetError());
      return false;
   }

   SDL_EnableUNICODE(1);
   SDL_EnableKeyRepeat(repeat_delay, repeat_interval);

   if(TTF_Init()==-1) 
   {
       gLogf("TTF_Init failed: %s\n", TTF_GetError());
       return false;
   }

   if(!PHYSFS_init(argv0))
   {
      gLogf("PHYSFS_init failed: %s\n", PHYSFS_getLastError());
      return false;
   }

   return true;
}




bool gEventLoop(gFactory *f, gWidget *mainwidget)
{
   SDL_Event   ev;

   while(1)
   {
      bool exit = false;
      int x, y;
      gWidget *top;

      // Dispatch queued messages
      while(SDL_PollEvent(&ev))
      {

         if(ev.type == SDL_QUIT)
         {
            exit = true;
            break;
         }
         else
         {
            bool claimed = false;

            mainwidget->eventHandler(mainwidget, &ev, &claimed);
         }
      }

      if(exit)
         break;

      // Update the mouse cursor
      SDL_GetMouseState(&x, &y);

      if((top = mainwidget->getTopWidget(mainwidget, x, y)))
         gSetCursor(f, top->getCursor(top, x, y));
      else
         gSetCursor(f, NULL);

      // Run timers
      gRunTimers();

      // Update?
      if(f->updateflag)
         gPerformUpdate(f, mainwidget);

      // Delay
      SDL_Delay(10);
   }

   return 0;
}