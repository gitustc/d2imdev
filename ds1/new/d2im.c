/*
 * =====================================================================================
 *
 *       Filename: d2im.c
 *        Created: 10/10/2013 01:19:53 AM
 *  Last Modified: 10/21/2013 08:31:15 PM
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
 *         Name:  d2im_read_config
 *  Description:  
 * =====================================================================================
 */
static int d2im_read_config ()
{

    iks    *p;
    iks    *t;
    int     e;

    e = iks_load( "./d2im.xml", &p);
    if(e != IKS_OK ){
        fprintf(stderr, "can not open d2im.xml.\n" );
        exit(0);
    }

    t = iks_child(p);

    while ( t ){

        /* window size */
        if ( iks_type( t ) == IKS_TAG && !strcmp( iks_name( t ), "win" ) ){
            fprintf(stdout, "width: %s\n", iks_find_cdata(t, "width") );
            fprintf(stdout, "height: %s\n", iks_find_cdata(t, "height") );
        }

        /* other test */
        if ( iks_type( t ) == IKS_TAG && !strcmp( iks_name( t ), "utf8" ) ){
            glb_d2im_inst.config = t;
            fprintf(stdout, "info: %s\n", iks_find_cdata(t, "chinese") );
            break;
        }

        t = iks_next(t);
    }

#if 0
    iks_delete(p);
#endif

    return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  d2im_init
 *  Description:  
 * =====================================================================================
 */


int d2im_init ()
{



    d2im_read_config();
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

    D2FONT     *font;
    D2EVENT     event;

    font = gewrp_load_font("./wqy-microhei.ttf", 12);
#if 0
    font = gewrp_load_font("simsun.ttf", 12);
#endif 
    if( font == NULL ){
        fprintf(stdout, "can not load font.\n" );
        exit(0);
    }

    gewrp_start_timer();

    while (1) {


        gewrp_clear(gewrp_map_rgb(0, 0, 0));

        gewrp_set_target_bitmap(gewrp_get_backbuff(gewrp_get_disp()));
        gewrp_draw_text(font, gewrp_map_rgb(250,250,50), 12, 12, 0, iks_find_cdata(glb_d2im_inst.config, "chinese")) ;

        gewrp_poll_event( gewrp_get_evtq(), (&event) );

        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                {
                    break;
                }

            case ALLEGRO_EVENT_KEY_DOWN:
                {
                    if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                        return 0;
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

        gewrp_flip();
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
