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


typedef enum
{
   tProcess = tLastBaseToken,
   tImage,
   tBlend,
   tAlpha,
   tAdditive,
   
   tSpans,
   tRows,
   tColumns,

   tRGB,
   tFont,
   tFlagSep,

   tSpan,
   tStart,
   tMid,
   tEnd,

   tStartBit,
   tMidBit,
   tGapColor,
   tEndBit,

   tEquals,
   tSemiColon,
   tOpenParan,
   tClosedParan,
   tOpenBracket,
   tClosedBracket,
   tComma,
   tColon,

   tIcons,
   tSet,
   tMinus,
   tCursors,
   tPlain,
   tBold,
   tItalic,
   tUnderline
} gSkinConst_e;



static gKeyword gSkinKeyList[] =
{
   {"process",    tProcess,   NULL},
   {"image",      tImage,     NULL},
   {"spans",      tSpans,     NULL},
   {"rows",       tRows,      NULL},
   {"columns",    tColumns,   NULL},
   {"blend",      tBlend,     NULL},
   {"alpha",      tAlpha,     NULL},
   {"additive",   tAdditive,  NULL},
   {"rgb",        tRGB,       NULL},
   {"fontspec",   tFont,      NULL},
   {"plain",      tPlain,     NULL},
   {"bold",       tBold,      NULL},
   {"italic",     tItalic,    NULL},
   {"underline",  tUnderline, NULL},
   {"span",       tSpan,      NULL},
   {"start",      tStart,     NULL},
   {"mid",        tMid,       NULL},
   {"end",        tEnd,       NULL},
   {"startbit",   tStartBit,  NULL},
   {"midbit",     tMidBit,    NULL},
   {"midcolor",   tGapColor,  NULL},
   {"endbit",     tEndBit,    NULL},
   {"icons",      tIcons,     NULL},
   {"set",        tSet,       NULL},
   {"cursors",    tCursors,   NULL},
   {"plain",      tPlain,     NULL},
   {"bold",       tBold,      NULL},
   {"italic",     tItalic,    NULL},
   {"underline",  tUnderline, NULL},
   {NULL, 0, NULL}
};



static gSymbol gSkinSymbolList[] =
{
   {"=",    tEquals},
   {";",    tSemiColon},
   {"(",    tOpenParan},
   {")",    tClosedParan},
   {"{",    tOpenBracket},
   {"}",    tClosedBracket},
   {",",    tComma},
   {"-",    tMinus},
   {"|",    tFlagSep},
   {":",    tColon},
   {"",     0}
};



// ----------------------------------------------------------------------------
// Utility functions

// Fills the given color struct with the color information that can be
// determined from the given hex code.
static void fillColor(SDL_Color *c, const char *hexcode)
{
   char  buf[5] = "";
   int   len = strlen(hexcode);

   if(len < 8)
      gSetError("Warning: Hex Color %s has insufficient data to form a true RGB tripplet.\n", hexcode);

   memset(c, 0, sizeof(SDL_Color));
   if(len >= 4)
   {
      sprintf(buf, "0x%.2s", hexcode + 2);
      c->r = atoi(buf);
   }
   if(len >= 6)
   {
      sprintf(buf, "0x%.2s", hexcode + 4);
      c->g = atoi(buf);
   }
   if(len >= 8)
   {
      sprintf(buf, "0x%.2s", hexcode + 6);
      c->r = atoi(buf);
   }
}


gSurface *loadImage(gTokenStream *ts, const char *name)
{
   gSurface    *surf;
   SDL_RWops   *ops;

   if(!PHYSFS_exists(name))
   {
      gSetError("findImage was unable to load the image %s from the skin file %s", name, ts->name);
      return NULL;
   }

   ops = PHYSFSRWOPS_openRead(name);
   if(!ops)
   {
      gSetError("findImage was unable to load the image %s from the skin file %s: failed _openRead", name, ts->name);
      return NULL;
   }

   if(!(surf = IMG_Load_RW(ops, true)))
   {
      gSetError("findImage was unable to load the image %s from the skin file %s: %s", name, ts->name, SDL_GetError());
      return NULL;
   }

   return surf;
}

// Loads an image based on the given image name.
gTempImage *findImage(gTokenStream *ts, const char *name, gSkinTemplate *s)
{
   gSurface    *surf;
   gTempImage  *ret;
   unsigned    i;

   for(i = 0; i < s->images->size; i++)
   {
      ret = (gTempImage *)gGetListItem(s->images, i);
      if(!stricmp(ret->name, name))
         return ret;
   }

   if(!(surf = loadImage(ts, name)))
      return NULL;

   ret = malloc(sizeof(gTempImage));
   ret->surface = surf;
   ret->name = strdup(name);

   gAppendListItem(s->images, ret);

   return ret;
}



// ----------------------------------------------------------------------------
// These are used by anything that parses text inside a process body

static gSkinTemplate *stemp = NULL;
static gTempProcess  *proc = NULL;
static gTempSpan     *span = NULL;

static SDL_Color     tcolor;

// ----------------------------------------------------------------------------
// Skin hooks
tpHookCode skinEHook(tpHookParms *parms)
{
   tPattern     *p = parms->pattern;
   gTokenStream *ts = p->tstream;
   int          level = 0;
   gToken       *t;

   tpClearInlist(parms);

   // Skip ahead to the next statement
   while((t = gGetToken(ts, p->i)) != NULL)
   {
      switch(t->type)
      {
         case tEOF:
            return tpAbort;
         case tProcess:
         case tSet:
         case tIcons:
         case tCursors:
            if(!level)
               return tpFallback;
            p->i++;
            break;
         case tClosedBracket:
            if(!level)
            {
               gSetError("Warning: %s(%i, %i): Unexpected '}' encountered", ts->name, t->linenum, t->charnum);
               tpIncrementCount(p, tpWarning);
            }
            else
               level--;

            p->i++;
            break;
         case tOpenBracket:
            if(!level)
            {
               gSetError("Warning: %s(%i, %i): Unexpected '{' encountered, skipping block.", ts->name, t->linenum, t->charnum);
               tpIncrementCount(p, tpWarning);
            }
            level++;
            p->i++;
            break;
         default:
            p->i++;
            break;
      }
   }

   // Loop did not resolve.
   return tpAbort;
}




// ----------------------------------------------------------------------------
// Process hooks

tpHookCode procEHook(tpHookParms *parms)
{
   tPattern     *p = parms->pattern;
   gTokenStream *ts = p->tstream;
   int          level = 0;
   gToken       *t;

   tpClearInlist(parms);

   // Skip ahead to the next statement
   while((t = gGetToken(ts, p->i)) != NULL)
   {
      switch(t->type)
      {
         case tSet:
         case tSpans:
         case tSpan:
            if(!level)
               return tpFallback;
            p->i++;
            break;
         case tEOF:
            proc = NULL;
            return tpAbort;
            break;
         case tClosedBracket:
            if(!level)
            {
               proc = NULL;
               return tpPopFallback;
            }

            level--;
            p->i++;
            break;
         case tOpenBracket:
            gSetError("Warning: %s(%i, %i): Unexpected '{' encountered, skipping block.", ts->name, t->linenum, t->charnum);
            tpIncrementCount(p, tpWarning);
            level++; p->i++;
            break;
         default:
            p->i++;
            break;
      }
   }

   // Loop did not resolve
   proc = NULL;
   span = NULL;
   return tpFatal;
}




// Called when the process header pattern has successfully finished. Assumes at 
// least two tokens in the inlist.
int makeProc(tpHookParms *parms)
{
   gToken       *pname, *parentname, *iname;
   gList        *inlist = parms->inlist;
   gTempProcess *parent = NULL;
   int          ret = tpNoError;

   pname = (gToken *)gGetListItem(inlist, 0);
   parentname = inlist->size == 3 ? (gToken *)gGetListItem(inlist, 1) : NULL;
   iname = inlist->size == 3 ? 
      (gToken *)gGetListItem(inlist, 2) : 
      (gToken *)gGetListItem(inlist, 1);

   if(parentname && !(parent = gFindTempProcess(parentname->token, stemp)))
   {
      gSetError("Error: Could not find parent process %s.", parentname->token);
      tpIncrementCount(parms->pattern, tpError);
   }
   
   if(!findImage(parms->pattern->tstream, iname->token, stemp))
   {
      gSetError("Error: Could not load image file %s. Process %s ignored.", iname->token, pname->token);
      ret = tpIncrementCount(parms->pattern, tpError);
   }
   else
   {
      proc = gAddProcess(pname->token, parent, stemp);

      if(proc->imgname)
         free(proc->imgname);

      proc->imgname = strdup(iname->token);
   }

   tpClearInlist(parms);

   return ret;
}


int clearProc(tpHookParms *parms)
{
   proc = NULL;
   span = NULL; // SoM: Make sure span is clear too...
   return tpNoError;
}



// ----------------------------------------------------------------------------
// Attribute hooks...


// Used to create a simple attribute based on the token type.
gSkinAttribute *simpleAttribute(const char *name, gToken *value)
{
   if(value->type == tInteger)
   {
      return gNewIntAttribute(name, atoi(value->token));
   }
   else if(value->type == tHexInt)
   {
      return gNewIntAttribute(name, strtol(value->token, NULL, 16));
   }
   else if(value->type == tDecimal)
   {
      return gNewFloatAttribute(name, (float)atof(value->token));
   }
   else if(value->type == tString || value->type == tIdentifier)
   {
      return gNewStringAttribute(name, value->token);
   }

   return NULL;
}



// Searches for, and removes, an attribute in the given list whose name
// matches the given one.
static void clearAtt(gList *list, const char *name)
{
   unsigned i;

   for(i = 0; i < list->size; i++)
   {
      gSkinAttribute *att = (gSkinAttribute *)list->list[i];
      if(!stricmp(name, att->name))
      {
         gDeleteListItem(list, i);
         return;
      }
   }
}


// Stores the given attribute. If the parser is currently in a process,
// the attribute is stored there, otherwise it is stored in the skin.
static void storeAtt(gSkinAttribute *att)
{
   gList *list;

   if(proc)
      list = proc->attributes;
   else
      list = stemp->attributes;

   clearAtt(list, att->name);
   gAppendListItem(list, att);
}


// Called when the pattern for an attribute set of a simple kind has completed.
// This function assumes that the inlist will have two items in it.
int simpleSet(tpHookParms *parms)
{
   gToken *name, *value;

   name = (gToken *)gGetListItem(parms->inlist, 0);
   value = (gToken *)gGetListItem(parms->inlist, 1);

   storeAtt(simpleAttribute(name->token, value));
  
   tpClearInlist(parms);
   return tpNoError;
}


// Called when the pattern for an attribute set of a RGB color has completed.
// This function assumes that the inlist will have one items in it.
int rgbSet(tpHookParms *parms)
{
   gToken *name;

   name = (gToken *)gGetListItem(parms->inlist, 0);

   storeAtt(gNewColorAttribute(name->token, tcolor.r, tcolor.g, tcolor.b));
   
   tpClearInlist(parms);
   return tpNoError;
}


// Called when the font attribute pattern has successfully completed.
// This function assumes that the inlist will have at least four
// items in it.
int fontSet(tpHookParms *parms)
{
   gToken   *name, *fontfile, *ptsize, *flag;
   unsigned i;
   int      size, style = 0;

   name = (gToken *)gGetListItem(parms->inlist, 0);
   fontfile = (gToken *)gGetListItem(parms->inlist, 1);
   ptsize = (gToken *)gGetListItem(parms->inlist, 2);

   size = atoi(ptsize->token);

   for(i = 3; i < parms->inlist->size; i++)
   {
      flag = (gToken *)gGetListItem(parms->inlist, i);

      switch(flag->type)
      {
         case tBold:
            style |= gStyleBold;
            break;
         case tUnderline:
            style |= gStyleUnderline;
            break;
         case tItalic:
            style |= gStyleItalic;
            break;
         default:
            break;
      }
   }

   storeAtt(gNewFontAttribute(name->token, fontfile->token, size, style, 
                              tcolor.r, tcolor.g, tcolor.b));

   tpClearInlist(parms);
   return tpNoError;
}





// ----------------------------------------------------------------------------
// RGB hooks
int colorR(tpHookParms *parms)
{
   tcolor.r = atoi(parms->t->token);
   return tpNoError;
}

int colorG(tpHookParms *parms)
{
   tcolor.g = atoi(parms->t->token);
   return tpNoError;
}

int colorB(tpHookParms *parms)
{
   tcolor.b = atoi(parms->t->token);
   return tpNoError;
}

int colorHex(tpHookParms *parms)
{
   fillColor(&tcolor, parms->t->token);
   return tpNoError;
}

// ----------------------------------------------------------------------------
// Span hooks

// This function will set the orientation of a skin process.
// This function assumes: 
// 1. proc points to a valid process. 
// 2. the current token is either tRows or tColumns
int spansSet(tpHookParms *parms)
{
   switch(parms->t->type)
   {
      case tRows:
         proc->orientation = gSpanRows;
         break;
      case tColumns:
         proc->orientation = gSpanColumns;
         break;
      default:
         break;
   }
   return tpNoError;
}


// This function creates a span. It assumes the inlist has two tokens.
int makeSpan(tpHookParms *parms)
{
   gTempSpan   **dest;
   gToken      *ori, *thtok;
   int         thickness, flags = 0;

   ori = (gToken *)gGetListItem(parms->inlist, 0);
   thtok = (gToken *)gGetListItem(parms->inlist, 1);

   thickness = atoi(thtok->token);

   switch(ori->type)
   {
      case tMid:
         dest = &proc->midspan;
         break;
      case tEnd:
         dest = &proc->endspan;
         flags = gSpanEndJustified;
         break;
      default:
         dest = &proc->startspan;
         break;
   }

   if(*dest == NULL)
      *dest = span = gCreateTempSpan();
   else
      span = *dest;

   span->thickness = thickness;
   span->flags = flags;

   tpClearInlist(parms);
   return tpNoError;
}



// This function sets span pointer to NULL. It is called at the end of a span 
// block
int spanDone(tpHookParms *parms)
{
   span = NULL;
   return tpNoError;
}



tpHookCode spanEHook(tpHookParms *parms)
{
   tPattern     *p = parms->pattern;
   gTokenStream *ts = p->tstream;
   int          level = 0;
   gToken       *t;

   tpClearInlist(parms);

   // Skip ahead to the next statement
   while((t = gGetToken(ts, p->i)) != NULL)
   {
      switch(t->type)
      {
         case tEOF:
            span = NULL;
            proc = NULL;
            return tpAbort;
            break;
         case tStartBit:
         case tMidBit:
         case tEndBit:
         case tGapColor:
            if(!level)
               return tpFallback;
            break;
         case tClosedBracket:
            if(!level)
            {
               span = NULL;
               return tpPopFallback;
            }
            else
               level--;

            p->i++;
            break;
         case tOpenBracket:
            gSetError("Warning: %s(%i, %i): Unexpected '{' encountered, skipping block.", ts->name, t->linenum, t->charnum);
            tpIncrementCount(p, tpWarning);
            level++;
            p->i++;
            break;
         default:
            p->i++;
            break;
      }
   }

   // Loop ended without resolution:
   proc = NULL; span = NULL;
   return tpAbort;
}


// This function is called after the bit pattern successfully executes. It 
// assumes 5 tokens are in the inlist
int makeBit(tpHookParms *parms)
{
   gToken    *idt, *xt, *yt, *wt, *ht;
   gTempBit  *bit;

   idt = (gToken *)gGetListItem(parms->inlist, 0);
   xt = (gToken *)gGetListItem(parms->inlist, 1);
   yt = (gToken *)gGetListItem(parms->inlist, 2);
   wt = (gToken *)gGetListItem(parms->inlist, 3);
   ht = (gToken *)gGetListItem(parms->inlist, 4);

   switch(idt->type)
   {
      case tMidBit:
         bit = span->centerbit ? span->centerbit 
            : (span->centerbit = malloc(sizeof(gTempBit)));
         span->centerflag = gBitRepeat;
         break;
      case tEndBit:
         bit = span->endbit ? span->endbit 
            : (span->endbit = malloc(sizeof(gTempBit)));
         break;
      default:
         bit = span->startbit ? span->startbit 
            : (span->startbit = malloc(sizeof(gTempBit)));
         break;
   }

   bit->flags = 0;
   bit->rect.x = atoi(xt->token);
   bit->rect.y = atoi(yt->token);
   bit->rect.w = (unsigned)atoi(wt->token);
   bit->rect.h = (unsigned)atoi(ht->token);

   tpClearInlist(parms);
   return tpNoError;
}

// Called when the fillcolor statement is processed successfully. 
int makeFill(tpHookParms *parms)
{
   span->centercolor.r = tcolor.r;
   span->centercolor.g = tcolor.g;
   span->centercolor.b = tcolor.b;
   span->centerflag = gColorFill;

   tpClearInlist(parms);
   return tpNoError;
}


// ----------------------------------------------------------------------------
// Icons

// This is used by both icons and cursors
static char *iconname = NULL;

// Called when the block of an icon definition is encountered. Simply finds
// the image, and sets the global.
// The exact same code is used by cursors. So now this is too!
int iconName(tpHookParms *parms)
{
   gTempImage *img;
   gToken     *imgname = (gToken *)gGetListItem(parms->inlist, 0);
   
   img = findImage(parms->pattern->tstream, imgname->token, stemp);

   if(!img)
   {
      gSetError("Error: Failed to load icon image %s\n", parms->t->token);
      return tpIncrementCount(parms->pattern, tpError);
   }

   iconname = img->name;

   tpClearInlist(parms);
   return tpNoError;
}


// Called when a successful icon definition is encountered.
int makeIcon(tpHookParms *parms)
{
   gToken      *name, *xt, *yt, *wt, *ht;
   gTempIcon   *icon;

   name = (gToken *)gGetListItem(parms->inlist, 0);
   xt = (gToken *)gGetListItem(parms->inlist, 1);
   yt = (gToken *)gGetListItem(parms->inlist, 2);
   wt = (gToken *)gGetListItem(parms->inlist, 3);
   ht = (gToken *)gGetListItem(parms->inlist, 4);

   icon = gAddIcon(name->token, stemp);
   icon->imgname = strdup(iconname);
   icon->rect.x = atoi(xt->token);
   icon->rect.y = atoi(yt->token);
   icon->rect.w = (unsigned)atoi(wt->token);
   icon->rect.h = (unsigned)atoi(ht->token);

   tpClearInlist(parms);
   return tpNoError;
}


// Called at the end of an icon block
int endIcon(tpHookParms *parms)
{
   iconname = NULL;
   return tpNoError;
}

// Called from the icon loop if there is an error. The objective is to find the
// first identifier, or closed bracket.
tpHookCode iconEHook(tpHookParms *parms)
{
   tPattern     *p = parms->pattern;
   gTokenStream *ts = p->tstream;
   gToken       *t;

   tpClearInlist(parms);

   // Skip ahead to the next statement
   while((t = gGetToken(ts, p->i)) != NULL)
   {
      switch(t->type)
      {
         case tEOF:
            proc = NULL;
            span = NULL;
            iconname = NULL;
            return tpAbort;
         case tIdentifier:
            return tpFallback;
         case tClosedBracket:
            iconname = NULL;
            return tpPopFallback;
         case tOpenBracket:
            gSetError("Warning: %s(%i, %i): Unexpected '{' encountered in block.", ts->name, t->linenum, t->charnum);
            tpIncrementCount(p, tpWarning);
         default:
            p->i++;
            break;
      }
   }

   // Loop ended without resolution:
   proc = NULL;
   span = NULL;
   iconname = NULL;
   return tpAbort;
}

// ----------------------------------------------------------------------------
// Cursors!

static gSurface *cursors = NULL;

// Called when a cursor block begins. This function assumes that inlist will have
// one element (the image name of the cursor file).
int loadCurs(tpHookParms *parms)
{
   gToken *namet;

   if(cursors)
      SDL_FreeSurface(cursors);

   namet = (gToken *)gGetListItem(parms->inlist, 0);

   if(!(cursors = loadImage(parms->pattern->tstream, namet->token)))
      return tpError;

   tpClearInlist(parms);
   return tpNoError;
}

// Called when the cursor block ends. Frees the temporary image file.
int freeCurs(tpHookParms *parms)
{
   if(cursors)
      SDL_FreeSurface(cursors);
   cursors = NULL;

   return tpNoError;
}


// Called when a cursor definition is successfully encountered.
int makeCursor(tpHookParms *parms)
{
   SDL_Color   transcolor = {255, 0, 255}, xorcolor = {0, 255, 255};
   gToken      *name, *xt, *yt, *wt, *ht, *xht, *yht;
   int         x, y, w, h, xhot, yhot;

   name = (gToken *)gGetListItem(parms->inlist, 0);
   xt = (gToken *)gGetListItem(parms->inlist, 1);
   yt = (gToken *)gGetListItem(parms->inlist, 2);
   wt = (gToken *)gGetListItem(parms->inlist, 3);
   ht = (gToken *)gGetListItem(parms->inlist, 4);
   xht = (gToken *)gGetListItem(parms->inlist, 5);
   yht = (gToken *)gGetListItem(parms->inlist, 6);

   x = atoi(xt->token);
   y = atoi(yt->token);
   w = atoi(wt->token);
   h = atoi(ht->token);
   xhot = atoi(xht->token);
   yhot = atoi(yht->token);

   gMakeCursor(cursors, &transcolor, &xorcolor, x, y, w, h, xhot, yhot, gAddCursor(name->token, stemp));

   tpClearInlist(parms);
   return tpNoError;
}



// Called from the icon loop if there is an error. The objective is to find the
// first identifier, or closed bracket.
tpHookCode cursEHook(tpHookParms *parms)
{
   tPattern     *p = parms->pattern;
   gTokenStream *ts = p->tstream;
   gToken       *t;

   tpClearInlist(parms);

   // Skip ahead to the next statement
   while((t = gGetToken(ts, p->i)) != NULL)
   {
      switch(t->type)
      {
         case tEOF:
            if(cursors)
               SDL_FreeSurface(cursors);
            cursors = NULL;
            return tpAbort;
         case tIdentifier:
            return tpFallback;
         case tClosedBracket:
            iconname = NULL;
            return tpPopFallback;
         case tOpenBracket:
            gSetError("Warning: %s(%i, %i): Unexpected '{' encountered in block.", ts->name, t->linenum, t->charnum);
            tpIncrementCount(p, tpWarning);
         default:
            p->i++;
            break;
      }
   }

   // Loop ended without resolution:
   if(cursors)
      SDL_FreeSurface(cursors);
   cursors = NULL;
   return tpAbort;
}




// ----------------------------------------------------------------------------
// Set hooks and ops

// Op which checks for all set data types (used as a preliminary gate)
bool opSetData(tpHookParms *parms, int steptype)
{
   switch(parms->t->type)
   {
      case tInteger:
      case tString:
      case tDecimal:
      case tHexInt:
      case tIdentifier:
      case tRGB:
      case tFont:
         return true;
      default:
         return false;
   }
}

// Op which checks for all font flags
bool opFontFlag(tpHookParms *parms, int steptype)
{
   switch(parms->t->type)
   {
      case tPlain:
      case tBold:
      case tItalic:
      case tUnderline:
         return true;
      default:
         return false;
   }
}

// Op which checks for the basic data types (string, integer, ect)
bool opBasicDat(tpHookParms *parms, int steptype)
{
   switch(parms->t->type)
   {
      case tInteger:
      case tString:
      case tDecimal:
      case tHexInt:
      case tIdentifier:
         return true;
      default:
         return false;
   }
}


// Op which checks for valid orientation values
bool opSpanOri(tpHookParms *parms, int steptype)
{
   switch(parms->t->type)
   {
      case tRows:
      case tColumns:
         return true;
      default:
         return false;
   }
}


// Op to check for span position types
bool opSpanLoc(tpHookParms *parms, int steptype)
{
   switch(parms->t->type)
   {
      case tStart:
      case tMid:
      case tEnd:
         return true;
      default:
         return false;
   }
}


// Op to check for bit types
bool opBitType(tpHookParms *parms, int steptype)
{
   switch(parms->t->type)
   {
      case tStartBit:
      case tMidBit:
      case tEndBit:
         return true;
      default:
         return false;
   }
}

  



// ----------------------------------------------------------------------------
// Skin Pattern.
// This comprises the program the parser executes.

tpStep skinSteps[] = {
// label,      stepOp, tokenType,     onTrue,  onFalse,  subLabel, tokenHook, errorHook, errorCode, errorMessage
// --- Main loop ---
{"main",     opNoComp,         0,   sfFBStay,  scSkip,      NULL,      NULL, skinEHook},
{ NULL,    opCompType,  tProcess, scPushBack,  scSkip, "process"},
{ NULL,    opCompType,      tSet, scPushBack,  scSkip,     "set"},
{ NULL,    opCompType,    tIcons, scPushBack,  scSkip,   "icons"},
{ NULL,    opCompType,  tCursors, scPushBack,  scSkip, "cursors"},
{ NULL,    opCompType,      tEOF,      scEnd, scThrow,      NULL,      NULL,      NULL,   tpError, "Unexpected '%s'"},
// --- Process start --- (first is 6)
{"process",opCompType,tOpenParan,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected '(' after 'process' keyword"},
{ NULL,    opCompType,tIdentifier,   sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected process name."},
{ NULL,    opCompType,    tComma,     scGoto,  scSkip,  "procimg"},
{ NULL,    opCompType,    tColon,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ',' or ':' after process name."},
{ NULL,    opCompType,tIdentifier,   sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected parent process name."},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ',' parent process name."},
{"procimg",opCompType,   tString,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected image name."},
{ NULL,    opCompType,tClosedParan,   scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ')'"},
{ NULL,    opCompType,tOpenBracket,   scGoto, scThrow,   "ploop",  makeProc,      NULL,   tpError, "'{' after process list."},
// -- Process loop -- (first is 14)
{"ploop",    opNoComp,         0,   sfFBStay,  scSkip,      NULL,      NULL, procEHook},
{ NULL,    opCompType,      tSet, scPushBack,  scSkip,     "set"},
{ NULL,    opCompType,    tSpans, scPushBack,  scSkip,   "spans"},
{ NULL,    opCompType,     tSpan, scPushBack,  scSkip,    "span"},
{ NULL,    opCompType,tClosedBracket,  scPop, scThrow,      NULL, clearProc,      NULL,   tpError, "Unexpected '%s' in process block."},
// --- Spans statement --- (first is 19)
{"spans",   opSpanOri,         0,     scNext, scThrow,      NULL,  spansSet,      NULL,   tpError, "Expected 'rows' or 'columns'."},
{ NULL,    opCompType,tSemiColon,      scPop, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ';' at end of spans statement"},
// --- Span statement --- (first is 21)
{"span",   opCompType,tOpenParan,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected '(' after span statement"},
{ NULL,    opSpanLoc,          0,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected 'start', 'mid' or 'end'"},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ','"},
{ NULL,    opCompType,  tInteger,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected span thickness"},
{ NULL,    opCompType,tClosedParan,   scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ')'"},
{ NULL,    opCompType,tOpenBracket,   scNext, scThrow,      NULL,  makeSpan,      NULL,   tpError, "Expected '{'"},
// -- Span loop -- (first is 27)
{"sploop",   opNoComp,         0,   sfFBStay,  scSkip,      NULL,      NULL, spanEHook},
{ NULL,     opBitType,         0,scPushBack|sfStore,scSkip,"bit"},
{ NULL,    opCompType, tGapColor, scPushBack,  scSkip,     "gap"},
{ NULL,    opCompType,tClosedBracket,  scPop, scThrow,      NULL,  spanDone,      NULL,   tpError, "Expected 'startbit', 'midbit', 'endbit', 'midcolor', or '}'"},
// -- Bit statement -- (first is 31)
{"bit",    opCompType,tOpenParan,     scPush, scThrow,    "xywh",      NULL,      NULL,   tpError, "Expected '('"},
{ NULL,    opCompType,tClosedParan,   scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ')'"},
{ NULL,    opCompType,tSemiColon,      scPop, scThrow,      NULL,   makeBit,      NULL,   tpError, "Expected ';' at end of bit statement"},
// -- midcolor statement -- (first is 34)
{"gap",      opNoComp,         0, scPushStay, scThrow,     "rgb"},
{ NULL,    opCompType,tSemiColon,      scPop, scThrow,      NULL,  makeFill,      NULL,   tpError, "Expected ';' at the end of 'midcolor' statement."},
// --- Set statement --- (first is 36)
{"set",    opCompType,tIdentifier,   sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected attribute name."},
{ NULL,    opCompType,   tEquals,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected '='"},
{ NULL,     opSetData,         0,     sfStay, scThrow,      NULL,      NULL,      NULL,   tpError, "Invalid property value '%s'"},
{ NULL,    opBasicDat,         0,    sfStore,  scSkip,      NULL, simpleSet},
{ NULL,    opCompType,      tRGB,     scPush,  scSkip,  "setrgb"},
{ NULL,    opCompType,     tFont,     scPush,  scSkip, "setfont"},
{ NULL,    opCompType,tSemiColon,      scPop, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ';' at end of set statement"},
// -- Set RGB -- (first is 43)
{"setrgb",      opNoComp,         0, scPushStay,    0,    "rgb"},
{ NULL,       opCompType,tSemiColon,scPop|sfStay,scThrow,   NULL,    rgbSet,      NULL,   tpError, "Expected ';' at end of set statement"},
// -- Set Font -- (first is 45)
{"setfont",opCompType,tOpenParan,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected '(' after fontspec"},
{ NULL,    opCompType,   tString,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected font name."},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ',' after font name."},
{ NULL,    opCompType,  tInteger,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected font size."},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ',' after font size."},
{ NULL,    opCompType,      tRGB,     scPush, scThrow,     "rgb",      NULL,      NULL,   tpError, "Expected 'rgb' and color spec."},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ',' after font color."},
{"fflag",  opFontFlag,         0,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected font flag."},
{ NULL,    opCompType,  tFlagSep,     scGoto,  scSkip,   "fflag"},
{ NULL,    opCompType,tClosedParan,    scPop, scThrow,      NULL,   fontSet,      NULL,   tpError, "Expected ')'"},
// --- Icons --- (first is 55)
{"icons",  opCompType,tOpenParan,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected '(' after 'icons'"},
{ NULL,    opCompType,   tString,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected a string with the icon image name."},
{ NULL,    opCompType,tClosedParan,   scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ')'"},
{ NULL,    opCompType,tOpenBracket,   scNext, scThrow,      NULL,  iconName,      NULL,   tpError, "Expected '{' and body of icons"},
// -- Icon loop -- (first is 59)
{ NULL,      opNoComp,         0,   sfFBStay,  scSkip,      NULL,      NULL, iconEHook},
{"icon",   opCompType,tIdentifier,   sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected icon name."},
{ NULL,    opCompType,    tComma,     scPush, scThrow,    "xywh",      NULL,      NULL,   tpError, "Expected ',' after icon name."},
{ NULL,    opCompType,tSemiColon,     scNext, scThrow,      NULL,  makeIcon,      NULL,   tpError, "Expected ';' at end of icon definition."},
{ NULL,    opCompType,tIdentifier,scGotoStay,  scSkip,    "icon"},
{ NULL,    opCompType,tClosedBracket,  scPop, scThrow,      NULL,   endIcon,      NULL,   tpError, "Expected '}' or icon name."},
// --- Cursors --- (first is 64)
{"cursors",opCompType,tOpenParan,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected '(' after 'cursors'"},
{ NULL,    opCompType,   tString,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected a string with the cursor image name."},
{ NULL,    opCompType,tClosedParan,   scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ')'"},
{ NULL,    opCompType,tOpenBracket,   scNext, scThrow,      NULL,  loadCurs,      NULL,   tpError, "Expected '{' and body of cursors"},
// -- Cursor loop -- (first is 68)
{ NULL,      opNoComp,         0,   sfFBStay,  scSkip,      NULL,      NULL, cursEHook},
{"cursor", opCompType,tIdentifier,   sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected cursor name."},
{ NULL,    opCompType,    tComma,     scPush, scThrow,    "xywh",      NULL,      NULL,   tpError, "Expected ',' after cursor name."},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ','"},
{ NULL,    opCompType,  tInteger,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected x-hotspot."},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ','"},
{ NULL,    opCompType,  tInteger,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected y-hotspot."},
{ NULL,    opCompType,tSemiColon,     scNext, scThrow,      NULL,makeCursor,      NULL,   tpError, "Expected ';' at end of cursor definition."},
{ NULL,    opCompType,tIdentifier,scGotoStay,  scSkip,  "cursor"},
{ NULL,    opCompType,tClosedBracket,  scPop, scThrow,      NULL,  freeCurs,      NULL,   tpError, "Expected '}' or cursor name."},
// --- General work patterns ---
// -- (r, g, b) --  (first is 78)
{"rgb",    opCompType,tOpenParan,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected '(' after 'rgb'"},
{ NULL,    opCompType,   tHexInt,     scGoto,  scSkip,  "rgbend",  colorHex},
{ NULL,    opCompType,  tInteger,     scNext, scThrow,      NULL,    colorR,      NULL,   tpError, "Expected red color component or hex color"},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ',' after red color component"},
{ NULL,    opCompType,  tInteger,     scNext, scThrow,      NULL,    colorG,      NULL,   tpError, "Expected green color component"},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ',' after green color component"},
{ NULL,    opCompType,  tInteger,     scNext, scThrow,      NULL,    colorB,      NULL,   tpError, "Expected blue color component"},
{"rgbend", opCompType,tClosedParan,    scPop, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ')' after blue color component"},
// -- x, y, w, h --  (first is 86)
{"xywh",   opCompType,  tInteger,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected x-coordinate."},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ','"},
{ NULL,    opCompType,  tInteger,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected y-coordinate."},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ','"},
{ NULL,    opCompType,  tInteger,    sfStore, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected width."},
{ NULL,    opCompType,    tComma,     scNext, scThrow,      NULL,      NULL,      NULL,   tpError, "Expected ','"},
{ NULL,    opCompType,  tInteger,sfStore|scPop, scThrow,    NULL,      NULL,      NULL,   tpError, "Expected height."},
{ NULL}};




static bool readSkin(gTextStream *stream, const char *name, gSkinTemplate *skin)
{
   gTokenStream   *ts;
   gTokenParms    *parms;
   tPattern       *pattern;
   bool           flag = false;

   stemp = skin;
   proc = NULL;
   span = NULL;
   cursors = NULL;
   iconname = NULL;

   parms = gNewParms();

   parms->symbollist = gSkinSymbolList;
   parms->keywlist = gSkinKeyList;
   parms->flags = 0;
   
   parms->setError = gSetError;
   parms->getError = gGetLastError;

   ts = gCreateTokenStream(parms, stream, name);

   pattern = tpNewPattern(skinSteps, ts);

   if(!gCheckError())
      flag = (tpExecutePattern(pattern) == tpNoError) ? true : false;

   tpFreePattern(pattern);
   gFreeParms(parms);
   gFreeStream(stream);

   proc = NULL;
   span = NULL;
   stemp = NULL;
   if(cursors)
      SDL_FreeSurface(cursors);
   cursors = NULL;
   iconname = NULL;

   return flag;
}



#define gMarkupFileName "skin.txt"

bool gReadSkinArchive(const char *achivename, gSkinTemplate *skin)
{
   gTextStream *stream;
   char        *buffer;
   int         bufferlen, bytesread;
   PHYSFS_file *markup;

   gResetError();

   // OK. Skin files are read from zip files now. 
   // <Ominous voice>INITIATE PHYSICSFS!</Ominous voice>
   if(!PHYSFS_addToSearchPath(achivename, 0))
   {
      gSetError("Failed to find skin archive %s\n", achivename);
      return false;
   }

   // Look for skin.txt
   if(!PHYSFS_exists(gMarkupFileName))
   {
      gSetError("Failed to load a skin from %s: archive doesn't contain %s", achivename, gMarkupFileName);
      return false;
   }

   markup = PHYSFS_openRead(gMarkupFileName);
   bufferlen = (int)PHYSFS_fileLength(markup);
   buffer = (char *)malloc(bufferlen);
   if((bytesread = (int)PHYSFS_read(markup, buffer, 1, bufferlen)) < bufferlen)
   {
      gSetError("Error reading %s from archive %s. Bytes read != file length. (%i != %i)", gMarkupFileName, achivename, bytesread, bufferlen);
      PHYSFS_close(markup);
      return false;
   }

   PHYSFS_close(markup);

   // Because the owner flag will be set, we don't have to free the buffer.
   stream = gStreamFromMemory(buffer, bufferlen, true);
   if(!stream)
   {
      gSetError("Internal error: failed to create a stream from the skin markup file...");
      return false;
   }

   return readSkin(stream, gMarkupFileName, skin);
}

#undef gMarkupFileName




