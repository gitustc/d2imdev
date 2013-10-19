/*
 * =====================================================================================
 *
 *       Filename: d2im.c
 *        Created: 10/10/2013 01:19:53 AM
 *  Last Modified: 10/19/2013 04:00:41 PM
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
#include "gewrp.h"


/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  d2im_init
 *  Description:  
 * =====================================================================================
 */




int d2im_init ()
{
    gewrp_init(0);
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

    GEWRP_EVENT     event;

    while (1) {

        fprintf(stdout, "here\n" );

        gewrp_poll_event( gewrp_get_evtq(), (&event) );

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                {
                    break;
                }

            case ALLEGRO_EVENT_KEY_DOWN:
                {
                    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                        return;
                    break;

                }

            case ALLEGRO_EVENT_TIMER:
                {
                    break;
                }
            default:
                {
                }
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
