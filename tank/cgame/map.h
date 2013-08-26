#ifndef _map_h_
#define _map_h_

#define BLOCK_NULL      0
#define BLOCK_BRICK     1
#define BLOCK_CONCRETE  2
#define BLOCK_FOREST    3
#define BLOCK_RIVER1    4
#define BLOCK_RIVER2    5
#define BLOCK_HAWK      6
#define BLOCK_STONE     7
#define BLOCK_MIX       8

extern ALLEGRO_BITMAP*  s_tile_img;
#define tile_image()    s_tile_img;

typedef struct _tile {
    int data[5];
}tile;

#define tile_type( p ) ((p)->data[0])
#define tile_sub_type( p, index ) ((p)->data[(index)+1])

#define MAP_WIDTH   13
#define MAP_HEIGHT  13

#define HAWK_PROTECTED  0
#define HAWK_BARE       1
#define HAWK_NORMAL     2


typedef struct _map {
    int         hawk_state;
    int         bare;
    int         map_click;
    tile        tiles[MAP_WIDTH*MAP_HEIGHT];
}map;


/*
 * public
 */

int map_load( map*, const char* );

#define map_tile( p, r, c ) ((p)->tiles[(r)*MAP_WIDTH+(c)])
#define map_pixel_tile( p, r, c ) ((p)->tiles[(r)/32*MAP_WIDTH+(c)/32])
#define map_pixel_type( p, r, c ) tile_type( &map_pixel_tile(p,r,c) )

void map_blit_tile( map*, int, int );
void map_blit_floor( map*, int, int );
void map_blit_cover( map*, int, int );

/*
 * private
 */


#define hawk_state( p )         ((p)->hawk_state)

/*
 * public
 */
void    map_update();


#endif
