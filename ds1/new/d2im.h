/*
 * =====================================================================================
 *
 *       Filename: d2im.h
 *        Created: 10/10/2013 01:14:52 AM
 *  Last Modified: 10/20/2013 06:48:32 AM
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

#include <iksemel.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>


struct D2IM_INST {
    iks * config;
};
typedef struct D2IM_INST D2IM_INST;

extern D2IM_INST        glb_d2im_inst;
#define d2im_inst()                 (glb_d2im_inst);





int d2im_init();
int d2im_run();
int d2im_quit();





#endif
