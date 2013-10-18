/*
 * =====================================================================================
 *
 *       Filename: d2im.h
 *        Created: 10/10/2013 01:14:52 AM
 *  Last Modified: 10/18/2013 12:12:19 AM
 *
 *    Description: game logic
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

#ifndef  _included_d2im_inc__
#define  _included_d2im_inc__



struct D2IM_INST {
};
typedef struct D2IM_INST D2IM_INST;
extern D2IM_INST        glb_d2im_inst;


int d2im_init();
int d2im_free();





#endif   /* ----- #ifndef _included_d2im_inc__  ----- */


