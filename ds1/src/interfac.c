#include "structs.h"
#include "wpreview.h"
#include "editobj.h"
#include "misc.h"
#include "animdata.h"
#include "anim.h"
#include "txtread.h"
#include "interfac.h"

struct timeval  _my_delay;
int             _my_count = 1;

void _my_set_d(){
    _my_delay.tv_sec  = 0;
    _my_delay.tv_usec = 40*1000;
}

void my_set_fps(){

    _my_count++;
    _my_set_d();

    ds1edit_counter_tick();
    if( _my_count == 26 ){
        ds1edit_counter_fps();
        _my_count = 1;
    }
    //select( 0, NULL, NULL, NULL, &_my_delay );

}


int _my_key_state(char *key)
{
    if(*key){
        while(*key){
        }
        return 1;
    }
    return 0;
}

#define RELEASE_KEY(keyname) (_my_key_state(key+KEY_##keyname))
#define KEY_STATE(keyname)   (!(!(key[KEY_##keyname])))

int  ds1_idx;
void show_all_layer(int b)
{
    int i;
    if(b){
        for (i=0; i<FLOOR_MAX_LAYER; i++) {
            glb_ds1.floor_layer_mask[i]  = 1;
        }
        for (i=0; i<WALL_MAX_LAYER; i++) {
            glb_ds1.wall_layer_mask[i]   = 1;
        }
        for (i=0; i<SHADOW_MAX_LAYER; i++) {
            glb_ds1.shadow_layer_mask[i] = 3;
        }
    }
}

void center_the_map(int b)
{
    int cx, cy, dx, dy;
    if(b){
        cx = glb_ds1.width/2 + 1;
        cy = glb_ds1.height/2;
        dx = (cy * -glb_ds1.tile_w / 2) + (cx * glb_ds1.tile_w / 2);
        dy = (cy *  glb_ds1.tile_h / 2) + (cx * glb_ds1.tile_h / 2);
        glb_ds1.own_wpreview.x0 = glb_ds1edit.win_preview.x0 = dx - glb_ds1edit.win_preview.w / 2;
        glb_ds1.own_wpreview.y0 = glb_ds1edit.win_preview.y0 = dy - glb_ds1edit.win_preview.h / 2;
    }
}

// ==========================================================================
// MAIN loop
void interfac_user_handler(int start_ds1_idx)
{
    //int  ds1_idx;
    //
    int  done, cx, cy;
    int  old_mouse_x = mouse_x, old_mouse_y=mouse_y, old_mouse_b=0;
    int  cur_mouse_z = 0, old_mouse_z = 0;
    int  ticks_elapsed;


    // init
    ds1_idx               = start_ds1_idx;
    done                  = FALSE;


    // main loop
    while (! done) {

        my_set_fps();

        old_mouse_x = mouse_x;
        old_mouse_y = mouse_y;
        old_mouse_b = mouse_b;

        cur_mouse_z = mouse_z;

        mouse_to_tile(ds1_idx, &cx, &cy);

        if (cx < 0) { cx = 0; } else if (cx >= glb_ds1.width)  { cx = glb_ds1.width  - 1; }
        if (cy < 0) { cy = 0; } else if (cy >= glb_ds1.height) { cy = glb_ds1.height - 1; }

        // check if need to redraw the screen because of floor animation
        ticks_elapsed = glb_ds1edit.ticks_elapsed;
        if ( ticks_elapsed && (glb_ds1.animations_layer_mask == 1)) {
            // animated floor rate = 10 fps
            // therefore it's at 2/5 of 25 fps
            // but internal unit is in 5th
            glb_ds1.cur_anim_floor_frame += ticks_elapsed * 2;
        } else {
            glb_ds1edit.ticks_elapsed = 0;
        }

        // redraw the whole screen
        wpreview_draw_tiles(ds1_idx);
        glb_ds1edit.fps++;



        if (key[KEY_UP]) {
            glb_ds1edit.win_preview.y0 -= glb_ds1.cur_scroll.keyb.y;
        }

        if (key[KEY_DOWN]) {
            glb_ds1edit.win_preview.y0 += glb_ds1.cur_scroll.keyb.y;
        }

        if (key[KEY_LEFT]) {
            glb_ds1edit.win_preview.x0 -= glb_ds1.cur_scroll.keyb.x;
        }

        if (key[KEY_RIGHT]) {
            glb_ds1edit.win_preview.x0 += glb_ds1.cur_scroll.keyb.x;
        }


        // read ds1_save() carefully!!!!!!!!!!!
        done = RELEASE_KEY(ESC);
    }
}
