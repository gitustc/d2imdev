/*
 * =====================================================================================
 *
 *       Filename: d2bitmap.h
 *        Created: 10/25/2013 03:08:14 PM
 *  Last Modified: 10/25/2013 08:14:57 PM
 *
 *    Description: definition of d2bitmap, and all operation based on it
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */



#ifndef  _included_d2bitmap_inc__
#define  _included_d2bitmap_inc__



#include "d2color.h"

typedef ALLGERO_BITMAP      D2BITMAP;

#define d2ge_bitmap_width( p )             al_get_bitmap_width( p )
#define d2ge_bitmap_height( p )            al_get_bitmap_height( p )



#endif
