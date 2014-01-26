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

#include "glist.h"
#include "gtokenize.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

static char *gComment1s             = "//";
static char *gComment1e             = "\n";
static char *gComment2s             = "/*";
static char *gComment2e             = "*/";


// ----------------------------------------------------------------------------
// gTokenParms
// Holds information used by the tokenizer engine to determine how to tokenize 
// the input stream.

void errorNOP(const char *fmt, ...)
{
}

const char *getErrorNOP(void)
{
   return NULL;
}


gTokenParms *gNewParms()
{
   gTokenParms *ret = (gTokenParms *)malloc(sizeof(gTokenParms));

   memset(ret, 0, sizeof(*ret));

   ret->comment1s = gComment1s;
   ret->comment1e = gComment1e;
   ret->comment2s = gComment2s;
   ret->comment2e = gComment2e;

   ret->setError = errorNOP;
   ret->getError = getErrorNOP;

   return ret;
}


void gFreeParms(gTokenParms *parameters)
{
   free(parameters);
}



// ----------------------------------------------------------------------------
// gToken
// This is the struct that holds a token

gToken *gCreateToken(const char *token, int type, int linenum, int charnum)
{
   gToken *ret = (gToken *)malloc(sizeof(gToken));
   memset(ret, 0, sizeof(*ret));

   ret->token = _strdup(token);
   ret->type = type;
   ret->linenum = linenum;
   ret->charnum = charnum;

   return ret;
}


void gFreeToken(void *object)
{
   gToken *token = (gToken *)object;

   if(token->token)
      free(token->token);

   free(token);
}




// ----------------------------------------------------------------------------
// gTokenStream
// This object is the means by which the tokenizer actually does most of the 
// work.

gTokenStream *gCreateTokenStream(gTokenParms *parameters, gTextStream *stream, const char *name)
{
   gTokenStream *ret = (gTokenStream *)malloc(sizeof(gTokenStream));
   memset(ret, 0, sizeof(*ret));

   ret->stream = stream;
   ret->parameters = parameters;
   ret->name = _strdup(name);
   ret->tokenbuf = malloc(sizeof(qstring_t));
   ret->charnum = ret->linenum = 1;

   ret->tcache = gNewList(gFreeToken);
   ret->cfirst = 0; ret->clast = -1;

   M_QStrInitCreate(ret->tokenbuf);

   return ret;
}



void gFreeTokenStream(gTokenStream *tokstrm)
{
   if(tokstrm->name)
      free(tokstrm->name);

   gFreeList(tokstrm->tcache);
   M_QStrFree(tokstrm->tokenbuf);
   free(tokstrm->tokenbuf);

   free(tokstrm);
}



void gResetTokenStream(gTokenStream *tokstrm)
{
   // Clear the token cache, reset the temporary token buffer.
   gClearList(tokstrm->tcache);

   tokstrm->charnum = tokstrm->linenum = 1;
   tokstrm->tokenbuf->buffer[0] = 0;

   // There are no more cached tokens
   tokstrm->cfirst = 0;
   tokstrm->clast = -1;

   // Seek the start of the text stream
   gSeekPos(tokstrm->stream, 0);
}


// Little helper functions.
static bool isNumeric(char c)
{
   return (c >= '0' && c <= '9') ? true : false;
}

static bool isHexChar(char c)
{
   return (isNumeric(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) ? true : false;
}

static bool isAlphaNumeric(char c)
{
   return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) ? true : false;
}

static bool isWhiteSpace(char c)
{
   return (c == ' ' || c == '\r' || c == '\t' || c == '\n' || c == '\v' || c == '\f') ? true : false;
}


static void countWhitespace(gTokenStream *tokstrm, const char *string, int length)
{
   int i;
   for(i = 0; i < length; i++)
   {
      if(string[i] == '\n')
      {
         tokstrm->linenum++;
         tokstrm->charnum = 1;
      }
      else if(string[i] != '\r')
         tokstrm->charnum++;
   }
}


static void skipWhitespace(gTokenStream *tokstrm, const char *string, int length)
{
   countWhitespace(tokstrm, string, length);
   gSeek(tokstrm->stream, length);
}


static void countChars(gTokenStream *tokstrm, int length)
{
   tokstrm->charnum += length;
}



static void skipChars(gTokenStream *tokstrm, int length)
{
   countChars(tokstrm, length);
   gSeek(tokstrm->stream, length);
}






static void skipComment(gTokenStream *tokstrm, const char *stop)
{
   gTextStream    *stream = tokstrm->stream;
   gTokenParms    *parms = tokstrm->parameters;
   const char     *str;
   int            len = strlen(stop);
   
   while(!gStreamEnd(stream) && (str = gReadahead(stream, len)))
   {
      if(!parms->strncmp(str, stop, len))
      {
         skipWhitespace(tokstrm, str, len);
         break;
      }

      skipWhitespace(tokstrm, str, 1);
   }
}





static int checkEscape(const char c, gTokenParms *parms)
{
   int i;
   gEscape *esc;

   if(!parms->escapelist)
      return -1;

   for(i = 0; (esc = parms->escapelist + i)->escchar; i++)
   {
      if(c == esc->escchar)
         return i;
   }

   return -1;
}


static gToken *parseString(gTokenStream *tokstrm)
{
   gTextStream    *stream = tokstrm->stream;
   gTokenParms    *parms = tokstrm->parameters;
   int            linestart, charstart;

   linestart = tokstrm->linenum;
   charstart = tokstrm->charnum;

   M_QStrClear(tokstrm->tokenbuf);

   while(!gStreamEnd(stream))
   {
      char ch = gReadChar(stream);

      // Don't allow the \n char to be escaped.
      if(ch == '\n')
      {
         // Error
         parms->setError("%s(%i, %i): Unterminated string literal.\n", tokstrm->name, linestart, charstart);
         return gCreateToken(M_QStrBuffer(tokstrm->tokenbuf), tString, linestart, charstart);
      }

      if(ch == '\"')
      {
         skipChars(tokstrm, 1);
         return gCreateToken(M_QStrBuffer(tokstrm->tokenbuf), tString, linestart, charstart);
      }

      if(!(parms->flags & gIgnoreEscapes) && ch == '\\')
      {
         int index;
         char *s = gReadahead(stream, 2);

         if(!s || strlen(s) != 2)
         {
            // Error
            if(s)
               M_QStrPutc(tokstrm->tokenbuf, *s);

            parms->setError("%s(%i, %i): Unterminated string literal.\n", tokstrm->name, linestart, charstart);
            return gCreateToken(M_QStrBuffer(tokstrm->tokenbuf), tString, linestart, charstart);
         }

         skipChars(tokstrm, 2);

         index = checkEscape(s[1], parms);
         if(index != -1)
            M_QStrPutc(tokstrm->tokenbuf, parms->escapelist[index].replacechar);
         else
            M_QStrPutc(tokstrm->tokenbuf, s[1]);
      }
      else
      {
         M_QStrPutc(tokstrm->tokenbuf, ch);
         skipChars(tokstrm, 1);
      }
   }

   // Error
   parms->setError("%s(%i, %i): Unterminated string literal.\n", tokstrm->name, linestart, charstart);

   return gCreateToken(M_QStrBuffer(tokstrm->tokenbuf), tString, linestart, charstart);
}





static gToken *parseHex(gTokenStream *tokstrm)
{
   gTextStream    *stream = tokstrm->stream;
   gTokenParms    *parms = tokstrm->parameters;
   int            linestart, charstart;
   char           *str;

   linestart = tokstrm->linenum;
   charstart = tokstrm->charnum;

   M_QStrClear(tokstrm->tokenbuf);

   str = gReadahead(stream, 3);

   if(!str || strlen(str) != 3)
   {
      parms->setError("%s(%i, %i): Expected a Hex value after '0x'", tokstrm->name, linestart, charstart);
      return gCreateToken(str, tHexInt, linestart, charstart);
   }

   M_QStrCat(tokstrm->tokenbuf, str);
   skipChars(tokstrm, 3);

   while(!gStreamEnd(stream))
   {
      char ch = gReadChar(stream);
      if(isHexChar(ch))
      {
         skipChars(tokstrm, 1);
         M_QStrPutc(tokstrm->tokenbuf, ch);
         continue;
      }

      break;
   }

   return gCreateToken(M_QStrBuffer(tokstrm->tokenbuf), tHexInt, linestart, charstart);
}





static int checkKeyword(const char *token, gTokenParms *parms)
{
   int i;
   gKeyword *kw;

   if(!parms->keywlist)
      return -1;

   for(i = 0; (kw = parms->keywlist + i)->token; i++)
   {
      if(((parms->flags & gIgnoreCase) && !_stricmp(token, kw->token)) 
         || !strcmp(token, kw->token))
         return i;
   }

   return -1;
}



static gToken *parseIdentifier(gTokenStream *tokstrm)
{
   gTextStream    *stream = tokstrm->stream;
   gTokenParms    *parms = tokstrm->parameters;
   int            linestart, charstart, index;
   int            type = tIdentifier;

   linestart = tokstrm->linenum;
   charstart = tokstrm->charnum;

   M_QStrClear(tokstrm->tokenbuf);

   while(!gStreamEnd(stream))
   {
      char ch = gReadChar(stream);
      if(isAlphaNumeric(ch) || isNumeric(ch) || ch == '_')
      {
         skipChars(tokstrm, 1);
         M_QStrPutc(tokstrm->tokenbuf, ch);
         continue;
      }

      break;
   }

   // Check here for constants.
   index = checkKeyword(M_QStrBuffer(tokstrm->tokenbuf), tokstrm->parameters);
   if(index != -1)
   {
      gKeyword *kw = &tokstrm->parameters->keywlist[index];

      type = kw->newtype;

      if(kw->newtoken)
         M_QStrSet(tokstrm->tokenbuf, kw->newtoken);
   }

   return gCreateToken(M_QStrBuffer(tokstrm->tokenbuf), type, linestart, charstart);
}



int checkSymbol(gTokenParms *parms, char *string)
{
   int len = strlen(string);
   int i;
   gSymbol *s;

   if(!parms->symbollist || !len)
      return -1;

   for(i = 0; (s = parms->symbollist + i)->token[0] != 0; i++)
   {
      if(s->token[0] == string[0] && !s->token[1])
         return i;

      else if(s->token[0] == string[0] && s->token[1] && string[1] && 
              s->token[1] == string[1] && !s->token[2])
         return i;

      else if(s->token[0] == string[0] && s->token[1] && string[1] && 
              s->token[1] == string[1] && s->token[2] && string[2] && 
              s->token[2] == string[2])
         return i;
   }

   return -1;
}




static gToken *parseNumber(gTokenStream *tokstrm)
{
   gTextStream    *stream = tokstrm->stream;
   gTokenParms    *parms = tokstrm->parameters;
   int            linestart, charstart;
   int            type = tInteger;

   linestart = tokstrm->linenum;
   charstart = tokstrm->charnum;

   M_QStrClear(tokstrm->tokenbuf);

   while(!gStreamEnd(stream))
   {
      char ch = gReadChar(stream);

      if(type == tInteger)
      {
         if(isNumeric(ch) || ch == '.')
         {
            M_QStrPutc(tokstrm->tokenbuf, ch);
            skipChars(tokstrm, 1);

            if(ch == '.')
               type = tDecimal;

            continue;
         }
      }
      else if(type == tDecimal)
      {
         if(isNumeric(ch))
         {
            M_QStrPutc(tokstrm->tokenbuf, ch);
            skipChars(tokstrm, 1);
            continue;
         }
         else if(ch == 'e' || ch == 'E')
         {
            skipChars(tokstrm, 1);
            M_QStrPutc(tokstrm->tokenbuf, ch);
            
            // Read-ahead and make sure there is at least one digit.
            ch = gReadChar(stream);
            if(ch == '+' || ch == '-')
            {
               M_QStrPutc(tokstrm->tokenbuf, ch);
               skipChars(tokstrm, 1);
               ch = gReadChar(stream);
            }

            if(gStreamEnd(stream) || !isNumeric(ch))
            {
               parms->setError("%s(%i, %i): Expected numeric value in exponent.", tokstrm->name, linestart, charstart);
               return gCreateToken(M_QStrBuffer(tokstrm->tokenbuf), type, linestart, charstart);
            }

            while(!gStreamEnd(stream) && isNumeric(ch))
            {
               M_QStrPutc(tokstrm->tokenbuf, ch);
               skipChars(tokstrm, 1);
               ch = gReadChar(stream);
            }

            gCreateToken(M_QStrBuffer(tokstrm->tokenbuf), type, linestart, charstart);
         }
      }

      break;
   }

   return gCreateToken(M_QStrBuffer(tokstrm->tokenbuf), type, linestart, charstart);
}





gToken *gGetNextToken(gTokenStream *tokstrm)
{
   gTextStream    *stream = tokstrm->stream;
   gTokenParms    *parms = tokstrm->parameters;

   char           rover, *string;
   gToken         *ret;
   int            max, len, index, stringlen;

   parms->strncmp = (parms->flags & gIgnoreCase) ? _strnicmp : strncmp;

   while(!gStreamEnd(stream))
   {
      // Bit different than the old tokenizer loop, this function simply finds and
      // returns the next token in the given stream.
      while((rover = gReadChar(stream)))
      {
         if(!isWhiteSpace(rover))
            break;

         if(rover == '\n' && parms->flags & gNewlineTokens)
         {
            gToken *t = gCreateToken("", tLineBreak, tokstrm->linenum, tokstrm->charnum);
            skipWhitespace(tokstrm, &rover, 1);
            return t;
         }

         skipWhitespace(tokstrm, &rover, 1);
      }

      if(!rover)
         break;

      // Determine the maximum read-ahead we need
      max = 3;
      if(parms->comment1s && (len = strlen(parms->comment1s)) > max)
         max = len;
      if(parms->comment2s && (len = strlen(parms->comment2s)) > max)
         max = len;

      string = gReadahead(stream, max);

      if(!string)
         break;

      stringlen = strlen(string);

      // Check comments. If a comment is encountered, the whitespace check 
      // needs to run again
      if(parms->comment1s && parms->comment1e)
      {
         len = strlen(parms->comment1s);

         if(!parms->strncmp(string, parms->comment1s, len))
         {
            skipWhitespace(tokstrm, string, len);
            skipComment(tokstrm, parms->comment1e);
            continue;
         }
      }
      if(parms->comment2s && parms->comment2e)
      {
         len = strlen(parms->comment2s);

         if(!parms->strncmp(parms->comment2s, string, len))
         {
            skipWhitespace(tokstrm, string, len);
            skipComment(tokstrm, parms->comment2e);
            continue;
         }
      }

      // Detect various token types.
      if(string[0] == '\"')
      {
         // String literal
         skipChars(tokstrm, 1);
         return parseString(tokstrm);
      }
      else if(stringlen >= 2 && string[0] == '0' && string[1] == 'x')
      {
         // Hex number
         if((ret = parseHex(tokstrm)))
            return ret;

         break;
      }
      else if(isNumeric(string[0]) || (string[0] == '.' && isNumeric(string[1])))
      {
         if((ret = parseNumber(tokstrm)))
            return ret;
         break;
      }
      else if((index = checkSymbol(tokstrm->parameters, string)) != -1)
      {
         gSymbol *symbol = &tokstrm->parameters->symbollist[index];
         gToken  *tok = gCreateToken(symbol->token, symbol->newtype, tokstrm->linenum, tokstrm->charnum);

         // Symbols could contain return chars in them.
         skipWhitespace(tokstrm, tok->token, strlen(tok->token));

         if(tok)
            return tok;
         break;
      }
      else if(isAlphaNumeric(string[0]) || string[0] == '_')
      {
         if((ret = parseIdentifier(tokstrm)))
            return ret;
         break;
      }
      // Add more types!
      else if(!(parms->flags & gIgnoreUnknowns))
      {
         // SoM: TODO: some times comment terminators such as '*/' will trigger 
         // this error message. Something should really be done about this.

         parms->setError("%s(%i, %i): Unknown char encountered.\n", tokstrm->name, tokstrm->linenum, tokstrm->charnum);
         // Skip the char
         gGetChar(stream);
         continue;
      }

      break;
   }

   tokstrm->endofstream = true;
   return gCreateToken("end of file", tEOF, tokstrm->linenum, tokstrm->charnum);
}




gToken *gGetToken(gTokenStream *tokstrm, int index)
{
   int i;

   if(index < tokstrm->cfirst)
   {
      // Can't go back in the token stream.
      return NULL;
   }
   else if(index > tokstrm->clast)
   {
      // Cache new tokens until the end of the stream.
      int count = index - tokstrm->clast;

      for(i = 0; i < count; i++)
      {
         if(tokstrm->endofstream)
            return tokstrm->tcache->list[tokstrm->tcache->size - 1];

         gAppendListItem(tokstrm->tcache, gGetNextToken(tokstrm));
         tokstrm->clast++;
      }
   }

   return tokstrm->tcache->list[index - tokstrm->cfirst];
}



void gClearTCache(gTokenStream *tokstrm)
{
   if(tokstrm->cfirst == tokstrm->clast)
      return;

   // size - 1 is the last item in the list... hehe. Leave at least one token
   // in the stream at all times please. :)
   gDeleteListRange(tokstrm->tcache, 0, tokstrm->tcache->size - 2);
   tokstrm->cfirst = tokstrm->clast;
}




