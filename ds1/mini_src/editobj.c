#include "structs.h"
#include "misc.h"
#include "error.h"
#include "anim.h"
#include "editobj.h"


// ==========================================================================
// helper of the qsort function, for object drawing order
int editobj_qsort_helper_drawing_order(const void * e1, const void * e2)
{
    int   ds1_idx = glb_ds1edit.obj_order_ds1_idx;
    int   i1      = * ((int *) e1),
          i2      = * ((int *) e2);
    OBJ_S * o1, * o2;


    // primary order key  : tile position
    // secondary oder key : modified sub-tile position in that tile
    o1 = & glb_ds1.obj[i1];
    o2 = & glb_ds1.obj[i2];
    if (o1->ty == o2->ty){
        if (o1->tx == o2->tx){
            if (glb_ds1edit.obj_sub_tile_order[o1->sx][o1->sy] == glb_ds1edit.obj_sub_tile_order[o2->sx][o2->sy]){
                return i1 - i2;
            }else{
                return glb_ds1edit.obj_sub_tile_order[o1->sy][o1->sx] - glb_ds1edit.obj_sub_tile_order[o2->sy][o2->sx];
            }
        }else{
            return o1->tx - o2->tx;
        }
    }
    return o1->ty - o2->ty;
}


// ==========================================================================
// sort objects to prepare the drawing function
void editobj_set_drawing_order(int ds1_idx)
{
    OBJ_S       *obj;
    int          i;


    //   prepare the sort
    //
    //
    //   typedef struct OBJ_S {{{
    //   {
    //
    //     long        type;
    //     long        id;
    //     long        x;     // sub-cell X
    //     long        y;     // sub-cell Y
    //     long        ds1_flags;
    //
    //     .......
    //     .......
    //
    //     // for sorting
    //     long        tx; // tile X
    //     long        ty; // tile Y
    //     long        sx; // sub-tile X
    //     long        sy; // sub-tile Y
    //
    //     .......
    //     .......
    //
    //   } OBJ_S;
    //
    //  上面的数据结构中
    //  tx和sx我现在估计可能是tile, 也就是菱形的块和字块
    //  x和y作者标明是sub-cell-X和sub-cell-Y, 这表示的是什么呢?
    //  事实上cell和subsell我还没怎么搞明白
    //
    //  但是最终程序左下角有个cell(x,y), 里面标明cell的计数方式是菱形的方式


    // 我一直没分清楚: cell sub-cell tile sub-tile
    // 输出大概是如此:

    // obj =    0, tx =   11, ty =    6, sx =    0, sy =    3
    // obj =    1, tx =   11, ty =    9, sx =    0, sy =    2
    // obj =    2, tx =   31, ty =   19, sx =    0, sy =    2
    // obj =    3, tx =   30, ty =   19, sx =    2, sy =    2
    // obj =    4, tx =   29, ty =   14, sx =    2, sy =    2
    // obj =    5, tx =   28, ty =   14, sx =    3, sy =    2
    // obj =    6, tx =   25, ty =   16, sx =    4, sy =    4
    // obj =    7, tx =   25, ty =   15, sx =    3, sy =    4
    // obj =    8, tx =   25, ty =   13, sx =    0, sy =    3
    // obj =    9, tx =   24, ty =   13, sx =    1, sy =    2
    // obj =   10, tx =   19, ty =    5, sx =    1, sy =    0
    // obj =   11, tx =   18, ty =    4, sx =    4, sy =    1
    // obj =   12, tx =   18, ty =    8, sx =    2, sy =    3
    // obj =   13, tx =   18, ty =    7, sx =    0, sy =    4
    // obj =   14, tx =   16, ty =    9, sx =    4, sy =    1
    // obj =   15, tx =   16, ty =    9, sx =    1, sy =    1
    // obj =   16, tx =   19, ty =   12, sx =    2, sy =    3
    // obj =   17, tx =   18, ty =   12, sx =    4, sy =    4
    // obj =   18, tx =   27, ty =   18, sx =    0, sy =    0
    // obj =   19, tx =   26, ty =    9, sx =    0, sy =    0
    // obj =   20, tx =   16, ty =   11, sx =    3, sy =    3
    // obj =   21, tx =   20, ty =   10, sx =    4, sy =    2
    // obj =   22, tx =   20, ty =    7, sx =    4, sy =    4
    // obj =   23, tx =   18, ty =    3, sx =    0, sy =    3
    // obj =   24, tx =   16, ty =    6, sx =    0, sy =    4
    // obj =   25, tx =   15, ty =    8, sx =    2, sy =    0
    // obj =   26, tx =   15, ty =   10, sx =    2, sy =    0
    // obj =   27, tx =   16, ty =   11, sx =    0, sy =    0
    // obj =   28, tx =    7, ty =    5, sx =    2, sy =    0
    // obj =   29, tx =    7, ty =    7, sx =    2, sy =    0
    // obj =   30, tx =    7, ty =    9, sx =    2, sy =    0
    // obj =   31, tx =    7, ty =   11, sx =    2, sy =    0
    // obj =   32, tx =    4, ty =    5, sx =    1, sy =    3
    // obj =   33, tx =   16, ty =    4, sx =    3, sy =    3
    // obj =   34, tx =    8, ty =    3, sx =    4, sy =    4
    // obj =   35, tx =    4, ty =    1, sx =    2, sy =    2
    // obj =   36, tx =    3, ty =    8, sx =    4, sy =    0
    // obj =   37, tx =    4, ty =    4, sx =    0, sy =    0
    // obj =   38, tx =    4, ty =    1, sx =    3, sy =    2
    // obj =   39, tx =    3, ty =   11, sx =    2, sy =    0
    // obj =   40, tx =    3, ty =    9, sx =    2, sy =    0
    // obj =   41, tx =    3, ty =    7, sx =    2, sy =    0
    // obj =   42, tx =    3, ty =    5, sx =    2, sy =    0
    // obj =   43, tx =   16, ty =   12, sx =    2, sy =    2
    // obj =   44, tx =   16, ty =   13, sx =    3, sy =    1
    // obj =   45, tx =   16, ty =    8, sx =    0, sy =    3
    // obj =   46, tx =   17, ty =    8, sx =    1, sy =    0
    // obj =   47, tx =   25, ty =   14, sx =    1, sy =    2
    // obj =   48, tx =   28, ty =   13, sx =    2, sy =    3

    // 基本可以看出sx和sy都在0~4之间, 而tx和ty也在width和height之间
    // 没有cell的信息
    //
    // }}}
    for(i=0; i < glb_ds1.obj_num; i++){
        obj     = & glb_ds1.obj[i];
        obj->tx = obj->x / 5L;
        obj->sx = obj->x % 5L;
        obj->ty = obj->y / 5L;
        obj->sy = obj->y % 5L;
        printf( "obj = %4ld, tx = %4ld, ty = %4ld, sx = %4ld, sy = %4ld\n", i, obj->tx, obj->ty, obj->sx, obj->sy );
    }

    // force unused objects to be in last position
    // 好理解..由于只有obj_num个, 后面的都为没用的
    // 直接把ty赋值为最大..所以到了最后, 所以还是y排序方法啊, 不过是ty, 所以成了2.5D
    //
    for (i = glb_ds1.obj_num; i < glb_ds1.current_obj_max; i++){
        glb_ds1.obj[i].ty = 32000L;
    }

    // re-init the drawing order table to the default
    for (i=0; i < glb_ds1.current_obj_max; i++){
        glb_ds1.drawing_order[i] = i;
    }

    // sort it
    glb_ds1edit.obj_order_ds1_idx = ds1_idx;
    // editobj_qsort_helper_drawing_order 这里就贴过来了
    // 不过大概过程是:
    // 如果ty不一致, 谁ty小先画谁
    // 如果ty相等, tx不一致, 则tx谁小先画谁
    // 如果tx和ty都是相等的, 则在obj_table中谁在前先画谁
    // 大概就是这个意思
    qsort( glb_ds1.drawing_order, glb_ds1.obj_num, sizeof(int), editobj_qsort_helper_drawing_order );


    for (i=0; i < glb_ds1.current_obj_max; i++){
        printf( "order = %4d, idx = %4d\n", i, glb_ds1.drawing_order[i] );
    }
    // 这一段的输出结果如下, 从49开始为正常排序, 因为上面把没用的obj都设置为了最大, 所以如此 {{{
    // order =    0, idx =   35
    // order =    1, idx =   38
    // order =    2, idx =   34
    // order =    3, idx =   23
    // order =    4, idx =   37
    // order =    5, idx =   33
    // order =    6, idx =   11
    // order =    7, idx =   42
    // order =    8, idx =   32
    // order =    9, idx =   28
    // order =   10, idx =   10
    // order =   11, idx =    0
    // order =   12, idx =   24
    // order =   13, idx =   41
    // order =   14, idx =   29
    // order =   15, idx =   13
    // order =   16, idx =   22
    // order =   17, idx =   36
    // order =   18, idx =   25
    // order =   19, idx =   45
    // order =   20, idx =   46
    // order =   21, idx =   12
    // order =   22, idx =   40
    // order =   23, idx =   30
    // order =   24, idx =    1
    // order =   25, idx =   15
    // order =   26, idx =   14
    // order =   27, idx =   19
    // order =   28, idx =   26
    // order =   29, idx =   21
    // order =   30, idx =   39
    // order =   31, idx =   31
    // order =   32, idx =   27
    // order =   33, idx =   20
    // order =   34, idx =   43
    // order =   35, idx =   17
    // order =   36, idx =   16
    // order =   37, idx =   44
    // order =   38, idx =    9
    // order =   39, idx =    8
    // order =   40, idx =   48
    // order =   41, idx =   47
    // order =   42, idx =    5
    // order =   43, idx =    4
    // order =   44, idx =    7
    // order =   45, idx =    6
    // order =   46, idx =   18
    // order =   47, idx =    3
    // order =   48, idx =    2
    // order =   49, idx =   49     //这里开始就是把ty设置为最大导致的, 因为他们都没用过诶
    // order =   50, idx =   50
    // order =   51, idx =   51
    // order =   52, idx =   52
    // order =   53, idx =   53
    // order =   54, idx =   54
    // order =   55, idx =   55
    // order =   56, idx =   56
    // order =   57, idx =   57
    // order =   58, idx =   58
    // order =   59, idx =   59
    // order =   60, idx =   60
    // order =   61, idx =   61
    // order =   62, idx =   62
    // order =   63, idx =   63
    // order =   64, idx =   64
    // order =   65, idx =   65
    // order =   66, idx =   66
    // order =   67, idx =   67
    // order =   68, idx =   68
    // order =   69, idx =   69
    // order =   70, idx =   70
    // order =   71, idx =   71
    // order =   72, idx =   72
    // order =   73, idx =   73
    // order =   74, idx =   74
    // order =   75, idx =   75
    // order =   76, idx =   76
    // order =   77, idx =   77
    // order =   78, idx =   78
    // order =   79, idx =   79
    // order =   80, idx =   80
    // order =   81, idx =   81
    // order =   82, idx =   82
    // order =   83, idx =   83
    // order =   84, idx =   84
    // order =   85, idx =   85
    // order =   86, idx =   86
    // order =   87, idx =   87
    // order =   88, idx =   88
    // order =   89, idx =   89
    // order =   90, idx =   90
    // order =   91, idx =   91
    // order =   92, idx =   92
    // order =   93, idx =   93
    // order =   94, idx =   94
    // order =   95, idx =   95
    // order =   96, idx =   96
    // order =   97, idx =   97
    // order =   98, idx =   98
    // order =   99, idx =   99
    // }}}
}


// ==========================================================================
// make the 'label' windows of all objects of 1 ds1
void editobj_make_obj_label(int ds1_idx)
{
    OBJ_LABEL_S * label;
    int         d, o, w, wtmp, h;
    long        type, id;


    for (o=0; o < glb_ds1.obj_num; o++) {
        type  = glb_ds1.obj[o].type;
        id    = glb_ds1.obj[o].id;
        d     = glb_ds1.obj[o].desc_idx;
        label = & glb_ds1.obj[o].label;

        w = 16 * 8;     // 16 chars
        h =  2 * 8 + 2; // 2 char lines + 2 pixel lines for readibility

        if (d != -1) {
            wtmp = 8 * strlen(glb_ds1edit.obj_desc[d].desc);
            if (wtmp > w){
                w = wtmp;
            }
        }
        label->rx    = 0;
        label->ry    = 0;
        label->w     = w;
        label->h     = h;
        label->flags = 0;
    }
}



// ==========================================================================
// search the description index of 1 object of 1 ds1, given its Type & Id
void editobj_make_obj_desc(int ds1_idx, int obj_idx)
{
    int    *nb_type_per_act_ptr;
    int     act, type, id, i;

    act  = glb_ds1.act;
    type = glb_ds1.obj[obj_idx].type;
    id   = glb_ds1.obj[obj_idx].id;

    // first try : search the exact act/type/id in data\obj.txt
    for(i=0; i < glb_ds1edit.obj_desc_num; i++){
        if( (glb_ds1edit.obj_desc[i].act  == act)  && (glb_ds1edit.obj_desc[i].type == type) && (glb_ds1edit.obj_desc[i].id   == id) ){
            // end
            // 必须要由act，type和id三个变量共同决定这个obj的位置 {{{
            // 三个才能决定在data/obj.txt中的行号，举例如下（省略了行号）：
            // act type id
            // 1	1	0
            // 1	1	1
            // 1	1	2
            // 1	1	3
            // 1	1	4
            // 1	1	5
            // 1	1	6
            // 1	1	7
            // 1	1	8
            // 1	2	0
            // 1	2	1
            // 1	2	2
            // 1	2	3
            // 2	1	0
            // 2	1	1
            // 2	1	2
            // 2	1	3
            // 2	2	0
            // 2	2	1
            // 2	2	2
            // 2	2	3
            // 2	2	4
            // 2	2	5
            // 3	1	0
            // 3	1	1
            // 3	1	2
            // 3	1	3
            // 3	1	4
            // 3	1	5
            // 3	1	6
            // }}}
            glb_ds1.obj[obj_idx].desc_idx = i;
            return;
        }
    }

    // not found in data\obj.txt
    // 我调试的过程中 下面的代码没有被执行

    // swap act/id according to object type
    if (type == 1){
        nb_type_per_act_ptr = & glb_config.nb_type1_per_act;
    }else if (type == 2){
        nb_type_per_act_ptr = & glb_config.nb_type2_per_act;
    }else{
        // shouldn't happen
        glb_ds1.obj[obj_idx].desc_idx = -1;
        return;
    }

    while(id < 0){
        // object of precedent act
        act--;
        id += (* nb_type_per_act_ptr);
    }
    while(id >= (* nb_type_per_act_ptr)){
        // object of next act
        act++;
        id -= (* nb_type_per_act_ptr);
    }

    // search again with these new Act / Id
    for(i=0; i < glb_ds1edit.obj_desc_num; i++){
        if( (glb_ds1edit.obj_desc[i].act  == act)  && (glb_ds1edit.obj_desc[i].type == type) && (glb_ds1edit.obj_desc[i].id   == id) ){
            // end
            glb_ds1.obj[obj_idx].desc_idx = i;
            return;
        }
    }

    // not found
    glb_ds1.obj[obj_idx].desc_idx = -1;
}
