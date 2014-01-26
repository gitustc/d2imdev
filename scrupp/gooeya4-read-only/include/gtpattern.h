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

#ifndef GTPATTERN_H
#define GTPATTERN_H

/**
 * \file gtpattern.h
 * \brief Contains definitions and prototypes for token pattern processing.
 *
 * A 'token pattern' is a set of instructions that operate based on a stream of
 * tokens. The 'token processor' steps through the instruction set using the
 * output of op codes as conditions. Each step has an op code that either
 * returns true or false and each step has codes for both cases. The token
 * processor has an exection stack which can be pushed and poped, and a list
 * of stored tokens which hook functions can then read specific input from
 * without having to wade through all the tokens to get them.
*/

#include "gstack.h"

#ifdef __cplusplus
extern "C"
{
#endif


// ----------------------------------------------------------------------------
// Token Patterns

#ifndef DOXYGEN_IGNORE
typedef struct tPattern tPattern; //!< Typedef of struct tPattern
typedef struct tpStep tpStep;     //!< Typedef of struct tpStep
#endif

/**
 * \struct tpHookParms
 * \brief Information sent to pattern hooks.
 *
 * Information sent by the token processor to both step, error, and op hooks.
*/
typedef struct
{
   tPattern   *pattern; //!< Token pattern being processed.  
   gToken     *t;       //!< Current token
   gList      *inlist;  //!< Stored input list.
   tpStep     *step;    //!< Pointer to the calling step.
} tpHookParms;


/**
 * \typedef bool (*tpOpFunc)(tpHookParms *parms, int steptype);
 * \brief Step-op function pointer
 *
 * Op pointers
 * These ops check the given token (parms->t) and returns true for matches
 * or false for not matching (criteria can varry).
 * 
 * @param[in] parms tpHookParms supplied by the token processor.
 * @param[in] steptype The token type specified in the current step.
 * @return true if the otken meets the criteria of the op function, or false if it does not.
*/
typedef bool (*tpOpFunc)(tpHookParms *parms, int steptype);


/**
 * Standard OPs
*/

/**
 * \fn bool opNoComp(tpHookParms *parms, int steptype)
 * \brief Returns true.
 *
 * This op is used for steps that need to proceed reguardless of the token type.
 * Examples of such cases: A step that sets the fallback point or an error hook.
 *
 * \see gtpattern.h::tpOpFunc
*/
bool opNoComp(tpHookParms *parms, int steptype);


/**
 * \fn bool opCompType(tpHookParms *parms, int steptype)
 * \brief
 *
 * Compares the type of the current token (parms->t) with the steptype parameter
 * and returns true if they match.
 * \see gtpattern.h::tpOpFunc
*/
bool opCompType(tpHookParms *parms, int steptype);





/**
 * \enum tpStepCode
 * \brief Codes to be executed by the token processor.
 *
 * These codes are assigned to onTrue and onFalse values in each token step 
 * and they define the actions the pattern takes depending on the return 
 * value of the stepOp. These make up the code half of the full action. 
 * \see gtpattern.h::tpStepFlags, gtpattern.h::tpStep
*/
typedef enum
{
   scNext = 0x0, //!< Continue to the next step

   scThrow,      //!< Call current error handling hook

   scPush,       //!< Push a new stack entry which executes from the label given

   scGoto,       //!< Move execution to the label given

   scPop,        //!< Pop the stack and continue execution from the previous entry

   scEnd,        //!< End execution. Completely stop it.

   // There are six different codes which means the mask for codes needs to be
   // at least 0x7
   scCodeMask = 0x7
} tpStepCode;



/**
 * \enum tpStepFlags
 * \brief Flags which can be combined with stepcodes.
 *
 * tpStepFlags are extra actions to be performed. These can be ORed together with
 * a step code to form the full action to be performed. The codes start from bit
 * 32 and work their way back down.
 * \see: gtpattern.h::tpStepCode, gtpattern.h::tpStep
*/
typedef enum
{
   /** This flag signifies that the current token should be stored in the output
       list which is passed to step hooks. */
   sfStore       = 0x8000000,

   /** This flag is only considered by scPush, and if set will reset the 
       current stack pointer to it's entry point (or the stack entry fallback point)
       beforing pushing the new stack entry */
   sfBack     = 0x4000000,

   /** This flag causes the step NOT to increment the token pointer within
       the stream. */
   sfStay   = 0x1000000,

   /** Sets the current step as both the back point the fallback point for the current
       stack entry. */
   sfSetFB = 0x800000,

} tpStepFlags;



/**
 * \def sfFBStay
 * Combined values of sfSetFB and sfStay. Action: Sets the fallback point and 
 * doesn't advance the token.
*/
#define sfFBStay      sfSetFB|sfStay

/**
 * \def scPushBack
 * Combined values of scPush and sfBack. Action: Returns execution of the current
 * stack entry to the stack entry point or last fallback point established, then
 * pushes the step label on the stack.
*/
#define scPushBack    scPush|sfBack

/**
 * \def scSkip
 * Combined values of scNext and sfStay. Action: "Skips" the step, that is, 
 * increments the step, but doesn't advance the token.
*/
#define scSkip        scNext|sfStay

/**
 * \def scGotoStay
 * Combined values of scGoto and sfStay. Action: Moves execution of the current
 * stack entry to the given label and doesn't advance the token.
*/
#define scGotoStay    scGoto|sfStay

/**
 * \def scPushStay
 * Combined values of scPush and sfStay. Action: Pushes the current token to the
 * input list and doesn't advance the token.
*/
#define scPushStay    scPush|sfStay


/**
 * \enum tpResultCode
 * \brief Error codes.
 *
 * These are the codes used to track errors. They can be set with the scThrow
 * code or returned from step hooks.
*/
typedef enum 
{
   tpNoError, //<! No error, continue execution
   tpWarning, //<! Warning code, the error hook function is still called.
   tpError,   //<! Error. The error hook function is called.
   tpFatal,   //<! Fatal error. Execution of the pattern Stops.
} tpResultCode;



/**
 * \enum tpHookCode
 * These codes are returned by the error handling hooks and tell the token
 * processor what to do next.
*/
typedef enum
{
   tpContinue, //!< Continue on with the current pattern.

   tpFallback, //!< Return execution to the nearest fallback point.

   tpPopFallback, //!< Pop the stack, then return execution to the nearest fallback point.

   tpAbort //!< Completely stop execution of the pattern.
} tpHookCode;




typedef int (*tpStepHook)(tpHookParms *);       //!< Typedef for step hook functions
typedef tpHookCode (*tpErrHook)(tpHookParms *); //!< Typedef for error hook functions


/**
 * \struct tpStep
 * \brief Instructional unit for the token processor.
 *
 * A token pattern is basically a table of instructional "steps" which traverse
 * the token stream and perform various actions based on the token types 
 * encountered (although custom op functions can be created to proceed based on
 * token content as well). 
*/
typedef struct tpStep
{
   char *label; //!< Label for this current step

   tpOpFunc stepOp; //!< Op function \see gtpattern.h::tpOpFunc

   int tokenType; //!< The token type supplied to the stepOp.

   int  onTrue;   //!< Action to be performed if stepOp returns true
   int  onFalse;  //!< Action to be performed if stepOp returns false

   char *subLabel; //!< Name of the label used for scPush and scGoto

   tpStepHook hook; //!< Hook function to be called of stepOp returns true (ignored if NULL)

   tpErrHook  errHook; //!< If not NULL, the error hook in the current stack top gets set to this.

   int elevel; //!< If the code executed is scThrow, this is the error level

   /** If executed action of the step is scThrow, this is the error message
       It is a printf format string and will be called with the following input
       - A string (with the stream name)
       - An integer (line number)
       - A second integer (column number) */
   char *emsg;
};




/**
 * \struct tPattern
 * \brief The struct representing a pattern.
 *
 * This is the struct passed to the token processor. Manually modification
 * of the members of this struct is _strongly_ discouraged. Please use 
 * the associated functions instead.
 * \see  gtpattern.h::tpNewPattern, gtpattner.h::tpResetPattern,
 *       gtpattern.h::tpFreePattern, gtpattern.h::tpClearInlist,
 *       gtpattern.h::tpIncrementCount, gtpattern.h::tpIncrementCount,
 *       gtpattern.h::tpExecutePattern
*/
typedef struct tPattern
{
   tpStep *stepList; //!< Instruction set for the pattern

   gStringHash *labelTable; //!< Hash table used for label name lookups

   gTokenStream *tstream; //!< Token stream the tokens are being read from

   int   i;       //!< Index within the token stream the token processor is at

   gStack *stack; //!< Operations stack

   int   ecount;  //!< Current error count
   int   wcount;  //!< Current warning count
};



/**
 * \fn tPattern *tpNewPattern(tpStep *stepList, gTokenStream *tstream)
 * \brief Creates a new token pattern.
 *
 * Creates a new tPattern object for use with the parsing functions.
 *
 * @param[in] stepList An array of tpStep structs. This is the body of the pattern.
 * @param[in] tstream gTokenStream object used to obtain tokens.
 * @returns new tPattern object.
*/
tPattern *tpNewPattern(tpStep *stepList, gTokenStream *tstream);


/**
 * \fn void tpResetPattern(tPattern *p)
 * \brief Resets a token pattern
 * 
 * This function clears the operations stack, resets the token stream,
 * clears error/warning counts, and resets the token index of the given 
 * token pattern. Returns it to it's initial state.
 *
 * @param[in] p Token pattern to reset.
*/
void tpResetPattern(tPattern *p);


/**
 * \fn void tpFreePattern(tPattern *p)
 * \brief Frees a tPattern object.
 *
 * Frees the given tPattern. This function will not attempt to free
 * stepList or close the token stream.
 *
 * @param p tPattern object to free.
*/
void tpFreePattern(tPattern *p);


/**
 * \fn void tpClearInlist(tpHookParms *p)
 * \brief Clears the input list.
 *
 * Clears out the inlist a token pattern, and clears the cache in the token stream.
 *
 * @param p Hook parms passed to the hook functions.
*/
void tpClearInlist(tpHookParms *p);


/**
 * \fn int tpIncrementCount(tPattern *p, int code)
 *
 * Increments either the warning count or the error count in the given pattern
 * depending on the code supplied. Returns code for chaining. 
 *
 * @param[in] p Pattern with counts to increment.
 * @param[in] code Error/Warning code that occured.
 * @return \a code
*/
int tpIncrementCount(tPattern *p, int code);


/**
 * \fn int tpExecutePattern(tPattern *p)
 * \brief Runs a token pattern in the processor.
 *
 * Executes the given pattern object and returns the highest error level
 * encountered during execution.
 *
 * @param[in] p Pattern to be executed.
*/
int tpExecutePattern(tPattern *p);


#ifdef __cplusplus
}
#endif


#endif