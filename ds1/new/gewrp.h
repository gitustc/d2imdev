/*
 * =====================================================================================
 *
 *       Filename: gewrp.h
 *        Created: 10/09/2013 10:22:06 PM
 *  Last Modified: 10/21/2013 08:14:27 PM
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
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_ttf.h>
#include "d2im.h"



typedef ALLEGRO_DISPLAY     GEWRP_DISP;
typedef ALLEGRO_TIMER       GEWRP_TIMER;
typedef ALLEGRO_BITMAP      GEWRP_BITMAP;
typedef ALLEGRO_EVENT_QUEUE GEWRP_EVTQ;
typedef ALLEGRO_EVENT       GEWRP_EVENT;


struct D2GE_INST {
    GEWRP_DISP     *disp;
    GEWRP_EVTQ     *evtq;
    GEWRP_TIMER    *timer;
    int32_t         fps;

};
typedef struct D2GE_INST D2GE_INST;

extern  D2GE_INST  glb_gewrp_inst;



int gewrp_init(uint32_t);
int gewrp_quit();


typedef ALLEGRO_FONT            D2FONT;
#define gewrp_load_font(path, size)     (al_load_font((path), (size), 0))


typedef ALLEGRO_COLOR           D2COLOR;
#define gewrp_map_rgb(r, g, b)  al_map_rgb((r), (g), (b))


#define GEWRP_ALIGN_LEFT        ALLEGRO_ALIGN_LEFT
#define GEWRP_ALIGN_RIGHT       ALLEGRO_ALIGN_RIGHT
#define GEWRP_ALIGN_CENTRE      ALLEGRO_ALIGN_CENTRE
#define gewrp_draw_text(font, color, x, y, flag, text)  (al_draw_text((font), (color), (x)*1.0, (y)*1.0, (flag), (text)))


#define gewrp_inst()                (glb_gewrp_inst)
#define gewrp_get_disp()            (glb_gewrp_inst.disp)
#define gewrp_get_evtq()            (glb_gewrp_inst.evtq)
#define gewrp_get_timer()           (glb_gewrp_inst.timer)
#define gewrp_get_fps()             (glb_gewrp_inst.fps)


#define gewrp_poll_event(evtq, pevt)    (al_wait_for_event( (evtq), (pevt)))




#define GEWRP_EVENT_DISPLAY_CLOSE   ALLEGRO_EVENT_DISPLAY_CLOSE


#define gewrp_flip()                al_flip_display()



#define gewrp_set_target_bitmap(p)   al_set_target_bitmap(p)
#define gewrp_get_backbuff(disp)     al_get_backbuffer(disp)





#define gewrp_clear(color)          al_clear_to_color(color)

#define gewrp_start_timer()         al_start_timer(gewrp_get_timer())
