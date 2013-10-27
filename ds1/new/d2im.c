/*
 * =====================================================================================
 *
 *       Filename: d2im.c
 *        Created: 10/10/2013 01:19:53 AM
 *  Last Modified: 10/26/2013 09:19:55 PM
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
 * global variables for d2im.c
 */

D2IM_INST       glb_d2im;

/*
 * ===  FUNCTION  ======================================================================
 *         Name:  d2im_read_config
 *  Description:  read configure ./d2im.xml
 * =====================================================================================
 */
static int d2im_read_config ()
{

    iks    *p;
    iks    *t;
    int     e;

    e = iks_load( "./d2im.xml", &p);

    glb_d2im.xml_conf = p;

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
            glb_d2im.xml_conf = t;
            fprintf(stdout, "info: %s\n", iks_find_cdata(t, "chinese") );
            break;
        }

        t = iks_next(t);
    }



    return 0;
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  d2im_init
 *  Description:  init
 * =====================================================================================
 */


int d2im_init ()
{


    d2im_read_config();

    if( !al_init() ){
        fprintf ( stderr, "fatal error: open allegro library failed...\n" );
        exit(0);
    }
    al_install_keyboard();
    al_install_mouse();

    al_init_font_addon();
    al_init_ttf_addon();


    glb_d2im.display = al_create_display(640, 480);

    if ( glb_d2im.display == NULL ) {
        fprintf ( stderr, "fatal error: open display failed...\n" );
        exit(0);
    }


    glb_d2im.fps = 60;
    glb_d2im.timer  = al_create_timer(1.0 / glb_d2im.fps);
    glb_d2im.event_queue = al_create_event_queue();
    al_register_event_source(glb_d2im.event_queue, al_get_keyboard_event_source());
    al_register_event_source(glb_d2im.event_queue, al_get_mouse_event_source());
    al_register_event_source(glb_d2im.event_queue, al_get_display_event_source(glb_d2im.display));
    al_register_event_source(glb_d2im.event_queue, al_get_timer_event_source(glb_d2im.timer));



    return 0;
}



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  d2im_run
 *  Description:  main loop here
 * =====================================================================================
 */
int d2im_run ()
{

    ALLEGRO_FONT     *font;
    ALLEGRO_EVENT     event;

    font = al_load_font("./wqy-microhei.ttf", 12, 0);
    if( font == NULL ){
        fprintf(stdout, "can not load font.\n" );
        exit(0);
    }

    al_start_timer(glb_d2im.timer);

    while (1) {


        al_clear_to_color(al_map_rgb(0, 0, 0));

        al_set_target_bitmap(al_get_backbuffer(glb_d2im.display));

        al_draw_text(font, al_map_rgb(250,250,50), 12.0, 12.0, 0, iks_find_cdata(glb_d2im.xml_conf, "chinese")) ;

        al_wait_for_event( glb_d2im.event_queue, &event );

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

        al_flip_display();
    }
    return 0;
}




/*
 * ===  FUNCTION  ======================================================================
 *         Name:  d2im_quit
 *  Description:  quit
 * =====================================================================================
 */
int d2im_quit ()
{

    return 0;
}
