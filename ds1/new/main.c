#include "main.h"

void init()
{
    srand(time(NULL));
    allegro_init();
    set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);

    install_keyboard();
    install_timer();

    set_color_depth(8);
    atexit(ds1edit_exit);

    ds1edit_init();
    set_window_title("D2IM-MAPEDITOR");

}







int main(int argc, char * argv[])
{

    init();

    glb_ds1edit.cur_gamma = glb_config.gamma;
    misc_read_gamma();

    // preview window update
    glb_ds1edit.win_preview.x0 = 0;
    glb_ds1edit.win_preview.y0 = 0;
    // 预览窗口大小和程序窗口大小 不是一回事哦
    glb_ds1edit.win_preview.w  = glb_config.screen.width;
    glb_ds1edit.win_preview.h  = glb_config.screen.height;


    // screen buffer (we're still in 8bpp color depth !)

    // we're making a big buffer, with 300 pixels on each 4 borders, and then we'll make the true screen buffer be a sub-bitmap of this buffer
    // this is to avoid potential problems with clipings, especially when using the functions from gfx_custom.c
    // yes, I'm hidding the problem under the carpet

    glb_config.screen.width  += 600;
    glb_config.screen.height += 600;
    glb_ds1edit.big_screen_buff = create_bitmap( glb_config.screen.width, glb_config.screen.height );
    if (glb_ds1edit.big_screen_buff == NULL){
        FATAL_EXIT("could not create big_screen_buff (%i*%i pixels).", glb_config.screen.width, glb_config.screen.height );
    }
    glb_config.screen.width  -= 600;
    glb_config.screen.height -= 600;

    glb_ds1edit.screen_buff = create_sub_bitmap( glb_ds1edit.big_screen_buff, 300, 300, glb_config.screen.width, glb_config.screen.height );
    if (glb_ds1edit.screen_buff == NULL){
        FATAL_EXIT("could not create sub-bitmap screen_buff (%i*%i pixels).", glb_config.screen.width, glb_config.screen.height );
    }

    // open all mpq
    ds1edit_open_all_mpq();

    // load palettes from disk, else from mpq
    ds1edit_load_palettes();

    // parse the command line
    if (misc_cmd_line_parse (argc, argv)){
        // 不太关心这一块
        // 所以跳过了
        FATAL_EXIT("problem in the command line.");
    }

    // create debug directory if necessary
    if (glb_ds1edit.cmd_line.debug_mode == TRUE){
        mkdir("debug");
    }

    // objects.txt
    //read_objects_txt();

    // obj.txt
    //read_obj_txt();

    if(glb_ds1edit.cmd_line.ds1_filename != NULL){
        //调试时候用的是.ini 所以没有在这里
        //这个差别不大 无非是1个ds和several个ds
        // .ds1
        // read the ds1
        if(glb_ds1edit.cmd_line.dt1_list_num != -1){
            // force dt1
            misc_open_1_ds1_force_dt1(ds1_idx);
        }else{
            // find dt1 list from .txt
            //
            // win_ds1edit <file.ds1>  <LvlType.txt ID> <LvlPrest.txt  DEF> [options]
            // LvlTypes.txt ID是指在lvltype.txt里面该ds1文件的类型（ID号，在levels.txt里面也有lvltype一列）， 
            // lvlPrest.txt DEF是指该ds1文件在lvlPrest.txt里面def一列对应的数值。
            // 不同的ds1文件可能有相同的ID和def值，主要查看LvlType.txt、lvlPrest.txt和levels.txt就可以知道了。
            //
            misc_open_1_ds1(
                    ds1_idx,
                    glb_ds1edit.cmd_line.ds1_filename,
                    glb_ds1edit.cmd_line.lvltype_id,
                    glb_ds1edit.cmd_line.lvlprest_def,
                    glb_ds1edit.cmd_line.resize_width,
                    glb_ds1edit.cmd_line.resize_height
                    );
        }
    }else{
        // bug
        fprintf(stderr, "main(), error.\nBug : neither .DS1 nor a .INI in the command line.");
    }

    if (argc >= 4){
        // at least 3 arguments (ds1 name + ID + DEF + options)
    } else if (argc == 2) {
        // 1 argument (assume it's a .ini file)
    } else {
        // syntax error
        printf("syntaxe 1 : ds1edit <file.ds1> <lvlTypes.txt Id> <lvlPrest.txt Def> [options]\n");
        printf("syntaxe 2 : ds1edit <file.ini>\n"
                "\n"
                "   file.ini in syntaxe 2 is a text file, each line for 1 ds1 to load,\n"
                "   3 elements : <lvlTypes.txt Id> <lvlPrest.txt Def> <file.ds1>\n");
        exit(DS1ERR_CMDLINE);
    }


    // colormaps
    misc_make_cmaps();

    // start
    fflush(stdout);
    fflush(stderr);
    if ( (glb_config.screen.depth !=  8) && (glb_config.screen.depth != 15) && (glb_config.screen.depth != 16) && (glb_config.screen.depth != 24) && (glb_config.screen.depth != 32)) {
        sprintf( tmp, "main(), error.\nInvalid color depth (%i).\nValid values are : 8, 15, 16, 24 and 32.\nCheck the 'Ds1edit.ini' file, at the 'screen_depth' line", glb_config.screen.depth);
        fprintf(stderr, tmp);
    }

    // set color depth for the screen
    set_color_depth(glb_config.screen.depth);

    request_refresh_rate(glb_config.screen.refresh);
    if (glb_config.fullscreen == TRUE) {
        if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, glb_config.screen.width, glb_config.screen.height, 0, 0) != 0) {
            sprintf( tmp, "main(), error.\ncould not initialize this graphical mode : " "%i*%i %ibpp %iHz %s.\nAllegro_error = \"%s\".", glb_config.screen.width, glb_config.screen.height, glb_config.screen.depth, glb_config.screen.refresh, "Fullscreen", allegro_error);
            fprintf(stderr, tmp);
        }
    } else {
        if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, glb_config.screen.width, glb_config.screen.height, 0, 0) != 0) {
            sprintf( tmp, "main(), error.\ncould not initialize this graphical mode : " "%i*%i %ibpp %iHz %s.\nAllegro_error = \"%s\".", glb_config.screen.width, glb_config.screen.height, glb_config.screen.depth, glb_config.screen.refresh, "Windowed", allegro_error);
            fprintf(stderr, tmp);
        }
    }
    glb_ds1edit.current_refresh_rate = get_refresh_rate();

    // video pages
    glb_ds1edit.video_page = create_video_bitmap( glb_config.screen.width, glb_config.screen.height);
    if (glb_ds1edit.video_page == NULL) {
        sprintf(tmp, "main(), error.\ncould not create video page 1.", glb_config.screen.width, glb_config.screen.height);
        fprintf(stderr, tmp);
    }


    // when the editor is minimized, stop it until the user returns
    set_display_switch_mode(SWITCH_PAUSE);

    // get back to a 8bpp color depth, for all the next BITMAP creation
    set_color_depth(8);

    text_mode(-1); // draw text as sprite, no background color


    if (install_mouse() == -1) {
        FATAL_EXIT("could not install the Mouse handler.\n");
    }
    set_mouse_speed(glb_config.mouse_speed.x, glb_config.mouse_speed.y);


    glb_ds1edit.win_preview.x0 = glb_ds1.own_wpreview.x0;
    glb_ds1edit.win_preview.y0 = glb_ds1.own_wpreview.y0;
    glb_ds1edit.win_preview.w  = glb_ds1.own_wpreview.w ;
    glb_ds1edit.win_preview.h  = glb_ds1.own_wpreview.h ;


    // main loop
    freopen("stderr.txt", "wt", stderr);
    interfac_user_handler(ds1_idx);

    if (glb_ds1edit.video_page != NULL)
        destroy_bitmap(glb_ds1edit.video_page);


    // end
    set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);
    // fclose(stdout);
    fflush(stdout);
    fflush(stderr);
    return DS1ERR_OK;
}
