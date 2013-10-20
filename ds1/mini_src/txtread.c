#include <string.h>
#include "structs.h"
#include "error.h"
#include "misc.h"
#include "dt1misc.h"
#include "mpqview.h"
#include "txtread.h"

#include "iksemel.h"


// ==========================================================================
// prepare the reading of a col value
// give del_char TRUE to replace the TAB and CR/LF chars by 0
//
// function set nb_char to # of characters there's in that value
// function set is_new_line if after col was a LF of CR/LF
// function return NULL if after col is EOF, else pointer on next col / line
// note : handle both LF and CR/LF line type
char * txt_gets(char * bptr, int * nb_char, int * is_new_line, int del_char)
{
    * is_new_line = FALSE;
    * nb_char = 0;

    // 	bptr指向的为一个连续字符串内存..该是被截掉头部的objects.txt
    //	向前面说的:
    //	abcb...
    //	bcd...
    //	cd...
    //	d...
    //	返回的为从开头开始,遇到的第一个制表符之后的新column,或者换行从新开始的column
    //	对啊..所以可以返回新的行的..这时候参数表中还要回带一个is_new_line
    //
    //	截头式的返回字符串
    //	由于里面把制表啥的换成了0,也就是NULL了
    //	所以这样是安全的
    //	我擦..那最后文件结尾怎么标记
    //	对了..没读文件,这里读的是连续内存
    //	如果当前是非0,表示有,继续,把遇到的第一个制表换成0
    //	并且返回下一个开始处
    //	如果遇到的是0,直接返回0,表示真的结尾了


    for (;;) {
        if (bptr[* nb_char] == 0) {
            // end of file
            return NULL;
        }
        else if (bptr[* nb_char] == '\t') {
            // tab
            if (del_char == TRUE){
                bptr[* nb_char] = 0;
            }
            return bptr + (* nb_char) + 1;
        } else if (bptr[* nb_char] == 0x0A) {
            // end of LF line
            * is_new_line = TRUE;
            if (del_char == TRUE){
                bptr[* nb_char] = 0;
            }
            return bptr + (* nb_char) + 1;
        } else if (bptr[* nb_char] == 0x0D) {
            // end of CR / LF line
            * is_new_line = TRUE;
            if (del_char == TRUE){
                bptr[* nb_char] = 0;
            }
            //		返回查找到的值
            return bptr + (* nb_char) + 2;
        } else{
            (* nb_char)++;
        }
    }
}


// ==========================================================================
void txt_count_header_cols(char * cur_col, int * col_count)
{
    int  nb_char, is_new_line;
    char * next_col = NULL;

    if ((cur_col == NULL) || (col_count == NULL)){
        return;
    }

    (* col_count) = 0;
    while (cur_col != NULL) {
        next_col = txt_gets(cur_col, & nb_char, & is_new_line, FALSE);

        // don't count empty headers or "*eol" column
        if (nb_char) {
            if (strncasecmp(cur_col, "*eol", 4) != 0){
                (* col_count)++;
            }
        }

        // next col
        if ((is_new_line) || (next_col == NULL)){
            return;
        }else{
            cur_col = next_col;
        }
    }
}


// ==========================================================================
char * txt_read_header(char * cur_col, TXT_S * txt)
{
    int  nb_char, i, col_pos = 0, is_new_line;
    char * next_col = NULL;


    while (cur_col != NULL) {
        next_col = txt_gets(cur_col, & nb_char, & is_new_line, TRUE);
        if (nb_char) {
            // search if that col header is one of the required (or one of the user defined filters)
            for (i=0; i < txt->col_num; i++) {
                if (stricmp(cur_col, txt->col[i].name) == 0) {
                    // equal
                    txt->col[i].pos = col_pos;
                }
            }
        }
        // this col done
        col_pos++;

        // next col
        if (is_new_line){
            return next_col;
        }else{
            cur_col = next_col;
        }
    }
    return NULL;
}


// ==========================================================================
int txt_check_type_and_size(char * cur_col, TXT_S * txt)
{

    //		这里的cur_col是送了一行过来,这一行装了txt文件的所有数据
    //		从第二行开始..直到最后一行...
    //		第一行是每列的名称...

    int  nb_char, i, col_pos = 0, is_new_line = FALSE, x;
    char * next_col;

    if (cur_col == NULL)
        return 1;

    // for all lines, check the string size of the interesting columns
    txt->line_num = 0;
    col_pos = 0;
    while (cur_col != NULL) {
        // read this col
        //这里会不断的"截头"
        //举例子:
        //abcdef...
        //bcdef...
        //cdef...
        //def...
        //ef...
        //f...
        //...
        //这种...
        next_col = txt_gets(cur_col, & nb_char, & is_new_line, FALSE);

        if (nb_char) {
            // does this col is one of the required ?
            for (i=0; i < txt->col_num; i++) {
                if (col_pos == txt->col[i].pos) {
                    // update its maximum size if needed
                    //这里没太看明白
                    if (nb_char > txt->col[i].size){
                        txt->col[i].size = nb_char;
                    }

                    // check its type
                    if (i >= txt->nb_required_cols) {
                        // user defined filter : force to be string
                        if (txt->col[i].type == CT_NULL){
                            txt->col[i].type = CT_STR;
                        }
                    } else {
                        if (txt->col[i].type == CT_NULL){
                            txt->col[i].type = CT_NUM;
                        }

                        if (txt->col[i].type != CT_STR) {
                            for (x=0; x < nb_char; x++) {
                                if ( ((cur_col[x] < '0') || (cur_col[x] > '9')) && cur_col[x] != '-') {
                                    txt->col[i].type = CT_STR;
                                    x = nb_char;
                                }
                            }
                        }
                    }
                }
            }
        }

        // this col done
        col_pos++;

        // next col
        if (is_new_line) {
            txt->line_num++;
            col_pos = 0;
        }
        cur_col = next_col;
    }
    return 0;
}


// ==========================================================================
int txt_fill_data(char * cur_col, TXT_S * txt)
{
    //cur_col不是一行..是所有行..弄成一个行传过来了
    //数据是从txt文件的第二行开始的..第一行是所有列的名称
    int  nb_char, i, col_pos = 0, is_new_line, cur_line = 0;
    char * next_col, * data_ptr, * sptr;
    long * lptr;

    if ((cur_col == NULL) || (txt->data == NULL))
        return 1;

    // for all lines, read the data
    col_pos = 0;
    data_ptr = txt->data;
    while (cur_col != NULL) {
        // read this col
        next_col = txt_gets(cur_col, & nb_char, & is_new_line, TRUE);

        if (nb_char) {
            // does this col is one of the required ?
            for (i=0; i < txt->col_num; i++) {
                if (col_pos == txt->col[i].pos) {
                    // copy it
                    sptr = data_ptr + txt->col[i].offset;
                    if (txt->col[i].type == CT_STR){
                        strcpy(sptr, cur_col);
                    }else if (txt->col[i].type == CT_NUM) {
                        lptr = (long *) sptr;
                        * lptr = atol(cur_col);
                    }
                }
            }
        }

        // this col done
        col_pos++;

        // next col
        if (is_new_line) {
            cur_line++;
            if (cur_line > txt->line_num){
                return 1;
            }else {
                data_ptr += txt->line_size;
                col_pos = 0;
            }
        }
        cur_col = next_col;
    }
    return 0;
}


// ==========================================================================
TXT_S * txt_destroy(TXT_S * txt)
{
    if (txt == NULL){
        return NULL;
    }
    if (txt->data != NULL){
        free(txt->data);
    }
    if (txt->col != NULL){
        free(txt->col);
    }
    free(txt);
    return NULL;
}


// ==========================================================================
void txt_get_user_filter_cols(char * cur_col, TXT_S * txt)
{
    int  nb_char, is_new_line;
    char * next_col = NULL;
    int  i = 0;
    char tmp[500] = "";
    int  size = 0;

    if ((cur_col == NULL) || (txt == NULL)){
        return;
    }

    while (cur_col != NULL) {
        next_col = txt_gets(cur_col, & nb_char, & is_new_line, FALSE);

        // don't count empty headers or "*eol" column
        if (nb_char) {
            if (strncasecmp(cur_col, "*eol", 4) != 0) {
                // is this column already in the required cols ?
                for (i=0; i < txt->nb_required_cols; i++) {
                    size = 0;
                    while ( (cur_col[size] != 0x00) && (cur_col[size] != '\t') && (cur_col[size] != 0x0D) && (cur_col[size] != 0x0A) ) {
                        size++;
                    }

                    if (strncasecmp(txt->col[i].name, cur_col, size) == 0){
                        break;
                    }
                }

                if (i >= txt->nb_required_cols) {
                    // this header is NOT already in the required cols, add it as a user defined filter

                    // search first free filter slot
                    while ((i < txt->col_num) && (strlen(txt->col[i].name) > 0)) {
                        i++;
                    }

                    if (i < txt->col_num) {
                        if (size >= TXT_COL_NAME_LENGTH) {
                            FATAL_EXIT("txt_get_user_filter_cols() : found a user defined filter header name that is more than %i characters : \"%.*s...\" (%i characters)" , TXT_COL_NAME_LENGTH - 1, TXT_COL_NAME_LENGTH - 1, cur_col, size);
                        }
                        strncpy(txt->col[i].name, cur_col, size);
                    }
                }
            }
        }

        // next col
        if ((is_new_line) || (next_col == NULL)){
            return;
        }else{
            cur_col = next_col;
        }
    }
}


// ==========================================================================
// note : the mem buffer MUST end by a 0
TXT_S * txt_load(char * mem, RQ_ENUM enum_txt, char * filename)
{
    //mem是txt的直接复制
    //enum_txt 是txt文件的格式..这里该是objects,因为有几种格式的txt文件..我擦
    //不知道为啥把filename带上

    TXT_S       *txt = NULL;
    int         i=0;
    int         size=0;
    int         all_col_ok = TRUE;
    char        *first_line = NULL;
    char        tmp[150] = "";
    char        **required_col = NULL;

    if (((enum_txt < 0)) || (enum_txt >= RQ_MAX)) {
        FATAL_EXIT("parameter 'enum_txt' = %i, should be between 0 and %i", enum_txt, RQ_MAX - 1);
    }

    //       这个完全是一个一个填写的, 举个例子: {{{
    //        char * txt_def_obj_req[] =
    //        {
    //            // number
    //            ("Act"),
    //            ("Type"),
    //            ("Id"),
    //            ("Direction"),
    //            ("Index"),
    //            ("Objects.txt_ID"),
    //            ("Monstats.txt_ID"),
    //
    //            // text
    //            ("Base"),
    //            ("Token"),
    //            ("Mode"),
    //            ("Class"),
    //            ("HD"), ("TR"), ("LG"), ("RA"), ("LA"), ("RH"), ("LH"), ("SH"),
    //            ("S1"), ("S2"), ("S3"), ("S4"), ("S5"), ("S6"), ("S7"), ("S8"),
    //            ("Colormap"),
    //            ("Description"),
    //
    //            NULL // DO NOT REMOVE !
    //        };
    //    而glb_txt_req_ptr[enum_txt]这些都是一个一个填写进去的
    //    这个required_col是一个列表中必须的,而这些必须的都是手动填写的...如上面
    //    }}}



    required_col = glb_txt_req_ptr[enum_txt];

    DEBUG_MESSAGE("loading %s\n", filename);

    // create a new TXT_S structure
    size = sizeof(TXT_S);
    txt = (TXT_S *) malloc(size);
    if (txt == NULL) {
        FATAL_EXIT("could not allocate %i bytes", size);
    }
    memset(txt, 0, size);

    // count the required columns
    // count the required columns,必须的项目的数量,这里统计一下
    //这里这个NULL就是手动初始化时候最后一个NULL必须有..不然要挂
    txt->nb_required_cols = 0;
    while (required_col[txt->nb_required_cols] != NULL) {
        txt->nb_required_cols++;
    }

    // count all the columns
    txt->col_num = 0;
    if (enum_txt == RQ_OBJ) {
        // editor's data/obj.txt : count all columns that have a header, to include user filter columns
        txt_count_header_cols(mem, & txt->col_num);
    } else {
        txt->col_num = txt->nb_required_cols;
    }

    txt->col = (TXT_COL_S *) calloc(txt->col_num + 1, sizeof(TXT_COL_S));
    if (txt->col == NULL) {
        txt = txt_destroy(txt);
        FATAL_EXIT("calloc() error for %i element * %i bytes each", txt->col_num + 1, sizeof(TXT_COL_S));
    }

    // init the cols, required cols first
    for (i=0; i < txt->col_num; i++) {
        txt->col[i].pos = -1;
        //		这里每一个是竖行的列...
        //		所以有100列, 看objects.txt也能看出来
        //		而不是有多少行, 这个文件有1K+行呢...
        if (i < txt->nb_required_cols) {
            strncpy(txt->col[i].name, required_col[i], TXT_COL_NAME_LENGTH);
        }
    }

    // finish the init of cols, non-required cols last
    if (txt->nb_required_cols != txt->col_num) {
        txt_get_user_filter_cols(mem, txt);
    }

    // stage 1, process header   
    first_line = txt_read_header(mem, txt);
    for (i=0; i < txt->col_num; i++) {
        if (txt->col[i].pos == -1) {
            WARNING_MESSAGE("could not find required column %i (%s)\n", i, txt->col[i].name);
            all_col_ok = FALSE;
        }
    }

    if (all_col_ok != TRUE) {
        txt = txt_destroy(txt);
        FATAL_EXIT("not all columns have been found\n");
    }

    // stage 2, search max size of all string columns except header
    if (txt_check_type_and_size(first_line, txt)) {
        txt = txt_destroy(txt);
        FATAL_EXIT("stage 2, error shouldn't have happen\n");
    }

    // stage 3, make the struct
    size = 0;
    for (i=0; i < txt->col_num; i++) {
        txt->col[i].offset = size;
        if (txt->col[i].type == CT_NUM) {
            size += sizeof(long);
        } else if (txt->col[i].type == CT_STR) {
            size += (txt->col[i].size + 1);
        }
    }
    //这里得到了一行的最大size
    txt->line_size = size;

    //分配M*N个连续内存单元, 并初始化为0
    txt->data = (char *) calloc(txt->line_num + 1, txt->line_size);
    if (txt->data == NULL) {
        txt = txt_destroy(txt);
        FATAL_EXIT("calloc() error for %i elements * %i bytes each\n", txt->line_num + 1, txt->line_size);
    }

    // stage 4, load the txt into the struct
    if (txt_fill_data(first_line, txt)) {
        txt = txt_destroy(txt);
        FATAL_EXIT("stage 4, error shouldn't have happen\n");
    }

    return txt;
}


// ==========================================================================
// load a .txt from a mpq (or mod dir) into mem
void * txt_read_in_mem(char * txtname)
{
    void * buff = NULL, * new_buff;
    int  entry;
    long len;
    char tmp[150];

    DEBUG_MESSAGE("want to read a txt from mpq : %s\n", txtname);
    //   直接的从txt文件中复制出来而已,复制到buff中
    entry = misc_load_mpq_file(txtname, (char **) & buff, & len, TRUE);
    if ((entry == -1) || (buff == NULL)) {
        FATAL_EXIT("file %s not found\n", txtname);
    }

    len++;
    //加长一个字节,就为了在在末尾填一个0
    new_buff = realloc(buff, len);
    if (new_buff == NULL) {
        if (buff != NULL) {
            free(buff);
        }
        FATAL_EXIT("could not reallocate %i bytes for %s\n", len, txtname);
    }

    if (new_buff != buff) {
        memcpy(new_buff, buff, len - 1);
    }

    * (((char *) new_buff) + len - 1) = 0;

    return buff;
}

// ==========================================================================
void txt_convert_dash(char * str)
{
    int i, s = strlen(str);


    for (i=0; i < s; i++) {
        if (str[i] == '\\' || str[i] == '/'){
            str[i] = '_';
        }
    }
}

// ==========================================================================
void txt_convert_slash(char * str)
{
    // 随便改吧, 这个函数就被调用过一次
    int i, s = strlen(str);


    for (i=0; i < s; i++) {
        if (str[i] == '\\'){
            str[i] = '/';
        }
        str[i] = tolower(str[i]);
    }
}


// ==========================================================================
// load lvlTypes.txt in mem, then load each dt1 for a 1 ds1
void txt_debug(char * file_path_mem, char * file_path_def, TXT_S * txt)
{
    FILE * out = NULL;
    int  i     = 0;


    if (glb_ds1edit.cmd_line.debug_mode != TRUE)
        return;

    if ((file_path_mem == NULL) || (file_path_def == NULL) || (txt == NULL))
        return;

    out = fopen(file_path_mem, "wb");
    if (out != NULL)
    {
        fwrite(txt->data, txt->line_size, txt->line_num, out);
        fclose(out);
    }

    out = fopen(file_path_def, "wt");
    if (out != NULL)
    {
        fprintf(out, "memory_col col_type %-*s txt_col_num data_type txt_data_size memory_offset\n",  TXT_COL_NAME_LENGTH, "header_name");
        fprintf(out, "---------- -------- %-.*s ----------- --------- ------------- -------------\n", TXT_COL_NAME_LENGTH, "------------------------------");

        for (i=0; i < txt->col_num; i++)
        {
            fprintf(
                    out,
                    "%10i %-8s %-*s %11i %-9s %13i %13i\n",
                    i,
                    (i >= txt->nb_required_cols) ? "Filter" : "Required",
                    TXT_COL_NAME_LENGTH,
                    txt->col[i].name,
                    txt->col[i].pos,
                    txt->col[i].type == CT_NUM ? "NUM" : "STR",
                    txt->col[i].size,
                    txt->col[i].offset
                   );
        }
        fclose(out);
    }
}


// ==========================================================================
// load lvlTypes.txt in mem, then load each dt1 for a 1 ds1
int read_lvltypes_txt(int ds1_idx, int type)
{
    TXT_S          *txt           = NULL;
    long           *lptr          = NULL;
    long           *tmp_long      = NULL;
    char           *buff;
    char           *sptr;

    char            lvltypes[]    = "data/global/excel/lvltypes.txt";
    char            tmp[150]      = "";
    char            name[256]     = "";
    char            ds1edt_file[] = "ds1edit.dt1";

    int             i;
    int             f;
    long            act;

    int             col_idx       = 0;
    int             file_1_idx    = 0;

    // load the file in mem
    if(glb_ds1edit.lvltypes_buff == NULL){
        //直接读取到buff中~无数据结构
        buff = txt_read_in_mem(lvltypes);
        if (buff == NULL){
            return -1;
        }

        // load the .txt in a more handy struct
        //熟悉了~
        txt = txt_load(buff, RQ_LVLTYPE, lvltypes);
        glb_ds1edit.lvltypes_buff = txt;
        free(buff);
        if (txt == NULL){
            return -1;
        }

        // debug files
        if (glb_ds1edit.cmd_line.debug_mode == TRUE){
            txt_debug(glb_path_lvltypes_mem, glb_path_lvltypes_def, txt);
        }
    }else{
        txt = glb_ds1edit.lvltypes_buff;
    }

    // search the good Id
    for(i=0; i < txt->line_num; i++){
        // 看样子type就是id阿
        // 这个type是哪里输入的呢？
        // 貌似每个(or每种？)ds1有一个惟一的type了？
        sptr = txt->data + (i * txt->line_size) + txt->col[misc_get_txt_column_num(RQ_LVLTYPE, "ID")].offset;
        lptr = (long *) sptr;
        if((* lptr) == type){
            // Id found
            DEBUG_MESSAGE( "Found ID %i in LvlTypes.txt at row %i, col %i\n", type, i + 1, txt->col[misc_get_txt_column_num(RQ_LVLTYPE, "ID")].pos );

            // check act
            sptr = txt->data + (i * txt->line_size) + txt->col[misc_get_txt_column_num(RQ_LVLTYPE, "Act")].offset;
            lptr = (long *) sptr;
            act  = * lptr;
            // 确定了type还要确定一下act
            if((act != glb_ds1.act) && (glb_ds1edit.cmd_line.no_check_act == FALSE)){
                FATAL_EXIT("Acts from lvltypes.txt (%li) and the Ds1 (%li) " "are different", act, glb_ds1.act );
            }


            // opening dt1  32个file 一个一个来
            // 都在txt文件中写着呢
            DEBUG_MESSAGE( "Found dt1files :\n" );
            file_1_idx = misc_get_txt_column_num(RQ_LVLTYPE, "File 1");
            //为什么是小于33?
            //只能有33个文件么?
            //我记得是每个啥最多只能有32个file
            //这里是33个..可能多了一个Data/ds1edit.dt1
            for (f=0; f<33; f++){
                glb_ds1.dt1_idx[f] = -1;
                if (f==0){
                    sprintf(tmp, "%s/%s", glb_ds1edit_data_dir, ds1edt_file);
                    DEBUG_MESSAGE("want to read a dt1 : %s\n", tmp);
                    //读取.dt1类型文件有个dt1edit可以看源代码~
                    glb_ds1.dt1_idx[f] = dt1_add_special(tmp);

                }else{
                    col_idx = file_1_idx + (f - 1);
                    sptr = txt->data + (i * txt->line_size) + txt->col[col_idx].offset;
                    if(txt->col[col_idx].type == CT_STR){
                        strcpy(tmp, sptr);
                    }else if (txt->col[col_idx].type == CT_NUM){
                        tmp_long = (long *) sptr;
                        sprintf(tmp, "%li", * tmp_long);
                    }
                    txt_convert_slash(tmp);
                    strcpy(name, glb_tiles_path);
                    strcat(name, tmp);
                    if(glb_ds1.dt1_mask[f]){
                        // 这个dt1mask貌似就是用不用当前dt1的...我忘了在哪里看的.. 
                        // 国外那个英文网站看到的
                        DEBUG_MESSAGE("want to read a dt1 from mpq : %s\n", name);
                        // 这里是真真加入一个dt1文件的地方
                        //
                        glb_ds1.dt1_idx[f] = dt1_add(name);
                    }else{
                        if((tmp[0] != '0') && tmp[1] != 0){
                            DEBUG_MESSAGE("\n(skip %s)\n", tmp);
                        }
                    }
                }
            }
            // end
            return act;
        }
    }
    FATAL_EXIT("could not find the ID %i in LvlTypes.txt\n", type);
    return -1;
}


// ==========================================================================
// load lvlPrest.txt into mem, then search the Dt1Mask, given the Def Id
int read_lvlprest_txt2(int ds1_idx, int def)
{



    iks    *p;
    iks    *t;
    int     e;
    int     b;

    long           d;
    long        mask;



    e = iks_load( "./res/demo/data/global/excel/lvlprest.xml", &p);

    if(e != IKS_OK ){
        fprintf(stderr, "open lvlprest.xml failed \n" );
        exit(0);
    }

    fprintf(stdout, "I am here, in read_lvlprest_txt2, def=%ld\n", def );

    t = iks_child(p);

    while ( t ){

        if ( iks_type( t ) == IKS_TAG && !strcmp( iks_name( t ), "desc" ) ){
            d       = atol(iks_find_cdata(t, "def"));
            mask    = atol(iks_find_cdata(t, "mask"));
            if( d == def){
                fprintf(stdout, "def: %ld, mask: %ld\n.", def, mask );
                break;
            }
        }
        t = iks_next(t);
    }

    iks_delete(p);

    for (b=0; b < DT1_IN_DS1_MAX; b++) {
        if (b == 0){
            glb_ds1.dt1_mask[b] = TRUE;
        }else{
            glb_ds1.dt1_mask[b] = mask & (1 << (b-1)) ? TRUE : FALSE;
        }
    }

}



// ==========================================================================
// load lvlPrest.txt into mem, then search the Dt1Mask, given the Def Id
int read_lvlprest_txt(int ds1_idx, int def)
{
    TXT_S       *txt;
    char        *buff;
    long        *lptr;
    char        *sptr;
    char        *found_ptr = NULL;
    char        *lvlprest = "data/global/excel/lvlprest.txt";

    char         tmp[150];
    int          i, b, mask, filename_colidx[6], filename_idx, length, c, found_nb, last_found=0, last_slash, found_col=0;


    // load the file in mem
    if(glb_ds1edit.lvlprest_buff == NULL){
        // 我调试的时候运行的这里
        // 直接读到内存中
        // buff中是该txt的所有数据
        buff = txt_read_in_mem( lvlprest );
        if(buff == NULL){
            return -1;
        }

        // load the .txt in a more handy struct
        // load到数据结构中...很熟悉的套路了
        txt = txt_load(buff, RQ_LVLPREST, lvlprest);
        glb_ds1edit.lvlprest_buff = txt;
        free(buff);
        if (txt == NULL){
            return -1;
        }

        // debug files
        if (glb_ds1edit.cmd_line.debug_mode == TRUE)
            txt_debug(glb_path_lvlprest_mem, glb_path_lvlprest_def, txt);
    }else{
        txt = glb_ds1edit.lvlprest_buff;
    }

    // search the good Def
    if(def == -1){
        //这下面没有运行哦
        printf("AUTOMATIC LVLPREST.DEF DETECTION\n");
        fflush(stdout);

        // automatic search, base on filename
        filename_colidx[0] = misc_get_txt_column_num(RQ_LVLPREST, "File1");
        filename_colidx[1] = misc_get_txt_column_num(RQ_LVLPREST, "File2");
        filename_colidx[2] = misc_get_txt_column_num(RQ_LVLPREST, "File3");
        filename_colidx[3] = misc_get_txt_column_num(RQ_LVLPREST, "File4");
        filename_colidx[4] = misc_get_txt_column_num(RQ_LVLPREST, "File5");
        filename_colidx[5] = misc_get_txt_column_num(RQ_LVLPREST, "File6");

        found_nb = 0;
        for(i=0; i < txt->line_num; i++){
            for(filename_idx = 0; filename_idx < 6; filename_idx++){
                sptr = txt->data + (i * txt->line_size) + txt->col[ filename_colidx[filename_idx] ].offset;
                if(sptr != NULL){
                    length = strlen(sptr);
                    if(length > 0) {
                        // search last '/' or last '\'
                        last_slash = 0;
                        for (c=0; c < length; c++) {
                            if ((sptr[c] == '/') || (sptr[c] == '\\')){
                                last_slash = c;
                            }
                        }

                        // compare this filename with our ds1 filename
                        if (last_slash){
                            last_slash++;
                        }
                        if (last_slash >= length){
                            last_slash = length - 1;
                        }
                        if (stricmp(sptr + last_slash, glb_ds1.filename) == 0) {
                            // found one
                            found_nb++;
                            last_found = i;
                            found_ptr  = sptr;
                            found_col  = filename_idx;
                        }
                    }
                }
            }
        }

        if (found_nb == 1) {
            // only one, so no error
            //这里没有运行过...
            //运行的是else分支~

            i = last_found;

            sptr = txt->data + (i * txt->line_size) + txt->col[misc_get_txt_column_num(RQ_LVLPREST, "Def")].offset;
            lptr = (long *) sptr;

            printf("Found \"%s\" in LvlPrest.txt at row %i (DEF = %li, 'File%i' = \"%s\")\n",
                    glb_ds1.filename,
                    i + 1,
                    * lptr,
                    found_col + 1,
                    found_ptr
                  );
            fflush(stdout);

            // dt1mask
            sptr = txt->data + (i * txt->line_size) + txt->col[misc_get_txt_column_num(RQ_LVLPREST, "Dt1Mask")].offset;
            lptr = (long *) sptr;
            mask = * lptr;

            printf("DT1MASK = %li in LvlPrest.txt at row %i, col %i\n",
                    mask, i + 1, txt->col[misc_get_txt_column_num(RQ_LVLPREST, "Dt1Mask")].pos);
            fflush(stdout);

            for (b=0; b < DT1_IN_DS1_MAX; b++) {
                if (b == 0){
                    glb_ds1.dt1_mask[b] = TRUE;
                }else{
                    glb_ds1.dt1_mask[b] = mask & (1 << (b-1)) ? TRUE : FALSE;
                }
            }

            // end
            return 0;
        } else {
            if (found_nb >= 2) {
                FATAL_EXIT("couldn't found ds1 filename \"%s\" in LvlPrest.txt for sure : present %i times\n", glb_ds1.filename, found_nb);
            } else {
                FATAL_EXIT("couldn't found ds1 filename \"%s\" in LvlPrest.txt\n", glb_ds1.filename);
            }
        }
    }else{
        //运行的是这里~
        for(i=0; i < txt->line_num; i++){
            sptr = txt->data + (i * txt->line_size) + txt->col[misc_get_txt_column_num(RQ_LVLPREST, "Def")].offset;
            // here because we have already load the text-based info into txt-structure,
            // so we can use *lptr == def directly, otherwise we can not
            lptr = (long *) sptr;
            if((* lptr) == def){
                // Def found
                DEBUG_MESSAGE("Found DEF %i in LvlPrest.txt at row %i, col %i\n", def, i + 1, txt->col[misc_get_txt_column_num(RQ_LVLPREST, "Def")].pos);

                // dt1mask
                sptr = txt->data + (i * txt->line_size) + txt->col[misc_get_txt_column_num(RQ_LVLPREST, "Dt1Mask")].offset;
                lptr = (long *) sptr;
                mask = * lptr;

                DEBUG_MESSAGE("DT1MASK = %li in LvlPrest.txt at row %i, col %i\n", mask, i + 1, txt->col[misc_get_txt_column_num(RQ_LVLPREST, "Dt1Mask")].pos);

                for (b=0; b < DT1_IN_DS1_MAX; b++){
                    if (b == 0){
                        glb_ds1.dt1_mask[b] = TRUE;
                    }else{
                        glb_ds1.dt1_mask[b] = mask & (1 << (b-1)) ? TRUE : FALSE;
                    }
                }

                // end
                return 0;
            }
        }





    }

    // end
    FATAL_EXIT("could not found the DEF %i in LvlPrest.txt\n", def);
    return -1; // useless, just to not have a warning under VC6
}


// ==========================================================================
// load the obj.txt into mem
int read_obj_txt(void)
{
    TXT_S      *txt;
    char       *buff;
    char       *sptr;
    long       *lptr;

    int         i;
    int         size;
    long        len;
    char        tmp[150];
    char        obj[] = "obj.txt";


    // load the file in mem
    if (glb_ds1edit.obj_buff == NULL){
        DEBUG_MESSAGE("want to read %s from %s\n", obj, glb_ds1edit_data_dir );
        // 直接拷贝到buff, 成功返回0, 失败返回-1
        if( mod_load_in_mem(glb_ds1edit_data_dir, obj, &buff, &len) == -1 ){
            FATAL_EXIT("read_obj_txt() : file %s not found", obj);
        }

        // load the .txt in a more handy struct
        // 很熟悉的套路了
        txt = txt_load(buff, RQ_OBJ, obj);
        glb_ds1edit.obj_buff = txt;
        free(buff);
        if (txt == NULL){
            return -1;
        }

        // debug files
        if (glb_ds1edit.cmd_line.debug_mode == TRUE){
            txt_debug(glb_path_obj_mem, glb_path_obj_def, txt);
        }
    }else{
        txt = glb_ds1edit.obj_buff;
    }

    // malloc
    glb_ds1edit.obj_desc_num = txt->line_num;
    size = glb_ds1edit.obj_desc_num * sizeof(OBJ_DESC_S);
    glb_ds1edit.obj_desc = (OBJ_DESC_S *) malloc(size);
    if (glb_ds1edit.obj_desc == NULL){
        FATAL_EXIT("not enough mem for %i bytes\n", size);
    }
    memset(glb_ds1edit.obj_desc, 0, size);

    // read the obj.txt
    // txt->line_num就是txt的每一行
    // 初始化每一个obj
    // 我不太懂obj.txt文件里面描述的是什么东西
    for (i=0; i < txt->line_num; i++){

        // act
        sptr = txt->data + (i * txt->line_size) + txt->col[misc_get_txt_column_num(RQ_OBJ, "Act")].offset;
        lptr = (long *) sptr;
        glb_ds1edit.obj_desc[i].act = * lptr;

        // type
        sptr = txt->data + (i * txt->line_size) + txt->col[misc_get_txt_column_num(RQ_OBJ, "Type")].offset;
        lptr = (long *) sptr;
        glb_ds1edit.obj_desc[i].type = * lptr;

        // id
        sptr = txt->data + (i * txt->line_size) + txt->col[misc_get_txt_column_num(RQ_OBJ, "ID")].offset;
        lptr = (long *) sptr;
        glb_ds1edit.obj_desc[i].id = * lptr;

        // description
        sptr = txt->data + (i * txt->line_size) + txt->col[misc_get_txt_column_num(RQ_OBJ, "Description")].offset;
        glb_ds1edit.obj_desc[i].desc = sptr;
    }

    // lookups, to avoid to search them again and again
    glb_ds1edit.col_obj_id = misc_get_txt_column_num(RQ_OBJ, "Objects.txt_ID");

    return 0;
}


// ==========================================================================
// load the objects.txt into mem
// 这个玩意儿可能是真正的动画人物, 因为有frame的概念
//
// TXT is the common Text file format found in Data\Global\Excel. So which file does we
// need from there? Objects.txt, because it have an Animation Speed data that override
// the one we can find in AnimData.d2. 
// That’s only for Objects of course, 
// not the Player Characters, Monsters, Overlays or Missiles animations.
int read_objects_txt(void)
{
    TXT_S    *txt;
    char     *buff;
    char     *objects = "data/global/excel/objects.txt";


    // load the file in mem
    if(glb_ds1edit.objects_buff == NULL){

        //	   把txt的内容直接读入到buff中
        //	   没有数据结构的
        //	   并且在buffer的尾部附加上一个0
        buff = txt_read_in_mem(objects);
        if (buff == NULL) {
            return -1;
        }

        // load the .txt in a more handy struct {{{

        //typedef struct TXT_S
        //{
        //   char      * data;           // buffer having all lines
        //   int       line_num;         // number of lines
        //   int       line_size;        // size of 1 line (in bytes)
        //   int       nb_required_cols; // number of required columns
        //   int       col_num;          // total number of columns (required + user defined filters)
        //   TXT_COL_S * col;            // pointer on a table of TXT_COL_S
        //} TXT_S;

        //	  这里写一个txt数据结构的例子
        //      是load完objects.txt后的数据结构:
        //	  txt = {
        //		 . *data			= \0;
        //		 . line_num			=	574
        //		 . line_size		=	432
        //		 . nb_required_cols	=	108
        //		 . col_num			=108
        //		 //这是一个指针..所以只能看到第一个元素~实际上有108个元素
        //		 . col = {
        //			 .name		=	"ID"
        //			 .pos		=	2
        //			 .type		=	CT_NUM
        //			 .size		=	3
        //			 .offset	=	0
        //		 }
        //	  }
        //
        //	  }}}

        txt = txt_load(buff, RQ_OBJECTS, objects);
        glb_ds1edit.objects_buff = txt;
        free(buff);
        if (txt == NULL){
            return -1;
        }
    }else{
        txt = glb_ds1edit.objects_buff;
    }

    // lookups, to avoid to search them again and again
    // objects.txt中

    glb_ds1edit.col_objects_id     = misc_get_txt_column_num(RQ_OBJECTS, "Id");

#if 0
    Animation Speed of Objects
        Having the final frames of the Arcane Teleport Pad animation is good, but not enough. What is
        its animation speed? As said before, Objects are not using animdata.d2, but rather
        Objects.txt. In this file we find many columns, but for us, only the FrameDelta0 to
        FrameDelta7 are interesting.

        Exactly like for the Animation Speed data in Animdata.d2, this speed is a relative
        speed (in 256th) of 25 fps
#endif

        glb_ds1edit.col_frame_delta[0] = misc_get_txt_column_num(RQ_OBJECTS, "FrameDelta0");
    glb_ds1edit.col_frame_delta[1] = misc_get_txt_column_num(RQ_OBJECTS, "FrameDelta1");
    glb_ds1edit.col_frame_delta[2] = misc_get_txt_column_num(RQ_OBJECTS, "FrameDelta2");
    glb_ds1edit.col_frame_delta[3] = misc_get_txt_column_num(RQ_OBJECTS, "FrameDelta3");
    glb_ds1edit.col_frame_delta[4] = misc_get_txt_column_num(RQ_OBJECTS, "FrameDelta4");
    glb_ds1edit.col_frame_delta[5] = misc_get_txt_column_num(RQ_OBJECTS, "FrameDelta5");
    glb_ds1edit.col_frame_delta[6] = misc_get_txt_column_num(RQ_OBJECTS, "FrameDelta6");
    glb_ds1edit.col_frame_delta[7] = misc_get_txt_column_num(RQ_OBJECTS, "FrameDelta7");

    glb_ds1edit.col_orderflag[0] = misc_get_txt_column_num(RQ_OBJECTS, "OrderFlag0");
    glb_ds1edit.col_orderflag[1] = misc_get_txt_column_num(RQ_OBJECTS, "OrderFlag1");
    glb_ds1edit.col_orderflag[2] = misc_get_txt_column_num(RQ_OBJECTS, "OrderFlag2");
    glb_ds1edit.col_orderflag[3] = misc_get_txt_column_num(RQ_OBJECTS, "OrderFlag3");
    glb_ds1edit.col_orderflag[4] = misc_get_txt_column_num(RQ_OBJECTS, "OrderFlag4");
    glb_ds1edit.col_orderflag[5] = misc_get_txt_column_num(RQ_OBJECTS, "OrderFlag5");
    glb_ds1edit.col_orderflag[6] = misc_get_txt_column_num(RQ_OBJECTS, "OrderFlag6");
    glb_ds1edit.col_orderflag[7] = misc_get_txt_column_num(RQ_OBJECTS, "OrderFlag7");
    return 0;

}
