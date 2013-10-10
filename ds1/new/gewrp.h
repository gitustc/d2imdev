/*
 * =====================================================================================
 *
 *       Filename: gewrp.h
 *        Created: 10/09/2013 10:22:06 PM
 *  Last Modified: 10/10/2013 01:42:15 AM
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



typedef ALLEGRO_DISPLAY     GEWRP_DISP;
typedef ALLEGRO_TIMER       GEWRP_TIMER;
typedef ALLEGRO_BITMAP      GEWRP_BITMAP;
typedef ALLEGRO_EVENT_QUEUE GEWRP_EVTQ;
typedef ALLEGRO_EVENT       GEWRP_EVENT;


struct GEWRP_GEINSTANCE {
    GEWRP_DISP     *disp;
    GEWRP_EVTQ     *evtq;
    GEWRP_TIMER    *timer;
    int32_t         fps;

};
typedef struct GEWRP_INST GEWRP_INST;

extern  GEWRP_INST  glb_gewrp_inst;





