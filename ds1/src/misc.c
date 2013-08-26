//汇总 orientation {{{
//看来我要列个表写一下orientation和数据类型的关系之类的
//我都被这个搞烦了..整个程序到处都是这个~
// 
//
//////////////////////////////////////////////////////////////////////////////////////////////////
//            // type
//            if (bt_ptr->orientation == 0)
//            {
//               // floor
//               bt_ptr->zero_line = 0;
//
//               if (b_ptr->animated == 0x01){
//                  bt_ptr->type = BT_ANIMATED;
//               } else {
//                  bt_ptr->type = BT_STATIC;
//               }
//            }
//            else if (bt_ptr->orientation == 13)
//            {
//               // shadow
//               bt_ptr->type      = BT_SHADOW;
//               bt_ptr->zero_line = - b_ptr->size_y;
//            }
//            else
//            {
//               // walls
//               if ((bt_ptr->orientation == 10) || (bt_ptr->orientation == 11))
//               {
//                  // special
//                  bt_ptr->type      = BT_SPECIAL;
//                  bt_ptr->zero_line = - b_ptr->size_y;
//               }
//               else if (bt_ptr->orientation == 15)
//               {
//                  // roof
//                  bt_ptr->type      = BT_ROOF;
//                  bt_ptr->roof_y    = b_ptr->roof_y;
//                  bt_ptr->zero_line = 0;
//               }
//               else if (bt_ptr->orientation < 15)
//               {
//                  // wall up
//                  bt_ptr->type      = BT_WALL_UP;
//                  bt_ptr->zero_line = - b_ptr->size_y;
//               }
//               else if (bt_ptr->orientation > 15)
//               {
//                  // wall down
//                  bt_ptr->type      = BT_WALL_DOWN;
//                  bt_ptr->zero_line = 96;
//               }
//            }
///////////////////////////////////////////////////////////////////////////////////////////////
//      这一段来自misc.c
//      意思就是:
//      orientation == 0    :   floor
//      orientation == 13   :   shadow
//      其他都是wall,这其中:
//          orientation == 10 || 11 :   special
//          orientation == 15       :   roof
//              不属于以上各类, 但是:
//                  orientation < 15    :   wall up
//                  orientation > 15    :   wall down
//                      这里的orientation不同会指示不同的墙体对齐方式么(也就是墙体朝向)? 暂时不清楚
//                      用dt1tool查了一下, 貌似orientation:
//                          树都是14
//                          墙体的对齐我还没搞明白,貌似的确是个朝向问题
//
//
//
//      今天突然发现, 关于这些东西, 也许我该去看看tile_editer( 就是鼠标右键后的界面 )
//      那边貌似能够提供不少东西
//
//在画图的时候,有这么一句:
//
//      if (w_ptr[n].orientation == 10)
//         order_data[n].height = 255;


//        以下来自dt1 tools:
//        // ploty adjustment
//        if (block_ptr->orientation & 0xF0){
//            //截取高4位,从16开始 相当于 orientation>15
//            ploty = 125; // walls bellow floor,也就是walls down
//        }
//        else if ((block_ptr->direction == 3) && (block_ptr->orientation == 0))
//            ploty = (screen_y / 2); // only floor
//        else if ((block_ptr->direction == 5) && (block_ptr->orientation == 15))
//            ploty = screen_y - 15 - 80; // only roof
//        else
//            ploty = screen_y - 15; // walls to the up
//
//            }}}

#include <string.h>
#include "structs.h"
#include "error.h"
#include "ds1misc.h"
#include "txtread.h"
#include "dt1misc.h"
#include "mpq/mpqview.h"
#include "misc.h"


// ==========================================================================
// make a palette from the original one, with the current gamma correction
void misc_pal_d2_2_vga(int pal_idx)
{
    UBYTE r, g, b;
    int   i, ridx;

    if(glb_ds1edit.d2_pal[pal_idx] == NULL){
        for (i=0; i<256; i++){
            glb_ds1edit.vga_pal[pal_idx][i].r = i >> 2;
            glb_ds1edit.vga_pal[pal_idx][i].g = i >> 2;
            glb_ds1edit.vga_pal[pal_idx][i].b = i >> 2;
        }
        return;
    }

    for (i=0; i<256; i++){
        //运行的这里
        // why mutiplied by 4 here?
        ridx = 4 * i;

        r = glb_ds1edit.d2_pal[pal_idx][ridx];
        g = glb_ds1edit.d2_pal[pal_idx][ridx + 1];
        b = glb_ds1edit.d2_pal[pal_idx][ridx + 2];

        r = glb_ds1edit.gamma_table[glb_ds1edit.cur_gamma][r];
        g = glb_ds1edit.gamma_table[glb_ds1edit.cur_gamma][g];
        b = glb_ds1edit.gamma_table[glb_ds1edit.cur_gamma][b];

        //这大概是"当前用于渲染的pal"的意思
        // max r,g,b =63
        glb_ds1edit.vga_pal[pal_idx][i].r = r >> 2;
        glb_ds1edit.vga_pal[pal_idx][i].g = g >> 2;
        glb_ds1edit.vga_pal[pal_idx][i].b = b >> 2;
    }   
}


// ==========================================================================
// helper of the qsort function, same order as in the game
int misc_qsort_helper_block_table_1(const void * e1, const void * e2)
{
    BLOCK_TABLE_S * bt1, * bt2;
    long          n1=0, n2=0;

    bt1 = (BLOCK_TABLE_S *) e1;
    bt2 = (BLOCK_TABLE_S *) e2;
    //可以从这里看出优先性:
    // orientation > main_index > sub_index > dt1_idx > rarity > block_idx
    if (bt1->orientation != bt2->orientation)
    {
        n1 = bt1->orientation;
        n2 = bt2->orientation;
    }
    else if (bt1->main_idx != bt2->main_idx)
    {
        n1 = bt1->main_idx;
        n2 = bt2->main_idx;
    }
    else if (bt1->sub_idx != bt2->sub_idx)
    {
        n1 = bt1->sub_idx;
        n2 = bt2->sub_idx;
    }
    else if (bt1->dt1_idx != bt2->dt1_idx)
    {
        n1 = bt1->dt1_idx_for_ds1;
        n2 = bt2->dt1_idx_for_ds1;

        if (n1 == 0) n1 = DT1_MAX;
        if (n2 == 0) n2 = DT1_MAX;
    }
    else if (bt1->rarity != bt2->rarity)
    {
        n1 = bt1->rarity;
        n2 = bt2->rarity;
    }
    else if (bt1->block_idx != bt2->block_idx)
    {
        n1 = bt1->block_idx;
        n2 = bt2->block_idx;
    }
    return n1 - n2;
}


// ==========================================================================
// helper of the qsort function, order for the end-user
// 搞不清楚在干啥
int misc_qsort_helper_block_table_2(const void * e1, const void * e2)
{
    BLOCK_TABLE_S * bt1, * bt2;
    long          n1=0, n2=0;

    bt1 = (BLOCK_TABLE_S *) e1;
    bt2 = (BLOCK_TABLE_S *) e2;

    /*
       if (bt1->dt1_idx != bt2->dt1_idx)
       {
       n1 = bt1->dt1_idx;
       n2 = bt2->dt1_idx;
       }
       else
     */
    if (bt1->main_idx != bt2->main_idx)
    {
        n1 = bt1->main_idx;
        n2 = bt2->main_idx;
    }
    else if (bt1->orientation != bt2->orientation)
    {
        n1 = bt1->orientation;
        n2 = bt2->orientation;
    }
    else if (bt1->sub_idx != bt2->sub_idx)
    {
        n1 = bt1->sub_idx;
        n2 = bt2->sub_idx;
    }

    // ----------------
    else if (bt1->dt1_idx != bt2->dt1_idx)
    {
        n1 = bt1->dt1_idx;
        n2 = bt2->dt1_idx;
    }
    // ----------------

    else if (bt1->rarity != bt2->rarity)
    {
        n1 = bt1->rarity;
        n2 = bt2->rarity;
    }
    else if (bt1->block_idx != bt2->block_idx)
    {
        n1 = bt1->block_idx;
        n2 = bt2->block_idx;
    }
    return n1 - n2;
}


// ==========================================================================
// after the 1st qsort, find the tiles the game will use
// it handles conflicts, and rarity of the tiles


void misc_check_tiles_conflicts(int ds1_idx)
{
    BLOCK_TABLE_S * bt = glb_ds1.block_table;
    long          old_o, old_m, old_s, o, m, s;
    int           old_d, d, done = FALSE, done2, start_i = 0, i, end_i, r;
    int           last_block, first_block;
    int           bt_max = glb_ds1.bt_num, b;
    long          sum_rarity, max_rarity;


    while ( ! done)
    {
        old_o = bt[start_i].orientation;
        old_m = bt[start_i].main_idx;
        old_s = bt[start_i].sub_idx;
        old_d = bt[start_i].dt1_idx;

        bt[start_i].used_by_game = bt[start_i].used_by_editor = FALSE;

        // how many tiles have the same Orientation / Main index / Sub index ?
        done2       = FALSE;
        sum_rarity  = bt[start_i].rarity;
        last_block  = start_i;
        i           = start_i + 1;
        max_rarity  = -1;
        if (bt[start_i].rarity)
            first_block = start_i;
        else
            first_block = -1;
        while ( ! done2)
        {
            o = bt[i].orientation;
            m = bt[i].main_idx;
            s = bt[i].sub_idx;
            d = bt[i].dt1_idx;
            r = bt[i].rarity;

            //BLOCK_S.rarity(dt1tool源代码中的)大概和BLOCK_S.frame差不多

            if ((old_o != o) || (old_m != m) || (old_s != s) || (i >= bt_max))
                done2 = TRUE;
            else
            {
                bt[i].used_by_game = bt[i].used_by_editor = FALSE;
                if (d == old_d)
                    last_block = i; // last block of the first dt1
                if (r)
                {
                    if (first_block == -1)
                        first_block = i; // first block having a rarity
                    //    (whichever the dt1 is is)
                    if (r > max_rarity)
                    {
                        first_block = i;
                        max_rarity = r;
                    }
                }
                sum_rarity += r;
                i++;
            }
        }
        end_i = i - 1;

        // which one of these tiles will use the game & the editor ?
        if (sum_rarity == 0)
        {
            // only last block of the first dt1
            bt[last_block].used_by_game = bt[last_block].used_by_editor = TRUE;
        }
        else
        {
            // editor : only first block having the highest rarity
            bt[first_block].used_by_editor = TRUE;

            // game   : same, but also others which have a non-zero rarity
            for (b = start_i; b <= end_i; b++)
            {
                if (bt[b].rarity)
                    bt[b].used_by_game = TRUE;
            }
        }

        // next
        if (i >= bt_max)
            done = TRUE;
        else
            start_i = i;
    }
}


// ==========================================================================
// make the block table of 1 ds1
void misc_make_block_table(int ds1_idx)
{
    //typedef struct BLOCK_TABLE_S
    //{
    //   // key
    //   int       dt1_idx_for_ds1;
    //   long      main_index;
    //   long      orientation;
    //   long      sub_index;
    //
    //   // datas
    //   int       dt1_idx;
    //   long      rarity;
    //   int       block_idx;
    //   BLK_TYP_E type;
    //   int       zero_line;
    //   int       roof_y;
    //
    //   // conflicts managment
    //   char      conflict;
    //   char      used_by_game;   // True / False
    //   char      used_by_editor; // True / False
    //
    //   // animated tile
    //   long      curr_frame;
    //   int       updated;
    //} BLOCK_TABLE_S;




    //typedef struct BLOCK_S
    //{
    //   long       direction;
    //   WORD       roof_y;
    //   UBYTE      sound;
    //   UBYTE      animated;
    //   long       size_y;
    //   long       size_x;
    //   // long       zeros1;
    //   long       orientation;
    //   long       main_index;
    //   long       sub_index;
    //   long       rarity;
    //   /*
    //   UBYTE      unknown_a;
    //   UBYTE      unknown_b;
    //   UBYTE      unknown_c;
    //   UBYTE      unknown_d;
    //   */
    //   UBYTE      sub_tiles_flags[25];
    //   // int        zeros2[7];
    //   long       tiles_ptr;
    //   long       tiles_length;
    //   long       tiles_number;
    //   // int        zeros3[12];
    //} BLOCK_S;


    //block估计是tile那一类..就是dt里面的块状结构
    //因为在block内部有一个sound域, 是人物踩在上面的声音

    //block_table是什么了? 因为我发现block_s和block_table_s有很多相似的域
    //比如 orientation, main_index之类的
    //但是block_table_s没有sound域

    BLOCK_TABLE_S    *bt_ptr;
    BLOCK_S          *b_ptr;
    char             tmp_str[80];
    int              i, d, b, size;
    int              n = 0;

    //#define DT1_IN_DS1_MAX        33
    //估计是32+1个ds1edit.dt1
    for (i=0; i<DT1_IN_DS1_MAX; i++){
        d = glb_ds1.dt1_idx[i];
        if(d != -1){
            n += glb_dt1[d].block_num;
        }
        //n获取的是所有dt1文件的block总数
    }
    //为毛自增了一下
    n++;
    //这里看来每个block都有一个block_table_s啊
    size = sizeof(BLOCK_TABLE_S) * n;
    glb_ds1.block_table = (BLOCK_TABLE_S *) malloc(size);
    if(glb_ds1.block_table == NULL){
        FATAL_EXIT("make_block_table(%i), not enough mem for %i bytes\n", ds1_idx, size );
    }
    memset(glb_ds1.block_table, 0, size);
    //block_table_num的意思
    glb_ds1.bt_num = n;

    // fill it
    bt_ptr = glb_ds1.block_table;

    //第一个手动的填写?
    //貌似第一个也没用...难道这就是上面n++的原因?


    //我现在怀疑这段代码是作者从另一个作者那里拿来修修补补用的
    bt_ptr->orientation     = -1;
    bt_ptr->main_idx      = -1;
    bt_ptr->sub_idx       = -1;
    bt_ptr->rarity          = -1;
    bt_ptr->dt1_idx_for_ds1 = -1;
    bt_ptr->dt1_idx         = -1;
    bt_ptr->block_idx       = -1;
    bt_ptr->roof_y          = -1;
    bt_ptr->type            = -1;
    bt_ptr->zero_line       = -1;

    //第二个block_table_s开始
    bt_ptr++;


    //这里把bh_buffer的东西全部填写到bt_table中了
    //bh_buffer的初始化过程参见 void dt1_bh_update(int i)
    for(i=0; i<DT1_IN_DS1_MAX; i++){
        d = glb_ds1.dt1_idx[i];
        if(d != -1){
            b_ptr = glb_dt1[d].bh_buffer;
            for(b=0; b<glb_dt1[d].block_num; b++){
                // sort-key
                //nmb,这些都是用来排序的

                bt_ptr->orientation = b_ptr->orientation;
                bt_ptr->main_idx  = b_ptr->main_idx;
                bt_ptr->sub_idx   = b_ptr->sub_idx;
                bt_ptr->rarity      = b_ptr->rarity;

                // datas
                //这些都是更加handy的block_table数据结构了
                //但是所有信息都直接来自dt1文件解析时候的block_header

                // block headers : block_num structs of BLOCK_S
                // void       * bh_buffer;

                //看来block_s是直接描述.dt1文件结构的数据结构
                //而block_table_s是更加倾向于程序本身的数据结构, 里面加入了一些在程序中被分配的各种编号和type~

                bt_ptr->dt1_idx_for_ds1 = i;
                bt_ptr->dt1_idx         = d;
                bt_ptr->block_idx       = b;
                bt_ptr->roof_y          = 0;
                bt_ptr->zero_line       = 0;

                // type  这里的zero_line是个什么？？？？？？？？？？？？？？？？？？？貌似很重要的样子 和dt1_tool那个代码结合看看
                if (bt_ptr->orientation == 0){
                    // floor
                    bt_ptr->zero_line = 0;

                    if (b_ptr->animated == 0x01){
                        bt_ptr->type = BT_ANIMATED;
                    } else {
                        bt_ptr->type = BT_STATIC;
                    }
                }else if(bt_ptr->orientation == 13){
                    // shadow
                    bt_ptr->type      = BT_SHADOW;
                    bt_ptr->zero_line = - b_ptr->size_y;
                } else {
                    // walls
                    if ((bt_ptr->orientation == 10) || (bt_ptr->orientation == 11)) {
                        // special
                        bt_ptr->type      = BT_SPECIAL;
                        bt_ptr->zero_line = - b_ptr->size_y;
                    } else if (bt_ptr->orientation == 15) {
                        // roof
                        bt_ptr->type      = BT_ROOF;
                        bt_ptr->roof_y    = b_ptr->roof_y;
                        bt_ptr->zero_line = 0;
                    } else if (bt_ptr->orientation < 15) {
                        // wall up
                        bt_ptr->type      = BT_WALL_UP;
                        bt_ptr->zero_line = - b_ptr->size_y;
                    } else if (bt_ptr->orientation > 15) {
                        // wall down
                        bt_ptr->type      = BT_WALL_DOWN;
                        bt_ptr->zero_line = 96;
                    }
                }

                // next elements
                bt_ptr++;
                b_ptr++;
            }
        }
    }

    // display dt1 idx & file, to help user
    printf("\ndt1_idx file\n" "------- ---------------------------------------------------------------------\n");
    for (d=0; d<DT1_IN_DS1_MAX; d++) {
        if (glb_ds1.dt1_idx[d] != -1) {
            for (i=0; i<DT1_MAX; i++) {
                if (i == glb_ds1.dt1_idx[d]) {
                    if (glb_dt1[i].ds1_usage > 0) {
                        printf("%7i %s\n", i, glb_dt1[i].name);
                    }
                }
            }
        }
    }

    // sort it for conflict
    qsort(glb_ds1.block_table, n, sizeof(BLOCK_TABLE_S), misc_qsort_helper_block_table_1);

    // conflicts managment
    //没搞懂
    misc_check_tiles_conflicts(ds1_idx);

    // sort it for useability
    qsort(glb_ds1.block_table, n, sizeof(BLOCK_TABLE_S), misc_qsort_helper_block_table_2);

    // display it
    printf("\nsorted block_table of glb_ds1 (%i blocks) :\n", ds1_idx, n);
    printf("block orientation main_idx sub_idx frame dt1_idx blk_idx roof_y type line0\n");
    printf("----- ----------- -------- ------- ----- ------- ------- ------ ---- -----\n");
    for (b=0; b<glb_ds1.bt_num; b++) {
        printf("%5i %11li %8li %7li %5li %7li %7li %6i %4i %5i",
                b,
                glb_ds1.block_table[b].orientation,
                glb_ds1.block_table[b].main_idx,
                glb_ds1.block_table[b].sub_idx,
                glb_ds1.block_table[b].rarity,
                glb_ds1.block_table[b].dt1_idx,
                glb_ds1.block_table[b].block_idx,
                glb_ds1.block_table[b].roof_y,
                glb_ds1.block_table[b].type,
                glb_ds1.block_table[b].zero_line
              );
        glb_ds1.block_table[b].conflict = FALSE;
        if (glb_ds1.block_table[b].rarity == 0) {
            if (b>1) {
                if ( (glb_ds1.block_table[b].orientation == glb_ds1.block_table[b-1].orientation) &&
                        (glb_ds1.block_table[b].main_idx  == glb_ds1.block_table[b-1].main_idx)  &&
                        (glb_ds1.block_table[b].sub_idx   == glb_ds1.block_table[b-1].sub_idx)
                   ) {
                    //没有执行到这里的..表示没有冲突~
                    glb_ds1.block_table[b].conflict = TRUE;
                }
            }
            if (b < (glb_ds1.bt_num-1)) {
                if ( (glb_ds1.block_table[b].orientation == glb_ds1.block_table[b+1].orientation) &&
                        (glb_ds1.block_table[b].main_idx  == glb_ds1.block_table[b+1].main_idx)  &&
                        (glb_ds1.block_table[b].sub_idx   == glb_ds1.block_table[b+1].sub_idx)
                   ) {
                    glb_ds1.block_table[b].conflict = TRUE;
                }
            }
        }
        if (glb_ds1.block_table[b].conflict) {
            printf(" *");
        }
        printf("\n");
    }
}


// ==========================================================================
// read the gamma correction file
void misc_read_gamma(void)
{
    FILE    *in;
    int      gt;
    int      i;
    int      v;
    char     tmp[150];
    char     gamma_path[100];

    sprintf(gamma_path, "%s/%s", glb_ds1edit_data_dir, "gamma.dat");
    in = fopen(gamma_path, "rb");
    if(in == NULL){
        FATAL_EXIT("misc_read_gamma() : can't open %s", gamma_path);
    }

    for(gt=GC_060; gt<GC_MAX; gt++){
        for(i=0; i<256; i++){
            v = fgetc(in);
            glb_ds1edit.gamma_table[gt][i] = v;
        }
    }
    fclose(in);
}


// ==========================================================================
// update all the palettes with the current gamma correction
void misc_update_pal_with_gamma(void)
{
    int i;

    for (i=0; i<ACT_MAX; i++)
        misc_pal_d2_2_vga(i);
}



// ==========================================================================
// color map helper
void misc_make_cmaps_helper(const PALETTE pal, int x, int y, RGB * rgb)
{
    if (x == COL_SHADOW)
    {
        // full white
        rgb->r = 63;
        rgb->g = 63;
        rgb->b = 63;
    }
    else if (x == COL_MOUSE)
    {
        // half way between 2/3 of (2 * src) & 1/3 dst
        // result is brighter & have more white
        rgb->r = (pal[x].r + pal[y].r * 4) / 3;
        rgb->g = (pal[x].g + pal[y].g * 4) / 3;
        rgb->b = (pal[x].b + pal[y].b * 4) / 3;
        if (rgb->r > 63) rgb->r = 63;
        if (rgb->g > 63) rgb->g = 63;
        if (rgb->b > 63) rgb->b = 63;
    }
    else
    {
        // common transparency, half way between src & dst
        rgb->r = (pal[x].r + pal[y].r) >> 1;
        rgb->g = (pal[x].g + pal[y].g) >> 1;
        rgb->b = (pal[x].b + pal[y].b) >> 1;
    }
}


// ==========================================================================
// make (or read) all colormaps for all palettes
void misc_make_cmaps(void)
{
    CMAP_E cm;
    int    a, cmap_ok, i, c, start;
    char   tmp[100];
    FILE   * out, * in;

    printf("\n");
    for (cm=0; cm < CM_MAX; cm++)
    {
        for (a=0; a < ACT_MAX; a++)
        {
            fprintf(stderr, ".");
            sprintf(tmp, "%s/cmap%i_%i.bin", glb_ds1edit_data_dir, a, cm);
            cmap_ok = FALSE;
            if (file_exists(tmp, -1, NULL) && (glb_ds1edit.pal_loaded[a] == TRUE))
            {
                // load the colormap from disk, instead of making it
                in = fopen(tmp, "rb");
                if (in == NULL){
                    printf("can't read %s\n", tmp);
                }else{
                    printf("loading %s\n", tmp);
                    fread(& glb_ds1edit.cmap[cm][a], sizeof(COLOR_MAP), 1, in);
                    fclose(in);
                    cmap_ok = TRUE;
                }
            }

            if (cmap_ok == FALSE)
            {
                // not found or can't be open, so create it

                if (cm == CM_SELECT)
                {
                    // color table
                    create_color_table(& glb_ds1edit.cmap[cm][a],
                            glb_ds1edit.vga_pal[a],
                            misc_make_cmaps_helper,
                            NULL);

                }
                else if (cm == CM_TRANS)
                {
                    create_trans_table(& glb_ds1edit.cmap[cm][a],
                            glb_ds1edit.vga_pal[a],
                            128, 128, 128,
                            NULL);
                }
                else if (cm == CM_SHADOW)
                {
                    for (c=0; c < 256; c++)
                    {
                        start = 1024 + (256 * (c/8));
                        for (i=0; i<256; i++)
                        {
                            glb_ds1edit.cmap[cm][a].data[c][i] =
                                glb_ds1edit.d2_pal[a][start + i];
                        }
                    }
                }

                out = fopen(tmp, "wb");
                if (out == NULL)
                    printf("can't write %s\n", tmp);
                else
                {
                    printf("saving %s\n", tmp);
                    fwrite(& glb_ds1edit.cmap[cm][a], sizeof(COLOR_MAP), 1, out);
                    fclose(out);
                }
            }
        }
    }
}


// ==========================================================================
// try to load 1 palette from the ./res/data/ directory
int misc_load_pal_from_disk(int pal_idx)
{
    char         tmp[100];
    FILE        *in;
    long         size;

    sprintf(tmp, "%s/pal%i.bin", glb_ds1edit_data_dir, pal_idx);
    if(file_exists(tmp, -1, NULL)){
        // load the palette from disk, instead of mpq
        in = fopen(tmp, "rb");
        if (in == NULL){
            DEBUG_MESSAGE("could not read %s\n", tmp);
        }else{
            // 求取文件大小的常用方法
            fseek(in, 0, SEEK_END);
            size = ftell(in);
            fseek(in, 0, SEEK_SET);

            // malloc
            glb_ds1edit.d2_pal[pal_idx] = (UBYTE *) malloc(size);
            if (glb_ds1edit.d2_pal[pal_idx] == NULL){
                FATAL_EXIT("misc_load_pal_from_disk() : not enough mem (%li bytes) for palette %i", size, pal_idx);
            }
            // filling it
            DEBUG_MESSAGE("loading %s\n", tmp);
            fread(glb_ds1edit.d2_pal[pal_idx], size, 1, in);
            fclose(in);
            glb_ds1edit.pal_size[pal_idx] = size;
            return TRUE;
        }
    }
    return FALSE;
}


// ==========================================================================
// save the palette in the data\ directory (for quicker access next time)
void misc_save_pal_on_disk(int pal_idx, UBYTE * d2_pal_ptr)
{
    char tmp[100];
    FILE * out;

    sprintf(tmp, "%spal%i.bin", glb_ds1edit_data_dir, pal_idx);
    out = fopen(tmp, "wb");
    if (out == NULL)
        printf("can't write %s\n", tmp);
    else
    {
        printf("saving %s\n", tmp);
        fwrite(d2_pal_ptr, glb_ds1edit.pal_size[pal_idx], 1, out);
        fclose(out);
    }
}


// ==========================================================================
// own version of fgets()
// any special char means End Of String now, but NOT spaces !
int misc_my_fgets(char * dst, int max, FILE * in)
{
    int      i = 0;
    int      c;

    if (max <= 0){
        return 0;
    }

    for(;;) {
        c = fgetc(in);
        if (c == EOF) {
            dst[i] = 0;
            if (i){
                return 1;
            }else{
                return 0;
            }
        }
        if ((i >= (max - 1)) || (c < 32)){
            dst[i] = 0;
            while ((c < 32) && (c != EOF)){
                c  = fgetc(in);
            }
            if (c != EOF){
                ungetc(c, in);
            }
            return 1;
        }
        dst[i] = c;
        i++;
    }
}


// ==========================================================================
// return start of ds1 file to open (multiple ds1 to open mode)
char * misc_search_name(char * tmp)
{
    int max, i=0;

    max = strlen(tmp);
    if (max < 1)
        return NULL;

    // skip spaces
    while ((i < max) && (tmp[i] <= 32))
        i++;
    if (i >= max)
        return NULL;

    // skip 1st number
    while ((i < max) && (tmp[i] > 32))
        i++;
    if (i >= max)
        return NULL;

    // skip spaces
    while ((i < max) && (tmp[i] <= 32))
        i++;
    if (i >= max)
        return NULL;

    // skip 2nd number
    while ((i < max) && (tmp[i] > 32))
        i++;
    if (i >= max)
        return NULL;

    // skip spaces
    while ((i < max) && (tmp[i] <= 32))
        i++;
    if (i >= max)
        return NULL;

    // found name
    return & tmp[i];
}


// ==========================================================================
// open 1 ds1, & all the dt1 it needs
void misc_open_1_ds1(int ds1_idx, char * name, int type, int def, int new_width, int new_height)
{
    // ds1
    
    DEBUG_MESSAGE("misc_open_1_ds1(\"%s\"):in\n", name );

    ds1_read(name, ds1_idx, new_width, new_height);

    // lvl*.txt (and loading dt1 from mpq)
    DEBUG_MESSAGE("searching Dt1Mask...\n");
    // 没怎么看~
    // 貌似这个函数最重要的就是关于dt1mask的问题和def的问题
    // dt1mask好像我在哪里看过关于它的解释
    read_lvlprest_txt(ds1_idx, def);

    DEBUG_MESSAGE("loading dt1 from mpq...\n");
    // read_lvltypes_txt()是正真读入dt1文件的地方哦
    // 终于开始加载图像数据了
    // 我调试的为tri_...ds1这个文件
    // 这个程序里面加载了32+1个dt1文件
    // 但是对于不同的ds1文件，如何确定加载哪些dt1文件？
    //
    // 看样子是根据tpye来的type的值为：glb_ds1edit.cmd_line.lvltype_id,
    // 是从cmd_line来的？ 是的 run tri_**.ds1 11 300 这里的11就是
    glb_ds1.txt_act = read_lvltypes_txt(ds1_idx, type);
    DEBUG_MESSAGE("txt_act = %i\n", glb_ds1.txt_act);

    // make the block_table of this ds1
    misc_make_block_table(ds1_idx);

    // prop to index in block table
    ds1_make_prop_2_block(ds1_idx);
    DEBUG_MESSAGE("misc_open_1_ds1(\"%s\"):out\n", name );
}


// ==========================================================================
// open 1 ds1, and all the the dt1 it'll use
void misc_open_1_ds1_force_dt1(int ds1_idx)
{
    int  d;
    char * ds1_name = glb_ds1edit.cmd_line.ds1_filename;
    int  new_width  = glb_ds1edit.cmd_line.resize_width;
    int  new_height = glb_ds1edit.cmd_line.resize_height;
    char ds1edt_file[] = "ds1edit.dt1";
    char tmp[150];


    // ds1
    printf("\nreading ds1 : %s\n", ds1_name);
    fflush(stdout);
    fprintf(stderr, "reading %s...", ds1_name);
    ds1_read(ds1_name, ds1_idx, new_width, new_height);
    fprintf(stderr, "done\n");

    // always open ds1edit.dt1
    sprintf(tmp, "%s%s", glb_ds1edit_data_dir, ds1edt_file);
    printf("\nwant to read a dt1 : %s\n", tmp);
    glb_ds1.dt1_idx[0] = dt1_add_special(tmp);

    // opening other dt1
    for (d=0; d < DT1_IN_DS1_MAX; d++)
        glb_ds1.dt1_idx[d + 1] = -1;

    printf("\nloading dt1 from disk...\n");

    for (d=0; d < glb_ds1edit.cmd_line.dt1_list_num; d++)
    {
        printf("\nwant to read a dt1 : %s\n", glb_ds1edit.cmd_line.dt1_list_filename[d]);
        glb_ds1.dt1_idx[d + 1] = dt1_add_special(glb_ds1edit.cmd_line.dt1_list_filename[d]);
    }

    glb_ds1.txt_act = glb_ds1.act;
    printf("\ndone\n");

    // make the block_table of this ds1
    misc_make_block_table(ds1_idx);

    // prop to index in block table
    ds1_make_prop_2_block(ds1_idx);
}


// ==========================================================================
// search the 2nd block table index of a upper / left corner tile
// (2nd corner tile = orientation 4)
int misc_seach_block_or4(int ds1_idx, BLOCK_TABLE_S * bt_ptr, int b,
        int m, int s)
{
    for(;;) {
        if (b >= glb_ds1.bt_num){
            return -1;
        } else {
            if ( (bt_ptr[b].orientation == 4) && (bt_ptr[b].main_idx  == m) && (bt_ptr[b].sub_idx   == s)) {
                return b;
            }
        }
        b++;
    }
}


// ==========================================================================
// load a file from mpq
//    first try in the mod directory, if not found in the 1st mpq it can
int misc_load_mpq_file(char * filename, char ** buffer, long * buf_len, int output)
{
    int  i, entry, max;


    // convert slash ('/') to backslash ('\\')
    // seems the mpq library expect backslash and not slash as directory separator
    strcpy(glb_ds1edit.strtmp, filename);
    max = strlen(glb_ds1edit.strtmp);
    for (i=0; i < max; i++) {
        if (glb_ds1edit.strtmp[i] == '/')
            glb_ds1edit.strtmp[i] = '\\';
    }

    // 1st try in mod directory
    //仅仅是从.txt文件中拷贝到内存中
    //这里是拷贝到了buffer中了
    for (i=0; i<MAX_MOD_DIR; i++) {
        entry = mod_load_in_mem( glb_config.mod_dir[i], glb_ds1edit.strtmp, buffer, buf_len );
        if (entry != -1) {
            if (output) {
                DEBUG_MESSAGE("found in %s\n", glb_config.mod_dir[i]);
            }
            return entry;
        }
    }

    // 2nd try, in a mpq
    for (i=0; i<MAX_MPQ_FILE; i++) {
        if (glb_mpq_struct[i].is_open != FALSE) {
            glb_mpq = & glb_mpq_struct[i];
            entry = mpq_batch_load_in_mem( glb_ds1edit.strtmp, buffer, buf_len, output);
            if (entry != -1) {
                if (output) {
                    DEBUG_MESSAGE("found in %s\n", glb_config.mpq_file[i]);
                }
                return entry;
            }
        }
    }

    // not found
    return -1;
}


// ==========================================================================
// return the column number of a txt file, given its name
int misc_get_txt_column_num(RQ_ENUM txt_idx, char * col_name)
{
    int      i = 0;
    char    *desc;
    char     tmp_str[256];

    char     txt_list[RQ_MAX][80] = {
        {"data/global/excel/lvltypes.txt"},
        {"data/global/excel/lvlprest.txt"},
        {"data/obj.txt"},
        {"data/global/excel/objects.txt"},
    };

    if (txt_idx >= RQ_MAX){
        return -1;
    }

    for(;;) {
        // 这个东西是以前手动填入的
        desc = glb_txt_req_ptr[txt_idx][i];
        if (desc == NULL){
            FATAL_EXIT("misc_get_txt_column_num() :\n   can't find <%s> in txt file <%i> <%s>\n", col_name, txt_idx, txt_list[txt_idx] );
        }else{
            if (stricmp(col_name, desc) == 0){
                return i;
            }else{
                i++;
            }
        }
    }
}


// ==========================================================================
int misc_is_numerical(char * str)
{
    int i, max = strlen(str);

    for (i=0; i < max; i++)
    {
        if ( (str[i] == '-') ||
                (str[i] == '+') ||
                (str[i] == '.') ||
                ((str[i] >= '0') && (str[i] <= '9'))
           )
        {
            // ok
        }
        else
            return FALSE;
    }
    return TRUE;
}


// ==========================================================================
// correct the pl2, for making allegro draw_sprite work as expected
void misc_pl2_correct(int i)
{
    UBYTE     * bptr;
    int       c;
    COLOR_MAP * cmap;

    // about COLOR_MAP, PALETTE {{{
    // max r = 63 but not 255 !!!
    // max g = 63 but not 255 !!!
    // max b = 63 but not 255 !!!
    //

    //  PALETTE defination:
    //  typedef PALETTE     RGB[PAL_SIZE] 
    //  #define PAL_SIZE    256
    //  typedef struct RGB {
    //     unsigned char r, g, b;
    //  } RGB;
    //
    //  example:
    //  RGB black = { 0,  0,  0  };
    //  RGB white = { 63, 63, 63 };
    //  RGB green = { 0,  63, 0  };
    //  RGB grey  = { 32, 32, 32 };
    //  
    // typedef struct COLOR_MAP {
    //    unsigned char data[PAL_SIZE][PAL_SIZE];
    // } COLOR_MAP;
    //
    //
    // colormaps index for COF effect
    // #define COF_25TRANS       53        // 53
    // #define COF_50TRANS      309        // 256
    // #define COF_75TRANS      565        // 256
    // #define COF_ALPHA        821        // 256
    // #define COF_LUMINANCE   1077        // 256
    // #define COF_ALPHABRIGHT 1461        // 384
    // }}}

    bptr = glb_ds1edit.d2_pal[i];
    for (c=0; c < 256; c++) {

        // just fill it 
        // not load from a file
        // we can see that all data[0][c]=c
        //
        cmap = (COLOR_MAP *) & bptr[COF_75TRANS * 256];
        cmap->data[0][c] = c;

        cmap = (COLOR_MAP *) & bptr[COF_50TRANS * 256];
        cmap->data[0][c] = c;

        cmap = (COLOR_MAP *) & bptr[COF_25TRANS * 256];
        cmap->data[0][c] = c;

        cmap = (COLOR_MAP *) & bptr[COF_ALPHA * 256];
        cmap->data[0][c] = c;

        cmap = (COLOR_MAP *) & bptr[COF_LUMINANCE * 256];
        cmap->data[0][c] = c;

        cmap = (COLOR_MAP *) & bptr[COF_ALPHABRIGHT * 256];
        cmap->data[0][c] = c;
    }
}


// ==========================================================================
// parse a command line
// return -1 if error, 0 otherwise
int misc_cmd_line_parse(int argc, char ** argv)
{
    int  i                  = 0;
    int  force_dt1_present  = FALSE;
    char * ext              = NULL;
    int  n                  = 0;
    int  lvltype_id_found   = 0;
    int  lvlprest_def_found = 0;


    // is -force_dt1 present ?
    for (i=1; i < argc; i++) {
        if (stricmp(argv[i], "-force_dt1") == 0) {
            force_dt1_present = TRUE; // no LvlType.txt ID and no LvlPrest.txt DEF expected in the arguments
        }
    }

    // scan all parameters 1 by 1
    // 从argv[1]开始一个一个扫描
    for (i=1; i < argc; i++) {
        if (i == 1) {
            // .ini or .ds1 ?
            ext = get_extension(argv[i]);
            if (stricmp(ext, "ds1") == 0) {
                glb_ds1edit.cmd_line.ds1_filename = argv[i];
            } else {
                WARNING_MESSAGE("the first argument must be a .ini or a .ds1 file\n");
                WARNING_MESSAGE("current extension = \"%s\")\n", ext);
                return -1;
            }
        } else if (stricmp(argv[i], "-force_dt1") == 0) {
            // -force_dt1, folowed by 1 to 32 .dt1 files
            i++;
            for (n=0; ((i + n) < argc) && (stricmp(get_extension(argv[i + n]), "dt1") == 0); n++) {
            }
            if ((n < 1) || (n > DT1_IN_DS1_MAX)) {
                printf("misc_cmd_line_parse(), error : there must be between 1 and %i .DT1 files folowing the -force_dt1 parameter\n", DT1_IN_DS1_MAX);
                return -1;
            }
            glb_ds1edit.cmd_line.dt1_list_num = n;
            for (n=0; n < glb_ds1edit.cmd_line.dt1_list_num; n++)
                glb_ds1edit.cmd_line.dt1_list_filename[n] = argv[i + n];
            i += (n - 1);
        } else if (stricmp(argv[i], "-force_pal") == 0) {
            // -force_pal, folowed by an act value
            i++;
            if (i >= argc)
            {
                printf("misc_cmd_line_parse(), error : the parameter -force_pal should be folowed by an act value\n");
                return -1;
            }
            if (misc_is_numerical(argv[i]))
            {
                n = atoi(argv[i]);
                if ((n < 1) || (n > 5))
                {
                    printf("misc_cmd_line_parse(), error : the act value folowing the -force_pal parameter must be between 1 and 5\n");
                    return -1;
                }
                glb_ds1edit.cmd_line.force_pal_num = n;
            }
            else
            {
                printf("misc_cmd_line_parse(), error : the act value folowing the -force_pal parameter is not a numerical value\n");
                return -1;
            }
        }
        else if (stricmp(argv[i], "-no_check_act") == 0)
        {
            // -no_check_act is a flag
            glb_ds1edit.cmd_line.no_check_act = TRUE;
        }
        else if ((force_dt1_present == FALSE) && (glb_ds1edit.cmd_line.ds1_filename != NULL))
        {
            if ( ! lvltype_id_found)
            {
                // LvlType.txt ID
                lvltype_id_found = 1;
                if (misc_is_numerical(argv[i]))
                    glb_ds1edit.cmd_line.lvltype_id = atoi(argv[i]);
                else
                {
                    printf("misc_cmd_line_parse(), error : the LvlType.txt ID value is not a numerical value\n");
                    return -1;
                }
            }
            else if ( ! lvlprest_def_found)
            {
                // LvlPrest.txt DEF
                lvlprest_def_found = 1;
                if (misc_is_numerical(argv[i]))
                    glb_ds1edit.cmd_line.lvlprest_def = atoi(argv[i]);
                else
                {
                    printf("misc_cmd_line_parse(), error : the LvlPrest.txt DEF value is not a numerical value\n");
                    return -1;
                }
            }
            else
            {
                printf("misc_cmd_line_parse(), error : unexpected \"%s\" parameter\n", argv[i]);
                return -1;
            }
        }
    }

    return 0;
}


// ==========================================================================
void misc_draw_screen(int mx, int my)
{
    BITMAP * mouse_sprite = glb_ds1edit.mouse_cursor[MOD_T];
    BITMAP * video_bmp    = glb_ds1edit.video_page;

    draw_sprite(glb_ds1edit.screen_buff, mouse_sprite, mx, my);
    blit( glb_ds1edit.screen_buff, video_bmp, 0, 0, 0, 0, glb_config.screen.width, glb_config.screen.height);
    show_video_bitmap(video_bmp);
}

// ==================================================================================
// for a given ds1, increase its objects structures by (at least) nb_objects elements
// return 0 if ok, -1 if error
// 没详细看, 估计也就添加一点内存之类的
int misc_increase_ds1_objects_max(int ds1_idx, long nb_objects)
{
    long old_max;
    long new_max;
    void * new_buffer;
    long old_size;
    long new_size;
    long nb_gran;


    if (nb_objects < 0)
        return 0;

    nb_gran = 1 + (nb_objects / OBJ_MAX_GRANULARITY);
    old_max = glb_ds1.current_obj_max;
    new_max = old_max + (nb_gran * OBJ_MAX_GRANULARITY);

    // drawing_order
    old_size = old_max * sizeof(int);
    new_size = new_max * sizeof(int);
    new_buffer = (int *) malloc(new_size);
    if (new_buffer == NULL)
        return -1;
    memset(new_buffer, 0x00, new_size);
    memcpy(new_buffer, glb_ds1.drawing_order, old_size);
    free(glb_ds1.drawing_order);
    glb_ds1.drawing_order = (int *) new_buffer;

    // obj
    old_size = old_max * sizeof(OBJ_S);
    new_size = new_max * sizeof(OBJ_S);
    new_buffer = (OBJ_S *) malloc(new_size);
    if (new_buffer == NULL)
        return -1;

    memset(new_buffer, 0x00, new_size);
    memcpy(new_buffer, glb_ds1.obj, old_size);
    free(glb_ds1.obj);
    glb_ds1.obj = (OBJ_S *) new_buffer;

    // obj_undo
    old_size = old_max * sizeof(OBJ_S);
    new_size = new_max * sizeof(OBJ_S);
    new_buffer = (OBJ_S *) malloc(new_size);
    if (new_buffer == NULL)
        return -1;
    memset(new_buffer, 0x00, new_size);
    memcpy(new_buffer, glb_ds1.obj_undo, old_size);
    free(glb_ds1.obj_undo);
    glb_ds1.obj_undo = (OBJ_S *) new_buffer;

    glb_ds1.current_obj_max = new_max;
    return 0;
}
