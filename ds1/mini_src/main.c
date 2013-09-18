#include "structs.h"
#include "error.h"
#include "mpqview.h"
#include "dt1misc.h"
#include "ds1misc.h"
#include "anim.h"
#include "txtread.h"
#include "misc.h"
#include "inicreat.h"
#include "iniread.h"
#include "animdata.h"
#include "interfac.h"


// typedef struct WRKSPC_DATAS_S{
//    char * id;
// } WRKSPC_DATAS_S;

// workspace datas saved in .ds1

int glb_map_test;

WRKSPC_DATAS_S glb_wrkspc_datas[WRKSPC_MAX] = {
    {("DS1EDIT_WRKSPC_TILE_X")},
    {("DS1EDIT_WRKSPC_TILE_Y")},
    {("DS1EDIT_WRKSPC_ZOOM")},
    {("DS1EDIT_VERSION")},
    {("DS1EDIT_SAVE_COUNT")}
};

// gamma correction string table
GAMMA_S glb_gamma_str[GC_MAX] = {
    {"0.60", GC_060}, {"0.62", GC_062}, {"0.64", GC_064},
    {"0.66", GC_066}, {"0.68", GC_068}, {"0.70", GC_070},
    {"0.72", GC_072}, {"0.74", GC_074}, {"0.76", GC_076},
    {"0.78", GC_078}, {"0.80", GC_080}, {"0.82", GC_082},
    {"0.84", GC_084}, {"0.86", GC_086}, {"0.88", GC_088},
    {"0.90", GC_090}, {"0.92", GC_092}, {"0.94", GC_094},
    {"0.96", GC_096}, {"0.98", GC_098}, {"1.00", GC_100},
    {"1.10", GC_110}, {"1.20", GC_120}, {"1.30", GC_130},
    {"1.40", GC_140}, {"1.50", GC_150}, {"1.60", GC_160},
    {"1.70", GC_170}, {"1.80", GC_180}, {"1.90", GC_190},
    {"2.00", GC_200}, {"2.10", GC_210}, {"2.20", GC_220},
    {"2.30", GC_230}, {"2.40", GC_240}, {"2.50", GC_250},
    {"2.60", GC_260}, {"2.70", GC_270}, {"2.80", GC_280},
    {"2.90", GC_290}, {"3.00", GC_300}
};

char * txt_def_lvltype_req[] = {
    ("Id"),      ("Act"),
    ("File 1"),  ("File 2"),  ("File 3"),  ("File 4"),  ("File 5"),
    ("File 6"),  ("File 7"),  ("File 8"),  ("File 9"),  ("File 10"),
    ("File 11"), ("File 12"), ("File 13"), ("File 14"), ("File 15"),
    ("File 16"), ("File 17"), ("File 18"), ("File 19"), ("File 20"),
    ("File 21"), ("File 22"), ("File 23"), ("File 24"), ("File 25"),
    ("File 26"), ("File 27"), ("File 28"), ("File 29"), ("File 30"),
    ("File 31"), ("File 32"),
    NULL // DO NOT REMOVE !
};

char * txt_def_lvlprest_req[] = {
    ("Def"),   ("Dt1Mask"),
    ("File1"), ("File2"), ("File3"), ("File4"), ("File5"), ("File6"),
    NULL // DO NOT REMOVE !
};

char * txt_def_obj_req[] = {
    // number
    ("Act"),
    ("Type"),
    ("Id"),
    ("Direction"),
    ("Index"),
    ("Objects.txt_ID"),
    ("Monstats.txt_ID"),

    // text
    ("Base"),
    ("Token"),
    ("Mode"),
    ("Class"),
    ("HD"), ("TR"), ("LG"), ("RA"), ("LA"), ("RH"), ("LH"), ("SH"),
    ("S1"), ("S2"), ("S3"), ("S4"), ("S5"), ("S6"), ("S7"), ("S8"),
    ("Colormap"),
    ("Description"),

    NULL // DO NOT REMOVE !
};

char * txt_def_objects_req[] = {
    // number
    ("Id"),
    ("SizeX"),
    ("SizeY"),
    ("FrameCnt0"),
    ("FrameCnt1"),
    ("FrameCnt2"),
    ("FrameCnt3"),
    ("FrameCnt4"),
    ("FrameCnt5"),
    ("FrameCnt6"),
    ("FrameCnt7"),
    ("FrameDelta0"),
    ("FrameDelta1"),
    ("FrameDelta2"),
    ("FrameDelta3"),
    ("FrameDelta4"),
    ("FrameDelta5"),
    ("FrameDelta6"),
    ("FrameDelta7"),
    ("CycleAnim0"),
    ("CycleAnim1"),
    ("CycleAnim2"),
    ("CycleAnim3"),
    ("CycleAnim4"),
    ("CycleAnim5"),
    ("CycleAnim6"),
    ("CycleAnim7"),
    ("Lit0"),
    ("Lit1"),
    ("Lit2"),
    ("Lit3"),
    ("Lit4"),
    ("Lit5"),
    ("Lit6"),
    ("Lit7"),
    ("BlocksLight0"),
    ("BlocksLight1"),
    ("BlocksLight2"),
    ("BlocksLight3"),
    ("BlocksLight4"),
    ("BlocksLight5"),
    ("BlocksLight6"),
    ("BlocksLight7"),
    ("Start0"),
    ("Start1"),
    ("Start2"),
    ("Start3"),
    ("Start4"),
    ("Start5"),
    ("Start6"),
    ("Start7"),
    ("BlocksVis"),
    ("Trans"),
    ("OrderFlag0"),
    ("OrderFlag1"),
    ("OrderFlag2"),
    ("OrderFlag3"),
    ("OrderFlag4"),
    ("OrderFlag5"),
    ("OrderFlag6"),
    ("OrderFlag7"),
    ("Mode0"),
    ("Mode1"),
    ("Mode2"),
    ("Mode3"),
    ("Mode4"),
    ("Mode5"),
    ("Mode6"),
    ("Mode7"),
    ("Yoffset"),
    ("Xoffset"),
    ("Draw"),
    ("Red"),
    ("Green"),
    ("Blue"),
    ("TotalPieces"),
    ("SubClass"),
    ("Xspace"),
    ("YSpace"),
    ("OperateRange"),
    ("Act"),
    ("Sync"),
    ("Flicker"),
    ("Overlay"),
    ("CollisionSubst"),
    ("Left"),
    ("Top"),
    ("Width"),
    ("Height"),
    ("BlockMissile"),
    ("DrawUnder"),
    ("HD"), ("TR"), ("LG"), ("RA"), ("LA"), ("RH"), ("LH"), ("SH"),
    ("S1"), ("S2"), ("S3"), ("S4"), ("S5"), ("S6"), ("S7"), ("S8"),

    // text
    ("Token"),

    NULL // DO NOT REMOVE !
};

char ** glb_txt_req_ptr[RQ_MAX] = {NULL, NULL, NULL, NULL};

CONFIG_S      glb_config;                     // global configuration datas
GLB_DS1EDIT_S glb_ds1edit;                    // global datas of the editor
GLB_MPQ_S     glb_mpq_struct [MAX_MPQ_FILE];  // global data of 1 mpq

DS1_S         glb_ds1;

DT1_S         * glb_dt1                = NULL; // dt1 datas 貌似这里是32个
char          glb_tiles_path        [] = "data/global/tiles/";
char          glb_ds1edit_data_dir  [] = "./res/data";
char          glb_ds1edit_tmp_dir   [] = "tmp"; // 前后光溜溜原则

// debug files
char          * glb_path_lvltypes_mem = "debug/editor.lvltypes.memory.bin";
char          * glb_path_lvltypes_def = "debug/d2.lvltypes.headers.txt";
char          * glb_path_lvlprest_mem = "debug/editor.lvlprest.memory.bin";
char          * glb_path_lvlprest_def = "debug/d2.lvlprest.headers.txt";
char          * glb_path_obj_mem      = "debug/editor.obj.memory.bin";
char          * glb_path_obj_def      = "debug/editor.obj.headers.txt";
char          * glb_path_objects_mem  = "debug/editor.objects.memory.bin";
char          * glb_path_objects_def  = "debug/d2.objects.headers.txt";


// ==========================================================================
// near the start of the prog
void ds1edit_init(void)
{
    FILE        *out;
    int          i;
    int          o;
    char         tmp[80];

    static struct{
        char      name[40];
        MODE_E    idx;
    }cursor[MOD_MAX] = {
        {"res/pcx/cursor_t.pcx", MOD_T},       // tiles
        {"res/pcx/cursor_o.pcx", MOD_O},       // objects
        {"res/pcx/cursor_p.pcx", MOD_P},       // paths
        {"res/pcx/cursor_l.pcx", MOD_L}        // lights
    };

    static int   dir4[4]   = { 0,  1,  2,  3};
    static int   dir8[8]   = { 4,  0,  5,  1,  6,  2,  7,  3 };
    static int   dir16[16] = { 4,  8,  0,  9,  5, 10,  1, 11,
        6, 12,  2, 13,  7, 14,  3, 15 };
    static int   dir32[32] = { 4, 16,  8, 17,  0, 18,  9, 19,
        5, 20, 10, 21,  1, 22, 11, 23,
        6, 24, 12, 25,  2, 26, 13, 27,
        7, 28, 14, 29,  3, 30, 15, 31 };
    static int   obj_sub_tile[5][5] = {
        { 0,  2,  5,  9, 14},
        { 1,  4,  8, 13, 18},
        { 3,  7, 12, 17, 21},
        { 6, 11, 16, 20, 23},
        {10, 15, 19, 22, 24} };

    DEBUG_MESSAGE("ds1edit_init()\n");

    memset( & glb_config,  0, sizeof(glb_config) );
    memset( & glb_ds1edit, 0, sizeof(glb_ds1edit) );

    // allocate mem for DT1 & DS1
    i = sizeof(DS1_S) * DS1_MAX;
    DEBUG_MESSAGE("allocate %i bytes for glb_ds1\n", i, DS1_MAX);
    memset(&glb_ds1, 0, sizeof(glb_ds1));

    i = sizeof(DT1_S) * DT1_MAX;
    DEBUG_MESSAGE("allocate %i bytes for glb_dt1[%i]\n", i, DT1_MAX);
    glb_dt1 = (DT1_S *) malloc(i);
    if(!glb_dt1){
        FATAL_EXIT("could not allocate %i bytes for the glb_dt1[%i] table.\n", i, DT1_MAX );
    }
    memset(glb_dt1, 0, i);

    for (i=0; i<MAX_MPQ_FILE; i++) {
        memset( & glb_mpq_struct[i], 0, sizeof(GLB_MPQ_S));
        glb_mpq_struct[i].is_open = FALSE;
    }

    // mouse cursors
    for (i=0; i<MOD_MAX; i++) {
        glb_ds1edit.mouse_cursor[i] = load_pcx(cursor[i].name, glb_ds1edit.dummy_pal);
        if(glb_ds1edit.mouse_cursor[i] == NULL){
            FATAL_EXIT("could not open the file \"%s\".\n", cursor[i].name);
        }
    }

    //char * txt_def_lvltype_req[] =
    //{
    //   ("Id"),      ("Act"),
    //   ("File 1"),  ("File 2"),  ("File 3"),  ("File 4"),  ("File 5"),
    //   ("File 6"),  ("File 7"),  ("File 8"),  ("File 9"),  ("File 10"),
    //   ("File 11"), ("File 12"), ("File 13"), ("File 14"), ("File 15"),
    //   ("File 16"), ("File 17"), ("File 18"), ("File 19"), ("File 20"),
    //   ("File 21"), ("File 22"), ("File 23"), ("File 24"), ("File 25"),
    //   ("File 26"), ("File 27"), ("File 28"), ("File 29"), ("File 30"),
    //   ("File 31"), ("File 32"),
    //   NULL // DO NOT REMOVE !
    //};
    // 为"必须"的列
    // 还有一些列为非必须的..只是为了增加可读性的
    // txt
    glb_txt_req_ptr[RQ_LVLTYPE]  = txt_def_lvltype_req;
    glb_txt_req_ptr[RQ_LVLPREST] = txt_def_lvlprest_req;
    glb_txt_req_ptr[RQ_OBJ]      = txt_def_obj_req;
    glb_txt_req_ptr[RQ_OBJECTS]  = txt_def_objects_req;

    // debug files
    remove(glb_path_lvltypes_mem);
    remove(glb_path_lvltypes_def);
    remove(glb_path_lvlprest_mem);
    remove(glb_path_lvlprest_def);
    remove(glb_path_obj_mem);
    remove(glb_path_obj_def);
    remove(glb_path_objects_mem);
    remove(glb_path_objects_def);

    // tables
    glb_ds1edit.new_dir1[0] = 0;
    for (i=0; i < 4; i++){
        glb_ds1edit.new_dir4[i] = dir4[i];
    }
    for (i=0; i < 8; i++){
        glb_ds1edit.new_dir8[i] = dir8[i];
    }
    for (i=0; i < 16; i++){
        glb_ds1edit.new_dir16[i] = dir16[i];
    }
    for (i=0; i < 32; i++){
        glb_ds1edit.new_dir32[i] = dir32[i];
    }


    //   for re-ordering sub-tile objects, from back to front
    //   static int   obj_sub_tile[5][5] = {
    //                            { 0,  2,  5,  9, 14},
    //                            { 1,  4,  8, 13, 18},
    //                            { 3,  7, 12, 17, 21},
    //                            { 6, 11, 16, 20, 23},
    //                            {10, 15, 19, 22, 24} };
    //   这可能是一个很重要的东西
    //   而且那个dt1文件的flag我估计理解错了
    //
    //
    //   int           glb_ds1edit.obj_sub_tile_order[5][5];
    //   奇了怪了..为什么不直接填写, 非要写这么个东西
    for (i=0; i < 5; i++){
        for (o=0; o < 5; o++){
            glb_ds1edit.obj_sub_tile_order[i][o] = obj_sub_tile[i][o];
        }
    }

    // init the default values of the command line
    glb_ds1edit.cmd_line.ds1_filename  = NULL;
    glb_ds1edit.cmd_line.ini_filename  = NULL;
    glb_ds1edit.cmd_line.lvltype_id    = -1;
    glb_ds1edit.cmd_line.lvlprest_def  = -1;
    glb_ds1edit.cmd_line.resize_width  = -1;
    glb_ds1edit.cmd_line.resize_height = -1;
    glb_ds1edit.cmd_line.force_pal_num = -1;
    glb_ds1edit.cmd_line.no_check_act  = FALSE;
    glb_ds1edit.cmd_line.dt1_list_num  = -1;
    for (i=0; i < DT1_IN_DS1_MAX; i++){
        glb_ds1edit.cmd_line.dt1_list_filename[i] = NULL;
    }

    // 2nd row of infos
    glb_ds1edit.show_2nd_row = FALSE;

    // video pages
    glb_ds1edit.video_page_num = 0;

    DEBUG_MESSAGE("ds1edit_init():out\n");
}


// ==========================================================================
UDWORD ds1edit_get_bitmap_size(BITMAP * bmp)
{
    UDWORD size = 0;

    if (bmp == NULL)
        return 0;

    size += sizeof(BITMAP);
    size += (sizeof(UBYTE *) * bmp->h);

    switch (bitmap_color_depth(bmp))
    {
        case  8 : size += bmp->w * bmp->h; break;
        case 15 :
        case 16 : size += 2 * (bmp->w * bmp->h); break;
        case 24 : size += 3 * (bmp->w * bmp->h); break;
        case 32 : size += 4 * (bmp->w * bmp->h); break;
        default :
                  break;
    }

    return size;
}



// ==========================================================================
UDWORD ds1edit_get_RLE_bitmap_size(RLE_SPRITE * rle)
{
    UDWORD size = 0;

    if (rle == NULL)
        return 0;

    size += sizeof(RLE_SPRITE);
    size += rle->size;

    return size;
}


// ==========================================================================
// automatically called at the end, with the help of atexit()
void ds1edit_exit(void)
{
    int i, z, b;


    printf("\nds1edit_exit()\n");

    // close all mpq
    for (i=0; i<MAX_MPQ_FILE; i++)
    {
        if (glb_mpq_struct[i].is_open != FALSE)
        {
            fprintf(stderr, "closing %s\n", glb_mpq_struct[i].file_name);
            fflush(stderr);

            glb_mpq = & glb_mpq_struct[i];
            mpq_batch_close();

            memset( & glb_mpq_struct[i], 0, sizeof(GLB_MPQ_S));
            glb_mpq_struct[i].is_open = FALSE;
        }
    }

    // free all mem
    fprintf(stderr, "exit, memory free :\n");
    fflush(stderr);

    // mouse cursor
    fprintf(stderr, "   * mouse cursor...\n");
    fflush(stderr);
    show_mouse(NULL);
    for (i=0; i<MOD_MAX; i++)
    {
        if (glb_ds1edit.mouse_cursor[i] != NULL)
        {
            destroy_bitmap(glb_ds1edit.mouse_cursor[i]);
            glb_ds1edit.mouse_cursor[i] = NULL;
        }
    }

    // screen buffer
    fprintf(stderr, "   * screen buffer...\n");
    fflush(stderr);
    if (glb_ds1edit.screen_buff != NULL)
    {
        destroy_bitmap(glb_ds1edit.screen_buff);
        glb_ds1edit.screen_buff = NULL;
    }

    if (glb_ds1edit.big_screen_buff != NULL)
    {
        destroy_bitmap(glb_ds1edit.big_screen_buff);
        glb_ds1edit.big_screen_buff = NULL;
    }

    // config, mpq name
    fprintf(stderr, "   * config, mpq names...\n");
    fflush(stderr);
    for (i=0; i<MAX_MPQ_FILE; i++)
    {
        if(glb_config.mpq_file[i] != NULL)
            free(glb_config.mpq_file[i]);
    }

    // config, mod directory
    fprintf(stderr, "   * config, mod directory name...\n");
    fflush(stderr);
    for (i=0; i<MAX_MOD_DIR; i++)
    {
        if(glb_config.mod_dir[i] != NULL)
            free(glb_config.mod_dir[i]);
    }

    // palettes
    fprintf(stderr, "   * palettes...\n");
    fflush(stderr);
    for (i=0; i<ACT_MAX; i++)
    {
        if(glb_ds1edit.d2_pal[i] != NULL)
        {
            free(glb_ds1edit.d2_pal[i]);
            glb_ds1edit.d2_pal[i] = NULL;
            glb_ds1edit.pal_size[i] = 0;
        }
    }

    // dt1
    fprintf(stderr, "   * DT1 files...\n");
    fflush(stderr);
    if (glb_dt1 != NULL)
    {
        for (i=0; i<DT1_MAX; i++)
            dt1_free(i);
    }

    // ds1
    fprintf(stderr, "   * DS1 files...\n");
    fflush(stderr);

    ds1_free();

    // objects descriptions
    fprintf(stderr, "   * objects descriptions...\n");
    fflush(stderr);
    if (glb_ds1edit.obj_desc != NULL)
    {
        anim_exit();
        free(glb_ds1edit.obj_desc);
        glb_ds1edit.obj_desc = NULL;
        glb_ds1edit.obj_desc_num = 0;
    }

    // walkable infos tiles
    fprintf(stderr, "   * walkable info tiles...\n");
    fflush(stderr);
    for (z=0; z<ZM_MAX; z++)
    {
        for (i=0; i<25; i++)
        {
            if (glb_ds1edit.subtile_nowalk[z][i] != NULL)
            {
                destroy_rle_sprite(glb_ds1edit.subtile_nowalk[z][i]);
                glb_ds1edit.subtile_nowalk[z][i] = NULL;
            }

            if (glb_ds1edit.subtile_nojump[z][i] != NULL)
            {
                destroy_rle_sprite(glb_ds1edit.subtile_nojump[z][i]);
                glb_ds1edit.subtile_nojump[z][i] = NULL;
            }
        }
    }

    if (glb_ds1edit.subtile_help != NULL)
    {
        destroy_bitmap(glb_ds1edit.subtile_help);
        glb_ds1edit.subtile_help = NULL;
    }


    // walkable infos tiles, combinations
    fprintf(stderr, "   * walkable info tiles combinations...\n");
    fflush(stderr);
    for (b=0; b<256; b++)
    {
        for (z=0; z<ZM_MAX; z++)
        {
            for (i=0; i<25; i++)
            {
                if (glb_ds1edit.subtile_flag[b][z][i] != NULL)
                {
                    destroy_rle_sprite(glb_ds1edit.subtile_flag[b][z][i]);
                    glb_ds1edit.subtile_flag[b][z][i] = NULL;
                }
            }
        }
    }

    // .txt buffers
    fprintf(stderr, "   * .txt buffers ...\n");
    fflush(stderr);
    if (glb_ds1edit.lvltypes_buff != NULL)
        glb_ds1edit.lvltypes_buff = txt_destroy(glb_ds1edit.lvltypes_buff);
    if (glb_ds1edit.lvlprest_buff != NULL)
        glb_ds1edit.lvlprest_buff = txt_destroy(glb_ds1edit.lvlprest_buff);
    if (glb_ds1edit.obj_buff != NULL)
        glb_ds1edit.obj_buff = txt_destroy(glb_ds1edit.obj_buff);

    // animdata.d2
    fprintf(stderr, "   * animdata.d2 buffer ...\n");
    fflush(stderr);
    if (glb_ds1edit.anim_data.buffer)
        free(glb_ds1edit.anim_data.buffer);

    // obj in ds1
    fprintf(stderr, "   * glb_ds1 & glb_dt1[] & glb_ds1.obj & glb_ds1.obj_undo ...\n");
    fflush(stderr);

    // ds1 & dt1
    if (glb_ds1.obj != NULL){
        free(glb_ds1.obj);
    }

    if (glb_ds1.obj_undo != NULL){
        free(glb_ds1.obj_undo);
    }

    if (glb_dt1 != NULL){
        free(glb_dt1);
    }

    fflush(stderr);
}




// ==========================================================================
// 1 tick each 1/25 of a second
void ds1edit_counter_tick(void)
{
    glb_ds1edit.ticks_elapsed++;
}
//END_OF_FUNCTION(ds1edit_counter_tick);


// ==========================================================================
// 1 tick each second
void ds1edit_counter_fps(void)
{
    glb_ds1edit.old_fps = glb_ds1edit.fps;
    glb_ds1edit.fps = 0;
}
//END_OF_FUNCTION(ds1edit_counter_fps);


// ==========================================================================
// open all mpq
void ds1edit_open_all_mpq(void)
{
    // 这个函数在使用mapedit时如果配置了mpq文件的路径
    // 就会被调用，否则不被调用
    int  i;

    for(i=0; i<MAX_MPQ_FILE; i++){
        if(glb_config.mpq_file[i] != NULL){
            DEBUG_MESSAGE("opening mpq %i : %s\n", i, glb_config.mpq_file[i]);
            glb_mpq = & glb_mpq_struct[i];
            mpq_batch_open(glb_config.mpq_file[i]);
        }
    }
}


// ==========================================================================
// load palettes of the 5 acts from disk, else from mpq
void ds1edit_load_palettes(void)
{
    //gamma只有一个
    //调色板倒是这里有一堆
    //
    //那个gamma估计是调节屏幕亮度的
    //调色板就是变色效果了
    int  i, entry;
    char palname[80], tmp[150];


    //#define ACT_MAX                5
    // act是关的意思, 一个关里面有若干任务
    // 我在想是不是每一关中的元素比较近似~ 
    // 比如第一关就全是草地 树木之类的东西
    // 第二关就是沙漠 枯树什么的
    for(i=0; i<ACT_MAX; i++){
        glb_ds1edit.pal_loaded[i] = TRUE;
        // first checking on disk
        // 这个就是保存到磁盘的tmp文件吧
        // 我调试时候返回true, 直接跳过花括号里面的了

        if(misc_load_pal_from_disk(i) == FALSE) {
            // not already on disk
            glb_ds1edit.pal_loaded[i] = FALSE;
            // make full path
            sprintf(palname, "data/global/palette/act%i/pal.pl2", i+1);
            // load the palette
            DEBUG_MESSAGE("want to read a palette from mpq : %s\n", palname);
            entry = misc_load_mpq_file( palname, (char **) & glb_ds1edit.d2_pal[i], & glb_ds1edit.pal_size[i], TRUE );
            if(entry == -1){
                DEBUG_MESSAGE("file \"%s\" not found.", palname );
                if (i < 4){
                    fprintf(stderr, tmp);
                }else{
                    printf("warning :\n%s\n", tmp);
                }
            }
            // save it for the next time
            misc_save_pal_on_disk(i, glb_ds1edit.d2_pal[i]);
        }

        // palette loaded, either from disk of from mpq, reorder it
        // can not understand well now, keep unkown
        misc_pl2_correct(i);
        misc_pal_d2_2_vga(i);
    }
}


int main(int argc, char * argv[])
{



    int         i;
    int         mpq_num = 0;
    int         mod_num = 0;
    int         ds1_idx = 0;
    int         res_w;
    int         res_h;

    char       tmp [512];
    char       tmp2[512];

    char       *ininame = "./ds1edit.ini";

    glb_map_test=0;

    // 程序初始化 {{{
    srand(time(NULL));
    allegro_init();
    set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);

    install_keyboard();
    install_timer();

    set_color_depth(8);
    atexit(ds1edit_exit);

    ds1edit_init();

    get_desktop_resolution( &res_w, &res_h);
    set_window_title("D2IM-MAPEDITOR");
    // }}}

    // check data/tmp directory
    // {{{
    sprintf(tmp, "%s/%s/.", glb_ds1edit_data_dir, glb_ds1edit_tmp_dir);
    if (!file_exists(tmp, -1, NULL)){
        sprintf(tmp, "%s/%s", glb_ds1edit_data_dir, glb_ds1edit_tmp_dir);
        mkdir(tmp);
    }
    // }}}


    // check if ds1edit.ini exists
    // {{{
    sprintf(tmp, "./ds1edit.ini");
    if(!file_exists(tmp, -1, NULL)){
        ini_create(tmp);
        FATAL_EXIT("no 'ds1edit.ini' file, create and reconfigure it.\n");
    }
    ini_read(ininame);
    // }}}


    // check mod directory and mpq files
    // {{{
    mod_num = 0;
    if(glb_config.mod_dir[0]){
        sprintf(tmp, "%s/.", glb_config.mod_dir[0]);
        if (!file_exists(tmp, -1, NULL)){
            WARNING_MESSAGE("could not find the mod directory defined in 'ds1edit.ini' :\n" "%s", tmp );
        }else{
            mod_num = 1;
        }
    }

    for(i=0; i<MAX_MPQ_FILE; i++){
        if (glb_config.mpq_file[i]){
            if(!file_exists(glb_config.mpq_file[i], -1, NULL)){
                WARNING_MESSAGE("could not find this mpq defined in 'ds1edit.ini' :%s\n", glb_config.mpq_file[i] );
            }else{
                mpq_num++;
            }
        }
    }
    if ((mod_num == 0) && (mpq_num == 0)){
        FATAL_EXIT("no mod directory and no mpq files available : it can't work.\n" );
    }else if (mpq_num < 4){
        WARNING_MESSAGE("not all the 4 mpq have been found\n");
    }
    // }}}

    // gamma correction
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
            printf("here==================fuck\n");
        }
    }else{
        // bug
        fprintf(stderr, "main(), error.\nBug : neither .DS1 nor a .INI in the command line.");
    }

    // syntaxe of the command line
    printf("============================================================\n");
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
    printf("============================================================\n");

#if 0
    // 上面load的是那地图和地图上的物品
    // 下面开始要load人物了
    // 没看懂这个函数
    animdata_load();

    // load necessary objects animation
    printf("loading ds1 objects animations :\n");
    fflush(stderr);
    anim_update_gfx(TRUE); // TRUE is for "show dot progression"

    printf("\n");
    fflush(stdout);
#endif

    // colormaps
    printf("\ncolor maps...");
    fprintf(stderr, "\ncolor maps");
    misc_make_cmaps();
    printf("done\n");
    fprintf(stderr, "done\n");

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
