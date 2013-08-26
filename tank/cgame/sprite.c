#include<stdio.h>
#include "sprite.h"


#define contain(a,b,c) (  (b)<=(c) ? ((a) >= (b) && (a) <= (c)) :  ((a) >= (c) && (a) <= (b)) )


int check_hit( sprite* p1, sprite* p2 ){
    int r1,r2,c1,c2;
    int w1,w2,h1,h2;
    r1=p1->row-p1->hot_row;
    r2=p2->row-p2->hot_row;
    c1=p1->col-p1->hot_col;
    c2=p2->col-p2->hot_col;
    w1=p1->width;
    w2=p2->width;
    h1=p1->height;
    h2=p2->height;
    return contain( r2,r1-h2,r1+h1) && contain( c2,c1-w2,c1+w1);
}


#if 0
=================================sprite================================
#endif

void    sprite_init( sprite* p ){
    p->click    = 0;
    p->row      = 0;
    p->col      = 0;
    p->hot_row  = 0;
    p->hot_col  = 0;
    p->width    = 0;
    p->height   = 0;
    p->blit     = NULL;
    p->update   = NULL;
}

#if 0
==============================static_sprite===========================
#endif

static void _static_sprite_update( static_sprite* p ){
    p->click ++;
}

void static_sprite_init( static_sprite* p ){
    sprite_init( (sprite*)p );
    p->update = _static_sprite_update;
}
/**************************************bonus****************************/
static _bonus_blit( sprite* p, int row_off, int col_off ){
    if( ((bonus*)p->click/4)%2 )return;
    _draw(
            bonus_image(),
            0, p->type*p->width,
            p->width, p->height,
            p->row - p->hot_row + row_off,
            p->col = p->hot_col + col_off
         );

}
void bonus_init( bonus* p ){
    static_sprite_init( (static_sprite*)p );
    p->width    = bonus_image()->width / 6;
    p->height   = bonus_image()->height;
    p->hot_row  = p->width/2;
    p->hot_col  = p->height /2;
    p->update   = _bonus_update;
    p->blit     = _bonus_blit;
    p->clicks   = 0;
}

void bonus_set( bonus* p, int bt, int row, int col ){
    p->type = bt;
    p->row  = row;
    p->col  = col;
}

/***********************************explode******************************************************/
static void _explode_blit( explode* p ){
    int br;
    int bc;
    p->clicks ++;
    br = p->row - p->hot_row + row_off;
    bc = p->col - p->hot_col + col_off;

    _draw(
            explode_get_image( p->type ),
            0,(p->clicks%3)*p->width,
            bc >= 0 ? p->width : p->width + bc,
            br >= 0 ? p->height : p->height + br,
            br >= 0 ? br : 0,
            bc >= 0 ? bc : 0
         );
}
void explode_set( explode* p, int et, int row, int col ){
    p->type     = et;
    p->row      = row;
    p->col      = col;
    p->clicks   = 0;

    switch (p->type){
        case EXPLODE_TILE:
            p->width = 28;
            p->height = 28;
            p->hot_row = 14;
            p->hot_col = 14;
            break;
        case EXPLODE_TANK:
        case EXPLODE_HAWK:
            p->width = 40;
            p->height = 40;
            p->hot_row = 20;
            p->hot_col = 20;
            break;
        default:
            break;
    }
}

/*********************************bore******************************************/
void bore_init( bore* p ){
    static_sprite_init( p );
    p->width    = 32;
    p->height   = 32;
    p->hot_row  = 16;
    p->hot_col  = 16;
}

void _bore_blit( bore* p, int row_off, int col_off ){
    _draw(
            bore_image(),
            0,(p->clicks%4)*p->width,
            p->width,p->height,
            p->row - p->hot_row + row_off,
            p->col - p->hot_col + col_off
            );
}

void bore_set( bore* p, int row, int col ){
    p->clicks = 0;
    p->row = row;
    p->col = col;
}

/******************************movable_sprite**************************************************/
void movable_sprite_init( movable_sprite* p ){
    sprite_init( (sprite*)p );
}

void movable_sprite_move( movable_sprite* p ){
    switch (p->dir){
        case DIR_UP:
            sprite_add_pos( p, -speed(), 0 );
            break;
        case DIR_DOWN:
            sprite_add_pos( p, speed(), 0 );
            break;
        case DIR_LEFT:
            sprite_add_pos( p, 0, -speed() );
            break;
        case DIR_RIGHT:
            sprite_add_pos( p, 0, speed() );
            break;
        default:
            break;
    }
}

/************************************bullet******************************************/
void bullet_init( bullet* p ){
    movable_sprite_init( (movable_sprite*)p );
    p->width = 8;
    p->height = 8;
    p->hot_row = 4;
    p->hot_col = 4;
}

void bullet_set( bullet* p, int dir, int row, int col){
    p->level = bl;
    p->row = row;
    p->col = col;
    p->dir = dir;
    p->speed = (1.+p->level*.2)*6;
}

static void _bullet_blit( bullet* p, int row_off,int col_off ){
    _draw(
            bullet_image(),
            0,(int)m_dir*8,
            8,8,
            m_row - m_hot_row + row_off,
            m_col - m_hot_col + col_off
         );
}

/*****************************************************/
void _tank_speed( tank* t ){
    return t->speed = (int)(t->level*0.7);
}
void tank_init( tank* t ){
    movable_sprite_init( t );
    t->speed = _tank_speed;
    t->width = 28;
    t->height = 28;
    t->hot_row = 14;
    t->hot_col = 14;
    t->level = 1;
    t->life = 1;
}


//Player
//=========================================================
//

void player_update( player* p ){

        if( p->moving ) _player_try_change_frame( p );
        if( p->shield > 0 ) p->shield -- ;
        if( p->wink > 0 ) p->wink --;

}

void _player_blit( int row_off, int col_off ){
    if(p->state == PLAYER_WINK){
        if((p->wink/3)%3)return;
    }

    //p->running ++ ;
    if(p->type == 1){
        _draw(
                player_image( 1 );
                p->dir*28,((p->running/2)%2 + 2*((int)p->level-1))*28,
                //p->dir*28,((p->running/2)%2 + 2*(int)p->level)*28,
                //waring level 0~N or 1~N
                28,28,
                p->row - p->hot_row + row_off,
                p->col - p->hot_col + col_off);
        if(p->shield){
            _draw(  
                    player_image(1);
                    32*((p->running/2)%2),0,
                    32,32,
                    p->row - p->hot_row - 4 + row_off + 2,
                    p->col - p->hot_col - 4 + col_off + 2,//加2为微调，无理论关系
            );
        }
    }else{
        _draw(
                player_image();
                p->dir*28,((p->running/2)%2 + 2*(p->level-1))*28,
                28,28,
                p->row - p->hot_row + row_off,
                p->col - p->hot_col + col_off);
        if(p->shield){
            _draw(
                    player_image();
                    32*((p->blit_click/2)%2),0,
                    32,32,
                    p->row - p->hot_row - 4 + row_off + 2,
                    p->col - p->hot_col - 4 + col_off + 2, //加2为微调，无理论关系
            );
        }
    }
}


void player_set( player* p, int pt, int row, int col )
    p->type = pt;
    p->row = row;
    p->col = col;
    p->dir = DIR_UP;
    p->level = 1;
    p->life = 1;
    p->state = PLAYER_NORMAL;
}

#define player_set_state( p, s ) do{(p)->state = (s); p->wink = 0; }while(0)

void _player_update( player* p ){
    _tank_update( p );
    if(p->state == PLAYER_WINK){
        p->wink ++;
        if(p->wink > ){
            p->state = PLAYER_NORMAL;
        }
    }
}

int player_hitted( player* p ){

    if(p->shield){
        return 0;
    }
    p->life -- ;
    if(p->life == 0){
        return 1;
    }else{
        return 0;
    }
}

//Enemy
//============================================================

void enemy_init( enemy* p ){
    tank_init( p );
    p->dir = DIR_DOWN;
}
void enemy_set( enemy* p, int et, int flash, int row, int col ){
    p->type = et;
    p->row = row;
    p->col = col;
    p->flash = flash;

    if( et == 3 )
        p->life = 3;
    else
        p->life = 1;
}

void _enemy_blit( int row_off, int col_off ){

    switch(p->type){
        case 1:
            if(p->flash){
                _draw(
                        enemy_image(),
                        (int)p->dir*28,((p->blit_click/2)%4)*28, //0 0 1 1 2 2 3 3 0 0 1 1 ....
                        28,28,
                        p->row - p->hot_row + row_off,
                        p->col - p->hot_col + col_off
                     );
            }else{
                _draw(
                        enemy_image(),
                        (int)p->dir*28,((p->blit_click/2)%2)*28, // 0 0 1 1 0 0 1 1 
                        28,28,
                        p->row - p->hot_row + row_off,
                        p->col - p->hot_col + col_off
                     );
            }
            break;

        case 2:
            if(p->flash){
                _draw(
                        enemy_image(),
                        (int)p->dir*28,(((p->blit_click/2)%4) + 4)*28,
                        28,28,
                        p->row - p->hot_row + row_off,
                        p->col - p->hot_col + col_off
                        );
            }else{
                _draw(
                        enemy_image(),
                        (int)p->dir*28,(((p->blit_click/2)%2) + 4)*28,
                        28,28,
                        p->row - p->hot_row + row_off,
                        p->col - p->hot_col + col_off
                        );

            }
            break;
        case 3:
            if(p->flash){
                _draw(
                        enemy_image(),
                        ((int)p->dir+4)*28,(((p->blit_click/2)%2) + ((p->blit_click/4)%2)*6)*28,
                        28,28,
                        p->row - p->hot_row + row_off,
                        p->col - p->hot_col + col_off);
            }else{
                _draw(
                        enemy_image(),
                        ((int)p->dir+4)*28,((p->blit_click/2)%2+(3-p->life_left)*2)*28 ,
                        28,28,
                        p->row - p->hot_row + row_off,
                        p->col - p->hot_col + col_off);
            }
            break;
        default:
            break;
    }


    if(p->shield){
                _draw(
                        enemy_shield_image(),
                        32*((p->shield_click/2)%2),0,
                        32,32,
                        p->row - p->hot_row - 4 + row_off + 2,
                        p->col - p->hot_col - 4 + col_off + 2 //加2为微调，无理论关系
                        );
    }
}

int enemy_can_attack( enemy* ep ){
    return ( ep->living && ep->freezing == 0 && ep->attack_click == 0 );
}

int enemy_can_move( enemy* ep ){
    return ( ep->living && ep->freezing == 0 );
}
int enemy_hitted( enemy* p ){

    if(p->flash){
        p->flash = 0;
        return 0;
    }
    if(p->shield){
        return 0;
    }
    p->life -- ;
    if(p->life == 0){
        return 1;
    }else{
        return 0;
    }
}
