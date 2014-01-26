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


#ifndef GOOEY_H
#define GOOEY_H

#include <SDL.h>
#include <physfs.h>
#include <SDL_Image.h>

#include "gbool.h"
#include "glist.h"


// Basic elements used in the interface
typedef SDL_Rect gRect;

typedef struct gScreenRect_s
{
   int x1, y1, x2, y2;
} gScreenRect;



// ----------------------------------------------------------------------------
// Logging. Including log files and console output through a single interface

// gCloseLogFile
// Closes the currently open log file (if any)
void gCloseLogFile(void);


// gStopScreenLog
// Stops log messages from being printed in the console window.
void gStopScreenLog(void);


// gOpenLog
// Optionally opens a log file and begins console output of log messages.
// If this function is never called, or if gOpenLog is called with a NULL file
// name and false for print output, any calls to gLog will not actually
// log anything. It is recommended this function be called to at least turn
// on console output at the start of the program.
bool gOpenLog(char *filename, bool append, bool print);


// gLog
// Logs a message via whatever streams are open.
void gLog(const char *str);


// gLogf
// Same as gLog but for a formatted string.
void gLogf(const char *fmt, ...);



// ----------------------------------------------------------------------------
// Error handling

// gSetError
// Sets the error string for Gooey. Max message length of 1024 characters.
// This also calls gLog with the resulting error message.
void gSetError(const char *fmt, ...);

// gGetLastError
// Returns the last set error string.
const char *gGetLastError(void);

// gResetError
// Used to set the last error string back to NULL.
void gResetError(void);

// gCheckError
// returns true if gSetError has been called and there is a pending error
bool gCheckError(void);



// System specific stuff
char *gGetClipboardText(void);
bool gCopyTextToClipboard(const char *text);


// Init and shutdown functions:
// Initializes SDL and starts the following subsystems:
// - Video
// - Timers
//
// Unicode characters are also enabled and the key repeat rate is set.
bool gInit (int sdlflags, int repeat_delay, int repeat_interval, const char *argv0);


typedef struct gWidget gWidget;
typedef struct gContainer gContainer;

#include "gvideo.h"
#include "gevent.h"
#include "gtokenize.h"
#include "gfactory.h"

#include "gfoundation.h"

#include "physfsrwops.h"


// Basic event loop
bool gEventLoop(gFactory *f, gWidget *mainwidget);

#endif //GOOEY_H

