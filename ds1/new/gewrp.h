/*
 * =====================================================================================
 *
 *       Filename: gewrp.h
 *        Created: 10/09/2013 10:22:06 PM
 *  Last Modified: 10/19/2013 04:01:02 PM
 *
 *    Description: wrap for game engine, to get a universe game api.
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




#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <allegro.h>
#include "d2im.h"



typedef ALLEGRO_DISPLAY     GEWRP_DISP;
typedef ALLEGRO_TIMER       GEWRP_TIMER;
typedef ALLEGRO_BITMAP      GEWRP_BITMAP;
typedef ALLEGRO_EVENT_QUEUE GEWRP_EVTQ;
typedef ALLEGRO_EVENT       GEWRP_EVENT;


struct GEWRP_INST {
    GEWRP_DISP     *disp;
    GEWRP_EVTQ     *evtq;
    GEWRP_TIMER    *timer;
    int32_t         fps;

};
typedef struct GEWRP_INST GEWRP_INST;

extern  GEWRP_INST  glb_gewrp_inst;


int gewrp_init(uint32_t);







#define gewrp_inst()                (glb_gewrp_inst)
#define gewrp_get_disp()            (glb_gewrp_inst.disp)
#define gewrp_get_evtq()            (glb_gewrp_inst.evtq)
#define gewrp_get_timer()           (glb_gewrp_inst.timer)
#define gewrp_get_fps()             (glb_gewrp_inst.fps)


#define gewrp_poll_event(evtq, pevt)    (al_wait_for_event( (evtq), (pevt)))




#define GEWRP_EVENT_DISPLAY_CLOSE   ALLEGRO_EVENT_DISPLAY_CLOSE
