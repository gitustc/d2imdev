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


#ifndef GTEXTSTREAM_H
#define GTEXTSTREAM_H

/**
 * \file gtextstream.h
 * \brief Text input stream abstraction.
 *
 * gTextStream is an abstraction which allows the tokenizer to access text from
 * a file or memory through the same buffered interface.
*/

#ifdef __cplusplus
extern "C"
{
#endif


// ----------------------------------------------------------------------------
// Text stream
// The buffered text object allows the tokenizer to tokenize a pre-allocated 
// buffer or from an open file stream.


/**
 * \struct gTextStream
 * \brief Text streaming abstraction.
 *
 * gTextStream provides a single interface for the tokenizer to stream text
 * from either a file or memory. Most fields are for internal use only. 
 * the management functions should really always be used with them.
 * \see  gtextstream.h::gStreamFromMemory, gtextstream.h::gStreamFromFilename,
 *       gtextstream.h::gFreeStream, gtextstream.h::gGetChar,
 *       gtextstream.h::gReadChar, gtextstream.h::gReadahead,
 *       gtextstream.h::gSeekPos, gtextstream.h::gSeek,
 *       gtextstream.h::gStreamEnd
*/
typedef struct gTextStream
{
#ifndef DOXYGEN_IGNORE
   bool           eofflag;
   int            streamlen;

   // Stream data (used internally)
   void           *data;

   // Function pointers for the various stream modes
   char           (*ggetchar)(struct gTextStream *);
   char           (*readchar)(struct gTextStream *);
   char*          (*readahead)(struct gTextStream *, unsigned int);
   void           (*seek)(struct gTextStream *, int);
   void           (*freestream)(struct gTextStream *);
#endif
} gTextStream;


/**
 * \fn gTextStream *gStreamFromMemory(char *memory, int length, bool owner)
 * \brief gTextStream from a memory buffer.
 *
 * Creates a text stream from a memory buffer. It is important to pass an 
 * accurate length value because a memory stream only has this value to find
 * the end of the stream.
 *
 * @param[in] memory The memory buffer to stream from
 * @param[in] length Length of the memory buffer.
 * @param[in] owner If set to true, gFreeStream will free the memory buffer.
 * @returns New gTextStream, NULL on failure
*/
gTextStream *gStreamFromMemory(char *memory, int length, bool owner);


/**
 * \fn gTextStream *gStreamFromFilename(const char *filename)
 * \brief gTextStream from a filename.
 *
 * Opens a file and creates a gTextStream from the handle.
 * 
 * @param[in] filename Name of the file to open and stream.
 * @return New gTextStream on success, NULL on failure (failed to open file)
*/
gTextStream *gStreamFromFilename(const char *filename);

/**
 * \def gFreeStream(txtstrm)
 * \brief Frees a gTextStream
 *
 * Frees \a txtstrm. If the stream is a memory buffer, the memory is 
 * freed. If the stream is a file, the filestream is clused.
*/
#define gFreeStream(txtstrm)  txtstrm->freestream(txtstrm)


/**
 * \def gGetChar(txtstrm)
 * \brief Gets a character
 *
 * Returns the next character from \a txtstrm and increments the file/memory
 * pointer. Returns 0 on EOF
*/
#define gGetChar(txtstrm) txtstrm->ggetchar(txtstrm)


/**
 * \def gReadChar(txtstrm)
 * \brief Reads a character
 *
 * Returns the next character from \a txtstrm but does not increment the 
 * file/memory pointer. Returns 0 on EOF
*/
#define gReadChar(txtstrm) txtstrm->readchar(txtstrm)


/**
 * \def gReadahead(txtstrm, count)
 * \brief Reads a string from a stream.
 *
 * Returns a pointer to a string containing the next (count) number of 
 * characters from the given stream. If the stream is exhausted, the 
 * remainder is returned. (returns NULL on EOF).
 * This function is a read-ahead function and does not increment 
 * the stream pointer.
*/
#define gReadahead(txtstrm, count) txtstrm->readahead(txtstrm, count)


/**
 * \fn void gSeekPos(gTextStream *stream, int pos)
 * \brief Seek to a position in the stream.
 *
 * Seeks an absolute position within the stream.
 *
 * @param[in] stream Stream to seek within
 * @param[in] pos Position within the stream.
*/
void gSeekPos(gTextStream *txtstrm, int pos);


/**
 * \def gSeek(stream, offset)
 * \brief Seek ahead or behind in a stream.
 *
 * Seeks ahead or behind by \a offset the given number of characters in \a stream.
*/
#define gSeek(txtstrm, offset) txtstrm->seek(txtstrm, offset)


/**
 * \fn bool gStreamEnd(gTextStream *stream)
 * \brief Detects end of stream.
 *
 *  @return true of the stream has reached the end of the buffer/file.
*/
bool gStreamEnd(gTextStream *txtstrm);



#endif