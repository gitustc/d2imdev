/*
 * =====================================================================================
 *
 *       Filename: d2im.h
 *        Created: 10/10/2013 01:14:52 AM
 *  Last Modified: 10/26/2013 09:19:15 PM
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

#include <stdio.h>
#include <iksemel.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>


struct D2IM_INST {
    iks                 *xml_conf;
    float               fps;

    ALLEGRO_DISPLAY     *display;
    ALLEGRO_TIMER       *timer;
    ALLEGRO_EVENT_QUEUE *event_queue;
};
typedef struct D2IM_INST D2IM_INST;
extern D2IM_INST        glb_d2im;





int d2im_init();
int d2im_run();
int d2im_quit();





#endif
