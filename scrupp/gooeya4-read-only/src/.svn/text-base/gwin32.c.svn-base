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


#ifdef WIN32

#include "gooeya4.h"
#include <windows.h>
#include <SDL_syswm.h>

// EW SYSTEM SPECIFIC CRAP
// All of it goes in here.
static HWND handle = NULL;



// SoM: Thanks Quasar!! :D
char *gGetClipboardText()
{
   HGLOBAL hMem;
   char *ret = NULL;

   if(handle == NULL)
   {
      // For windows, this function gets the HWND handle from SDL
      SDL_SysWMinfo info;

      if(SDL_GetWMInfo(&info))
      {
         gSetError("%s", SDL_GetError());
         return NULL;
      }

      handle = info.window;
   }

   if(!OpenClipboard(handle)) 
      return NULL;
    
   if((hMem = GetClipboardData(CF_TEXT)))
   {
      const char *lptr = GlobalLock(hMem);
      ret = strdup(lptr);
      GlobalUnlock(hMem);
   }

   CloseClipboard();

   return ret;
}




bool gCopyTextToClipboard(const char *text)
{
   int len;
   HGLOBAL    hglbCopy;
   char       *lockPtr;

   if(handle == NULL)
   {
      // For windows, this function gets the HWND handle from SDL
      SDL_SysWMinfo info;

      if(SDL_GetWMInfo(&info))
      {
         gSetError("%s", SDL_GetError());
         return false;
      }

      handle = info.window;
   }

   if(!text)
      return false;

   len = strlen(text) + 1;
   
   // open the clipboard
   if(!OpenClipboard(handle))
      return false;

   // empty any current contents
   if(!EmptyClipboard())
      return false;

   // allocate global memory object
   if((hglbCopy = GlobalAlloc(GMEM_SHARE | GMEM_MOVEABLE, len)) == NULL)
   {
      CloseClipboard();
      return false;
   }

   // lock the memory and copy the string to it
   lockPtr = (char *)GlobalLock(hglbCopy);
   strncpy(lockPtr, text, len);
   GlobalUnlock(hglbCopy);

   // throw this crap onto the clipboard
   SetClipboardData(CF_TEXT, hglbCopy);

   CloseClipboard();

   return true;
}

#endif
