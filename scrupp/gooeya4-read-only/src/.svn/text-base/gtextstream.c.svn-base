// Emacs style mode select -*- C++ -*-
// ----------------------------------------------------------------------------
//
// Copyright(C) 2009 Stephen McGranahan
//
// This file is part of gParse
//
// gParse is free software: you can redistribute it and/or modify
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
// gParse is a text file parsing system which operates with a few basic
// behaviors predefined, and others optionalized. This creates a token 
// generator that is at once very simple to use and works well for most
// situations.
//
// ----------------------------------------------------------------------------

#include "gtokenize.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// ----------------------------------------------------------------------------
// Text stream.
// The buffered text object allows the tokenizer to tokenize a pre-allocated 
// buffer or from an open file stream.


typedef struct
{
   // Memory buffers.
   // Simple, easy to manage.
   char           *memory;
   const char     *rover;
   bool           owner;

   char           *tempstr;
   int            buffermax;
} memStream;

typedef struct
{
   // File streams
   // A bit trickier. The input from a filestream needs to be buffered to
   // an extent, but not the entire file. The temp buffer starts at 10 chars
   // and expands each time gGetChars is called with a number > than the
   // buffer size.
   char           *fbuffer;
   char           *tempstr;
   int            buffermax;
   // fbufferlen is the length of the string inside the buffer.
   int            fbufferlen;

   // fbufferpos is the position in the filestream the buffer starts
   int            fbufferpos;

   int            fpos;
   FILE           *f;
} fileStream;



static gTextStream *newStream()
{
   gTextStream *ret = (gTextStream *)malloc(sizeof(gTextStream));
   memset(ret, 0, sizeof(*ret));
   return ret;
}


// ----------------------------------------------------------------------------
// File streams
// File streams are fairly complex little things that offer, among other things,
// buffered input of files. This makes accessing text from a file look similar
// to accessing text from memory.


// "Moves" a buffer based on the integer offset provided. If the offset is
// negative, the stream moves backwards. This function clips the movement to
// the first and last characters of the text file. After the offset, the code
// will shift the data in the buffer, and clear the newly exposed data.
void shiftFileBuffer(gTextStream *stream, int offset)
{
   fileStream  *fs = (fileStream *)stream->data;
   char        *buffer = fs->fbuffer; 
   int         size = fs->fbufferlen;
   int         i;


   if(fs->fbufferpos + offset < 0)
      offset = -fs->fbufferpos;

   fs->fbufferpos += offset;

   if(fs->fbufferpos >= stream->streamlen)
   {
      fs->fbuffer[0] = 0;
      fs->fbufferlen = 0;
      fs->fbufferpos = stream->streamlen;
      stream->eofflag = true;
      return;
   }

   if(fs->fbufferpos + size > stream->streamlen)
      fs->fbufferlen = stream->streamlen - fs->fbufferpos;

   if(offset == 0)
      return;
   if(abs(offset) >= size)
   {
      memset(buffer, 0, size);
      return;
   }

   if(offset > 0)
   {
      for(i = 0; i < size - offset; i++)
         buffer[i] = buffer[i + offset];

      for(; i < size; i++)
         buffer[i] = 0;
   }
   else
   {
      for(i = size - 1; i + offset >= 0; i--)
         buffer[i] = buffer[i + offset];

      for(; i >= 0; i--)
         buffer[i] = ' ';
   }
}



// This acts as a wrapper function to maintain consistency between the file
// pointer and the data contained in the struct. It is only used by internal
// functions.
static void fileSeek(gTextStream *stream, int offset, int type)
{
   fileStream  *fs = (fileStream *)stream->data;
   int         position = fs->fpos;

   if(type == SEEK_SET)
      position = offset;
   else if(type == SEEK_CUR)
      position += offset;
   else if(type == SEEK_END)
      position = stream->streamlen + offset;
   else
      return;

   if(position < 0)
   {
      position = 0;
      stream->eofflag = false;
   }
   else if(position > stream->streamlen)
   {
      position = stream->streamlen;
      stream->eofflag = true;
   }
   else
      stream->eofflag = false;

   fseek(fs->f, position, SEEK_SET);
   fs->fpos = ftell(fs->f);
}


// Acts as a wrapper for fread to maintain consistency between the FILE 
// pointer and internal data.
static int fileRead(void *data, int size, int count, gTextStream *stream)
{
   fileStream  *fs = (fileStream *)stream->data;
   int         ret;

   if(stream->eofflag)
      return 0;

   ret = fread(data, size, count, fs->f);
   fs->fpos += ret;
   return ret;
}



// This function returns the next character in the stream and 'pushes' the 
// input buffer ahead one character. Returns 0 on EOF
static char getCharFile(gTextStream *stream)
{
   fileStream  *fs = (fileStream *)stream->data;
   char        ret = 0;

   if(stream->eofflag)
      return 0;

   ret = fs->fbuffer[0];
   shiftFileBuffer(stream, 1);
   
   if(fs->fpos < stream->streamlen)
      fileRead(&fs->fbuffer[fs->fbufferlen - 1], 1, 1, stream);

   // An empty buffer means EOF
   if(fs->fbufferlen == 0)
   {
      stream->eofflag = true;
      return 0;
   }

   return ret;
}



// Returns the next character in the stream, but does not move the buffer.
static char readCharFile(gTextStream *stream)
{
   fileStream  *fs = (fileStream *)stream->data;
   char        ret = 0;

   if(stream->eofflag)
      return 0;

   ret = fs->fbuffer[0];

   if(fs->fbufferlen == 0)
      stream->eofflag = true;

   return ret;
}


// Attempts to read the next (count) characters into the file buffer. The 
// amount returned may be cut short if the end of the stream is encountered.
// A pointer to the null-terminated tempstring member is returned on success
// or NULL on EOF.
static char *readAheadFile(gTextStream *stream, unsigned int count)
{
   fileStream  *fs = (fileStream *)stream->data;
   int         toread;

   if(stream->eofflag)
      return NULL;

   // Check for EOF
   if(fs->fbufferpos >= stream->streamlen)
   {
      fs->fbufferlen = 0;
      fs->fbuffer[0] = 0;

      stream->eofflag = true;
      return NULL;
   }

   // Make sure the temp buffers are big enough for the requested data.
   if(fs->buffermax <= (int)count)
   {
      // read-ahead has exceeded buffer length, so increase buffer size.
      int newsize = count + 1;

      fs->fbuffer = (char *)realloc(fs->fbuffer, newsize);
      fs->tempstr = (char *)realloc(fs->tempstr, newsize);
      memset(fs->fbuffer + fs->buffermax, 0, newsize - fs->buffermax); 
      fs->buffermax = newsize;
   }

   // The characters needed might already be buffered. 
   toread = count - fs->fbufferlen;

   if(toread > 0)
   {
      // TODO: This should REALLY be done prior to the buffer re-allocation.
      // If the function is called requesting MAX_UINT characters, the function
      // will currently try to allocate that amount even if the file only has
      // one character left!!
      if(fs->fpos + toread >= stream->streamlen)
         toread = stream->streamlen - fs->fpos;

      fileRead(fs->fbuffer + fs->fbufferlen, 1, toread, stream);
      fs->fbuffer[fs->buffermax - 1] = 0;
   }

   if(fs->fbufferlen == 0)
   {
      stream->eofflag = true;
      return NULL;
   }

   // tempstr is returned because it is NULL-terminated.
   strncpy(fs->tempstr, fs->fbuffer, count);
   fs->tempstr[count] = 0;

   return fs->tempstr;
}




static void seekFile(gTextStream *stream, int offset)
{
   fileStream  *fs = (fileStream *)stream->data;
   int         pos = fs->fbufferpos;
   int         len;

   if(offset == 0 || (offset > 0 && stream->eofflag))
      return;

   shiftFileBuffer(stream, offset);

   if(offset < 0)
      offset = pos - fs->fbufferpos;
   else
      offset = fs->fbufferpos - pos;

   pos = fs->fbufferpos;

   if(pos >= stream->streamlen)
   {
      fileSeek(stream, 1, SEEK_END);
      fs->fbufferpos = stream->streamlen;
      fs->fbufferlen = 0;
      fs->fbuffer[0] = 0;
      stream->eofflag = true;
   }
   else
   {
      if(abs(offset) >= fs->fbufferlen)
      {
         int clamped;
         len = fs->buffermax - 1;
         if(fs->fbufferpos + len > stream->streamlen)
            len = stream->streamlen - fs->fbufferpos;

         if(fs->fpos != fs->fbufferpos)
            fileSeek(stream, fs->fbufferpos, SEEK_SET);
         
         if(!stream->eofflag)
         {
            clamped = fileRead(fs->fbuffer, 1, len, stream);

            fs->fbufferlen = clamped;
            fs->fbuffer[clamped] = 0;
         }

         if(fs->fbufferlen == 0)
            stream->eofflag = true;
      }
      else
      {
         if(offset < 0)
         {
            int readlen = -offset;

            if(fs->fpos != fs->fbufferpos)
               fileSeek(stream, fs->fbufferpos, SEEK_SET);

            fileRead(fs->fbuffer, 1, readlen, stream);
         }
         else
         {
            int keeplen = fs->fbufferlen - offset;
            int readlen = fs->buffermax - 1 - keeplen;
            int clamped;

            if(fs->fpos != fs->fbufferpos + keeplen)
               fileSeek(stream, fs->fbufferpos + keeplen, SEEK_SET);

            if(fs->fpos < stream->streamlen)
               clamped = fileRead(fs->fbuffer + keeplen, 1, readlen, stream);
            else
               clamped = 0;

            fs->fbufferlen = keeplen + clamped;

            if(fs->fbufferlen == 0)
               stream->eofflag = true;
         }

         fs->fbuffer[fs->fbufferlen] = 0;
      }
   }
}



static void freeStreamFile(gTextStream *stream)
{
   fileStream  *fs = (fileStream *)stream->data;

   if(fs->fbuffer)
      free(fs->fbuffer);

   if(fs->tempstr)
      free(fs->tempstr);

   fclose(fs->f);

   free(fs);

   free(stream);
}




gTextStream *gStreamFromFile(FILE *file)
{
   gTextStream *ret;
   fileStream  *fs;

   if(!file)
      return NULL;

   ret = newStream();

   ret->data = (fs = malloc(sizeof(fileStream)));
   memset(fs, 0, sizeof(*fs));

   fs->f = file;

   fseek(file, 0, SEEK_END);
   ret->streamlen = ftell(file);
   fseek(file, 0, SEEK_SET);

   fs->buffermax = 10;
   fs->fbuffer = malloc(sizeof(char) * fs->buffermax);
   fs->tempstr = malloc(sizeof(char) * fs->buffermax);
   fs->tempstr[0] = 0;

   if(ret->streamlen < 9)
      fs->fbufferlen = fread(fs->fbuffer, 1, ret->streamlen, file);
   else
      fs->fbufferlen = fread(fs->fbuffer, 1, 9, file);

   fs->fbuffer[fs->fbufferlen] = 0;

   fs->fbufferpos = 0;
   fs->fpos = ftell(file);

   ret->ggetchar = getCharFile;
   ret->readchar = readCharFile;
   ret->seek = seekFile;
   ret->readahead = readAheadFile;
   ret->freestream = freeStreamFile;

   return ret;
}


gTextStream *gStreamFromFilename(const char *filename)
{
   FILE *f = fopen(filename, "rb");

   if(!f)
      return NULL;

   return gStreamFromFile(f);
}




// ----------------------------------------------------------------------------
// Memory streams

static char getCharMemory(gTextStream *stream)
{
   memStream   *sd = (memStream *)stream->data;
   char        ret = 0;

   if(stream->eofflag)
      return 0;

   ret = *sd->rover;
   sd->rover ++;

   if(sd->rover - sd->memory >= stream->streamlen)
      stream->eofflag = true;

   return ret;
}


static char readCharMemory(gTextStream *stream)
{
   memStream   *sd = (memStream *)stream->data;
   char        ret = 0;

   if(stream->eofflag)
      return 0;

   if(sd->rover - sd->memory >= stream->streamlen)
      stream->eofflag = true;
   else
      ret = *sd->rover;

   return ret;
}


static char *readAheadMemory(gTextStream *stream, unsigned int count)
{
   memStream   *sd = (memStream *)stream->data;

   if(stream->eofflag)
      return NULL;

   if(sd->buffermax <= (int)count)
   {
      int newsize = count + 1;
      sd->tempstr = (char *)realloc(sd->tempstr, newsize);
      memset(sd->tempstr, 0, newsize);
      sd->buffermax = newsize;
   }
   
   if((sd->rover - sd->memory) >= stream->streamlen)
   {
      stream->eofflag = true;
      return NULL;
   }

   if((sd->rover - sd->memory) + (int)count >= stream->streamlen)
      count = stream->streamlen - (sd->rover - sd->memory);

   strncpy(sd->tempstr, sd->rover, count);
   sd->tempstr[count] = 0;

   return sd->tempstr;
}


static void seekMemory(gTextStream *stream, int offset)
{
   memStream   *sd = (memStream *)stream->data;
   const char  *base = sd->memory, *marker = sd->rover;

   if(offset == 0 || (offset > 0 && stream->eofflag))
      return;

   if(offset < 0 && marker + offset < base)
   {
      sd->rover = sd->memory;
      stream->eofflag = stream->streamlen > 0 ? false : true;
   }
   else if(marker + offset >= base + stream->streamlen)
   {
      sd->rover = sd->memory + stream->streamlen;
      stream->eofflag = true;
   }
   else
   {
      sd->rover += offset;
      stream->eofflag = false;
   }
}


static void freeStreamMemory(gTextStream *stream)
{
   memStream   *sd = (memStream *)stream->data;

   if(sd->owner)
      free(sd->memory);

   if(sd->tempstr)
      free(sd->tempstr);
     
   free(sd);
   free(stream);
}



gTextStream *gStreamFromMemory(char *memory, int length, bool owner)
{
   memStream   *sd;
   gTextStream *ret;


   if(!memory || length < 0)
      return NULL;

   ret = newStream();

   ret->data = (sd = malloc(sizeof(memStream)));
   memset(sd, 0, sizeof(sd));

   ret->streamlen = length;

   sd->memory = memory;
   sd->rover = memory;
   sd->owner = owner;

   sd->buffermax = 10;
   sd->tempstr = malloc(sizeof(char) * sd->buffermax);
   sd->tempstr[0] = 0;

   ret->ggetchar = getCharMemory;
   ret->readchar = readCharMemory;
   ret->seek = seekMemory;
   ret->readahead = readAheadMemory;
   ret->freestream = freeStreamMemory;

   return ret;
}



// ----------------------------------------------------------------------------
// Generic stream functions.

// gSeekPos
// Seeks an absolute position within the stream
void gSeekPos(gTextStream *txtstrm, int pos)
{
   int offset;

   if(pos < 0)
      pos = 0;
   if(pos >= txtstrm->streamlen)
      pos = txtstrm->streamlen - 1;

   if(txtstrm->seek = seekMemory)
   {
      memStream *sd = (memStream *)txtstrm->data;
      offset = pos - (sd->rover - sd->memory);
   }
   else
   {
      fileStream *fs = (fileStream *)txtstrm->data;
      offset = pos - fs->fbufferpos;
   }

   gSeek(txtstrm, offset);
}


// gStreamEnd
// Returns true of the stream has reached the end of the buffer/file.
bool gStreamEnd(gTextStream *txtstrm)
{
   return txtstrm->eofflag;
}
