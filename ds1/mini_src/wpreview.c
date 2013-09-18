#include <math.h>
#include <string.h>
#include "structs.h"
#include "gfx_custom.h"
#include "misc.h"
#include "editobj.h"
#include "error.h"
#include "wpreview.h"


// ==========================================================================
// draw a sprite with a tint
void wpreview_shape(BITMAP * tmp_bmp, int x0, int y0, int ds1_idx, COL_E col_idx)
{
    if (glb_ds1edit.cmd_line.force_pal_num == -1) {
        color_map = & glb_ds1edit.cmap[CM_SELECT][glb_ds1.act - 1];
    } else {
        color_map = & glb_ds1edit.cmap[CM_SELECT][glb_ds1edit.cmd_line.force_pal_num - 1];
    }
    draw_lit_sprite(glb_ds1edit.screen_buff, tmp_bmp, x0, y0, col_idx);
}


// ==========================================================================
// helper of qsort, for tile drawing priority
int qsort_helper_order_data(const void *e1, const void *e2)
{
    ORDER_DATA_S * o1, * o2;

    o1 = (ORDER_DATA_S *) e1;
    o2 = (ORDER_DATA_S *) e2;

    // damn microsoft vc6 implementation of qsort() #=?*@!!
    if (o1->height == o2->height) {
        return o1->idx - o2->idx; // else it reverse the original order !
    }

    return o1->height - o2->height;
}


// ==========================================================================
// draw shadow layer
void wpreview_draw_s(int ds1_idx, int x, int y, int mx, int my, int z, int selected)
{
    BLOCK_TABLE_S * bt_ptr;
    BITMAP        * tmp_bmp;
    CELL_S_S      * s_ptr;
    int           n, t, bt_idx, dt1_idx, block_idx, y1, color;
    int           ux1, ux2, ux3, ux4, uy1, uy2, uy3;


    t     = (y * glb_ds1.shadow_line) + (x * glb_ds1.shadow_num);
    s_ptr = glb_ds1.shadow_buff + t;

    for (n=0; n<glb_ds1.shadow_num; n++) {
        color = -1;

        bt_idx = s_ptr[n].bt_idx; // index in block table

        if (s_ptr[n].prop4 & 0x80) {
            // binary : 1000-0000
            bt_idx = -1; // consider that tile as "unknown"
            continue;
        }

        bt_ptr = glb_ds1.block_table + bt_idx; // pointer in block table

        if (bt_ptr->type != BT_SHADOW) {
            continue; // only shadows
        }

        dt1_idx   = bt_ptr->dt1_idx;
        block_idx = bt_ptr->block_idx;

        tmp_bmp = * (glb_dt1[dt1_idx].block_zoom[z] + block_idx);

        if (tmp_bmp == NULL) {
            continue;
        }

        y1 = my - glb_ds1edit.win_preview.y0 - bt_ptr->zero_line;
        y1 += glb_ds1.tile_h; // shadow, like walls, are lower than floors
        //    (and than roofs) by 80 pixels

        if ( (y1 + tmp_bmp->h) < 0) {
            continue;
        }
        if (y1 >= glb_ds1edit.win_preview.h) {
            continue;
        }

        if (selected) {
            color = COL_MOUSE;
        }

        if ((color != -1) && (glb_ds1edit.mode != MOD_L)) {
            wpreview_shape(tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1, ds1_idx, color);
        } else {
            switch(glb_ds1.shadow_layer_mask[0]) {
                case 0 : // nothing to draw
                    break;

                case 1 : // normal sprite
                    draw_sprite(glb_ds1edit.screen_buff, tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1);
                    break;

                case 2 : // only 1 color, full white sprite
                    wpreview_shape(tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1, ds1_idx, COL_SHADOW);
                    break;

                case 3 : // transparent
                    if (glb_ds1edit.cmd_line.force_pal_num == -1) {
                        color_map = & glb_ds1edit.cmap[CM_TRANS][glb_ds1.act - 1];
                    } else {
                        color_map = & glb_ds1edit.cmap[CM_TRANS][glb_ds1edit.cmd_line.force_pal_num - 1];
                    }
                    draw_trans_sprite( glb_ds1edit.screen_buff, tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1);
                    break;
            }
        }
    }
}


// ==========================================================================
// prepare animated floor for a frame to draw
void wpreview_reiinit_animated_floor(int ds1_idx)
{
    BLOCK_TABLE_S * bt_ptr;
    int i;

    bt_ptr = glb_ds1.block_table;
    for (i=0; i < glb_ds1.bt_num; i++) {
        if (bt_ptr->type == BT_ANIMATED) {
            bt_ptr->updated = FALSE;
        }
        bt_ptr++;
    }
}


// ==========================================================================
// draw floor layer
void wpreview_draw_f(int ds1_idx, int x, int y, int mx, int my, int z, int selected)
{

    BLOCK_TABLE_S * bt_ptr;
    BITMAP        * tmp_bmp;
    CELL_F_S      * f_ptr;
    int           n, t, bt_idx, dt1_idx, block_idx, y1, color;
    ORDER_DATA_S  order_data[4];
    int           dist, c1, c2, c3, c4;
    int           ux1, ux2, ux3, ux4, uy1, uy2, uy3;


    t     = (y * glb_ds1.floor_line) + (x * glb_ds1.floor_num);
    f_ptr = glb_ds1.floor_buff + t;

    // search the order in which to draw the 4 floor layers of this tile
    for (n=0; n<glb_ds1.floor_num; n++) {
        order_data[n].idx    = n;
        order_data[n].height = f_ptr[n].prop1;
    }
    qsort(order_data, glb_ds1.floor_num, sizeof(ORDER_DATA_S), qsort_helper_order_data);

    for (n=0; n<glb_ds1.floor_num; n++) {
        color = -1;

        bt_idx = f_ptr[order_data[n].idx].bt_idx; // index in block table

        if (bt_idx == 0) {
            // no tiles here
            continue;
        }

        if (f_ptr[order_data[n].idx].prop4 & 0x80) {
            // binary : 1000-0000
            bt_idx = -1; // consider that tile as "unknown"
            continue;
        }


        bt_ptr = glb_ds1.block_table + bt_idx; // pointer in block table

        if ( (bt_ptr->type != BT_STATIC) && (bt_ptr->type != BT_ANIMATED) ) {
            continue; // only floors
        }

        if (bt_ptr->type == BT_ANIMATED) {
            // find the frame of an animated floor to draw
            if (bt_ptr->updated == FALSE) {
                bt_ptr->curr_frame = (glb_ds1.cur_anim_floor_frame / 5) % (bt_ptr->rarity + 1);
                bt_ptr->updated = TRUE;
            }
            bt_ptr += bt_ptr->curr_frame - bt_ptr->rarity;
        }

        dt1_idx   = bt_ptr->dt1_idx;
        block_idx = bt_ptr->block_idx;

        tmp_bmp = * (glb_dt1[dt1_idx].block_zoom[z] + block_idx);

        if (tmp_bmp == NULL) {
            continue;
        }

        y1 = my - glb_ds1edit.win_preview.y0 - bt_ptr->zero_line;
        if ( (y1 + tmp_bmp->h) < 0) {
            continue;
        }
        if (y1 >= glb_ds1edit.win_preview.h) {
            continue;
        }

        if (selected) {
            color = COL_MOUSE;
        }

        if (color != -1) {
            wpreview_shape(tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1, ds1_idx, color);
        } else {
            draw_sprite(glb_ds1edit.screen_buff, tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1);
        }
    }
}


// ==========================================================================
// draw wall layer
void wpreview_draw_w(int ds1_idx, int x, int y, int mx, int my, int z, int selected, int upper)
{
    BLOCK_TABLE_S * bt_ptr;
    BITMAP        * tmp_bmp;
    CELL_W_S      * w_ptr;
    int           n, t, bt_idx, dt1_idx, block_idx, m, s, y1;
    int           done, found, o, color;
    ORDER_DATA_S  order_data[4];
    int           dist, c1=0, c2=0;
    int           ux1, ux2, ux3, ux4, uy1, uy2, uy3;


    t     = (y * glb_ds1.wall_line) + (x * glb_ds1.wall_num);
    w_ptr = glb_ds1.wall_buff + t;

    // how to access the res {{{
    //  区分这几个idx,不然很难弄懂程序
    //
    //  wptr[n].bt_idx
    //  block_table + bt_idx
    //  bt_ptr->main_index
    //  bt_ptr->sub_index
    //  bt_ptr->dt1_idx
    //  bt_ptr->block_idx
    //  tmp_bmp = * (glb_dt1[dt1_idx].block_zoom[z] + block_idx)
    //

    // 1. 由cell的xy坐标和wall_buff获得w_ptr,w_ptr指向的wall_buff按照[0][1][2][3][0][1][2][3][0][1][2][3][0][1][2][3]....这样轮次排
    //
    //           typedef struct CELL_W_S
    //           {
    //               UBYTE prop1;
    //               UBYTE prop2;
    //               UBYTE prop3;
    //               UBYTE prop4;
    //               UBYTE orientation;
    //               int   bt_idx;
    //               UBYTE flags;
    //           } CELL_W_S;
    // 2. 由w_ptr[n]->bt_idx和block_table得到bt_ptr
    //          typedef struct BLOCK_TABLE_S
    //          {
    //              // key
    //              int       dt1_idx_for_ds1;
    //              long      main_index;
    //              long      orientation;
    //              long      sub_index;
    //          
    //              // datas
    //              int       dt1_idx;
    //              long      rarity;
    //              int       block_idx;
    //              BLK_TYP_E type;
    //              int       zero_line;
    //              int       roof_y;
    //          
    //              // conflicts managment
    //              char      conflict;
    //              char      used_by_game;   // True / False
    //              char      used_by_editor; // True / False
    //          
    //              // animated tile
    //              long      curr_frame;
    //              int       updated;
    //          } BLOCK_TABLE_S;
    // 3. 再按照如下方式得到bmp，从而可以绘图
    //      dt1_idx   = bt_ptr->dt1_idx;
    //      block_idx = bt_ptr->block_idx;
    //      tmp_bmp   = * (glb_dt1[dt1_idx].block_zoom[z] + block_idx);
    //
    //      }}}

    for (n=0; n<glb_ds1.wall_num; n++) {
        order_data[n].idx    = n;
        order_data[n].height = w_ptr[n].prop1;

        //typedef struct ORDER_DATA_S
        //{
        //   int idx;
        //   int height;
        //} ORDER_DATA_S;


        // draw only lower or upper wall, according to 'upper' param

        //      orientation == 0    :   floor
        //      orientation == 13   :   shadow
        //      其他都是wall,这其中:
        //          orientation == 10 || 11 :   special
        //          orientation == 15       :   roof
        //              不属于以上各类, 但是:
        //                  orientation < 15    :   wall up
        //                  orientation > 15    :   wall down
        if (w_ptr[n].orientation == 10) {
            order_data[n].height = 255;
        }
    }
    // qsort_helper_order_data中:
    // return o1->height - o2->height;
    // 可见排序还是一句height来的
    // 还有一个, 按理说wall类型也就只需要排一次 但是这里看来是每次调用这个函数都会被排列一次
    // 是不是有无用功了..还是怕中途有更新???
    qsort(order_data, glb_ds1.wall_num, sizeof(ORDER_DATA_S), qsort_helper_order_data);

    for (n=0; n<glb_ds1.wall_num; n++) {
        // 这个地方其实也让我很迷糊
        // 这个for循环在每个tile被画的时候都会执行
        // 那么这个长度为wall_num的数组到底装的是lowerwall一层，还是所有的wall(包括upper wall 之类的?)
        // 如果包含所有，为何在每次画lower wall时候都for一下？
        // 难道几个wall的排序不定么？
        // 或者说wall的总层数不超过wall_num么
        // 
        // 几次画完？这样搞一下 貌似一次画完
        // 然而并不是。。因为draw_w这个函数调用了多次
        // 后面可以看到，roof也在wall_buf里面阿
        color = -1;


        // draw only lower or upper wall, according to 'upper' param

        //      orientation == 0    :   floor
        //      orientation == 13   :   shadow
        //      其他都是wall,这其中:
        //          orientation == 10 || 11 :   special
        //          orientation == 15       :   roof
        //              不属于以上各类, 但是:
        //                  orientation < 15    :   wall up
        //                  orientation > 15    :   wall down

        o = w_ptr[order_data[n].idx].orientation;
        //这两个就是检测一下, 如果>=15 必然为downwall,这时候如果是upper
        //就没有必要画了..下一个同理
        if ((upper == TRUE) && (o >= 15)) {
            continue;
        }
        if ((upper == FALSE) && (o <= 15)) {
            continue;
        }

        //w_ptr = glb_ds1.wall_buff + t;
        //从这里可以看出wall_buff的排列方式
        //4个wall_num
        //按照[0][1][2][3][0][1][2][3][0][1][2][3][0][1][2][3]....这样轮次排着的..这样才能像下面一行这样寻址
        bt_idx = w_ptr[order_data[n].idx].bt_idx; // index in block table

        // no tiles here
        if (bt_idx == 0){
            continue;
        }

        // special tile asked to draw later
        if (glb_ds1.special_layer_mask && ((o == 10) || (o == 11))) {
            continue;
        }

        //prop4指示的是visiablity么?
        // binary : 0b1000,0000
        if (w_ptr[order_data[n].idx].prop4 & 0x80) {
            // hidden
            if ((o != 10) && (o != 11)) {
                bt_idx = -1; // a hidden floor --> "unknown"
                continue;
            }
        }


        //查表查表..就他娘的查表
        bt_ptr = glb_ds1.block_table + bt_idx; // pointer in block table
        // only walls or special tiles, but no roof
        if ( (bt_ptr->type != BT_WALL_UP)  && (bt_ptr->type != BT_WALL_DOWN) && (bt_ptr->type != BT_SPECIAL)){
            continue;
        }

        dt1_idx   = bt_ptr->dt1_idx;
        block_idx = bt_ptr->block_idx;

        //这个地方的block已经处理过了..成了一张图了...
        tmp_bmp = * (glb_dt1[dt1_idx].block_zoom[z] + block_idx);

        if (tmp_bmp == NULL) {
            continue;
        }

        // y1 = my - glb_ds1edit.win_preview.y0 - bt_ptr->zero_line;// * glb_ds1.height_mul / glb_ds1.height_div;
        // 验证了一下  果然是用于缩放的
        y1 = my - glb_ds1edit.win_preview.y0 - bt_ptr->zero_line;
        //my                            是菱形块的左角？
        //glb_ds1edit.win_preview.y0    nozz
        //bt_ptr->zero_line             这么说 zero_line就是个和当前tile对齐的值(不是subtile)
        //glb_ds1.height_mul   ? 我估计这两个是用于缩放的
        //glb_ds1.height_div   ?
        // walls are lower than floors (and than roofs) by 80 pixels
        // 作者说的大概是这个意思

        //                                      +---------------------+
        //                                      |                     |
        //                                      |      WALL           |
        //                                      |                     |
        //                                      |                     |
        //           +----------------+-------->+-   -   -   -/\      |
        //           | begin of tile  |   |     |           /    \    |
        //           +----------------+   |     |         /        \  |
        //                                |     |       /            \|
        //                              80px    |     /               |\
        //           +----------------+   |     |   /                 |  \
        //           | begin of wall  |   |     | /                   |    \
        //           +----------------+-------> *---------------------+     *
        //                                        \                        /
        //                                          \     FLOOR          /
        //                                            \                /
        //                                              \            /
        //                                                \        /
        //                                                  \    /
        //                                                    \/

        y1 += glb_ds1.tile_h; 
        if ( (y1 + tmp_bmp->h) < 0 || y1 >= glb_ds1edit.win_preview.h) {
            continue;
        }

        //typedef enum COL_E
        //{
        //    COL_FLOOR    =  29, // dark  grey,   selected without mouse
        //    COL_FLOOR_M  =  32, // light grey,   selected with mouse
        //    COL_WALL     = 151, // dark  blue,   selected without mouse
        //    COL_WALL_M   = 162, // light blue,   selected with mouse
        //    COL_MOUSE    = 255, // white,        not selected, with mouse
        //    COL_TMPSEL   =  92, // orange,       left-click selection, without mouse
        //    COL_TMPSEL_M = 108, // orange,       left-click selection, with mouse
        //    COL_PASTE_OK = 132, // green,        no conflicts when pasting
        //    COL_PASTE_KO =  98, // red,          conflicts when pasting
        //    COL_SHADOW   = 168  // yellow        shadow easy to view
        //} COL_E; // tile's color index in color map



        //             
        //   显示的小四边形，如此所示
        //   逐个描述关于orientation的问题
        //
        //              12*
        //             *   *
        //            *     *
        //           *       *
        //          *         *
        //       10.5         1.5
        //        *             *
        //       *               *
        //      *                 *
        //     *                   *
        //    9          0          3
        //     *                   *
        //      *                 *
        //       *               *
        //       7.5           4.5
        //         *           *
        //          *         *
        //           *       *
        //            *     *
        //             *   *
        //              *6*
        //                
        //

        // for debug 仅仅在tri_xxx.ds1中间测试，所以不准确
        // 0    木有反应
        // 1    9~10.5消失，有一个10.5~12的没消失，其余都消失
        // 2    6~9 12~3的不带转角的基本消失，有一个没消失
        // 3    10.5~1.5的转角消失
        // 4    木有反应
        // 5    那块1中10.5~12没消失的消失了
        // 6    和2互补了那块没消失的
        // 7    木有反应
        // 8    木有反应
        // 9    不想测试了
#if 0
        if (bt_ptr->orientation == 9) {
            continue;
        }
#endif

        if (selected) {
            wpreview_shape(tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1, ds1_idx, COL_MOUSE);
        }else{
            draw_sprite(glb_ds1edit.screen_buff, tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1);
        }


        // upper-left corner
        // block和wall都有一个orientation的问题..不知道这两个是个什么关系
        // 这一块看不懂
        if (bt_ptr->orientation == 3) {



            // 那个菱形
            // 左下为4, 右下为3..基本上
            // 这两个为啥要特别对待??????????????
            // 是那个高亮选中的原因么?
            // search the o=4 m=m s=s
            m = bt_ptr->main_idx;
            s = bt_ptr->sub_idx;
            done = FALSE;
            found = FALSE;


            // 如果当前bt_ptr指向的o为3
            // 那么直接切换到下一个bt_ptr,此时如果o还是小于4（1，2，3）, 那么bt_idx++
            // bt_ptr = glb_ds1.block_table + bt_idx;
            // 看着一行，感觉bt_ptr++和bt_idx++行为是一样的阿？？？？？
            //

            while( ! done) {
                // 若是越界而没找到,失败
                // 若是尚未越界并且 orientation小于4, bt_idx指向下一个
                // 若是等于4,并且其他信息吻合, nice, 就是他


                // 越界没
                if (bt_idx >= glb_ds1.bt_num){
                    done = TRUE;
                }else{
                    bt_ptr++;
                    if (bt_ptr->orientation < 4){
                        // 如果有一个bt_ptr的o为3
                        //
                        bt_idx++;
                    }else{
                        //尼玛这一块是为神马啊
                        if (bt_ptr->orientation == 4)
                            if ((bt_ptr->main_idx == m) && (bt_ptr->sub_idx == s))
                                done = found = TRUE;
                    }
                }
            }

            if (found == TRUE) {
                dt1_idx   = bt_ptr->dt1_idx;
                block_idx = bt_ptr->block_idx;
                tmp_bmp   = * (glb_dt1[dt1_idx].block_zoom[z] + block_idx);

                if (tmp_bmp == NULL) continue;

                y1 = my - glb_ds1edit.win_preview.y0 - bt_ptr->zero_line * glb_ds1.height_mul / glb_ds1.height_div;
                y1 += glb_ds1.tile_h; // walls are lower than floors (and than roofs) by 80 pixels
                if( ( (y1 + tmp_bmp->h) < 0)|| (y1 >= glb_ds1edit.win_preview.h)){
                    continue;
                }

                if (selected) {
                    wpreview_shape(tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1, ds1_idx, COL_MOUSE);
                }else{
                    draw_sprite(glb_ds1edit.screen_buff, tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1);
                }
            }
        }
    }
}


// ==========================================================================
// draw roof layer
void wpreview_draw_r(int ds1_idx, int x, int y, int mx, int my, int z, int selected)
{
    BLOCK_TABLE_S * bt_ptr;
    BITMAP        * tmp_bmp;
    CELL_W_S      * r_ptr;
    int           n, t, bt_idx, dt1_idx, block_idx, y1, color;
    ORDER_DATA_S  order_data[4];


    t     = (y * glb_ds1.wall_line) + (x * glb_ds1.wall_num);
    // 看到没有, roof也在wall里面阿
    r_ptr = glb_ds1.wall_buff + t;

    for (n=0; n<glb_ds1.wall_num; n++) {
        order_data[n].idx    = n;
        order_data[n].height = r_ptr[n].prop1;
    }
    qsort(order_data, glb_ds1.wall_num, sizeof(ORDER_DATA_S), qsort_helper_order_data);

    for (n=0; n<glb_ds1.wall_num; n++) {
        color = -1;
        if (glb_ds1.wall_layer_mask[order_data[n].idx] == 0) {
            continue;
        }


        bt_idx = r_ptr[order_data[n].idx].bt_idx; // index in block table

        if (bt_idx <= 0) {
            // no tiles here
            continue;
        }

        if (r_ptr[order_data[n].idx].prop4 & 0x80) {
            // binary : 1000-0000
            bt_idx = -1; // consider this tile as "unknown"
        }

        if (bt_idx <= 0) {
            continue;
        }

        bt_ptr = glb_ds1.block_table + bt_idx; // pointer in block table

        if (bt_ptr->type != BT_ROOF) {
            continue; // only roof
        }

        dt1_idx   = bt_ptr->dt1_idx;
        block_idx = bt_ptr->block_idx;

        tmp_bmp = * (glb_dt1[dt1_idx].block_zoom[z] + block_idx);

        if (tmp_bmp == NULL) {
            continue;
        }

        y1 = my - glb_ds1edit.win_preview.y0 - bt_ptr->zero_line;

        // roof height
        y1 -= bt_ptr->roof_y * glb_ds1.height_mul / glb_ds1.height_div;

        if ( (y1 + tmp_bmp->h) < 0) {
            continue;
        }
        if (y1 >= glb_ds1edit.win_preview.h) {
            continue;
        }

        if (selected) {
            color = COL_MOUSE;
        }
        if (color != -1) {
            wpreview_shape(tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1, ds1_idx, color);
        } else {
            draw_sprite(glb_ds1edit.screen_buff, tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1);
        }
    }
}


// ==========================================================================
// draw special tiles
void wpreview_draw_sp(int ds1_idx, int x, int y, int mx, int my, int z, int selected)
{
    BLOCK_TABLE_S * bt_ptr;
    BITMAP        * tmp_bmp;
    CELL_W_S      * w_ptr;
    int           n, t, bt_idx, dt1_idx, block_idx, y1, o;
    int           color;
    ORDER_DATA_S  order_data[4];
    int           dist, c1, c2;
    int           ux1, ux2, ux3, ux4, uy1, uy2, uy3;


    t     = (y * glb_ds1.wall_line) + (x * glb_ds1.wall_num);
    w_ptr = glb_ds1.wall_buff + t;

    for (n=0; n<glb_ds1.wall_num; n++) {
        order_data[n].idx    = n;
        order_data[n].height = w_ptr[n].prop1;
        if (w_ptr[n].orientation == 10) {
            order_data[n].height = 255;
        }
    }
    qsort(order_data, glb_ds1.wall_num, sizeof(ORDER_DATA_S), qsort_helper_order_data);

    for (n=0; n<glb_ds1.wall_num; n++) {
        color = -1;
        if (glb_ds1.wall_layer_mask[order_data[n].idx] == 0) {
            continue;
        }

        bt_idx = w_ptr[order_data[n].idx].bt_idx; // index in block table

        if (bt_idx == 0) {
            // no tile here
            continue;
        }

        o = w_ptr[order_data[n].idx].orientation;

        if ((o != 10) && (o != 11)) {
            // not a special tile
            continue;
        } else {
            // special tile

            if (bt_idx <= 0) {
                continue;
            }
        }

        bt_ptr    = glb_ds1.block_table + bt_idx; // pointer in block table
        dt1_idx   = bt_ptr->dt1_idx;
        block_idx = bt_ptr->block_idx;

        tmp_bmp = * (glb_dt1[dt1_idx].block_zoom[z] + block_idx);

        if (tmp_bmp == NULL) {
            continue;
        }

        y1 = my - glb_ds1edit.win_preview.y0 - bt_ptr->zero_line;
        y1 += glb_ds1.tile_h; // walls are lower than floors (and than roofs) by 80 pixels
        if ( (y1 + tmp_bmp->h) < 0) {
            continue;
        }
        if (y1 >= glb_ds1edit.win_preview.h) {
            continue;
        }

        if (selected) {
            color = COL_MOUSE;
        }
        if (color != -1) {
            wpreview_shape(tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1, ds1_idx, color);
        } else {
            draw_sprite(glb_ds1edit.screen_buff, tmp_bmp, mx - glb_ds1edit.win_preview.x0, y1);
        }
    }
}


// ==========================================================================
// which tile (or sub-tile) is at this coordinates ?
void coord_to_tile(int ds1_idx, int ax, int ay, int * layer_x, int * layer_y)
{
    int bx, by, cx, cy, rx, ry;

    // ax & ay adjustement, depending of the current mode
    if ( (glb_ds1edit.mode == MOD_O) || (glb_ds1edit.mode == MOD_P) || (glb_ds1edit.mode == MOD_L) ) {
        ax *= 5;
        ay *= 5;
    }

    // negative tile coordinates adjustment
    if (ax < 0) {
        ax -= glb_ds1.tile_w;
    }
    if (ay < 0) {
        ay -= glb_ds1.tile_h;
    }

    // search the Major tile's Coordinates
    bx = ax / glb_ds1.tile_w;
    by = ay / glb_ds1.tile_h;
    cx = bx  + by;
    cy = -bx + by;

    // fine adjustements (Minor tile's Coordinates)
    rx = ax % glb_ds1.tile_w;
    ry = ay % glb_ds1.tile_h;
    if (ax < 0){
        rx = glb_ds1.tile_w + rx;
    }
    if (ay < 0){
        ry = glb_ds1.tile_h + ry;
    }

    if (ry >= -rx / 2 + glb_ds1.tile_w/2 + glb_ds1.tile_h/2){
        cx++;
    } else if (ry < -rx / 2 + glb_ds1.tile_h/2) {
        cx--;
    } else if (glb_ds1.tile_w/2 - ry >= -rx / 2 + glb_ds1.tile_w/2 + glb_ds1.tile_h/2){
        cy--;
    } else if (glb_ds1.tile_w/2 - ry < -rx / 2 + glb_ds1.tile_h/2) {
        cy++;
    }

    // end
    * layer_x = cx;
    * layer_y = cy;
}


// ==========================================================================
// which tile (or sub-tile) is under the mouse ?
void mouse_to_tile(int ds1_idx, int * layer_x, int * layer_y)
{
    int ax, ay;

    // tile under mouse
    ax = glb_ds1edit.win_preview.x0 + mouse_x;
    ay = glb_ds1edit.win_preview.y0 + mouse_y;

    coord_to_tile(ds1_idx, ax, ay, layer_x, layer_y);
}


// ==========================================================================
// as expected, change zoom of 1 ds1
void change_zoom(int ds1_idx, ZOOM_E z)
{
    int mul, div, dx, dy, cx, cy;

    if (z < ZM_11){
        z = ZM_11;
    }else if (z > ZM_116){
        z = ZM_116;
    }
    z = ZM_11;

    dx = glb_ds1.own_wpreview.x0 + glb_ds1.own_wpreview.w / 2;
    dy = glb_ds1.own_wpreview.y0 + glb_ds1.own_wpreview.h / 2;

    // which tile (or sub-tile) is at this coordinates ?
    coord_to_tile(ds1_idx, dx, dy, &cx, &cy);
    cx++; // don't know why, but it is needed to keep the zoom centered

    switch(z){
        case ZM_11  : mul = 1, div =  1;  break;
        case ZM_12  : mul = 1, div =  2;  break;
        case ZM_14  : mul = 1, div =  4;  break;
        case ZM_18  : mul = 1, div =  8;  break;
        case ZM_116 : mul = 1, div = 16;  break;
        default     : mul = 1, div =  1;  break;
    }

    glb_ds1.cur_zoom   = z;
    glb_ds1.height_mul = mul;
    glb_ds1.height_div = div;
    glb_ds1.tile_w     = 160 * mul / div;
    glb_ds1.tile_h     = 80 * mul / div;
    if (glb_config.lower_speed_zoom_out){
        glb_ds1.cur_scroll.keyb.x = glb_config.scroll.keyb.x * mul / div;
        glb_ds1.cur_scroll.keyb.y = glb_config.scroll.keyb.y * mul / div;
        glb_ds1.cur_scroll.mouse.x = glb_config.scroll.mouse.x * mul / div;
        glb_ds1.cur_scroll.mouse.y = glb_config.scroll.mouse.y * mul / div;
    }

    if (glb_ds1edit.mode == MOD_T){
        //这玩意儿就是一个斜变换, 把tile的歇着计数方式变换到正常计算方式
        dx = (cy * -glb_ds1.tile_w / 2) + (cx * glb_ds1.tile_w / 2);
        dy = (cy *  glb_ds1.tile_h / 2) + (cx * glb_ds1.tile_h / 2);
    }
    else
    {
        dx = (cy * -glb_ds1.tile_w / 10) + (cx * glb_ds1.tile_w / 10);
        dy = (cy *  glb_ds1.tile_h / 10) + (cx * glb_ds1.tile_h / 10);
    }

    glb_ds1.own_wpreview.x0 = dx - glb_ds1.own_wpreview.w / 2;
    glb_ds1.own_wpreview.y0 = dy - glb_ds1.own_wpreview.h / 2;
}




// ==========================================================================
void wpreview_obj_animate(void)
{
    TXT_S     * txt = glb_ds1edit.obj_buff;
    COF_S     * cof;
    int       o;
    long      n, nb_ticks = glb_ds1edit.ticks_elapsed;


    glb_ds1edit.ticks_elapsed = 0;

    if (txt == NULL) {
        return;
    }

    // search frame number to draw for all TYPE of object
    for (o=0; o < txt->line_num; o++) {
        if (glb_ds1edit.obj_desc[o].usage_count) {
            cof = glb_ds1edit.obj_desc[o].cof;
            if (cof == NULL) {
                continue;
            }
            if (cof->fpd == 0) {
                continue;
            }
            if (cof->spd_div == 0) {
                continue;
            }

            //typedef struct COF_S
            //{
            //   UBYTE     lay;
            //   UBYTE     fpd; // frames per direction
            //   UBYTE     dir;
            //   long      xoffset;
            //   long      yoffset;
            //   LAY_INF_S lay_inf[COMPOSIT_NB];
            //   UBYTE     * priority;
            //   long      cur_frame;
            //   int       cur_dir;
            //   long      spd_mul;
            //   long      spd_div;
            //   long      spd_mod; // = is (mul % div), for extra precision
            //   long      orderflag; // from data\global\excel\objects.txt, 0 1 or 2
            //} COF_S;
            //

            //   long      n, nb_ticks = glb_ds1edit.ticks_elapsed;
            //   nb_ticks 说白了就是全局变量的拷贝
            //
            // default animation speed
            // animation speed, 说白了就是帧间差
            //每秒钟25个click
            //cof->spd_mul = 1;
            //cof->spd_div = 256;

            //   long      n, nb_ticks = glb_ds1edit.ticks_elapsed;
            //   glb_ds1edit.ticks_elapsed = 0;
            //这两句...nb_ticks就是上一次调用这个函数和这一次调用这个函数之间间隔的ticks数目
            //而不是历史上所有的ticks
            //注意这个程序是没有cpu使用率控制的

            //n=384, mul=176, div=256, frame_increasement=1
            //n=256, mul=256, div=256, frame_increasement=1
            //n=160, mul=140, div=256, frame_increasement=0
            //n=224, mul=100, div=256, frame_increasement=0
            //n=256, mul=160, div=256, frame_increasement=1
            //n=256, mul=128, div=256, frame_increasement=1
            //n=256, mul=128, div=256, frame_increasement=1
            //n=512, mul=256, div=256, frame_increasement=2
            //n=352, mul=112, div=256, frame_increasement=1
            //n=400, mul=104, div=256, frame_increasement=1
            //n=480, mul=176, div=256, frame_increasement=1
            //n=512, mul=256, div=256, frame_increasement=2
            //n=440, mul=140, div=256, frame_increasement=1
            //n=424, mul=100, div=256, frame_increasement=1

            //soga 我懂了~
            //虽然没有存储从开始到现在的所有的时间(像我以前那样)
            //但是他这里存储了一个模值(%),对周期取模
            //所以结果是一样的, 还不用担心溢出问题
            //看起来nb_ticks为上一次调用和这一次调用之间的差值, 实际统计了从开始到现在的所有时间ticks,再对周期取模了而已
            //周期是cof->fpd...嗯
            n = nb_ticks * cof->spd_mul + cof->spd_mod;
            cof->cur_frame += n / cof->spd_div;
            //printf("n=%d, mul=%d, div=%d, frame_increasement=%d\n",n,cof->spd_mul,cof->spd_div,n/cof->spd_div);
            cof->spd_mod    = n % cof->spd_div;
            //这个地方干嘛不取模, 而是这么搞?
            while (cof->cur_frame >= cof->fpd) {
                cof->cur_frame -= cof->fpd;
            }
        }
    }
}



// ==========================================================================
void wpreview_draw_an_object(int ds1_idx, int o)
{
    COF_S     * cof;
    LAY_INF_S * lay;
    BITMAP    ** bmp_ptr, * bmp;
    int       c, f, x, y, d, dx0, dy0, dx, dy, p, col_black, col_white;
    UBYTE     * bptr;
    UBYTE     new_frame;


    d = glb_ds1.obj[o].desc_idx;

    if (d == -1) {
        return;
    }

    if (glb_ds1edit.obj_desc[d].usage_count == 0) {
        return;
    }

    cof = glb_ds1edit.obj_desc[d].cof;
    if (cof == NULL) {
        return;
    }
    if (cof->fpd == 0) {
        return;
    }

    // random starting animation frame
    //UBYTE       frame_delta;
    new_frame = cof->cur_frame + glb_ds1.obj[o].frame_delta;
    while (new_frame >= cof->fpd) {
        new_frame -= cof->fpd;
    }

    x   = glb_ds1.obj[o].x;
    y   = glb_ds1.obj[o].y;
    // 这个dx0 dy0 的算法和mx my的算法很像
    // 但是这里x+3 y-2 是什么意思？？？默认一个obj大小不止一个tile么？？j
    dx0 = ((y-2) * -glb_ds1.tile_w / 10) + ((x+3) * glb_ds1.tile_w / 10);
    dy0 = 4 + ((y-2) *  glb_ds1.tile_h / 10) + ((x+3) * glb_ds1.tile_h / 10);

    col_black = makecol(  0,   0,   0);
    col_white = makecol(255, 255, 255);

    // body
    for (c=0; c < cof->lay; c++) {
#if 0
debug:
        cof->cur_dir = random()%2;
        添加一句后墙这些没影响
            npc也没影响
            地上的尸体，火等有影响
            还有那个吊着一个笼子的也有影响
#endif 



            p  = (cof->cur_dir * cof->lay * cof->fpd);
        p += c + (new_frame * cof->lay);
        p  = cof->priority[p];
        if (p >= COMPOSIT_NB) {
            continue;
        }

        lay = & cof->lay_inf[p];
        if (lay == NULL) {
            continue;
        }
        bmp_ptr = lay->bmp;
        if (bmp_ptr == NULL) {
            continue;
        }
        if (new_frame >= lay->bmp_num) {
            f = lay->last_good_frame;
        } else {
            f = new_frame;
        }
        bmp = bmp_ptr[f];
        if (bmp == NULL) {
            continue;
        }

#if 0
        most important here;

        dx0 = ((y-2) * -glb_ds1.tile_w / 10) + ((x+3) * glb_ds1.tile_w / 10);
        dy0 = 4 + ((y-2) *  glb_ds1.tile_h / 10) + ((x+3) * glb_ds1.tile_h / 10);

#endif 
        dx = dx0 - glb_ds1edit.win_preview.x0 + cof->xoffset + lay->off_x;
        dy = dy0 - glb_ds1edit.win_preview.y0 + cof->yoffset + lay->off_y;
        if (((dx + bmp->w) < 0) || ((dy + bmp->h) < 0)) {
            continue;
        }
        if ( (dx >= glb_ds1edit.win_preview.w) || (dy >= glb_ds1edit.win_preview.h)) {
            continue;
        }

        // normal drawing
        if ((lay->trans_a) && (lay->trans_b <= 6) && (lay->trans_b != 5)) {
            // valid gfx effect
            if (glb_ds1edit.cmd_line.force_pal_num == -1) {
                bptr = glb_ds1edit.d2_pal[glb_ds1.act - 1];
            } else {
                bptr = glb_ds1edit.d2_pal[glb_ds1edit.cmd_line.force_pal_num - 1];
            }
            switch(lay->trans_b) {
                case 0 : bptr += (256 * COF_75TRANS);     break;
                case 1 : bptr += (256 * COF_50TRANS);     break;
                case 2 : bptr += (256 * COF_25TRANS);     break;
                case 3 : bptr += (256 * COF_ALPHA);       break;
                case 4 : bptr += (256 * COF_LUMINANCE);   break;
                case 6 : bptr += (256 * COF_ALPHABRIGHT); break;
            }
            color_map = (COLOR_MAP *) bptr;
            draw_trans_sprite(glb_ds1edit.screen_buff, bmp, dx, dy);
        } else {
            // normal colors
            draw_sprite(glb_ds1edit.screen_buff, bmp, dx, dy);
        }

        // end
        if (new_frame < lay->bmp_num) {
            lay->last_good_frame = f;
        }
    }

}


// ==========================================================================
void wpreview_draw_an_object_shad(int ds1_idx, int o)
{
    COF_S     * cof;
    LAY_INF_S * lay;
    BITMAP    ** bmp_ptr, * bmp;
    int       c, f, x, y, d, dx0, dy0, dx, dy, p, offx, offy;
    UBYTE     * bptr;
    UBYTE     new_frame;

    d = glb_ds1.obj[o].desc_idx;

    if (d == -1) {
        return;
    }

    if (glb_ds1edit.obj_desc[d].usage_count == 0) {
        return;
    }

    cof = glb_ds1edit.obj_desc[d].cof;
    if (cof == NULL) {
        return;
    }
    if (cof->fpd == 0) {
        return;
    }

    new_frame = cof->cur_frame + glb_ds1.obj[o].frame_delta;
    while (new_frame >= cof->fpd) {
        new_frame -= cof->fpd;
    }

    x   = glb_ds1.obj[o].x;
    y   = glb_ds1.obj[o].y;

    dx0 = ((y-2) * -glb_ds1.tile_w / 10) + ((x+3) * glb_ds1.tile_w / 10);
    dy0 = 4 + ((y-2) *  glb_ds1.tile_h / 10) + ((x+3) * glb_ds1.tile_h / 10);

    // only projected shadow
    for (c=0; c < cof->lay; c++) {
        p  = (cof->cur_dir * cof->lay * cof->fpd);
        p += c + (new_frame * cof->lay);
        p  = cof->priority[p];
        if (p >= COMPOSIT_NB) {
            continue;
        }

        lay = & cof->lay_inf[p];
        if (lay == NULL) {
            continue;
        }

        // shadow for this layer ?
        if (lay->shad_a == 0) {
            continue;
        }

        bmp_ptr = lay->bmp;
        if (bmp_ptr == NULL) {
            continue;
        }
        if (new_frame >= lay->bmp_num) {
            f = lay->last_good_frame;
        } else {
            f = new_frame;
        }
        bmp = bmp_ptr[f];
        if (bmp == NULL) {
            continue;
        }
        dx = dx0 - glb_ds1edit.win_preview.x0 + cof->xoffset + lay->off_x;
        dy = dy0 - glb_ds1edit.win_preview.y0 + cof->yoffset + lay->off_y;
        if (((dx + bmp->w) < 0) || ((dy + bmp->h) < 0)) {
            continue;
        }
        if ( (dx >= glb_ds1edit.win_preview.w) || (dy >= glb_ds1edit.win_preview.h)) {
            continue;
        }

        // stretch drawing of shadow

        if (lay->trans_a) {
            // transparent layer, very few shadows
            if (glb_ds1edit.cmd_line.force_pal_num == -1) {
                bptr = glb_ds1edit.d2_pal[glb_ds1.act - 1];
            } else {
                bptr = glb_ds1edit.d2_pal[glb_ds1edit.cmd_line.force_pal_num - 1];
            }
            bptr += (256 * 32); // 28th level of transparency table on 32
        } else {
            // darker shadow
            if (glb_ds1edit.cmd_line.force_pal_num == -1) {
                bptr = glb_ds1edit.d2_pal[glb_ds1.act - 1];
            } else {
                bptr = glb_ds1edit.d2_pal[glb_ds1edit.cmd_line.force_pal_num - 1];
            }
            bptr += (256 * 14); // 10th level of transparency table on 32
        }

        offx = cof->xoffset + lay->off_x;
        offy = cof->yoffset + lay->off_y;

        dx = dx0 - glb_ds1edit.win_preview.x0 + offx;
        dy = dy0 - glb_ds1edit.win_preview.y0 + offy;

        stretch_trans_shadow_8bpp( glb_ds1edit.screen_buff, bmp, dx, dy, 1, bptr, dy0 - glb_ds1edit.win_preview.y0);

    }
}


// ==========================================================================
void wpreview_draw_obj_tile_shad(int ds1_idx, int x, int y, int * cur_idx)
{
    OBJ_S * obj;
    int   o;


    // 1st quick check
    if ((* cur_idx) >= glb_ds1.obj_num) {
        return;
    }

    // go on 1st potential object to draw
    o   = glb_ds1.drawing_order[* cur_idx];
    obj = & glb_ds1.obj[o];
    while (obj->ty < y) {
        // search line
        (* cur_idx) ++;
        if ( (* cur_idx) >= glb_ds1.obj_num ) {
            return;
        }
        o   = glb_ds1.drawing_order[* cur_idx];
        obj = & glb_ds1.obj[o];
    } if (obj->ty == y) {
        while (obj->tx < x) {
            // search column
            (* cur_idx) ++;
            if ( (* cur_idx) >= glb_ds1.obj_num ) {
                return;
            }
            o   = glb_ds1.drawing_order[* cur_idx];
            obj = & glb_ds1.obj[o];
        }
    } else {
        return;
    }

    // draw all objects on this tile, if any
    for (;;) {
        if ((obj->tx == x) && (obj->ty == y)) {
            // draw this obj



            {
                int tmpi;
                UBYTE infos[25];


                misc_search_walk_infos(0, x, y, infos);
                for(tmpi=0;tmpi<25;tmpi++){
                    if(infos[tmpi]&0x04 || infos[tmpi]&0x09){
                        break;
                    }
                }
                if(tmpi==25){
                    wpreview_draw_an_object_shad(ds1_idx, o);
                }
            }



            //wpreview_draw_an_object_shad(ds1_idx, o);

            // next obj
            (* cur_idx) ++;
            if ( (* cur_idx) >= glb_ds1.obj_num) {
                return;
            }
            o   = glb_ds1.drawing_order[* cur_idx];
            obj = & glb_ds1.obj[o];
        } else {
            return;
        }
    }
}


// ==========================================================================
void wpreview_draw_obj_tile_0_2(int ds1_idx, int x, int y, int * cur_idx)
{
    //orderflag来自object.txt，这里可能需要读一下以下内容，才能搞明白orderflag的用法
    //

#if 0
link: http://hi.baidu.com/hubingjy/blog/item/a965e802750ea0703812bbe5.html
#endif



          // 如何画一个obj的过程，这个函数其实有了一定的优化，不用在整个obj的table中寻找
          // 而是增加了一个drawing_order数组。
          // 我们也完全可以就寻找数组中x和y都与当前的tx和ty相等的就行，不过效率就低了
          //
          // 1. 根据drawing_order获得当前第一个需要画的obj在glb_ds1.obj中的位置
          // 2. 根据glb_ds1.obj的信息，获得obj在glb_ds1.obj_desc信息，这个信息最初来自txt文件
          // 3. 根据obj的desc信息，开始获得这个obj在desc的table中的信息
          // 4. 根据这个信息绘图




          // 这个地方的cur_idx是个好东西
          // 完全可以没有它 但是有了 就提高了效率
          // 如果没有 每个tile都要搜索整个数组
          // 有了后 就会从当前cur_idx开始
          //
          // 因为数组是排序好了的
          // 所以直接从cur_idx开始不会有问题
          OBJ_S * obj;
      int   o, d;
      COF_S * cof;


      // 1st quick check
      if ((* cur_idx) >= glb_ds1.obj_num) {
          return;
      }

      // go on 1st potential object to draw
      // drawing_order 是一个数组，按照y优先x然后的原则进行了排序。
      // drawing_order 的数组内容是glb_ds1.obj的数组index
      // glb_ds1.obj内的obj是没有进行排序的，排序后的元素下标放到了drawing_order
      o   = glb_ds1.drawing_order[* cur_idx];
      obj = & glb_ds1.obj[o];
      while (obj->ty < y) {
          // search line
          // 先寻找y
          (* cur_idx) ++;
          if ( (* cur_idx) >= glb_ds1.obj_num ) {
              return;
          }
          o   = glb_ds1.drawing_order[* cur_idx];
          obj = & glb_ds1.obj[o];
      }
      if (obj->ty == y) {
          while (obj->tx < x) {
              // search column
              // 再寻找x
              (* cur_idx) ++;
              if ( (* cur_idx) >= glb_ds1.obj_num ) {
                  return;
              }
              o   = glb_ds1.drawing_order[* cur_idx];
              obj = & glb_ds1.obj[o];
          }
      } else {
          return;
      }

      // draw all objects on this tile, if any
      for (;;) {
          if ((obj->tx == x) && (obj->ty == y)) {
              // draw this obj
              // 找到该画的第一个obj
              // 获得desc_idx，这个desc_idx是在txt中获取的，我记得
              d = glb_ds1.obj[o].desc_idx;
              if (d != -1) {
                  if (glb_ds1edit.obj_desc[d].usage_count) {
                      // 根据这个desc_idx获得这个obj的绘画参数
                      cof = glb_ds1edit.obj_desc[d].cof;
                      if (cof != NULL) {
                          // printf(" cof is not NULL .................\n");
                          if ((cof->orderflag == 0) || (cof->orderflag == 2)) {


                              int tmpi;
                              UBYTE infos[25];


                              misc_search_walk_infos(0, x, y, infos);
                              for(tmpi=0;tmpi<25;tmpi++){
                                  if(infos[tmpi]&0x04 || infos[tmpi]&0x09){
                                      break;
                                  }
                              }
                              if(tmpi==25){
                                  wpreview_draw_an_object(ds1_idx, o);
                              }
                          }
                      }
                  }
              }

              // next obj
              (* cur_idx) ++;
              if ( (* cur_idx) >= glb_ds1.obj_num) {
                  return;
              }
              o   = glb_ds1.drawing_order[* cur_idx];
              obj = & glb_ds1.obj[o];
          }else{
              return;
          }
      }
}


// ==========================================================================
void wpreview_draw_obj_tile_1(int ds1_idx, int x, int y, int * cur_idx)
{
    OBJ_S * obj;
    int   o, d;
    COF_S * cof;


    // 1st quick check
    if ((* cur_idx) >= glb_ds1.obj_num) {
        return;
    }

    // go on 1st potential object to draw
    o   = glb_ds1.drawing_order[* cur_idx];
    obj = & glb_ds1.obj[o];
    while (obj->ty < y) {
        // search line
        (* cur_idx) ++;
        if ( (* cur_idx) >= glb_ds1.obj_num ) {
            return;
        }
        o   = glb_ds1.drawing_order[* cur_idx];
        obj = & glb_ds1.obj[o];
    }
    if (obj->ty == y) {
        while (obj->tx < x) {// search column
            (* cur_idx) ++;
            if ( (* cur_idx) >= glb_ds1.obj_num ) {
                return;
            }
            o   = glb_ds1.drawing_order[* cur_idx];
            obj = & glb_ds1.obj[o];
        }
    } else {
        return;
    }

    // draw all objects on this tile, if any
    for(;;) {
        if ((obj->tx == x) && (obj->ty == y)) {
            // draw this obj
            d = glb_ds1.obj[o].desc_idx;
            if (d != -1) {
                if (glb_ds1edit.obj_desc[d].usage_count) {
                    cof = glb_ds1edit.obj_desc[d].cof;
                    if (cof != NULL) {
                        if (cof->orderflag == 1) {
                            wpreview_draw_an_object(ds1_idx, o);
                        }
                    }
                }
            }

            // next obj
            (* cur_idx) ++;
            if ( (* cur_idx) >= glb_ds1.obj_num) {
                return;
            }
            o   = glb_ds1.drawing_order[* cur_idx];
            obj = & glb_ds1.obj[o];
        } else {
            return;
        }
    }
}




// ==========================================================================
// draw all tiles of this ds1
void wpreview_draw_tiles(int ds1_idx)
{
    int               x, y, base_x = 0, base_y = 0, mx, my;
    int               cx, cy, dx, dy, z, x1, x2, x3, x4, y1, y2, y3, i, select;
    static int        old_pal = -1;
    char              * mode;
    UBYTE             walkinfo[25];
    int               objdraw_cur_idx = 0;
    void              (* fptr_wi) (int, int, int, UBYTE *);
    PATH_EDIT_WIN_S   * pwin = & glb_ds1.path_edit_win;


    z = glb_ds1.cur_zoom;
    clear(glb_ds1edit.screen_buff);

    // handle palette
    if (glb_ds1edit.cmd_line.force_pal_num == -1) {
        // use .ds1 act value for palette
        if (old_pal != glb_ds1.act - 1) {
            old_pal = glb_ds1.act - 1;
            set_palette(glb_ds1edit.vga_pal[old_pal]);
        }
    } else {
        // use force_pal value for palette
        if (old_pal != glb_ds1edit.cmd_line.force_pal_num - 1) {
            old_pal = glb_ds1edit.cmd_line.force_pal_num - 1;
            set_palette(glb_ds1edit.vga_pal[old_pal]);
        }
    }

    // which tile is under the mouse ?
    mouse_to_tile(ds1_idx, &cx, &cy);
    if (cx < 0) { cx = 0; } else if (cx >= glb_ds1.width)  { cx = glb_ds1.width  - 1; }
    if (cy < 0) { cy = 0; } else if (cy >= glb_ds1.height) { cy = glb_ds1.height - 1; }


#if 0
    // update objects animation, only if in 'running' animation mode
    if (glb_ds1.animations_layer_mask == 1) {
        wpreview_obj_animate();
    }
#endif

    // loop 1A : lower walls, floors, shadows of dt1
    for (y=0; y<glb_ds1.height; y++) {
        base_x = y * -glb_ds1.tile_w / 2;
        base_y = y * glb_ds1.tile_h / 2;
        for (x=0; x<glb_ds1.width; x++) {
            select = FALSE;
            if ((glb_ds1edit.mode == MOD_T) && (x==cx) && (y==cy))
                select = TRUE;
            mx = base_x + x * glb_ds1.tile_w / 2;
            if ((mx >= glb_ds1edit.win_preview.x0-glb_ds1.tile_w) && (mx < glb_ds1edit.win_preview.x0 + glb_ds1edit.win_preview.w)) {
                my = base_y + x * glb_ds1.tile_h / 2;
                wpreview_draw_w(ds1_idx, x, y, mx, my, z, select, FALSE); // lower walls
                wpreview_draw_f(ds1_idx, x, y, mx, my, z, select);        // floors
                wpreview_draw_s(ds1_idx, x, y, mx, my, z, select);        // shadows of dt1, what's this????
            }
        }
    }

#if 0
    // loop 1B : shadows of objects
    objdraw_cur_idx = 0;
    for (y=0; y<glb_ds1.height; y++) {
        base_x = y * -glb_ds1.tile_w / 2;
        base_y = y * glb_ds1.tile_h / 2;
        for (x=0; x<glb_ds1.width; x++) {
            select = FALSE;
            if ((glb_ds1edit.mode == MOD_T) && (x==cx) && (y==cy)) {
                select = TRUE;
            }
            mx = base_x + x * glb_ds1.tile_w / 2;
            if ((mx >= glb_ds1edit.win_preview.x0-glb_ds1.tile_w) && (mx < glb_ds1edit.win_preview.x0 + glb_ds1edit.win_preview.w)) {
                // shadows of objects
                wpreview_draw_obj_tile_shad(ds1_idx, x, y, & objdraw_cur_idx);
            }
        }
    }
#endif


#if 0
    // loop 2 : objects with orderflag set to 1 (optional)
    // 我注释了着一块 没发现有什么特别的
    objdraw_cur_idx = 0;
    if (glb_ds1.animations_layer_mask) {
        for (y=0; y<glb_ds1.height; y++) {
            base_x = y * -glb_ds1.tile_w / 2;
            base_y = y * glb_ds1.tile_h / 2;
            for (x=0; x<glb_ds1.width; x++) {
                select = FALSE;
                if ((glb_ds1edit.mode == MOD_T) && (x==cx) && (y==cy)) {
                    select = TRUE;
                }
                mx = base_x + x * glb_ds1.tile_w / 2;
                if ((mx >= glb_ds1edit.win_preview.x0-glb_ds1.tile_w) && (mx < glb_ds1edit.win_preview.x0 + glb_ds1edit.win_preview.w)) {
                    // objects of this tile
                    wpreview_draw_obj_tile_1(ds1_idx, x, y, & objdraw_cur_idx);
                }
            }
        }
    }
#endif


    // loop 3 : upper walls, objects with orderflag set to 0 or 2
    // orderflag到底是个什么东西
    // 这个可能要借助一下那个如何从mpq中抽取gif的文章才能看懂
    objdraw_cur_idx = 0;
    for (y=0; y<glb_ds1.height; y++) {
        base_x = y * -glb_ds1.tile_w / 2;
        base_y = y * glb_ds1.tile_h / 2;
        for (x=0; x<glb_ds1.width; x++) {
            select = FALSE;
            if ((glb_ds1edit.mode == MOD_T) && (x==cx) && (y==cy)) {
                select = TRUE;
            }
            mx = base_x + x * glb_ds1.tile_w / 2;
            if ((mx >= glb_ds1edit.win_preview.x0-glb_ds1.tile_w) && (mx < glb_ds1edit.win_preview.x0 + glb_ds1edit.win_preview.w)) {
                my = base_y + x * glb_ds1.tile_h / 2;
                wpreview_draw_w(ds1_idx, x, y, mx, my, z, select, TRUE);  // upper walls
                //wpreview_draw_obj_tile_0_2(ds1_idx, x, y, & objdraw_cur_idx);
            }
        }
    }

    // loop 4 : roofs
    for (y=0; y<glb_ds1.height; y++) {
        base_x = y * -glb_ds1.tile_w / 2;
        base_y = y * glb_ds1.tile_h / 2;
        for (x=0; x<glb_ds1.width; x++) {
            select = FALSE;
            if ((glb_ds1edit.mode == MOD_T) && (x==cx) && (y==cy)) {
                select = TRUE;
            }
            mx = base_x + x * glb_ds1.tile_w / 2;
            if ((mx >= glb_ds1edit.win_preview.x0-glb_ds1.tile_w) && (mx < glb_ds1edit.win_preview.x0 + glb_ds1edit.win_preview.w)) {
                my = base_y + x * glb_ds1.tile_h / 2;
                wpreview_draw_r (ds1_idx, x, y, mx, my, z, select); // roofs
            }
        }
    }

#if 0
    // loop 5 : special tiles (optional)
    if (glb_ds1.special_layer_mask) {
        for (y=0; y<glb_ds1.height; y++) {
            base_x = y * -glb_ds1.tile_w / 2;
            base_y = y * glb_ds1.tile_h / 2;
            for (x=0; x<glb_ds1.width; x++) {
                select = FALSE;
                if ((glb_ds1edit.mode == MOD_T) && (x==cx) && (y==cy)) {
                    select = TRUE;
                }
                mx = base_x + x * glb_ds1.tile_w / 2;
                if ((mx >= glb_ds1edit.win_preview.x0-glb_ds1.tile_w) && (mx < glb_ds1edit.win_preview.x0 + glb_ds1edit.win_preview.w)) {
                    my = base_y + x * glb_ds1.tile_h / 2;
                    wpreview_draw_sp(ds1_idx, x, y, mx, my, z, select); // special
                }
            }
        }
    }
#endif


    dx = (cy * -glb_ds1.tile_w / 2) + (cx * glb_ds1.tile_w / 2);
    dy = (cy *  glb_ds1.tile_h / 2) + (cx * glb_ds1.tile_h / 2);

    x1 = dx - glb_ds1edit.win_preview.x0;
    x2 = x1 + glb_ds1.tile_w / 2 - 1;
    x3 = x1 + glb_ds1.tile_w / 2;
    x4 = x1 + glb_ds1.tile_w - 1;

    y1 = dy - glb_ds1edit.win_preview.y0;
    y2 = y1 + glb_ds1.tile_h / 2 - 1;
    y3 = y1 + glb_ds1.tile_h - 2;

    line(glb_ds1edit.screen_buff, x1, y2, x2, y1, 129);
    line(glb_ds1edit.screen_buff, x3, y1, x4, y2, 129);
    line(glb_ds1edit.screen_buff, x3, y3, x4, y2, 129);
    line(glb_ds1edit.screen_buff, x1, y2, x2, y3, 129);


    misc_draw_screen(mouse_x, mouse_y);

    wpreview_reiinit_animated_floor(ds1_idx);
}

