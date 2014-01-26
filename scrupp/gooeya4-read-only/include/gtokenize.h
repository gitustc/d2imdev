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

#ifndef GTOKENIZE_H
#define GTOKENIZE_H

/** \file gtokenize.h
    \brief Definitions and prototypes for the tokenizer/lexer.

    Contains definitions and prototypes for the tokenizer/lexer.
 */

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include "qstring.h"
#include "glist.h"
#include "gshash.h"
#include "gtextstream.h"





// ----------------------------------------------------------------------------
// gTokenParms
// Holds information used by the tokenizer engine to determine how to tokenize 
// the input stream.


/** 
 * \struct gKeyword
 * \brief Keyword definition object.
 *
 * Struct which is used to define a keyword. In gParse a keyword is a token, 
 * which fits the criteria for a tIdentifier token (see: gTokenType_e),
 * that matches one of the token members of a list of gKeyword structs. A
 * list of these structs must be terminated with an entry containing a NULL
 * "token" member.
*/ 
typedef struct
{
   char *token;      //!< original char token
   int  newtype;     //!< new type
   char *newtoken;   //!< new token (optional)
} gKeyword;


/**
 * \struct gSymbol
 * \brief Symbol/operator definifion object.
 *
 * A struct which is used to define a symbol. A symbol can be any grouping of
 * characters between 1 and 3 characters, plus one zero terminating character,
 * in length. An array of gSymbol structs must end with the token being equal
 * to ""
*/ 
typedef struct
{
   char token[4];    //!< String (max 3 chars + 0x0)
   int  newtype;     //!< new type
} gSymbol;



/** 
 * \struct gEscape
 * \brief Escape sequence definition object
 *
 * An escape character is a character following a backslash (\) in a string
 * literal. If the escape character is not found in the parms escape list,
 * both characters are replaced with the character following the slash.
 * If the gIgnoreEscapes flag is enabled in the gTokenParms passed
 * to the tokenizer, these will be ignored all together. 
*/
typedef struct
{
   char escchar;     //!< Character to look for after a slash
   char replacechar; //!< Character to replace the two characters with.
} gEscape;



/**
 * \enum gParseFlags_e
 * \brief Tokenizer flags.
 *
 * Enum containing flag values which can be combined in the flags field of 
 * gTokenParms 
*/
typedef enum
{
   gIgnoreCase = 0x1,      //!< Ignore case when marking keywords
   gNewlineTokens = 0x2,   //!< Add a newline token when a newline char is encountered
   gIgnoreUnknowns = 0x4,  //!< Ignore chars that are unknown or unparsable.
   gIgnoreEscapes = 0x8,   //!< Do not process escape sequences in string literals eg. "\n"
} gParseFlags_e;


/**
 * \typedef gErrorFunc
 * \brief Type of function pointer for error message setting functions.
 *
 * Type of function pointer for error message setting functions. The function
 * this points to could also perform logging.
 *
 * @param[in]  fmt Format string for the message
 * @param[in]  ... Variable parameters as dictated by the format string.
*/
typedef void (*gErrorFunc)(const char *fmt, ...);

/**
 * \typedef gGetErrorFunc
 * \brief Type of function point for getting error messages.
 *
 * Type used to specify a valid error message function. This function should
 * not take any parameters (in fact, should have a void parameter list) but 
 * should return the last error message set by the setError function.
 *
 * @return last error message set or NULL if setError has not been called.
*/
typedef const char *(*gGetErrorFunc)(void);

/** 
 * \typedef strCompFunc
 * \brief Used internally to store either strcmp or stricmp based on flags.
 *
 * Used internally to store either strcmp or stricmp based on flags.
*/
typedef int  (*strCompFunc)(const char *, const char *, unsigned int);


/** 
 * \struct gTokenParms
 * \brief Tokenizer/Lexer parameters.
 *
 * Struct used to pass parameters to the tokenizer/lexer.
 * \see  gtokenize.h::gNewParms, gtokenize.h::gFreeParms
*/
typedef struct 
{
   gKeyword   *keywlist;    //!< list of keywords used in tokenizing
   gSymbol    *symbollist;  //!< list of symbols used in tokenizing
   gEscape    *escapelist;  //!< List of escape sequence chars in string literals

   /**
    * \brief Start string of fist comment type (default: "//")
    *
    * Comments:
    * The tokenizer supports up to two comment styles. Each specified by the 
    * strings. commentXs is the marker denoting the beginning of a comment
    * (eg. //) and commentXe is the marker denoting the end
    * (eg. line-end). These strings are never modified so it's safe to assign string
    * constants to them.
   */
   char  *comment1s, *comment1e; //!< End string for the first comment type (default: "\n")
   /** \brief Start string of the second comment type (default: "/*") */
   char  *comment2s, *comment2e; //!< End string for the second comment type (default: "*/")

   int        flags;       //!< parser flags (defaults: 0) \see gParseFlags_e

   gErrorFunc setError;    //!< Pointer to the function used to set errors (defaults: internal NOP function)
   gGetErrorFunc getError; //!< Pointer to the function used to get errors (defaults: internal NOP function)

   strCompFunc    strncmp; //!< This is set internally and should not be set
} gTokenParms;



/**
 * \fn gTokenParms *gNewParms()
 * 
 * Allocates and initializes and then returns a new gTokenParms structure.
 *
 * @return A new allocated and initialized gTokenParms object.
*/
gTokenParms *gNewParms();


/** 
 * \fn void gFreeParms(gTokenParms *parameters)
 * \brief Frees a gTokenParms structure.
 *
 * Frees an allocated gTokenParms structure. Currently this just calls free
 * but it is recommended this function be used.
 *
 * @param[in] parameters A pointer to the gTokenParms object to be freed.
*/
void gFreeParms(gTokenParms *parameters);




// ----------------------------------------------------------------------------
// gToken
// This is the struct that holds a token

/**
 * \enum gTokenType_e
 * \brief Base token types
 *
 * This is the starting nub of the token type list. It is a nub in that it is 
 * meant to be contined by the program using gParse to include other token
 * types the program might need. When making an enum of custom token types, 
 * simply make the first new token type = tLastBaseToken.
*/
typedef enum
{
   tUnknown,      //!< A token that could not be classified
   tIdentifier,   //!< Starts with a letter or _ and contains A-Z, a-z, 0-9, and _
   tInteger,      //!< Starts with a number and contains only 0-9
   tHexInt,       //!< Starts with 0x and contains 0-9, a-f, and A-F
   tDecimal,      /**< Starts with either . or 0-9 and contains one . 
                       and optionally IEEE formatting stuff... */
   tString,       //!< A quoted string

   tLineBreak,    //!< Line end (only generated if gNewlineTokens flag is set)

   tEOF,          //!< End of file token

   tLastBaseToken,//!< First custom token type 
} gTokenType_e;



/** 
 * \struct gToken
 * \brief Token structure.
 * 
 * This is the struct that holds the actual tokens generated by the tokenizer
 * and lexer.
 * \see  gtokenize.h::gCreateToken, gtokenize.h::gFreeToken
*/
typedef struct
{
   int          type;     //!< Type holds the gTokenType_e or user value
   char         *token;   //!< The actual string which comprises the token
   int          linenum;  //!< Line number the token occurs on
   int          charnum;  //!< Char number the token occurs on (within the line)
} gToken;


/**
 * \fn gToken *gCreateToken(const char *token, int type, int linenum, int charnum)
 * \brief Creates a token.
 *
 * Creates and returns a single token struct.
 *
 * @param[in] token The actual string contents of the new token
 * @param[in] type The token type of the new token. \see gTokenType_e
 * @param[in] linenum The line number the token occured on. 
 * @param[in] charnum The column number the token occured on. 
 * @return A new gToken object with copies of the token string and given info.
*/
gToken *gCreateToken(const char *token, int type, int linenum, int charnum);


/**
 * \fn void gFreeToken(void *object)
 * \brief Frees a token.
 *
 * Frees a single token. Changed for use with gList. Use this function
 * to free gToken objects created with gCreateToken.
 *
 * @param[in] object The token to be freed.
*/
void gFreeToken(void *object);



// ----------------------------------------------------------------------------
// gTokenStream
// This object is the means by which the tokenizer actually does most of the 
// work.

/** 
 * \struct gTokenStream
 * \brief Interface used to tokenize a gTextStream.
 *
 * This struct contains the parameters, text stream, and flags. It also is
 * responsible for managing the token cache (see: gGetToken, and gClearTCache)
 * These structs should really only be created with gCreateTokenStream and 
 * freed with gFreeTokenStream.
 * \see  gtokenize.h::gCreateTokenStream, gtokenize.h::gFreeTokenStream,
 *       gtokenize.h::gResetTokenStream, gtokenize.h::gGetNextToken,
 *       gtokenize.h::gGetToken, gtokenize.h::gClearTCache
*/
typedef struct gTokenStream
{
   gTokenParms *parameters;      //!< Pointer to the parameters object
   gTextStream *stream;          //!< Pointer to the text stream.

   qstring_t   *tokenbuf;        //!< Temporary buffer used by the tokenizing functions

   char        *name;            //!< Name of the file or stream the tokens are from.

   int         linenum;          //!< Current line number within the text stream
   int         charnum;          //!< Current char number within the text stream

   bool        endofstream;      //!< Set when gGetNextToken reaches the end of the stream
   
   gList       *tcache;          //!< Current list of cached tokens.
   int         cfirst;           //!< First token index in the cache
   int         clast;            //!< Last token index in the cache
} gTokenStream;



/**
 * \fn gTokenStream *gCreateTokenStream(gTokenParms *parms, gTextStream *stream, const char *name)
 * \brief Creates a token stream.
 *
 * Creates a new tokenstream from the given parameters. If 'parms' or 'stream' is 
 * NULL, the function will not create a new token stream and will return NULL.
 *
 * @param[in] parms Parameters the tokenizer/lexer should use for this stream
 * @param[in] stream Text stream the tokens should come from.
 * @param[in] name The name of the stream (used for error reporting)
 * @returns New token stream.
*/
gTokenStream *gCreateTokenStream(gTokenParms *parms, gTextStream *stream, const char *name);


/**
 * \fn void gFreeTokenStream(gTokenStream *tokstrm)
 * \brief Frees a token stream made with gCreateTokenStream
 *
 * Frees a tokenstream made with gCreateTokenStream. It should be noted this
 * function does NOT free the parms object or the stream. Those need to be freed
 * separately after the stream is freed.
 *
 * @param[in] tokstrm The stream to be freed.
*/
void gFreeTokenStream(gTokenStream *tokstrm);


/**
 * \fn void gResetTokenStream(gTokenStream *tokstrm)
 * \brief Resets a token stream.
 *
 * This function clears all cache and resets the text stream back to the
 * beginning of the file/buffer it is streaming, returning the token stream
 * back to it's initial state.
 *
 * @param[in] tokstrm Token stream to be reset.
*/
void gResetTokenStream(gTokenStream *tokstrm);


/**
 * \fn gToken *gGetNextToken(gTokenStream *tokstrm)
 * \brief Returns the next token in the stream.
 *
 * Parses, lexes and returns the next token in the stream. This function does
 * not cache the tokens, so the token this function returns has to be freed
 * manually. It's generally best to avoid this function and use gGetToken.
 *
 * @param[in] tokstrm The token stream to get the next token from.
 * @return Next token in the stream.
*/
gToken *gGetNextToken(gTokenStream *tokstrm);


/**
 * \fn gToken *gGetToken(gTokenStream *tokstrm, int index)
 * \brief Returns a token at the given index in the stream.
 *
 * Returns the token at the given index. Index must be >= the first token in the
 * streams cache--that is, >= strea->cfirst. If index > stream->clast, 
 * the cache of the stream is read-ahead to index. Returns NULL if index was less
 * than cfirst. If not enough tokens could be parsed from the stream to get to
 * index, the last token in the stream is returned (tEOF). This function should
 * NOT be used with the same stream as gGetNextToken.
 *
 * @param[in] tokstrm The token stream to get the token from.
 * @param[in] index The index within the stream to get the token from.
 * @return Token at index.
*/
gToken *gGetToken(gTokenStream *tokstrm, int index);


/**
 * \fn void gClearTCache(gTokenStream *tokstrm)
 * \brief Clears the token cache inside a token stream.
 *
 * Clears the token cache within the stream. All but the very last token cached 
 * are freed.
 *
 * @param[in] tokstrm Token stream to clear the cache of.
*/
void gClearTCache(gTokenStream *tokstrm);


#include "gtpattern.h"


#ifdef __cplusplus
}
#endif

#endif //GTOKENIZE_H


