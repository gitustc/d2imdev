/*
 * =====================================================================================
 *
 *       Filename: gewrp.h
 *        Created: 10/09/2013 10:22:06 PM
 *  Last Modified: 10/21/2013 08:41:03 PM
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



typedef ALLEGRO_DISPLAY     D2DISPLAY;
typedef ALLEGRO_TIMER       D2TIMER;
typedef ALLEGRO_BITMAP      D2BITMAP;
typedef ALLEGRO_EVENT_QUEUE D2EVENTQ;
typedef ALLEGRO_EVENT       D2EVENT;


struct D2GE_INST {
    D2DISPLAY       *disp;
    D2EVENTQ        *eventq;
    D2TIMER         *timer;
    int32_t         fps;

};
typedef struct D2GE_INST D2GE_INST;

extern  D2GE_INST  glb_d2ge_inst;
#define d2ge_get_inst()         (&glb_d2ge_inst)



int d2ge_init(uint32_t);
int d2ge_quit();


typedef ALLEGRO_FONT            D2FONT;
#define d2font_load(path, size)     (al_load_font((path), (size), 0))


typedef ALLEGRO_COLOR           D2COLOR;
#define d2color_map_rgb(r, g, b)  al_map_rgb((r), (g), (b))


#define GEWRP_ALIGN_LEFT        ALLEGRO_ALIGN_LEFT
#define GEWRP_ALIGN_RIGHT       ALLEGRO_ALIGN_RIGHT
#define GEWRP_ALIGN_CENTRE      ALLEGRO_ALIGN_CENTRE
#define d2ge_draw_text(font, color, x, y, flag, text)  (al_draw_text((font), (color), (x)*1.0, (y)*1.0, (flag), (text)))


#define d2ge_get_instance()                (glb_d2ge_inst)
#define d2ge_get_display()         (glb_d2ge_inst.disp)
#define d2ge_get_eventq()           (glb_d2ge_inst.evtq)
#define d2ge_get_timer()           (glb_d2ge_inst.timer)
#define d2ge_get_fps()             (glb_d2ge_inst.fps)


#define d2ge_poll_event(evtq, pevt)    (al_wait_for_event( (evtq), (pevt)))




#define GEWRP_EVENT_DISPLAY_CLOSE   ALLEGRO_EVENT_DISPLAY_CLOSE


#define d2ge_flip()                al_flip_display()



#define d2ge_set_target_bitmap(p)   al_set_target_bitmap(p)
#define d2ge_get_backbuff(disp)     al_get_backbuffer(disp)





#define d2ge_clear(color)          al_clear_to_color(color)

#define d2ge_start_timer()         al_start_timer(d2ge_get_timer())
