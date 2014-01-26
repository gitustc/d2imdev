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
#include <stdio.h>

// ----------------------------------------------------------------------------
// Logging. Including log files and console output through a single interface

static FILE *logfile = NULL;
static bool consoleout = false;



void gCloseLogFile(void)
{
   if(logfile)
      fclose(logfile);
}


void gStopScreenLog(void)
{
   consoleout = false;
}


bool gOpenLog(char *filename, bool append, bool print)
{
   consoleout = filename ? print : true;

   if(filename)
   {
      // Open Log file
      if(logfile)
         gCloseLogFile();

      if(!(logfile = fopen(filename, append ? "wa" : "w")))
      {
         gSetError("Failed to open log file %s\n", filename);
         return false;
      }
   }

   return true;
}


void gLog(const char *str)
{
   if(logfile)
      fprintf(logfile, str);

   if(consoleout)
      printf(str);
}


void gLogf(const char *fmt, ...)
{
   static char buf[1025];
   va_list     argptr;

   va_start  (argptr, fmt);
#ifdef _MSC_VER 
   _vsnprintf (buf, 1024, fmt, argptr);
#else
   vsnprintf (buf, 1024, fmt, argptr);
#endif
   va_end    (argptr);

   gLog(buf);
}


// ----------------------------------------------------------------------------
// Error handling

static char errorstr[1026] = "";
static bool iserror = false;


void gSetError(const char *fmt, ...)
{
   int         len;
   va_list     argptr;

   va_start  (argptr, fmt);
#ifdef _MSC_VER 
   _vsnprintf (errorstr, 1024, fmt, argptr);
#else
   vsnprintf (errorstr, 1024, fmt, argptr);
#endif
   va_end    (argptr);

   len = strlen(errorstr);
   if(errorstr[len - 1] != '\n')
   {
      errorstr[len] = '\n';
      errorstr[len + 1] = 0;
   }

   gLog(errorstr);
   iserror = true;
}



const char *gGetLastError(void)
{
   return errorstr;
}



void gResetError(void)
{
   errorstr[0] = 0x0;
   iserror = false;
}



bool gCheckError(void)
{
   return iserror;
}