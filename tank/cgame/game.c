#include "game.h"

#if 0
public
#endif

void game_init( game* gp ){
    srand((int)time(NULL)%10000);
    _alf_allegro_init( gp );
    _res_init( gp );
    _set_state( gp, GAME_START );
}
void game_run( game* gp ){
    while ( gp->game_state != GAME_END ){
        _process_event( gp );
    }
}
int game_over( game* p ){
}

#if 0
private
#endif

void _allegro_init( game* gp ){

    if( !al_init()){
        abort_example("\nFailed to init Allegro...\n");
        return ALLEGRO_INIT_ERROR;
    }

    if (!al_init_image_addon()) {
        abort_example("\nFailed to init IIO addon.\n");
        return ALLEGRO_IMAGE_ADDON_INIT_ERROR;
    }

    al_init_font_addon();

    gp->display = al_create_display( MAP_WIDTH, MAP_HEIGHT );

    if (!gp->display) {
        abort_example("\nError creating display.\n");
        return 1;
    }

    if (!al_install_keyboard()) {
        abort_example("\nError installing keyboard.\n");
        return 1;
    }

    gp->timer = al_create_timer(1.0 / GLB_GAME_FPS);

    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(gp->timer));
    al_register_event_source(queue, al_get_display_event_source(gp->display));
    al_start_timer(gp->timer);
}

static void _res_init( game* gp ){
    _alf_bmp_load( p->menu, "./img/menu.bmp" );
    _alf_bmp_load( p->world, "./img/world.bmp" );
    _alf_bmp_load( p->win, "./img/win.bmp" );
    _alf_bmp_load( p->over, "./img/over.bmp" );
}


static int _set_state( game* gp, int new_state ){

    gp->game_click = 0;
    switch ( new_state ){
        case GAME_RUN:
            switch( gp->state ){
                case GAME_MENU:
                    gp->game_level = 1;
                case GAME_WIN:
                    game_level ++;
                default:
                    player_bore[0].active = 1;
                    player_bore[1].active = 1;
                    ememy_bore[0].active = 1;

                    for( int i=0; int i<2; ++i ){
                        for( int j=0; int j<4; ++j ){
                            player_kill_enemy[i][j] = 0;
                        }
                    }
                    gp->state = new_state;
                    return;
            }

        case GAME_START:
            gp->state = new_state;
            return;
        case GAME_MENU:
            switch( gp->state ){
                case GAME_START:
                case GAME_RUN:
                default:
                    gp->state = new_state;
                    return;
            }

        case GAME_WIN:
            return;


        case GAME_OVER:

            return;
        case GAME_END:


        default:
            return;
    }
}


void _process_event( game* gp ){

    al_evt event;
    al_wait_for_event( gp->event_queue, &event );

    switch ( event.type ){
        case ALEVT_KEY_DOWN:
            _process_key_down_event( gp, &event );
            return;
        case ALEVT_KEY_UP:
            _process_key_up_event( gp, &event );
            return;
        case ALEVT_DISPLAY_CLOSE:
            _process_display_close( gp, &event );
            return;
        case ALEVT_TIMER:
            _process_timer_event( gp, &event );
            return;
    }
}

#if 0
========================_process_***_event======================================
#endif

void _process_key_down_event( game* p, al_evt* evt ){

    switch( p->state ){

        case GAME_RUN:
            switch( (evt->keyboard).keycode ){
                case ALK_W:
                    gp->player[0].moving = 1;
                    gp->player[0].dir = DIR_UP;
                    return;
                case ALK_A:
                    gp->player[0].moving = 1;
                    gp->player[0].dir = DIR_RIGHT;
                    return;
                case ALK_S:
                    gp->player[0].moving = 1;
                    gp->player[0].dir = DIR_DOWN;
                    return;
                case ALK_D:
                    gp->player[0].moving = 1;
                    gp->player[0].dir = DIR_LEFT;
                    return;
                case ALK_UP:
                    gp->player[gp->double_mode].moving = 1;
                    gp->player[gp->double_mode].dir = DIR_UP;
                    return;
                case ALK_DOWN:
                    gp->player[gp->double_mode].moving = 1;
                    gp->player[gp->double_mode].dir = DIR_DOWN;
                    return;
                case ALK_RIGHT:
                    gp->player[gp->double_mode].moving = 1;
                    gp->player[gp->double_mode].dir = DIR_RIGHT;
                    return;
                case ALK_LEFT:
                    gp->player[gp->double_mode].moving = 1;
                    gp->player[gp->double_mode].dir = DIR_LEFT;
                    return;
                case ALK_J:
                    _player_try_attack( p, 0 );return;
                case ALK_SPACE:
                    _player_try_attack( p, p->double_mode );return;
                case ALK_ESC:
                    _set_state( p, GAME_WIN );return;
                default:
                    return;
            }

        case GAME_START:
            switch( (evt->keyboard).keycode ){
                case ALK_J:
                case ALK_SPACE:
                case ALK_ENTER:
                    _set_state( p, GAME_MENU );return;
                case ALK_ESC:
                    _set_state( p, GAME_END );return;
                default:
                    return;
            }
        case GAME_MENU:
            switch( (evt->keyboard).keycode ){
                case ALK_J:
                case ALK_SPACE:
                case ALK_ENTER:
                    _set_state( p, GAME_RUN );return;
                case ALK_ESC:
                    _set_state( p, GAME_END );return;
                default:
                    return;
            }
        case GAME_WIN:
            switch( (evt->keyboard).keycode ){
                case ALK_J:
                case ALK_SPACE:
                case ALK_ENTER:
                    _set_state( p, GAME_RUN );return;
                case ALK_ESC:
                    _set_state( p, GAME_MENU );return;
                default:
                    return;
            }
        case GAME_OVER:
            switch( (evt->keyboard).keycode ){
                case ALK_J:
                case ALK_SPACE:
                case ALK_ENTER:
                case ALK_ESC:
                    _set_state( p, GAME_MENU );return;
                default:
                    return;
            }
        default:
            return;
    }
}

#if 0
=====================================timer event================================
#endif

void _process_timer_event( game* gp, al_evt* evt ){
    switch( gp->state ){
        case GAME_RUN:
            _process_timer_event_on_run( gp );return;
        case GAME_START:
            _process_timer_event_on_start( gp );return;
        case GAME_MENU:
            _process_timer_event_on_menu( gp );return;
        case GAME_WIN:
            _process_timer_event_on_win( gp );return;
        case GAME_OVER:
            _process_timer_event_on_over( gp );return;
        default:
            return;
    }
}


void _process_timer_event_on_run( game* gp, al_evt* pevt ){

    map_update( &(gp->map), pevt );

    _enemy_bullet_update( gp, pevt );
    _player_bullet_update( gp, pevt );

    _player_update( gp, pevt );
    _enemy_update( gp, pevt );

    _player_bore_update( gp, pevt );
    _enemy_bore_update( gp, pevt );

    _bonus_update( gp, pevt );
    _explode_update( gp, pevt );

    _message_update( gp, pevt );

    _screen_update_on_run( gp );

}

void _enemy_bullet_update( game* gp, al_evt* pevt ){
    _enemy_bullet_update( gp );
    _enemy_bullet_hit_player( gp );
    _enemy_bullet_hit_tile( gp );
    _enemy_bullet_hit_fence( gp );

}

void _enemy_bullet_hit_player( game* gp ){

    for( int i=0; i<=gp->double_mode; ++i ){
        for( int j=0; j<ENEMY_BULLET_MAX; ++ j ){
            if( check_hit( gp->player+i, gp->enemy_bullet+j ) ){
                //不幸击中
                if( player_hitted( gp->player+i ) ){
                    //不幸死亡(没带套子)
                    _create_explode( EXPLODE_BIG, (gp->player+i)->row, (gp->player+i)->col );
                    (gp->player+i)->active = FALSE;
                    (gp->enemy_bullet+j)->active = FALSE;
                    sound_play( ENEMY_KILL_PLAYER );
                }else{
                    //检查下一个
                    sound_play( ENEMY_HIT_PLAYER );
                }
            }
        }
    }
}

void _enemy_bullet_hit_fence( game* gp ){
    int row, col;
    for( int i=0; i<ENEMY_BULLET_MAX; ++i ){
        row = gp->enemy_bullet[i]->row;
        col = gp->enemy_bullet[i]->col;
        if( row>=13*32 || row<=0 || col>=13*32 || col<=0 ){
            _create_small_explode(row, col);
            sound_play( BLOCK_HITTED );
            ( gp->enemy_bullet+i )->active = 0;
        }
    }
}

void _enemy_bullet_update( game* gp ){
    for( int i=0; i<ENEMY_BULLET_MAX;++i ){
        gp->enemy_bullet[i]->move( gp->enemy_bullet+i );
    }
}


void _player_bullet_update( game* gp, al_evt* pevt ){
    _player_bullet_update( gp );
    _player_bullet_hit_enemy( gp );
    _player_bullet_hit_player( gp );
    _player_bullet_hit_player_bullet( gp );
    _player_bullet_hit_enemy_bullet( gp );
    _player_bullet_hit_tile( gp );
    _player_bullet_hit_fence( gp );
}

void _player_bullet_update( game* gp ){
    for( int i=0;i<gp->double_mode;++i ){
        for( int j=0;j<PLAYER_BULLET_MAX;++j ){
            movable_sprite_move( gp->player_bullet[i]+j );
        }
    }
}

void _player_bullet_hit_enemy( game* gp ){
    //第i个玩家的第j颗子弹击中第k个敌人?
    for( int i=0;i<=gp->double_mode;++i ){
        for( int j=0;j<PLAYER_MAX_BULLET;++j ){

            if( gp->player_bullet[i][j].active ){

                for( int k=0;k<ENEMY_MAX;++k ){
                    if( gp->enemy[k].active && check_hit( gp->player_bullet[i]+j, gp->enemy+k ) ){
                        if( gp->enemy[k].type = ENEMY_WITH_BONUS ){
                            _create_bonus( gp );
                            gp->enemy[k].type = !ENEMY_WITH_BONUS;
                            sound_play( ENEMY_HITTED );
                        }else{
                            if( gp->enemy[k].state == TANK_PROTECTED ){
                                sound_play( ENEMY_HITTED );
                            }else{
                                gp->enemy[k].life --;
                                if( gp->enemy[k].life == 0 ){
                                    sound_play( ENEMY_KILLED );
                                    _create_big_explode( gp->player_bullet[i][j].row, gp->player_bullet[i][j].col );

                                }else{
                                    sound_play( ENEMY_HITTED );
                                }
                            }
                        }
                        gp->player_bullet[i][j].active = 0;
                        break;
                    }
                }
            }
        }
    }
}


int _bullet_hit_bullet( game* pg, bullet* pb ){
    bullet* ptm;
    for( int i=0;i<BULLET_MAX;++i ){
        ptm = gp->bullet+i;
        if( pb!=ptm && ptm->active && pb->type!=ptm->type && check_hit( pb, ptm ) ){
            ptm->active = 0;
            pb->active = 0;
            return 1;
        }
    }
    return 0;
}

int _bullet_hit_tank( game* gp, bullet* pb ){
    tank* pt;
    for( int i=0;i<TANK_MAX;++i ){
        pt = gp->tank+i;
        if( pt->active && check_hit( pt, pb ) ){
            pb->active = 0;
            if( pt->type != pb->type ){
                if( pt->hitted( pt ) ){
                    sound_play( SOUND_TANK_KILLED );
                    pt->active = 0;
                }else{
                    sound_play( SOUND_TANK_HITTED );
                }
            }else{
                if( !pt->protected ){
                    sound_play( SOUND_TANK_HITTED );
                    tank_set_state( pt, TANK_WINK );
                }
            }
            return 1;
        }
    }
    return 0;
}


void _player_bullet_hit_tile( game* gp ){

    int ind0,ind1,row,col,dir;
    tile *pt0,*pt1;
    int need_create_small_explode,need_create_hawk_explode;

    for( int i=0;i<=double_mode;++i ){
        for( int j=0;j<=PLAYER_BULLET_MAX;++j ){
            if( gp->player_bullet[i][j].active ){
                row = gp->player_bullet[i][j].row;
                col = gp->player_bullet[i][j].col;
                dir = gp->player_bullet[i][j].dir;
                _get_hit_info( gp,&pt0,&ind0,&pt1,&ind1,row,col,dir);

                need_create_small_explode = 0;
                need_create_hawk_explode = 0;
                switch( tile_sub_type( pt0,ind0 ) ){
                    case BLOCK_BRICK:
                        need_create_small_explode = 1;
                        _destroy_tile_block(pt0,ind0);
                        break;

                    case BLOCK_CONCRETE:
                        need_create_small_explode = 1;
                        if( gp->player_bullet[i][j].level > 2 ){
                            _destroy_tile_block(pt0,ind0);
                        }
                        break;
                    case BLOCK_HAWK:
                        need_create_hawk_explode = 1;
                        pt0->type[0]=BLOCK_STONE;
                        break;
                    case BLOCK_STONE:
                        need_create_small_explode = 1;
                        break;
                    default:
                        break;
                }

                switch( tile_sub_type( pt1,ind1 ) ){
                    case BLOCK_BRICK:
                        need_create_small_explode = 1;
                        _destroy_tile_block(pt1,ind1);
                        break;

                    case BLOCK_CONCRETE:
                        need_create_small_explode = 1;
                        if( gp->player_bullet[i][j].level > 2 ){
                            _destroy_tile_block(pt1,ind1);
                        }
                        break;
                    case BLOCK_HAWK:
                        need_create_hawk_explode = 1;
                        pt1->type[0]=BLOCK_STONE;
                        break;
                    case BLOCK_STONE:
                        need_create_small_explode = 1;
                        break;
                    default:
                        break;
                }

                if(need_create_hawk_explode){
                    _create_hawk_explode( gp );
                    gp->player_bullet[i][j].active = 0;
                    sound_play( HAWK_KILLED );
                }
                if (need_create_small_explode){
                    _create_small_explode(row,col);
                    gp->player_bullet[i][j].active = 0;
                    sound_play( BLOCK_HITTED );
                }
            }
        }
    }
}


static int _bullet_hit_fence( game* pg, bullet* pb ){
#if 0
    need use hot_row and hot_col?
        if so, dir is also need
#endif
            sprite* self = (sprite*)pb;
    int row, col;
    row = self->row;
    col = self->col;

    if( row>=13*32 || row<=0 || col>=13*32 || col<=0 ){
        _create_explode( row, col, SMALL_EXPLODE );
        sound_play( SOUND_BLOCK_HITTED );
        pb->active = 0;
    }
    return pb->active;
}



static void _process_key_up_event( game* p, al_evt* evt ){

    switch( p->state ){
        case GAME_RUN:
            switch( (evt->keyboard).keycode ){
                case ALK_W:
                case ALK_A:
                case ALK_S:
                case ALK_D:
                    p->player[0]->state = !MOVING;return;
                case ALK_UP:
                case ALK_DOWN:
                case ALK_RIGHT:
                case ALK_LEFT:
                    p->player[1]->state = !MOVING;return;
                default:
                    return;
            }
        case GAME_START:
        case GAME_MENU:
        case GAME_WIN:
        case GAME_OVER:
        default:
            return;
    }
}






void _player_try_attack( game* p, int i ){

    if( p->player_living[i] && 
            p->player_bullet_living[i] < PLAYER_MAX_BULLET && 
            !p->player_freezing &&
            p->player[i].state != PLAYER_WINK ){

        bore_create( &player_bores[i], 16+12*32,16+(4+i*4)*32);
        player_bore_living[i] = 1;
    }
}

void _try_move_tank( game* g, tank* pt ){

    int     begin_row,begin_col,width,height,bt,speed,dir;

    begin_row = pt->row - pt->hot_row;
    begin_col = pt->col - pt->hot_col;
    width = pt->width;
    height = pt->height;
    dir = pt->dir;
    speed = pt->speed;

    switch( dir ){

        case DIR_UP:
            //1:是否出界
            if(begin_row - speed < 0 )return ;
            //2.是否地图可达
            //从角点采样2+1个点 因为地图最小块是16  
            bt = map_pixel_type( &(g->map), begin_row - speed, begin_col );
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;           
            bt = map_pixel_type( &(g->map), begin_row - speed, begin_col + 16 );
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;           
            bt = map_pixel_type( &(g->map), begin_row - speed, begin_col + width );
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;

            //3.是否被其他车辆阻塞
            //a.敌人
            pt->row -= speed;
            for( int i=0;i<ENEMY_MAX;++i ){
                if( gp->enemy[i].active && gp->enemy[i]!=pt && check_hit( g->enemy+i, pt ) ){
                    pt->row += speed;
                    return ;
                }
            }
            //b.玩家
            for( int i=0;i<gp->double_mode;++i){
                if( pt!=pg->player+i && check_hit( pg->player+i, pt ) ){
                    pt->row += speed;
                    return;
                }
            }
            //c. enemy bore
            for( int i=0;i<3;++i ){
                if( gp->enemy_bore[i].active && check_hit( g->enemy_bore+i, pt ) ){
                    pt->row += speed;
                    return ;
                }
            }
            //player bore
            for( int i=0;i<2;++i ){
                if( gp->player_bore[i].active && check_hit( g->player_bore+i, pt ) ){
                    pt->row += speed;
                    return ;
                }
            }
            return ;

        case DIR_RIGHT:
            //1:是否出界
            if(begin_col + width + speed  > GLB_MAP_PIXEL_WIDTH )return;
            //2.是否地图可达
            //从角点采样2+1个点 因为地图最小块是16  
            bt = map_pixel_type( &(g->map), begin_row,begin_col+with+speed );
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;           
            bt = map_pixel_type( &(g->map), begin_row + 16, begin_col+width+speed);
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;           
            bt = map_pixel_type( &(g->map), begin_row +height,begin_col+width+speed); 
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;

            //3.是否被其他车辆阻塞
            //a.敌人
            pt->col += speed;
            for( int i=0;i<ENEMY_MAX;++i ){
                if( gp->enemy[i].active && gp->enemy[i]!=pt && check_hit( g->enemy+i, pt ) ){
                    pt->col -= speed;
                    return ;
                }
            }
            //b.玩家
            for( int i=0;i<gp->double_mode;++i){
                if( pt!=pg->player+i && check_hit( pg->player+i, pt ) ){
                    pt->col -= speed;
                    return;
                }
            }
            //c. enemy bore
            for( int i=0;i<3;++i ){
                if( gp->enemy_bore[i].active && check_hit( g->enemy_bore+i, pt ) ){
                    pt->col -= speed;
                    return ;
                }
            }
            //player bore
            for( int i=0;i<2;++i ){
                if( gp->player_bore[i].active && check_hit( g->player_bore+i, pt ) ){
                    pt->col -= speed;
                    return ;
                }
            }
            return ;


        case DIR_DOWN:
            if(begin_row + height + speed  > MAP_PIXEL_HEIGHT )return;

            bt = map_pixel_type( &(g->map), begin_row + height + speed,begin_col );
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;           
            bt = map_pixel_type( &(g->map), begin_row + height + speed,begin_col + 16);
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;           
            bt = map_pixel_type( &(g->map), begin_row + height + speed,begin_col + width );
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;

            //3.是否被其他车辆阻塞
            //a.敌人
            pt->row += speed;
            for( int i=0;i<ENEMY_MAX;++i ){
                if( gp->enemy[i].active && gp->enemy[i]!=pt && check_hit( g->enemy+i, pt ) ){
                    pt->row -= speed;
                    return ;
                }
            }
            //b.玩家
            for( int i=0;i<gp->double_mode;++i){
                if( pt!=pg->player+i && check_hit( pg->player+i, pt ) ){
                    pt->row -= speed;
                    return;
                }
            }
            //c. enemy bore
            for( int i=0;i<3;++i ){
                if( gp->enemy_bore[i].active && check_hit( g->enemy_bore+i, pt ) ){
                    pt->row -= speed;
                    return ;
                }
            }
            //player bore
            for( int i=0;i<2;++i ){
                if( gp->player_bore[i].active && check_hit( g->player_bore+i, pt ) ){
                    pt->row -= speed;
                    return ;
                }
            }
            return ;

        case DIR_LEFT:
            if(begin_col - speed  < 0 )return ;

            bt = map_pixel_type( &(g->map), begin_row , begin_col-speed);
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;           
            bt = map_pixel_type( &(g->map), begin_row + 16, begin_col-speed);
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;           
            bt = map_pixel_type( &(g->map), begin_row + height ,begin_col-speed); 
            if(bt != BLOCK_NULL && bt != BLOCK_TREE )return ;

            //3.是否被其他车辆阻塞
            //a.敌人
            pt->col -= speed;
            for( int i=0;i<ENEMY_MAX;++i ){
                if( gp->enemy[i].active && gp->enemy[i]!=pt && check_hit( g->enemy+i, pt ) ){
                    pt->col += speed;
                    return ;
                }
            }
            //b.玩家
            for( int i=0;i<gp->double_mode;++i){
                if( pt!=pg->player+i && check_hit( pg->player+i, pt ) ){
                    pt->col += speed;
                    return;
                }
            }
            //c. enemy bore
            for( int i=0;i<3;++i ){
                if( gp->enemy_bore[i].active && check_hit( g->enemy_bore+i, pt ) ){
                    pt->col += speed;
                    return ;
                }
            }
            //player bore
            for( int i=0;i<2;++i ){
                if( gp->player_bore[i].active && check_hit( g->player_bore+i, pt ) ){
                    pt->col += speed;
                    return ;
                }
            }
            return ;
        default:
            return;

    }
}


void _process_timer_on_run( game* gp ){
    if( gp->state_click < ms2click( 1500 ) ){
        return;
    }else if( gp->state_click < ms2click( 3500 ) ){
#define _ms2000 ms2click(2000)
#define _ms5000 ms2click(5000)
#define _mscurr ms2click(gp->state_click)
        _alf_tint_draw( gp->logo, (_ms2000*1.0-abs(2*_mscurr-_ms5000))/_ms2000 );
    }else{
        _set_state( gp, GAME_MENU );
    }
}

void _process_timer_on_select( game* gp ){
    _alf_draw( gp->menu_bmp[gp->double_mode] );
}


void _process_timer_on_win( game* gp ){
    _alf_draw( gp->main_screen );
    _alf_draw_text( 21,379,"%d", gp->game_level );
    for( int i=0;i<=gp->double_mode;++i ){
        if( gp->state_click > ms2click( 500 ) ){
            _alf_draw_text( 130,150+i*(473-150),"%d",gp->player[i].life );
        }
        if( gp->state_click > ms2click( 1000 ) ){
            _alf_draw_text( 169,150+i*(473-150),"%d",gp->player[i].level );
        }
        if( gp->state_click > ms2click( 1500 ) ){
            _alf_draw_text( 220,160+i*(473-160),"%d",gp->player_kill[i][0] );
            _alf_draw_text( 220,230+i*(473-230),"%d",gp->player_kill[i][0] );
        }
        if( gp->state_click > ms2click( 2000 ) ){
            _alf_draw_text( 270,160+i*(473-160),"%d",gp->player_kill[i][1] );
            _alf_draw_text( 270,230+i*(473-230),"%d",gp->player_kill[i][1] );
        }
        if( gp->state_click > ms2click( 2500 ) ){
            _alf_draw_text( 317,160+i*(473-160),"%d",gp->player_kill[i][2] );
            _alf_draw_text( 317,230+i*(473-230),"%d",gp->player_kill[i][2] );
        }
        if( gp->state_click > ms2click( 2500 ) ){
            _alf_draw_text( 350,160+i*(473-160),"%d",gp->player_kill[i][3] );
            _alf_draw_text( 350,230+i*(473-230),"%d",gp->player_kill[i][3] );
        }
        if( gp->state_click > ms2click( 3000 ) ){
            _alf_draw_text( 428,185+i*(505-185),"%d",player_score );
        }
        if( gp->state_click > ms2click( 3500 ) ){
            if( gp->player_score[i] > 300 ){
                _alf_draw_text( 428,185+i*(505-185),
                        "Player %d add %d Life !",
                        i,gp->player_score[i]);
            }
        }
        if( gp->state_click > ms2click( 4000 ) ){
            _state_state( gp, GAME_RUN );
        }
    }
}


void _process_timer_on_over( game* gp ){
    int h,c;
    if( gp->state_click <= ms2click(1500) ){
        h = ((32*13-160)/2+160.)*(gp->state_click*1./click(1500));
        c = (32*13- 248)/2+GLB_COL_OFF;
        _alf_draw_on_run( gp );
        _alf_draw_region( gp->game_over,0,0,248,(h>160?160:h),32*13+GLB_ROW_OFF-h,c );
    }else if( gp->state_click <= ms2click( 2500 ) ){
        _alf_draw_on_run( gp , tint );
        _alf_tint_draw_region();
    }else{
        _set_state( gp,GAME_MENU );
    }
}

static void _enemy_try_attack( game* gp )
    for( int i=0;i<ENEMY_MAX;++i ){
        if( gp->enemy[i].active && enemy_can_attack( gp->enemy+i ) ){
            enemy_attack( &(gp->enemy[i]) );
            _create_enemy_bullet( gp );
        }
    }
}



void Game::create_bonus(){
    m_bonus_living = true;
    //m_bonus.create(BonusHelmet,game_random(100,350),game_random(100,350));
    m_bonus.create((BonusType)game_random(0,5),game_random(100,350),game_random(100,350));
    //m_bonus.create(BonusBomb,game_random(100,350),game_random(100,350));
}


void _update_screen_on_run( game* gp ){

    _alf_draw( gp->world );
    gp->map.draw_floor( GLB_ROW_OFF, GLB_COL_OFF );

    for( int i=0;i<=gp->double_mode;++i ){
        if( gp->player_bore[i].active ){
            gp->player_bore[i]->draw( gp->player_bore+i, GLB_ROW_OFF, GLB_COL_OFF );
        }
        for( int j=0;j<PLAYER_BULLET_MAX;++j ){
            if( gp->player_bullet[i][j].active ){
                gp->player_bullet[i][j].draw( gp->player_bullet[i]+j, GLB_ROW_OFF, GLB_COL_OFF );
            }
        }
        if( gp->player[i].active ){
            gp->player[i].draw( gp->player+i, GLB_ROW_OFF, GLB_COL_OFF );
        }
    }

    for( int i=0;i<ENEMY_BULLET_MAX;++i ){
        if( gp->enemy_bullet[i].active ){
            gp->enemy_bullet[i].draw( gp->enemy_bullet+i, GLB_ROW_OFF, GLB_COL_OFF );
        }
    }
    for( int i=0;i<3;++i ){
        if( gp->enemy_bore[i].active ){
            gp->enemy_bore[i].draw( gp->enemy_bore+i, GLB_ROW_OFF, GLB_COL_OFF );
        }
    }
    for( int i=0;i<ENEMY_MAX;++i ){
        if( gp->enemy[i].active ){
            gp->enemy[i].draw( gp->enemy+i, GLB_ROW_OFF, GLB_COL_OFF );
        }
    }
    for( int i=0;i<SMALL_EXPLODE_MAX;++i ){
        if( gp->small_explode[i].active ){
            gp->small_explode[i].draw( gp->small_explode+i, GLB_ROW_OFF, GLB_COL_OFF );
        }
    }
    for( int i=0;i<BIG_EXPLODE_MAX;++i ){
        if( gp->big_explode[i].active ){
            gp->big_explode[i].draw( gp->big_explode+i, GLB_ROW_OFF, GLB_COL_OFF );
        }
    }
    if( gp->hawk_explode.active ){
        gp->hawk_explode.draw( &(gp->hawk_explode) );
    }
    if( gp->bonus.active ){
        gp->bonus.draw( &(gp->bonus) );
    }
    gp->map.draw_roof( GLB_ROW_OFF, GLB_COL_OFF );

    /*enemy left*/
    _alf_draw_text( 388,563, "%d", ENEMY_MAX-(
                gp->player_score_count[0]+
                gp->player_score_count[1]+
                gp->player_score_count[2]));

    /*player left*/
    for( int i=0;i<=gp->double_mode;++i ){
        _alf_draw_text( 168+i*(289-168),545, gp->player[i].life );
        _alf_draw_text( 205+i*(545-205),545, gp->player[i].level );
    }

    _alf_draw_text( 77,568,"%d",gp->game_level );

}


void _create_small_explode( int row, int col ){
    for( int i=0;i<SMALL_EXPLODE_MAX;++i ){
        if( !gp->small_explode[i].active ){
            explode_init( gp->small_explode+i, SMALL_EXPLODE, row, col );
            return;
        }
    }
}

void _create_big_explode( int row, int col ){
    for( int i=0;i<BIG_EXPLODE_MAX;++i ){
        if( !gp->big_explode[i].active ){
            explode_init( gp->small_explode+i, BIG_EXPLODE, row, col );
            return;
        }
    }
}

void _create_hawk_explode( int row, int col ){
    explode_init( gp->hawk_explode, BIG_EXPLODE, 12*32+16, 6*32+16 );
}

void _create_enemy_bore( game* gp ){
    for( int i=0;i<3;++i ){
        if( !gp->enemy_bore[i].active ){
            bore_init( gp->enemy_bore+i,16,16+32*6*i );
            return;
        }
    }

    void _create_player_bore( int i ){
        bore_init( gp->player_bore+i, 16+12*32,16+(4+i*4)*32 );
    }

    void _create_enemy_bullet( game* gp, int p ){
        for( int i=0;i<ENEMY_BULLET_MAX;++i ){
            if( !gp->enemy_bullet[i].active ){
                bullet_init( gp->enemy_bullet+i,
                        gp->enemy[p].row, gp->enemy[p].col,
                        gp->enemy[p].dir, gp->enemy[p].level );
                return;
            }
        }
    }

    void _create_player_bullet( game* gp, int p ){
        for( int i=0;i<PLAYER_BULLET_MAX;++i ){
            if( !gp->player_bullet[p][i].active ){
                bullet_init( gp->player_bullet[p]+i,
                        gp->player[p].row, gp->player[p].col,
                        gp->player[p].dir, gp->player[p].level );
                return;
            }
        }
    }



}




static void _try_create_enemy_bore( game* gp ){
    if( _enemy_active_num( gp ) < 6 && 
            _enemy_left_num( gp ) > 0 && 
            _last_enemy_bore_click( gp ) > ms2click( 3000 ) ||
            _enemy_left_num( gp ) > 17 ){


        //以下检测能否现在生成
        //主要是在生成地看看是否有车
        //
        //先生成,再检测,有问题就马上撤掉

        int tmp = m_last_enemy_bore_click;
        create_enemy_bore(cou);
        for(int i =0;i<m_enemy_living;i++){
            if(m_enemy[i].check_hit(m_enemy_bore[m_enemy_bore_living-1])){
                m_enemy_bore_living --;
                m_enemy_left ++;
                m_last_enemy_bore_click = tmp;
                return;
            }
        }
        for(int i=0;i<=!m_single_player;i++){
            if(m_player_living[i] && m_player[i].check_hit(m_enemy_bore[m_enemy_bore_living-1])){
                m_enemy_bore_living --;
                m_enemy_left ++;
                m_last_enemy_bore_click = tmp;
                return;
            }
        }
        cou ++;
        cou%=3;
    }
    debug(" out  create enemy bore if need and could\n");
}

void Game::try_create_player_bore(){
    debug("cd create player bore if need and could\n");
    for(int i=0;i<=(int)!m_single_player;i++){
        if( !m_player_living[i]         && 
                m_player_life_left[i]>0     && 
                !m_player_bore_living[i]){

            create_player_bore(i);
            for(int j =0;j<m_enemy_living;j++){
                if(m_enemy[j].check_hit(m_player_bore[i])){
                    remove_player_bore(i);
                    return;
                }
            }
            if(!m_single_player             && 
                    m_player_living[(int)!i]        &&
                    m_player_bore[i].check_hit(m_player[(int)!i])){
                remove_player_bore(i);
                return;
            }
        }
        debug("out create player bore if need and could\n");
    }
}

static void _update_bullet( game* gp ){
    sprite* self = (sprite*)(gp->bullet);
    for( int i=0;i<BULLET_MAX;++i ){
        if( self->active ){
            self->update( self );
            _bullet_hit_bullet( gp, (bullet*)self );
            _bullet_hit_tank( gp, (bullet*)self );
            _bullet_hit_tile( gp, (bullet*)self );
            _bullet_hit_fence( gp, (bullet*)self );
        }
    }
}




static void _update_bonus( game* gp ){
    sprite* self = ((sprite*)(&(gp->bonus)));
    if( self->active ){
        self->update( self );
        if( self->click == 0 ){
            self->active = 0;
        }
        for( int i=0;i<ENEMY_MAX;++i ){
            if( gp->enemy[i].active && check_hit( gp->enemy+i,self ) ){
                self->active = 0;
                sound_play( ENEMY_GET_BONUS );
                switch( ((bonus*)self)->type ){
                    case BONUS_LIFE:
                        show_message( gp, ENEMY_GET_BONUS_LIFE, 9000 );
                        for( int j=0;j<ENEMY_MAX;++j ){
                            if( gp->enemy[j].active ){
                                gp->enemy[j].live ++;
                            }
                        }
                        break;
                    case BOUNS_CLOCK:
                        show_message( gp, ENEMY_GET_BONUS_CLOCK, 9000 );
                        for( int j=0;j<gp->double_mode;++j ){
                            if( gp->player[j].active ){
                                tank_set_state( TANK_FREEZINF );
                            }
                        }
                        break;
                    case BOUNS_SHOVEL:
                        show_message( gp, ENEMY_GET_BONUS_SHOVEL, 9000 );
                        map_set_hawk_bare( &(gp->map) );
                        break;
                    case BONUS_BOMB:
                        show_message( gp, ENEMY_GET_BONUS_BOMB, 9000 );
                        for( int j=0;j<gp->double_mode;++j ){
                            if( gp->player[j].active ){
                                gp->player[j].active = 0;
                                _create_explode( gp->player[j].row, gp->player[j].col, BIG_EXPLODE );
                            }
                        }
                        break;
                    case BONUS_STAR:
                        show_message( gp, ENEMY_GET_BONUS_STAR, 9000 );
                        for( int j=0;j<ENEMY_MAX;++j ){
                            if( gp->enemy[j].active ){
                                gp->enemy[j].level ++;
                            }
                        }
                        break;
                    case BONUS_HELMET:
                        show_message( gp, ENEMY_GET_BONUS_HELMET, 9000 );
                        for( int j=0;j<ENEMY_MAX;++j ){
                            if( gp->enemy[j].active ){
                                tank_set_state( (tank*)(gp->enemy+j), TANK_SHIELD );
                            }
                        }
                        break;
                    default:
                        break;
                }
                return;
            }
        }

        for( int i=0;i<gp->double_mode;++i ){
            if( gp->player[i].active && check_hit( gp->player+i,self ) ){
                self->active = 0;
                sound_play( PLAYER_GET_BONUS );
                switch( ((bonus*)self)->type ){
                    case BONUS_LIFE:
                        show_message( gp, PLAYER_GET_BONUS_LIFE, 9000 );
                        gp->player[i].life ++;
                        break;
                    case BOUNS_CLOCK:
                        show_message( gp, PLAYER_GET_BONUS_CLOCK, 9000 );
                        for( int j=0;j<ENEMY_MAX;++j ){
                            if( gp->enemy[j].active ){
                                tank_set_state( gp->enemy+j, TANK_FREEZING );
                            }
                        }
                        break;
                    case BOUNS_SHOVEL:
                        show_message( gp, ENEMY_GET_BONUS_SHOVEL, 9000 );
                        map_set_hawk_protected( &(gp->map) );
                        break;
                    case BONUS_BOMB:
                        show_message( gp, PLAYER_GET_BONUS_BOMB, 9000 );
                        for( int j=0;j<ENEMY_MAX;++j ){
                            if( gp->enemy[j].active ){
                                gp->enemy[j].active = 0;
                                _create_explode( gp->enemy[j].row, gp->enemy[j].col, BIG_EXPLODE );
                            }
                        }
                        break;
                    case BONUS_STAR:
                        show_message( gp, PLAYER_GET_BONUS_STAR, 9000 );
                        gp->player[i].level ++;
                        break;
                    case BONUS_HELMET:
                        show_message( gp, PLAYER_GET_BONUS_HELMET, 9000 );
                        tank_set_state( gp->player+i, TANK_SHIELD );
                        break;
                    default:
                        break;
                }
            }
            return;
        }
    }
}

static void _update_tank( game* gp ){
    _update_enemy( gp );
    _update_player( gp );
}



static void _update_tank( game* gp ){
    sprite* self;
    for( int i=0;i<TANK_MAX;++i ){
        self = gp->tank+i;
        if( self->active ){
            self->update(self);
            self->try_attack( self );
            self->try_move( self );
        }
    }
}



static void _update_explode( game* gp ){
    sprite* self;
    for( int i=0;i<EXPLODE_MAX;++i ){
        self = (sprite*)(gp->explode+i);
        if( self->active ){
            self->update(self);
            if( self->click == 0 ){
                self->active = 0;
            }
        }
    }
}

static void _bore2player( game* gp, int i ){
    gp->player_bore[i].living = 0;
    player_init( &(gp->player[i]) );
}


static void _update_bore( game* gp ){
    _update_player_bore( gp );
    _update_enemy_bore( gp );
}

static void _update_player_bore( game* gp ){
    try_create_player_bore( gp );
    for( int i=0; i<=!single_mode; ++i ){
        bore_update( &(gp->player_bore[i]) );
        if( bore_time_out( &(gp->player_bore[i]) ) ){
            _bore2player( gp,i );
        }
    }
}

static void _update_enemy_bore( game* gp ){
    for( int i=0;i<3;++i ){
        if( !gp->enemy_bore[i].active && _enemy_active_num( gp ) < ENEMY_ACTIVE_MAX ){
            _create_bore( gp, i );
        }
    }
    for( int i=0; i<3; ++i ){
        sprite* self = (sprite*)(gp->enemy_bore+i);
        if( self->active ){
            self->update(self);
            if( self->click == 0 ){
                _bore2enemy( gp, (bore*)self );
                self->active = 0;
            }
        }
    }
}


static void _bore2enemy( game* gp, bore* pb){
    for( int i=0; i<ENEMY_MAX; ++i ){
        if( !gp->enemy[i].active ){
            enemy_init( gp->enemy + i, row, col, random_in(0,2) );
            return;
        }
    }
}

static void _show_message( game* gp, int message, int ms ){
    gp->message_click = ms2click( ms );
    gp->message = message;
}

static void _get_hit_info( map* p, tile** pt0, int* ind0, tile** pt1, int* ind1, int row, int col, int dir ){

    switch (dir){
        case DIR_UP:
        case DIR_DOWN:
            *pt0 = map_cood_to_tile( p, row,col-8 );
            *pt1 = map_cood_to_tile( p, row,col+8 );
            *ind0= tile_block_index( row,col-8 );
            *ind1= tile_block_index( row,col+8 );
            return;
        case DIR_RIGHT:
        case DIR_LEFT:
            *pt0 = map_cood_to_tile( p, row-8,col );
            *pt1 = map_cood_to_tile( p, row+8,col );
            *ind0= tile_block_index( row-8,col );
            *ind1= tile_block_index( row+8,col );
            return;
        default:
            return;
    }
}

static void _destroy_tile_block( tile* pt, int ind ){
    pt->type[ind]=BLOCK_NULL;
    if( pt->type[1]== BLOCK_NULL &&
            pt->type[2]== BLOCK_NULL &&
            pt->type[3]== BLOCK_NULL &&
            pt->type[4]== BLOCK_NULL ){
        pt->type[0] = BLOCK_NULL;
    }else{
        pt->type[0] = BLOCK_MIX;
    }
}
