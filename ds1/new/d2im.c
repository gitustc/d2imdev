/*
 * =====================================================================================
 *
 *       Filename: d2im.c
 *        Created: 10/10/2013 01:19:53 AM
 *  Last Modified: 10/10/2013 01:36:03 AM
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

#include "d2im.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  d2im_init
 *  Description:  
 * =====================================================================================
 */




int d2im_init ()
{
    gewrp_init();
    return 0;
}









/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  d2im_run
 *  Description:  
 * =====================================================================================
 */
int d2im_run ()
{

    GEWRP_EVENT
    while (1) {

        al_wait_for_event(glb_gewrp.evtq, &event);

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                return;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    return;
                break;

            case ALLEGRO_EVENT_TIMER:
                need_draw = true;
                break;
        }
    }
    return 0;
}




/*
 * ===  FUNCTION  ======================================================================
 *         Name:  d2im_quit
 *  Description:  
 * =====================================================================================
 */
int d2im_quit ()
{

    return 0;
}
