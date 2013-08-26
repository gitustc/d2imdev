#include <stdio.h>
#include "map.h"

ALLEGRO_BITMAP*     s_img_tile;
#define _tile_block_index(row,col) ( ((row)%32 <16 ? 0:2 ) + ((col)%32 < 16 ? 0:1 ) )



int map_load( map* p, const char* path ){

    char ch;
	int  cou = 0;

	FILE* fp = fopen(path,"rb");
	if (!fp){
		fprintf(stderr,"fail to load file: %s \n",path);
		return false;
	}

	while( cou < 13*13 ){
		ch = fgetc(fp);
		if( ch >='0' && ch <= '8' ){

			if(ch != '8'){
                /*
                 * not a mix block;
                 * the top layer
                 */

                tile_type(     map_tile( p, cou/MAP_WIDTH, cou%MAP_WIDTH )    ) = ch-'0';
                tile_sub_type( map_tile( p, cou/MAP_WIDTH, cou%MAP_WIDTH ), 0 ) = ch-'0';
                tile_sub_type( map_tile( p, cou/MAP_WIDTH, cou%MAP_WIDTH ), 1 ) = ch-'0';
                tile_sub_type( map_tile( p, cou/MAP_WIDTH, cou%MAP_WIDTH ), 2 ) = ch-'0';
                tile_sub_type( map_tile( p, cou/MAP_WIDTH, cou%MAP_WIDTH ), 3 ) = ch-'0';

			}else{
                /*
                 *mix block
                 *fill the top layer
                 */

                tile_type(     map_tile( p, cou/MAP_WIDTH, cou%MAP_WIDTH )    ) = ch-'0';

                /*
                 * fill the sub layer
                 */
				for ( int i =0;i <4;i++){
					ch = fgetc(fp);
					if(ch >= '0' && ch <= '7'){
                        /*
                         * the block consists of 2*2 sub blocks
                         */
                        tile_sub_type( map_tile( p, cou/MAP_WIDTH, cou%MAP_WIDTH ), i ) = ch-'0';
					}else{
                        i--;
                    }
				}
			}
            cou ++;
        }
	}

	while( (ch=fgetc(fp)) != EOF){
		if(ch <='8' && ch >='0'){
			goto end;
        }
	}

	fclose(fp);
	fprintf(stderr,"succeed in loading %s\n",path);
	return 0;

	end:
	fclose(fp);
	fprintf(stderr,"incorrect map : %s\n",path);
	return 1;
}

void map_blit_floor( map* p, int row_off, int col_off ){

	int src_row_i;
	int src_col_i;
	int dst_row;
    int dst_col;
    int tmp_type;
    int tmp_type_i;

	for(int row=0;row<13;row++){
		for(int col=0;col<13;col++){


            tmp_type = map_tile_type( p, row,col );
			dst_row = row_off + 32*row;
			dst_col = col_off + 32*col;


            if( tmp_type != BLOCK_MIX ){
                if( tmp_type != BLOCK_FOREST ){
                    _draw( tile_image(),
                            0,tmp_type*32,
                            32,32,dst_row,dst_col);
                }
			}else{
                /*
                 * unnecessary
                 * you can draw the forest, and then redraw it...
                 */
                for(int i=0;i<4;i++){
					src_row_i = (i/2) * 16;
					src_col_i = (i%2) * 16;
                    tmp_type_i = map_sub_tile_type( p, row, col, i );
		
                    if( tmp_type_i != BLOCK_FOREST ){
						_draw( tile_image(),
                                src_row_i,src_col_i+tmp->type(i)*32,
								16,16,
								dst_row+src_row_i,dst_col+src_col_i);
					}
				}
			}
		}
}


void map_update_river( map* p ){

    int tmp_type;
    int tmp_type_i;

	for(int row=0;row<13;row++)
		for(int col=0;col<13;col++){

            tmp_type = map_tile_type( p, row, col );

            if( tmp_type != BLOCK_MIX ){
                if( tmp_type == BLOCK_RIVER1 ){
                    map_tile_type( p, row, col ) = BLOCK_RIVER2;
                }else if( tmp_type == BLOCK_RIVER2 ){
                    map_tile_type( p, row, col ) = BLOCK_RIVER1;
                }
            }else{
				for(int i=0;i<4;i++){
                    tmp_type_i = tile_sub_type( map_tile( p, row, col ), i );
                    if( tmp_type_i == BLOCK_RIVER1 ){
                        tile_sub_type( map_tile( p, row, col ), i ) = BLOCK_RIVER2;
                    }else if( tmp_type_i == BLOCK_RIVER2 ){
                        tile_sub_type( map_tile( p, row, col ), i ) = BLOCK_RIVER1;
                    }
				}
			}
		}
}

void map_blit_cover( map* p, int row_off, int col_off ){

	int src_row_i;
	int src_col_i;
	int dst_row;
    int dst_col;
    int tmp_type;
    int tmp_type_i;

	for(int row=0;row<13;row++){
		for(int col=0;col<13;col++){


            tmp_type = map_tile_type( p, row,col );
			dst_row = row_off + 32*row;
			dst_col = col_off + 32*col;


            if( tmp_type != BLOCK_MIX ){
                if( tmp_type == BLOCK_FOREST ){
                    _draw( tile_image(),
                            0,tmp_type*32,
                            32,32,
                            dst_row,dst_col,
                            RGB(0,0,0));
                }
			}else{
                for(int i=0;i<4;i++){
					src_row_i = (i/2) * 16;
					src_col_i = (i%2) * 16;
                    tmp_type_i = map_sub_tile_type( p, row, col, i );
		
                    if( tmp_type_i == BLOCK_FOREST ){
						_draw( tile_image(),
                                src_row_i,src_col_i+tmp->type(i)*32,
								16,16,
								dst_row+src_row_i,dst_col+src_col_i,
                                RGB(0,0,0));
					}
				}
			}
		}
}



void map_blit_tiles( map* p, int row_off, int col_off ){

	int src_row_i;
	int src_col_i;
	int dst_row;
    int dst_col;
    int tmp_type;
    int tmp_type_i;

	for(int row=0;row<13;row++){
		for(int col=0;col<13;col++){


            tmp_type = map_tile_type( p, row,col );
			dst_row = row_off + 32*row;
			dst_col = col_off + 32*col;


            if( tmp_type != BLOCK_MIX ){
                _draw( tile_image(),
                        0,tmp_type*32,
                        32,32,
                        dst_row,dst_col,
                        RGB(0,0,0));
			}else{
                for(int i=0;i<4;i++){
					src_row_i = (i/2) * 16;
					src_col_i = (i%2) * 16;
                    tmp_type_i = map_sub_tile_type( p, row, col, i );
		
                    _draw( tile_image(),
                            src_row_i,src_col_i+tmp->type(i)*32,
                            16,16,
                            dst_row+src_row_i,dst_col+src_col_i,
                            RGB(0,0,0));
				}
			}
		}
}

void map_update( map* p ){

    river_click ++;
    if( river_click == GAME_FPS*RIVER_UPDATE_SECOND ){
        update_river();
        river_click = 0;
    }

    switch( p->hawk_state ){
        case HAWK_NORMAL:
            return;
        case HAWK_PROTECTED:
            p->hawk_click ++;
            if( hawk_click > GAME_FPS*HAWK_PROTECTED_SECOND ){
                map_set_hawk_state( p, HAWK_NORMAL );
            }
            return;
        case HAWK_BARE:
            p->hawk_click ++;
            if( hawk_click > GAME_FPS*HAWK_BARE_SECOND ){
                map_set_hawk_state( p, HAWK_NORMAL );
            }
            return;
    }

}

/*
 * hawk_click control
 * tiles control
 */
void map_set_hawk_state( map* p, int new_state ){
    p->hawk_click = 0;
    switch( new_state ){
        case HAWK_PROTECTED:
            switch( p->hawk_state ){
                case HAWK_BARE:
                    goto to_normal;
                case HAWK_NORMAL:
                    goto to_protected;
                case HAWK_PROTECTED:
                    return;

            }
        case HAWK_NORMAL:
            switch( p->hawk_state ){
                case HAWK_BARE:
                case HAWK_PROTECTED:
                    goto to_normal;
                case HAWK_NORMAL:
                    return;
            }
        case HAWK_BARE:
            switch( p->hawk_state ){
                case HAWK_NORMAL:
                    goto to_bare;
                    return;
                case HAWK_PROTECTED:
                    goto to_normal;
                case HAWK_BARE:
                    return;
            }
    }

to_normal:
    p->hawk_state = HAWK_NORMAL;
    tile_type( map_tile( p,11,5 ) ) = BLOCK_MIX;
    tile_sub_type( map_tile( p,11,5 ),0 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,5 ),1 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,5 ),2 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,5 ),2 ) = BLOCK_BRICK;

    tile_type( map_tile( p,11,6 ) ) = BLOCK_MIX;
    tile_sub_type( map_tile( p,11,6 ),0 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,6 ),1 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,6 ),2 ) = BLOCK_BRICK;
    tile_sub_type( map_tile( p,11,6 ),2 ) = BLOCK_BRICK;
	
    tile_type( map_tile( p,11,7 ) ) = BLOCK_MIX;
    tile_sub_type( map_tile( p,11,7 ),0 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,7 ),1 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,7 ),2 ) = BLOCK_BRICK;
    tile_sub_type( map_tile( p,11,7 ),2 ) = BLOCK_NULL;

    tile_type( map_tile( p,12,5 ) ) = BLOCK_MIX;
    tile_sub_type( map_tile( p,12,5 ),0 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,12,5 ),1 ) = BLOCK_BRICK;
    tile_sub_type( map_tile( p,12,5 ),2 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,12,5 ),2 ) = BLOCK_BRICK;
	
    tile_type( map_tile( p,12,7 ) ) = BLOCK_MIX;
    tile_sub_type( map_tile( p,12,7 ),0 ) = BLOCK_BRICK;
    tile_sub_type( map_tile( p,12,7 ),1 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,12,7 ),2 ) = BLOCK_BRICK;
    tile_sub_type( map_tile( p,12,7 ),2 ) = BLOCK_NULL;
    return;

to_protected:
    p->hawk_state = HAWK_PROTECTED;
    tile_type( map_tile( p,11,5 ) ) = BLOCK_MIX;
    tile_sub_type( map_tile( p,11,5 ),0 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,5 ),1 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,5 ),2 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,5 ),2 ) = BLOCK_CONCRETE;

    tile_type( map_tile( p,11,6 ) ) = BLOCK_MIX;
    tile_sub_type( map_tile( p,11,6 ),0 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,6 ),1 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,6 ),2 ) = BLOCK_CONCRETE;
    tile_sub_type( map_tile( p,11,6 ),2 ) = BLOCK_CONCRETE;
	
    tile_type( map_tile( p,11,7 ) ) = BLOCK_MIX;
    tile_sub_type( map_tile( p,11,7 ),0 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,7 ),1 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,11,7 ),2 ) = BLOCK_CONCRETE;
    tile_sub_type( map_tile( p,11,7 ),2 ) = BLOCK_NULL;

    tile_type( map_tile( p,12,5 ) ) = BLOCK_MIX;
    tile_sub_type( map_tile( p,12,5 ),0 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,12,5 ),1 ) = BLOCK_CONCRETE;
    tile_sub_type( map_tile( p,12,5 ),2 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,12,5 ),2 ) = BLOCK_CONCRETE;
	
    tile_type( map_tile( p,12,7 ) ) = BLOCK_MIX;
    tile_sub_type( map_tile( p,12,7 ),0 ) = BLOCK_CONCRETE;
    tile_sub_type( map_tile( p,12,7 ),1 ) = BLOCK_NULL;
    tile_sub_type( map_tile( p,12,7 ),2 ) = BLOCK_CONCRETE;
    tile_sub_type( map_tile( p,12,7 ),2 ) = BLOCK_NULL;
    return;

to_bare:
    tile_type( map_tile( p,11,5 ) ) = BLOCK_NULL;
    tile_type( map_tile( p,11,6 ) ) = BLOCK_NULL;
    tile_type( map_tile( p,11,7 ) ) = BLOCK_NULL;
    tile_type( map_tile( p,12,5 ) ) = BLOCK_NULL;
    tile_type( map_tile( p,12,7 ) ) = BLOCK_NULL;
    return;
}

