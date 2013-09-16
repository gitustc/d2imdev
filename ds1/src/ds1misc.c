#include <string.h>
#include "structs.h"
#include "dt1misc.h"
#include "error.h"
#include "editobj.h"
#include "wpreview.h"
#include "ds1misc.h"
#include "misc.h"

#if 0
In a DS1 you can find 2 kind of units. 
The Type 1 units are Monsters/NPC, taken from Data\Global\Excel\MonStats.txt, Data\Global\Excel\MonPlace.txt, and Data\Global\Excel\SuperUniques.txt. Each Type 1 units can have Paths. 
The Type 2 units are Objects, taken from Data\Global\Excel\Objects.txt, such as Chest, Shrine, Torch...
#endif

void test_add_objects(){
    return;

    int *p;
    int i,j,k;

    p = &(glb_ds1.obj_num);


    if( *p > glb_ds1.current_obj_max ){
        return;
    }

    printf("in test_add_object===============================\n");

    for (i=1;i<3;i++){
        for (j=1;j<150;j++){

            for (k=1;k<5;k++){
                int tmpx, tmpy, tmpi;
                UBYTE  infos[25];

                tmpi=0;
                tmpx = rand()%glb_ds1.width;
                tmpy = rand()%glb_ds1.height;

                misc_search_walk_infos(0, tmpx, tmpy, infos);
#if 0
                for(tmpi=0;tmpi<25;tmpi++){
                    if(infos[tmpi]&0x04 || infos[tmpi]&0x09){
                        printf("no-----------------------------\n");
                        break;
                    }
                }
                if(tmpi==25){
#endif     
                    if( *p > glb_ds1.current_obj_max ){
                        return;
                    }
                    test_add_one_object(0, p, tmpx*5, tmpy*5, i, j);
#if 0
                }
#endif 
            }
        }
    }





}
void test_add_one_object(
        int ds1_idx,
        int *p,
        int x,
        int y,
        int type,
        int id
        )
{
    memcpy(glb_ds1.obj+(*p), glb_ds1.obj, sizeof(OBJ_S));
    glb_ds1.obj[(*p)].x = x;
    glb_ds1.obj[(*p)].y = y;
    glb_ds1.obj[(*p)].type = type;
    glb_ds1.obj[(*p)].id = id;
    editobj_make_obj_desc(ds1_idx, (*p));
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("type=%d, id=%d, x=%d, y=%d\n", glb_ds1.obj[(*p)].type, glb_ds1.obj[(*p)].id, glb_ds1.obj[(*p)].x, glb_ds1.obj[(*p)].y);
    printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    (*p)++;
}


// ==========================================================================
// as expected, save the ds1
//    if is_tmp_file == TRUE, will save a file "*.tmp", else "*-nnn.ds1"
void ds1_save(int ds1_idx, int is_tmp_file)
{
    CELL_W_S * w_ptr, *w_p;
    CELL_S_S * s_ptr, *s_p;
    CELL_F_S * f_ptr, *f_p;
    CELL_T_S * t_ptr, *t_p;
    int      i=0, x, y, t, cur_o=0, p, used, flen;
    FILE     * out;
    long     n, npc = 0, file_count,
             save_wall_num, save_floor_num, save_shadow_num;
    char     tmp[512], tmp_name[256], * cptr;
    int      ax, ay, cx, cy, is_data;


    // if no ds1 here, don't process it
    if (strlen(glb_ds1.name) == 0)
        return;

    // file must have an extension
    strcpy(tmp, glb_ds1.name);
    if (strlen(tmp) < 4) {
        FATAL_EXIT("ds1save(), glb_ds1.ds1_name < 4 chars : \"%s\" (is_tmp_file = %i)", ds1_idx, glb_ds1.name, is_tmp_file);
    }

    // set the .TMP or .DS1 extension
    tmp[strlen(tmp)-4] = 0;
    if (is_tmp_file)
    {
        // save a ds1 with the extension .tmp
        sprintf(tmp_name, "%s.tmp", tmp);
        if (file_exists(tmp_name, -1, NULL))
            remove(tmp_name);
    } else {
        // save a ds1 with the extension .ds1
        // 1st, rename the current .ds1 to an incremental name (for backup)

        // find the 1st free slot
        sprintf(tmp_name, "%s-%03i.ds1", tmp, i);
        while (file_exists(tmp_name, -1, NULL))
        {
            i++;
            sprintf(tmp_name, "%s-%03i.ds1", tmp, i);
        }

        // rename the original ds1 to that incremental name
        if (rename(glb_ds1.name, tmp_name)) {
            FATAL_EXIT( "ds1save(), couldn't rename %s to %s", glb_ds1.name, tmp_name);
        }

        // then, we can save the ds1
        strcpy(tmp_name, glb_ds1.name);
    }

    // save the ds1, either with a .tmp or a .ds1 extension
    out = fopen(tmp_name, "wb");
    if (out == NULL) {
        FATAL_EXIT("ds1save(), can't write %s", tmp_name);
    }
    if(glb_map_test){

        // version
        n = 18;
        fwrite(& n, 4, 1, out);

        // width
        n = glb_ds1.width;
        fwrite(& n, 4, 1, out);

        // height
        n = glb_ds1.height;
        fwrite(& n, 4, 1, out);

        // act
        n = glb_ds1.act;
        fwrite(& n, 4, 1, out);

        // filenames

        // count how many filenames (not workspace datas)
        fwrite(&(glb_ds1.file_num), 4, 1, out);
        cptr = glb_ds1.file_buff;
        for (i=0; i < glb_ds1.file_num; i++)
        {
            // next filename
            flen = strlen(cptr) + 1;
            fwrite(cptr, flen, 1, out);
            cptr += flen;
        }

        // minimize ds1 size
        save_wall_num  = glb_ds1.wall_num;
        save_floor_num = glb_ds1.floor_num;
        if (glb_config.minimize_ds1 == TRUE)
        {
            // how many wall layers are really used ?
            // (keep a minimum of 1)
            for (i = save_wall_num - 1; i >= 1; i--)
            {
                used = FALSE;

                for (y=0; y < glb_ds1.height; y++)
                {
                    for (x=0; x < glb_ds1.width; x++)
                    {
                        t = (y * glb_ds1.wall_line) + (x * glb_ds1.wall_num);
                        w_ptr = glb_ds1.wall_buff + t + i;
                        if ((w_ptr->prop1 | w_ptr->prop2 | w_ptr->prop3 | w_ptr->prop4 |
                                    w_ptr->orientation) != 0)
                        {
                            used = TRUE;
                            break;
                        }
                    }
                    if (used)
                        break;
                }
                if (used)
                    break;
                else
                    save_wall_num--;
            }

            // how many floor layers are really used ?
            // (keep a minimum of 1)
            for (i = save_floor_num - 1; i >= 1; i--)
            {
                used = FALSE;

                for (y=0; y < glb_ds1.height; y++)
                {
                    for (x=0; x < glb_ds1.width; x++)
                    {
                        t = (y * glb_ds1.floor_line) + (x * glb_ds1.floor_num);
                        f_ptr = glb_ds1.floor_buff + t + i;
                        if ((f_ptr->prop1 | f_ptr->prop2 | f_ptr->prop3 | f_ptr->prop4) != 0)
                        {
                            used = TRUE;
                            break;
                        }
                    }
                    if (used)
                        break;
                }
                if (used)
                    break;
                else
                    save_floor_num--;
            }
        }


        // wall num
        fwrite(& save_wall_num,  4, 1, out);

        // floor num
        fwrite(& save_floor_num, 4, 1, out);

        // shadow num
        save_shadow_num=1;
        fwrite(& save_shadow_num, 4, 1, out);

        // walls
        w_p = glb_ds1.wall_buff;
        for (i=0; i < save_wall_num; i++){
            for (y=0; y < glb_ds1.height; y++){
                for (x=0; x < glb_ds1.width; x++){
                    fputc(w_p->prop1, out);
                    fputc(w_p->prop2, out);
                    fputc(w_p->prop3, out);
                    fputc(w_p->prop4, out);
                    fputc(w_p->orientation, out);
                    w_p++;
                }
            }
        }

        // floors
        f_p = glb_ds1.floor_buff;
        for (i=0; i < save_floor_num; i++){
            for (y=0; y < glb_ds1.height; y++) {
                for (x=0; x < glb_ds1.width; x++) {
                    fputc(f_p->prop1, out);
                    fputc(f_p->prop2, out);
                    fputc(f_p->prop3, out);
                    fputc(f_p->prop4, out);
                }
            }
        }

        // shadows
        s_p = glb_ds1.shadow_buff;
        for (i=0; i < glb_ds1.shadow_num; i++){
            for (y=0; y < glb_ds1.height; y++){
                for (x=0; x < glb_ds1.width; x++){
                    fputc(s_p->prop1, out);
                    fputc(s_p->prop2, out);
                    fputc(s_p->prop3, out);
                    fputc(s_p->prop4, out);
                }
            }
        }


        // objects
        fwrite(&glb_ds1.obj_num, 4, 1, out);
        for (i=0; i < glb_ds1.obj_num; i++){
            fwrite(&glb_ds1.obj[i].type,      4, 1, out);
            fwrite(&glb_ds1.obj[i].id,        4, 1, out);
            fwrite(&glb_ds1.obj[i].x,         4, 1, out);
            fwrite(&glb_ds1.obj[i].y,         4, 1, out);
            fwrite(&glb_ds1.obj[i].ds1_flags, 4, 1, out);
        }
    }else{

        // version
        n = 18;
        fwrite(& n, 4, 1, out);

        // width
        n = glb_ds1.width - 1;
        fwrite(& n, 4, 1, out);

        // height
        n = glb_ds1.height - 1;
        fwrite(& n, 4, 1, out);

        // act
        n = glb_ds1.act - 1;
        fwrite(& n, 4, 1, out);

        // tag type
        fwrite(& glb_ds1.tag_type, 4, 1, out);

        // filenames

        // count how many filenames (not workspace datas)
        file_count = 0;
        cptr = glb_ds1.file_buff;
        for (i=0; i < glb_ds1.file_num; i++)
        {
            flen = strlen(cptr) + 1;
            is_data = FALSE;
            // for all workspace datas
            for(y=0; y < WRKSPC_MAX; y++)
            {
                // check if it's one of our datas

                // for all characters
                x = 0;
                while ( (cptr[x] != 0x00) && (glb_wrkspc_datas[y].id[x] != 0x00) && (cptr[x] == glb_wrkspc_datas[y].id[x])) {
                    // continue on next character
                    x++;
                }
                if ((glb_wrkspc_datas[y].id[x] == 0x00) && (cptr[x]== '=')) {
                    // this is our data, don't count it
                    is_data = TRUE;
                    break;
                }
            }
            if (is_data == FALSE)
                file_count++;
            // next filename
            cptr += flen;
        }
        if (glb_config.workspace_enable)
            file_count += WRKSPC_MAX;
        {
            long cc=7;
            //fwrite(& file_count, 4, 1, out);
            fwrite(& cc, 4, 1, out);
        }

        // write filenames (not workspace datas)
        cptr = glb_ds1.file_buff;
        for (i=0; i < glb_ds1.file_num; i++)
        {
            flen = strlen(cptr) + 1;
            is_data = FALSE;
            // for all workspace datas
            for(y=0; y < WRKSPC_MAX; y++)
            {
                // check if it's one of our datas

                // for all characters
                x = 0;
                while ( (cptr[x] != 0x00) &&
                        (glb_wrkspc_datas[y].id[x] != 0x00) &&
                        (cptr[x] == glb_wrkspc_datas[y].id[x])
                      )
                {
                    // continue on next character
                    x++;
                }
                if ((glb_wrkspc_datas[y].id[x] == 0x00) && (cptr[x]== '='))
                {
                    // this is our data, skip it
                    is_data = TRUE;
                    break;
                }
            }
            if (is_data == FALSE)
                fwrite(cptr, flen, 1, out);

            // next filename
            cptr += flen;
        }

        if (glb_config.workspace_enable)
        {
            // write all workspace datas
            ax = glb_ds1edit.win_preview.x0 + glb_ds1edit.win_preview.w / 2;
            ay = glb_ds1edit.win_preview.y0 + glb_ds1edit.win_preview.h / 2;
            coord_to_tile(ds1_idx, ax, ay, & cx, & cy);
            if ( (glb_ds1edit.mode == MOD_O) ||
                    (glb_ds1edit.mode == MOD_P) ||
                    (glb_ds1edit.mode == MOD_L) )
            {
                cx /= 5;
                cy /= 5;
            }
            for (i=0; i < WRKSPC_MAX; i++)
            {
                switch (i)
                {
                    case WRKSPC_TILE_X :
                        sprintf(tmp, "%s=%i", glb_wrkspc_datas[i].id, cx);
                        fwrite(tmp, strlen(tmp) + 1, 1, out);
                        break;

                    case WRKSPC_TILE_Y :
                        sprintf(tmp, "%s=%i", glb_wrkspc_datas[i].id, cy);
                        fwrite(tmp, strlen(tmp) + 1, 1, out);
                        break;

                    case WRKSPC_ZOOM :
                        sprintf(tmp, "%s=%i", glb_wrkspc_datas[i].id, glb_ds1.cur_zoom);
                        fwrite(tmp, strlen(tmp) + 1, 1, out);
                        break;

                    case WRKSPC_VERSION :
                        sprintf(tmp, "%s=Build YYYY/MM/DD", glb_wrkspc_datas[i].id);
                        fwrite(tmp, strlen(tmp) + 1, 1, out);
                        break;

                    case WRKSPC_SAVE_COUNT :
                        glb_ds1.save_count++;
                        sprintf(tmp, "%s=%lu", glb_wrkspc_datas[i].id, glb_ds1.save_count);
                        fwrite(tmp, strlen(tmp) + 1, 1, out);
                        break;
                }
            }
        }

        // minimize ds1 size
        save_wall_num  = glb_ds1.wall_num;
        save_floor_num = glb_ds1.floor_num;
        if (glb_config.minimize_ds1 == TRUE)
        {
            // how many wall layers are really used ?
            // (keep a minimum of 1)
            for (i = save_wall_num - 1; i >= 1; i--)
            {
                used = FALSE;

                for (y=0; y < glb_ds1.height; y++)
                {
                    for (x=0; x < glb_ds1.width; x++)
                    {
                        t = (y * glb_ds1.wall_line) + (x * glb_ds1.wall_num);
                        w_ptr = glb_ds1.wall_buff + t + i;
                        if ((w_ptr->prop1 | w_ptr->prop2 | w_ptr->prop3 | w_ptr->prop4 |
                                    w_ptr->orientation) != 0)
                        {
                            used = TRUE;
                            break;
                        }
                    }
                    if (used)
                        break;
                }
                if (used)
                    break;
                else
                    save_wall_num--;
            }

            // how many floor layers are really used ?
            // (keep a minimum of 1)
            for (i = save_floor_num - 1; i >= 1; i--)
            {
                used = FALSE;

                for (y=0; y < glb_ds1.height; y++)
                {
                    for (x=0; x < glb_ds1.width; x++)
                    {
                        t = (y * glb_ds1.floor_line) + (x * glb_ds1.floor_num);
                        f_ptr = glb_ds1.floor_buff + t + i;
                        if ((f_ptr->prop1 | f_ptr->prop2 | f_ptr->prop3 | f_ptr->prop4) != 0)
                        {
                            used = TRUE;
                            break;
                        }
                    }
                    if (used)
                        break;
                }
                if (used)
                    break;
                else
                    save_floor_num--;
            }
        }


        // wall num
        fwrite(& save_wall_num,  4, 1, out);

        // floor num
        fwrite(& save_floor_num, 4, 1, out);

        // walls
        for (i=0; i < save_wall_num; i++)
        {
            // props layer
            for (y=0; y < glb_ds1.height; y++)
            {
                for (x=0; x < glb_ds1.width; x++)
                {
                    t = (y * glb_ds1.wall_line) + (x * glb_ds1.wall_num);
                    w_ptr = glb_ds1.wall_buff + t + i;
                    fputc(w_ptr->prop1, out);
                    fputc(w_ptr->prop2, out);
                    fputc(w_ptr->prop3, out);
                    fputc(w_ptr->prop4, out);
                }
            }

            // orientation layer
            for (y=0; y < glb_ds1.height; y++)
            {
                for (x=0; x < glb_ds1.width; x++)
                {
                    t = (y * glb_ds1.wall_line) + (x * glb_ds1.wall_num);
                    w_ptr = glb_ds1.wall_buff + t + i;
                    fputc(w_ptr->orientation, out);
                    fputc(0, out);
                    fputc(0, out);
                    fputc(0, out);
                }
            }
        }

        // floors
        for (i=0; i < save_floor_num; i++)
        {
            // props layer
            for (y=0; y < glb_ds1.height; y++)
            {
                for (x=0; x < glb_ds1.width; x++)
                {
                    t = (y * glb_ds1.floor_line) + (x * glb_ds1.floor_num);
                    f_ptr = glb_ds1.floor_buff + t + i;
                    fputc(f_ptr->prop1, out);
                    fputc(f_ptr->prop2, out);
                    fputc(f_ptr->prop3, out);
                    fputc(f_ptr->prop4, out);
                }
            }
        }

        // shadows
        for (i=0; i < glb_ds1.shadow_num; i++)
        {
            // props layer
            for (y=0; y < glb_ds1.height; y++)
            {
                for (x=0; x < glb_ds1.width; x++)
                {
                    t = (y * glb_ds1.shadow_line) + (x * glb_ds1.shadow_num);
                    s_ptr = glb_ds1.shadow_buff + t + i;
                    fputc(s_ptr->prop1, out);
                    fputc(s_ptr->prop2, out);
                    fputc(s_ptr->prop3, out);
                    fputc(s_ptr->prop4, out);
                }
            }
        }

        // optional tag layer
        if (glb_ds1.tag_type)
        {
            for (i=0; i<glb_ds1.tag_num; i++)
            {
                // props layer
                for (y=0; y < glb_ds1.height; y++)
                {
                    for (x=0; x < glb_ds1.width; x++)
                    {
                        t = (y * glb_ds1.tag_line) + (x * glb_ds1.tag_num);
                        t_ptr = glb_ds1.tag_buff + t + i;
                        fwrite( & t_ptr->num, 4, 1, out);
                    }
                }
            }
        }

        // objects
        fwrite(&glb_ds1.obj_num, 4, 1, out);
        for (i=0; i < glb_ds1.obj_num; i++)
        {
            fwrite(&glb_ds1.obj[i].type,      4, 1, out);
            fwrite(&glb_ds1.obj[i].id,        4, 1, out);
            fwrite(&glb_ds1.obj[i].x,         4, 1, out);
            fwrite(&glb_ds1.obj[i].y,         4, 1, out);
            fwrite(&glb_ds1.obj[i].ds1_flags, 4, 1, out);
            if (glb_ds1.obj[i].path_num)
                npc++;
        }

        // optional groups
        if (glb_ds1.tag_type)
        {
            // put a 0 dword
            n = 0;
            fwrite(&n, 4, 1, out);

            // # of groups
            fwrite(&glb_ds1.group_num, 4, 1, out);

            // groups
            // what is group????????????????????
            for (i=0; i < glb_ds1.group_num; i++)
            {
                fwrite(&glb_ds1.group[i].tile_x, 4, 1, out);
                fwrite(&glb_ds1.group[i].tile_y, 4, 1, out);
                fwrite(&glb_ds1.group[i].width,  4, 1, out);
                fwrite(&glb_ds1.group[i].height, 4, 1, out);
                fwrite(&glb_ds1.group[i].unk,    4, 1, out);
            }
        }

        // npc paths
        fwrite(&npc, 4, 1, out);
        for (i=0; i < npc; i++)
        {
            while ( ! glb_ds1.obj[cur_o].path_num)
                cur_o++;
            fwrite(&glb_ds1.obj[cur_o].path_num, 4, 1, out);
            fwrite(&glb_ds1.obj[cur_o].x,        4, 1, out);
            fwrite(&glb_ds1.obj[cur_o].y,        4, 1, out);
            for (p=0; p < glb_ds1.obj[cur_o].path_num; p++)
            {
                fwrite(&glb_ds1.obj[cur_o].path[p].x,      4, 1, out);
                fwrite(&glb_ds1.obj[cur_o].path[p].y,      4, 1, out);
                fwrite(&glb_ds1.obj[cur_o].path[p].action, 4, 1, out);
            }
            cur_o++;
        }
    }

    // end
    fclose(out);
}















typedef struct {
    uint32_t    version;
    int32_t     width;
    int32_t     height;

    int32_t     wall_num;
    int32_t     floor_num;
    int32_t     shadow_num;
    int32_t     tag_num;
}D2SENSE;

D2SENSE     glb_sense;

// ==========================================================================
// memory free of a ds1 (with the dt1 it uses, and some other buffers)
int ds1_free()
{
    int d, total = 0;

    if (strlen(glb_ds1.name) == 0){
        return 0;
    }

    fprintf(stderr, "      . %s\n", glb_ds1.name);
    fflush(stderr);

    // dt1 of this ds1
    for (d=0; d<DT1_IN_DS1_MAX; d++){
        dt1_del(glb_ds1.dt1_idx[d]);
    }

    // block table
    if(glb_ds1.block_table != NULL){
        total += glb_ds1.bt_num * sizeof(BLOCK_TABLE_S);
        free(glb_ds1.block_table);
    }

    // file buffer
    if(glb_ds1.file_buff != NULL){
        total += glb_ds1.file_len;
        free(glb_ds1.file_buff);
    }

    // floors
    if(glb_ds1.floor_buff != NULL){
        total += glb_ds1.floor_buff_len;
        free(glb_ds1.floor_buff);
    }

    // shadow
    if (glb_ds1.shadow_buff != NULL){
        total += glb_ds1.shadow_buff_len;
        free(glb_ds1.shadow_buff);
    }

    // tag
    if (glb_ds1.tag_buff != NULL){
        total += glb_ds1.tag_buff_len;
        free(glb_ds1.tag_buff);
    }

    // walls
    if (glb_ds1.wall_buff != NULL) {
        total += glb_ds1.wall_buff_len;
        free(glb_ds1.wall_buff);
    }

    // groups
    if (glb_ds1.group != NULL){
        free(glb_ds1.group);
    }

    // end
    memset(&glb_ds1, 0, sizeof(DS1_S));
    fprintf(stderr, "         bytes free = %i\n", total);
    fflush(stderr);
    return total;
}


// ==========================================================================
// search the wall tile, giving the orientation/prop1/2/3/4
void wprop_2_block(int i, CELL_W_S * w_ptr)
{
    long             orientation;
    long             main_index;
    long             sub_index;
    int              b;

    BLOCK_TABLE_S    * bt_ptr = glb_ds1.block_table;

    w_ptr->bt_idx = -1; // not found by default
    if (w_ptr->prop1 == 0) {
        // 第一个性质: 指定当前是否有tile?
        // no tile here
        w_ptr->bt_idx = 0;
        return;
    }

    orientation = w_ptr->orientation;
    main_index  = (w_ptr->prop3 >> 4) + ((w_ptr->prop4 & 0x03) << 4);
    sub_index   = w_ptr->prop2;
    for (b=0; b<glb_ds1.bt_num; b++) {
        if ( (bt_ptr->orientation == orientation) && (bt_ptr->main_idx  == main_index)  && (bt_ptr->sub_idx   == sub_index)   && (bt_ptr->used_by_editor)) {
            w_ptr->bt_idx = b;
            return;
        }
        bt_ptr++;
    }

    // trick of O=18 M=3 I=1 ---> O=19 M=3 I=0
    if ((orientation == 18) || (orientation == 19)) {
        if (orientation == 18) {
            orientation = 19;
        } else {
            orientation = 18;
        }

        // search with same sub_index (just in case)
        bt_ptr = glb_ds1.block_table;
        for (b=0; b<glb_ds1.bt_num; b++) {
            if ( (bt_ptr->orientation == orientation) && (bt_ptr->main_idx  == main_index)  && (bt_ptr->sub_idx   == sub_index)   && (bt_ptr->used_by_editor) ) {
                w_ptr->bt_idx = b;
                return;
            }
            bt_ptr++;
        }

        // search with sub_index = 0
        sub_index = 0;
        bt_ptr = glb_ds1.block_table;
        for (b=0; b<glb_ds1.bt_num; b++) {
            if ( (bt_ptr->orientation == orientation) && (bt_ptr->main_idx  == main_index)  && (bt_ptr->sub_idx   == sub_index)   && (bt_ptr->used_by_editor) ) {
                w_ptr->bt_idx = b;
                return;
            }
            bt_ptr++;
        }
    }
}


// ==========================================================================
// search the floor tile, giving the prop1/2/3/4
void fprop_2_block(int i, CELL_F_S * f_ptr)
{
    long          main_index, sub_index;
    int           b;
    BLOCK_TABLE_S * bt_ptr = glb_ds1.block_table;

    f_ptr->bt_idx = -1; // not found by default
    if (f_ptr->prop1 == 0) {
        // no tile here
        f_ptr->bt_idx = 0;
        return;
    }

    main_index  = (f_ptr->prop3 >> 4) + ((f_ptr->prop4 & 0x03) << 4);
    sub_index   = f_ptr->prop2;
    for (b=0; b<glb_ds1.bt_num; b++) {
        if ( (bt_ptr->orientation == 0)          && (bt_ptr->main_idx  == main_index) && (bt_ptr->sub_idx   == sub_index)  && (bt_ptr->used_by_editor) ) {
            f_ptr->bt_idx = b;
            return;
        }
        bt_ptr++;
    }
}


// ==========================================================================
// search the shadow tile, giving the prop1/2/3/4
void sprop_2_block(int i, CELL_S_S * s_ptr)
{
    BLOCK_TABLE_S * bt_ptr = glb_ds1.block_table;
    long          main_index, sub_index;
    int           b;

    s_ptr->bt_idx = -1; // not found by default
    if (s_ptr->prop1 == 0) {
        // no tile here
        s_ptr->bt_idx = 0;
        return;
    }

    main_index  = (s_ptr->prop3 >> 4) + ((s_ptr->prop4 & 0x03) << 4);
    sub_index   = s_ptr->prop2;
    for (b=0; b<glb_ds1.bt_num; b++) {
        if ( (bt_ptr->orientation == 13)         && (bt_ptr->main_idx  == main_index) && (bt_ptr->sub_idx   == sub_index)  && (bt_ptr->used_by_editor) ) {
            s_ptr->bt_idx = b;
            return;
        }
        bt_ptr++;
    }
}


// ==========================================================================
// search all the tiles (ds1 to dt1 relations)
void ds1_make_prop_2_block(int i)
{
    CELL_F_S         *f_ptr;
    CELL_S_S         *s_ptr;
    CELL_W_S         *w_ptr;

    int              x, y, n;

    w_ptr = glb_ds1.wall_buff;
    f_ptr = glb_ds1.floor_buff;
    s_ptr = glb_ds1.shadow_buff;

    for (y=0; y<glb_ds1.height; y++) {
        for (x=0; x<glb_ds1.width; x++) {

            for (n=0; n<glb_ds1.wall_num; n++) {
                // 这里是将wall的prop1~prop4和block_table(也就是dt1文件内容)联系起来的关键部分!!!!!!!!!!!!!!!!!!!!!!!!!{{{
                // 其他也是把 看一下重要部分
                //
                //   w_ptr->bt_idx = -1; // not found by default
                //   if (w_ptr->prop1 == 0) {
                //       // 第一个性质: 指定当前是否有tile?
                //       // no tile here
                //      w_ptr->bt_idx = 0;
                //      return;
                //   }
                //   }}}

                //   orientation = w_ptr->orientation;
                //   main_index  = (w_ptr->prop3 >> 4) + ((w_ptr->prop4 & 0x03) << 4);
                //   sub_index   = w_ptr->prop2;
                wprop_2_block(i, w_ptr);
                w_ptr++;
            }
            for (n=0; n<glb_ds1.floor_num; n++) {
                fprop_2_block(i, f_ptr);
                f_ptr++;
            }
            for (n=0; n<glb_ds1.shadow_num; n++)
            {
                sprop_2_block(i, s_ptr);
                /*
                   if (s_ptr->prop1)
                   {
                   printf("shadow %3i, %3i : %3i %3i %3i %3i --> ",
                   x, y,
                   s_ptr->prop1,
                   s_ptr->prop2,
                   s_ptr->prop3,
                   s_ptr->prop4);
                   sprop_2_block(i, s_ptr);
                   if (s_ptr->bt_idx == -1)
                   printf("NOT FOUND with orientation 13\n");
                   else
                   printf("block table %i\n", s_ptr->bt_idx);
                   }
                   else
                   sprop_2_block(i, s_ptr);
                   */ 
                s_ptr++;
            }
        }
    }
}


// ==========================================================================
// read a ds1
//    load the file into memory, then copy the datas into the right structures
//    init some var, to prepare its display (zoom, center, layer mask...)
// if new_width OR new_height is <= 0, then no change to the size of the ds1, else
// it'll crop or add necessary Tiles 
//
// ds1文件中到底有什么？我关心的
// 有版本信息 有长宽 有wall这些的layer层数
// wall floor shadow 这些的prop1 prop2 prop3 prop4
// 但是没有他们的图像数据
// 
// 有obj相对于tile下的subtile的xy偏移
// 最关键的有obj在obj.txt中的desc_id
// 
int ds1_read(const char * ds1name, int ds1_idx, int new_width, int new_height)
{
    FILE        * in;
    CELL_F_S    * f_ptr[FLOOR_MAX_LAYER];
    CELL_S_S    * s_ptr[SHADOW_MAX_LAYER];
    CELL_T_S    * t_ptr[TAG_MAX_LAYER];
    CELL_W_S    * w_ptr[WALL_MAX_LAYER];
    CELL_W_S    * o_ptr[WALL_MAX_LAYER];
    OBJ_LABEL_S * label;

    int         o, x, y, nb_layer, size, n, p, ds1_len, done, cx, cy, dx, dy;
    int         current_valid_obj_idx=0;
    int         max_subtile_width;
    int         max_subtile_height;
    long        w_num, f_num, s_num, t_num, * ptr, npc, path;
    int         lay_stream[14];
    int         dir_lookup[25] = {
        0x00, 0x01, 0x02, 0x01, 0x02,
        0x03, 0x03, 0x05, 0x05, 0x06,
        0x06, 0x07, 0x07, 0x08, 0x09,
        0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
        0x0F, 0x10, 0x11, 0x12, 0x14 };
    UBYTE       * bptr;
    void        * ds1_buff;
    int         obj_path_warn_wrote = FALSE, last_o, nb;
    char        tmp[150], * cptr;
    long        tile_x, tile_y, zoom;
    long        incr;


    strcpy(glb_ds1.name, ds1name);

    // initialise max number of objects
    // 100
    // 作者把这类max都存储到变量内部了
    glb_ds1.current_obj_max = OBJ_MAX_START;

    size = glb_ds1.current_obj_max * sizeof(int);
    glb_ds1.drawing_order = (int *)calloc(glb_ds1.current_obj_max, sizeof(int));
    if(glb_ds1.drawing_order == NULL){
        FATAL_EXIT("not enough mem (%i bytes) for glb_ds1.drawing_order of %s\n", size, ds1name);
    }

    // obj
    size = glb_ds1.current_obj_max * sizeof(OBJ_S);
    glb_ds1.obj = (OBJ_S *) calloc(glb_ds1.current_obj_max, sizeof(OBJ_S));
    if (glb_ds1.obj == NULL){
        FATAL_EXIT("not enough mem (%i bytes) for glb_ds1.obj of %s\n", size, ds1name);
    }


    // layers mask
    for (x=0; x < WALL_MAX_LAYER; x++){
        glb_ds1.wall_layer_mask[x] = 1;
    }

    for (x=0; x < FLOOR_MAX_LAYER; x++){
        glb_ds1.floor_layer_mask[x] = 1;
    }

    for (x=0; x < SHADOW_MAX_LAYER; x++){
        glb_ds1.shadow_layer_mask[x] = 3; // transparent is default
    }

    glb_ds1.objects_layer_mask    = OL_NONE;
    glb_ds1.paths_layer_mask      = 1;
    glb_ds1.walkable_layer_mask   = 0;
    glb_ds1.subtile_help_display  = 1;
    glb_ds1.animations_layer_mask = 1;
    glb_ds1.special_layer_mask    = 1;

    // load from disk into memory
    //读入一个.ds1
    in = fopen(ds1name, "rb");
    if(in == NULL){
        FATAL_EXIT("can't open %s\n", ds1name);
    }
    fseek(in, 0, SEEK_END);
    ds1_len = ftell(in);
    fseek(in, 0, SEEK_SET);
    ds1_buff = (void *) malloc(ds1_len);
    if(ds1_buff == NULL){
        fclose(in);
        FATAL_EXIT("not enough mem (%i bytes) for %s\n", ds1_len, ds1name);
    }
    fread(ds1_buff, ds1_len, 1, in);
    fclose(in);

    // inits
    w_num = 0; // # of wall & orientation layers
    f_num = 0; // # of floor layer
    s_num = 1; // # of shadow layer, always here
    t_num = 0; // # of tag layer
    ptr   = (long *)ds1_buff;


    if(glb_map_test){

        // version width height {{{
        glb_ds1.version = *(ptr++);

        // widht
        glb_ds1.width = *(ptr++);

        // height
        glb_ds1.height = *(ptr++);

        // act
        glb_ds1.act = *(ptr++);
        // }}}

        // filenames {{{
        glb_ds1.file_num = *(ptr++);
        n = 0;
        printf("filenames       : %li\n", glb_ds1.file_num);

        for(x=0; x<glb_ds1.file_num; x++){
            // 我调试的这个ds1中file_num = 9 {{{
            // 分别是:
            // 1 : \d2\data\global\tiles\act4\mesa\floor.tg1
            // 2 : \d2\data\global\tiles\act4\mesa\inv_wall.tg1
            // 3 : \d2\data\global\tiles\act4\mesa\stairs.tg1
            // 4 : \d2\data\global\tiles\act4\fort\foregate.tg1
            // 5 : \d2\data\global\tiles\act4\fort\plaza.tg1
            // 6 : \d2\data\global\tiles\act4\fort\interior.tg1
            // 7 : \d2\data\global\tiles\act4\fort\exterior.tg1
            // 8 : \d2\data\global\tiles\act1\barracks\warp.tg1
            // 9 : \d2\data\global\tiles\act1\barracks\inviswal.tg1
            // 没有移动ptr 而是移动下标n
            // 有个strlen,所以必然每个文件名后有一个\0,不然不能工作
            // 这里只是打印了每个文件名
            // 读入.file_buff在后面 }}}
            printf("   %2i : %s\n", x + 1, ((char *) ptr) + n);
            n += (strlen( ((char *) ptr) + n) + 1);
        }

        glb_ds1.file_buff = (void *) malloc(n);
        if(glb_ds1.file_buff == NULL){
            free(ds1_buff);
            FATAL_EXIT("not enough mem (%i bytes) for files in the ds1\n", n);
        }
        glb_ds1.file_len = n;
        //读入所有文件名
        //到一个文件缓冲区
        memcpy(glb_ds1.file_buff, ptr, n);
        ptr = (long *) (((char *) ptr) + n);
        // }}}

        // floor, wall, shadow 决定自己写这一块，作者的方法太难懂了{{{
        // 虽然我懂了
        {
            CELL_F_S    *f_p;
            CELL_S_S    *s_p;
            CELL_W_S    *w_p;

            glb_ds1.wall_num   = *(ptr++);//4
            glb_ds1.floor_num  = *(ptr++);//2
            glb_ds1.shadow_num = *(ptr++);//1

            glb_sense.version    = glb_ds1.version;
            glb_sense.width      = glb_ds1.width;
            glb_sense.height     = glb_ds1.height;

            glb_sense.floor_num  =  glb_ds1.floor_num;
            glb_sense.wall_num   =  glb_ds1.wall_num;
            glb_sense.shadow_num =  glb_ds1.shadow_num;


            // floor buffer
            glb_ds1.floor_line     = new_width * glb_ds1.floor_num;
            glb_ds1.floor_len      = glb_ds1.floor_line * new_height;
            glb_ds1.floor_buff_len = glb_ds1.floor_len * sizeof(CELL_F_S);
            glb_ds1.floor_buff     = (CELL_F_S *) malloc(glb_ds1.floor_buff_len);
            if(glb_ds1.floor_buff == NULL){
                free(ds1_buff);
                FATAL_EXIT("not enough mem (%i bytes) for floor buffer\n", glb_ds1.floor_buff_len);
            }
            memset(glb_ds1.floor_buff, 0, glb_ds1.floor_buff_len);


            // shadow buffer
            glb_ds1.shadow_line     = new_width * glb_ds1.shadow_num;
            glb_ds1.shadow_len      = glb_ds1.shadow_line * new_height;
            glb_ds1.shadow_buff_len = glb_ds1.shadow_len * sizeof(CELL_S_S);
            glb_ds1.shadow_buff     = (CELL_S_S *) malloc(glb_ds1.shadow_buff_len);
            if(glb_ds1.shadow_buff == NULL){
                free(ds1_buff);
                FATAL_EXIT("not enough mem (%i bytes) for shadow buffer\n", glb_ds1.shadow_buff_len);
            }
            memset(glb_ds1.shadow_buff, 0, glb_ds1.shadow_buff_len);



            // wall buffer
            glb_ds1.wall_line     = new_width * glb_ds1.wall_num;
            glb_ds1.wall_len      = glb_ds1.wall_line * new_height;
            glb_ds1.wall_buff_len = glb_ds1.wall_len * sizeof(CELL_W_S);
            glb_ds1.wall_buff     = (CELL_W_S *) malloc(glb_ds1.wall_buff_len);
            if(glb_ds1.wall_buff == NULL){
                free(ds1_buff);
                FATAL_EXIT("not enough mem (%i bytes) for wall buffer\n", glb_ds1.wall_buff_len);
            }
            memset(glb_ds1.wall_buff, 0, glb_ds1.wall_buff_len);


            // read tiles of layers
            bptr  = (UBYTE *) ptr;


            // wall
            w_p=glb_ds1.wall_buff;
            for(n=0; n < glb_ds1.wall_num; n++){
                for(y=0; y < glb_ds1.height; y++){
                    for(x=0; x < glb_ds1.width; x++){
                        w_p->prop1 = *(bptr++);
                        w_p->prop2 = *(bptr++);
                        w_p->prop3 = *(bptr++);
                        w_p->prop4 = *(bptr++);
                        w_p->orientation = *(bptr++); //注意这里
                        w_p++;
                    }
                }
            }

            // floor
            f_p=glb_ds1.floor_buff;
            for(n=0; n < glb_ds1.floor_num; n++){
                for(y=0; y < glb_ds1.height; y++){
                    for(x=0; x < glb_ds1.width; x++){
                        f_p->prop1 = *(bptr++);
                        f_p->prop2 = *(bptr++);
                        f_p->prop3 = *(bptr++);
                        f_p->prop4 = *(bptr++);
                        f_p++;
                    }
                }
            }
            //shadow
            s_p=glb_ds1.shadow_buff;
            for(n=0; n < glb_ds1.shadow_num; n++){
                for(y=0; y < glb_ds1.height; y++){
                    for(x=0; x < glb_ds1.width; x++){
                        s_p->prop1 = *(bptr++);
                        s_p->prop2 = *(bptr++);
                        s_p->prop3 = *(bptr++);
                        s_p->prop4 = *(bptr++);
                        s_p++;
                    }
                }
            }

        }
        // }}}

        // now we're on the objects data
        ptr = (long *) bptr;

        glb_ds1.obj_num = 0;
        if (glb_ds1.version >= 2){
            glb_ds1.obj_num = *(ptr++);

            printf("objects         : %li\n", glb_ds1.obj_num);

            if(glb_ds1.obj_num > glb_ds1.current_obj_max){
                // 太多肿么办, 增加一些内存
                incr = 1 + glb_ds1.obj_num - glb_ds1.current_obj_max;
                if (misc_increase_ds1_objects_max(ds1_idx, incr) != 0){
                    free(ds1_buff);
                    FATAL_EXIT("too many objects (%ld), editor max is <%i>\n", glb_ds1.obj_num, glb_ds1.current_obj_max);
                }
            }

            current_valid_obj_idx = 0;
            // width和height都是以tile为单位
            // 而一个tile有5个subtile..所以这里这样
            max_subtile_width     = new_width * 5;
            max_subtile_height    = new_height * 5;
            for(n=0; n < glb_ds1.obj_num; n++){
                //初始化object的都在这里..貌似一个一个的填写~
                glb_ds1.obj[current_valid_obj_idx].type  = *ptr;
                ptr++;
                glb_ds1.obj[current_valid_obj_idx].id    = *ptr;
                ptr++;
                x = glb_ds1.obj[current_valid_obj_idx].x = *ptr;
                ptr++;
                y = glb_ds1.obj[current_valid_obj_idx].y = *ptr;
                ptr++;

                // 我的调试, 测试x是全局还是当前tile内部 输出结果如下, 这个结果让我有点迷茫, 显然不是全局的 {{{
                // 但是也不像tile内部的啊~ 因为tile的大小为160*80, 但是这里有( 152, 97 )出现
                // 奇怪 这里的glb_ds1.obj[m].x和y到底是相对哪里的，很奇怪
                // 反正不该是tile（就是cell？）的像素的
                // 难道是相对于subtile的恒纵坐标的？看下面的比较关系，因该是的
                //
                // if((x >= 0) && (x < max_subtile_width) && (y >= 0) && (y < max_subtile_height)):
                // x =   55, y =   33
                // x =   55, y =   47
                // x =  155, y =   97
                // x =  152, y =   97
                // x =  147, y =   72
                // x =  143, y =   72
                // x =  129, y =   84
                // x =  128, y =   79
                // x =  125, y =   68
                // x =  121, y =   67
                // x =   92, y =   43
                // x =   90, y =   39
                // x =   84, y =   46
                // x =   81, y =   46
                // x =   97, y =   63
                // x =   94, y =   64
                // x =  135, y =   90
                // x =  130, y =   45
                // x =   83, y =   58
                // x =  104, y =   52
                // x =  104, y =   39
                // x =   90, y =   18
                // x =   80, y =   34
                // x =   77, y =   40
                // x =   77, y =   50
                // x =   80, y =   55
                // x =   37, y =   25
                // x =   37, y =   35
                // x =   37, y =   45
                // x =   37, y =   55
                // x =   21, y =   28
                // x =   83, y =   23
                // x =   44, y =   19
                // x =   22, y =    7
                // x =   19, y =   40
                // x =   20, y =   20
                // x =   23, y =    7
                // x =   17, y =   55
                // x =   17, y =   45
                // x =   17, y =   35
                // x =   17, y =   25
                // x =   82, y =   62
                // x =   83, y =   66
                // x =   80, y =   43
                // x =   86, y =   40
                // x =  126, y =   72
                // x =  142, y =   68
                //
                //  }}}
                printf( "x = %4d, y = %4d\n", x, y);

                if (glb_ds1.version > 5){
                    // flags
                    glb_ds1.obj[current_valid_obj_idx].ds1_flags = * ptr;
                    ptr++;
                }

                // integrity check (not done by the game I believe) 看来相对关系是相对于subtile的xy坐标的
                if((x >= 0) && (x < max_subtile_width) && (y >= 0) && (y < max_subtile_height)){
                    // some init for the paths of this object
                    glb_ds1.obj[current_valid_obj_idx].path_num = 0;
                    glb_ds1.obj[current_valid_obj_idx].desc_idx = -1;
                    glb_ds1.obj[current_valid_obj_idx].flags    = 0;

                    glb_ds1.obj[current_valid_obj_idx].frame_delta = rand()%256;

                    // 我现在突然明白, 难道subtile指的是那种菱形方块,subcell指的是正方形小块? {{{
                    //
                    // 也就是说tile和cell是同级的, 只不过一个为菱形, 一个为正方形而已?
                    //typedef struct OBJ_LABEL_S
                    //{
                    //   int rx, ry; // upper/left corner of the label, relative to the sub-cell
                    //               // (in pixels, at zoom of 1:1)
                    //   int w, h;   // width & height (pixels)
                    //   int x0, y0; // pixels position on screen
                    //   int flags;
                    //   // for moving
                    //   int old_rx;
                    //   int old_ry;
                    //} OBJ_LABEL_S;
                    // }}}
                    label = & glb_ds1.obj[current_valid_obj_idx].label;
                    label->rx = label->ry = label->w = label->h = label->flags = 0;

                    //     我吧 editobj_make_obj_desc(ds1_idx, current_valid_obj_idx)这个函数的内容复制到这里来 好好看把 {{{
                    //         这个函数的功能就是在obj.txt中，根据current_valid_obj_idx寻找到在obj.txt中的desc_id
                    //         那个:
                    //              x = glb_ds1.obj[current_valid_obj_idx].x = *ptr;
                    //              y = glb_ds1.obj[current_valid_obj_idx].y = *ptr;
                    //              是相对于subtile的xy坐标的，而每个obj貌似还有个label->rx和ry偏移
                    //                  这个偏移量是相对于subtile内部像素的么？？？？？

                    //     act  = glb_ds1.act;
                    //     type = glb_ds1.obj[obj_idx].type;
                    //     id   = glb_ds1.obj[obj_idx].id;

                    //     // first try : search the exact act/type/id in data\obj.txt
                    //     for(i=0; i < glb_ds1edit.obj_desc_num; i++){
                    //         if( (glb_ds1edit.obj_desc[i].act  == act)  && (glb_ds1edit.obj_desc[i].type == type) && (glb_ds1edit.obj_desc[i].id   == id) ){
                    //             // end
                    //             glb_ds1.obj[obj_idx].desc_idx = i;
                    //             return;
                    //         }
                    //     }
                    //
                    // }}}
                    editobj_make_obj_desc(ds1_idx, current_valid_obj_idx);
                    current_valid_obj_idx++;
                }else{
                    // don't use that object
                    memset( & glb_ds1.obj[current_valid_obj_idx], 0, sizeof(glb_ds1.obj[current_valid_obj_idx]) );
                }
            }
        }else{
            printf("no objects\n");
        }

        // update the new number of objects in that ds1
        glb_ds1.obj_num = current_valid_obj_idx;


        // 前面是读取npc路径的
        // 我删去了
        // 但是。。npc也是obj
        // 所以大哦现在才sort
        // sort objects, for drawing purpose
        // 很关键的函数~
        editobj_set_drawing_order(ds1_idx);

        // internal
        glb_ds1.cur_zoom           = ZM_11;
        glb_ds1.tile_w             = 160;
        glb_ds1.tile_h             = 80;
        glb_ds1.height_mul         = 1;
        glb_ds1.height_div         = 1;
        glb_ds1.cur_scroll.keyb.x  = glb_config.scroll.keyb.x;
        glb_ds1.cur_scroll.keyb.y  = glb_config.scroll.keyb.y;
        glb_ds1.cur_scroll.mouse.x = glb_config.scroll.mouse.x;
        glb_ds1.cur_scroll.mouse.y = glb_config.scroll.mouse.y;

        // center it
        cx = glb_ds1.width/2 + 1;
        cy = glb_ds1.height/2;
        dx = (cy * -glb_ds1.tile_w / 2) + (cx * glb_ds1.tile_w / 2);
        dy = (cy *  glb_ds1.tile_h / 2) + (cx * glb_ds1.tile_h / 2);

        glb_ds1.own_wpreview.x0 = dx - glb_config.screen.width  / 2;
        glb_ds1.own_wpreview.y0 = dy - glb_config.screen.height / 2;
        glb_ds1.own_wpreview.w  = glb_config.screen.width;
        glb_ds1.own_wpreview.h  = glb_config.screen.height;

        change_zoom(ds1_idx, ZM_14); // start with zoom of 1:4 in this ds1

        // some inits
        tile_x = tile_y = zoom = -1;


        // end, some last init
        free(ds1_buff);
        strncpy(glb_ds1.undo.tag, "UNDO", sizeof(glb_ds1.undo.tag));
        glb_ds1.undo.cur_buf_num = -1;
        glb_ds1.path_edit_win.obj_idx = -1;

        ds1_save(0,TRUE);
    }else{
        // version
        glb_ds1.version = *ptr;
        ptr++;

        // widht
        // width 和height都加了1, 为什吗
        glb_ds1.width = (* ptr) + 1;
        ptr++;

        // height
        glb_ds1.height = (* ptr) + 1;
        ptr++;

        // will we use new size ?
        if ((new_width <= 0) || (new_height <= 0)) {
            new_width  = glb_ds1.width;
            new_height = glb_ds1.height;
        }

        // act
        // act1到底是个神马意思
        glb_ds1.act = 1;
        //我调试的时候version是18了
        if(glb_ds1.version >= 8){
            glb_ds1.act = (* ptr) + 1;
            ptr++;
            if (glb_ds1.act > 5){
                glb_ds1.act = 5;
            }
        }

        // tag_type
        glb_ds1.tag_type = 0;
        if(glb_ds1.version >= 10){
            glb_ds1.tag_type = *ptr;
            ptr++;

            // adjust eventually the # of tag layer
            if ((glb_ds1.tag_type == 1) || (glb_ds1.tag_type == 2)){
                t_num = 1;
            }
        }

        // debug
        printf("\nversion         : %li\n", glb_ds1.version);
        printf("width           : %li\n", glb_ds1.width);
        printf("height          : %li\n", glb_ds1.height);
        printf("act             : %li\n", glb_ds1.act);
        printf("tag_type        : %li\n", glb_ds1.tag_type);



        // filenames
        glb_ds1.file_num = 0;
        if(glb_ds1.version >= 3){
            glb_ds1.file_num = * ptr;
            ptr++;
            n = 0;
            printf("filenames       : %li\n", glb_ds1.file_num);

            for(x=0; x<glb_ds1.file_num; x++){
                // 我调试的这个ds1中file_num = 9 {{{
                // 分别是:
                // 1 : \d2\data\global\tiles\act4\mesa\floor.tg1
                // 2 : \d2\data\global\tiles\act4\mesa\inv_wall.tg1
                // 3 : \d2\data\global\tiles\act4\mesa\stairs.tg1
                // 4 : \d2\data\global\tiles\act4\fort\foregate.tg1
                // 5 : \d2\data\global\tiles\act4\fort\plaza.tg1
                // 6 : \d2\data\global\tiles\act4\fort\interior.tg1
                // 7 : \d2\data\global\tiles\act4\fort\exterior.tg1
                // 8 : \d2\data\global\tiles\act1\barracks\warp.tg1
                // 9 : \d2\data\global\tiles\act1\barracks\inviswal.tg1
                // 没有移动ptr 而是移动下标n
                // 有个strlen,所以必然每个文件名后有一个\0,不然不能工作
                // 这里只是打印了每个文件名
                // 读入.file_buff在后面 }}}
                printf("   %2i : %s\n", x + 1, ((char *) ptr) + n);
                n += (strlen( ((char *) ptr) + n) + 1);
            }

            glb_ds1.file_buff = (void *) malloc(n);
            if(glb_ds1.file_buff == NULL){
                free(ds1_buff);
                FATAL_EXIT("not enough mem (%i bytes) for files in the ds1\n", n);
            }
            glb_ds1.file_len = n;
            //读入所有文件名
            //到一个文件缓冲区
            memcpy(glb_ds1.file_buff, ptr, n);
            ptr = (long *) (((char *) ptr) + n);
        } else{
            printf("no filenames\n");
        }

        // skip 2 dwords ?
        if ((glb_ds1.version >= 9) && (glb_ds1.version <= 13)){
            ptr += 2;
        }

        // number of wall, floor and tag layers
        //tag layer到底是个神马东西
        if(glb_ds1.version >= 4){
            // 进入
            // number of wall (and orientation) layers
            w_num = * ptr;
            ptr++;

            // number of floor layers
            if(glb_ds1.version >= 16){
                f_num = * ptr;
                ptr++;
            }else{
                f_num = 1; // default # of floor layer
            }
        }else{
            // in version < 4
            // # of layers hardcoded
            w_num = 1;
            f_num = 1;
            t_num = 1;
        }

        // which order ?
        if (glb_ds1.version < 4) {
            lay_stream[0] =  1; // wall 1
            lay_stream[1] =  9; // floor 1
            lay_stream[2] =  5; // orientation 1
            lay_stream[3] = 12; // tag
            lay_stream[4] = 11; // shadow
            nb_layer = 5;
        }else{
            // 运行这里
            // lay_stream是个神马东西
            // 这里彻底迷茫看不懂
            // 最后生成的:
            // lay_stream[]的值如下:
            // N表示没有初始化
            //    lay_stream[0]  = 1
            //    lay_stream[1]  = 5
            //    lay_stream[2]  = 2
            //    lay_stream[3]  = 6
            //    lay_stream[4]  = 3
            //    lay_stream[5]  = 7
            //    lay_stream[6]  = 4
            //    lay_stream[7]  = 8
            //    lay_stream[8]  = 9
            //    lay_stream[9]  = 10
            //    lay_stream[10] = 11
            //    lay_stream[11] = N
            //    lay_stream[12] = N
            //    lay_stream[13] = N
            nb_layer = 0;
            for (x=0; x<w_num; x++) {
                lay_stream[nb_layer++] = 1 + x; // wall x
                lay_stream[nb_layer++] = 5 + x; // orientation x
            }
            for (x=0; x<f_num; x++){
                lay_stream[nb_layer++] = 9 + x; // floor x
            }
            if (s_num){
                lay_stream[nb_layer++] = 11;    // shadow
            }
            if (t_num){
                lay_stream[nb_layer++] = 12;    // tag
            }
        }
        // 输出: layers          : (2 * 4 walls) + 2 floors + 1 shadow + 0 tag
        // 为什么wall是2*4呢????
        printf("layers:  (2 * %li walls) + %li floors + %li shadow" " + %li tag\n", w_num, f_num, s_num, t_num);

        // layers num
        if(glb_config.always_max_layers){
            f_num = FLOOR_MAX_LAYER;
            w_num = WALL_MAX_LAYER;
        }
        glb_ds1.floor_num  = f_num;    //2
        glb_ds1.shadow_num = s_num;    //1
        glb_ds1.tag_num    = t_num;    //0
        glb_ds1.wall_num   = w_num;    //4

        glb_sense.version    = glb_ds1.version;
        glb_sense.width      = glb_ds1.width;
        glb_sense.height     = glb_ds1.height;
        glb_sense.wall_num   = w_num;
        glb_sense.floor_num  = f_num;
        glb_sense.tag_num    = t_num;

        //以下是四组buffer
        // floor buffer
        glb_ds1.floor_line     = new_width * glb_ds1.floor_num;
        glb_ds1.floor_len      = glb_ds1.floor_line * new_height;
        glb_ds1.floor_buff_len = glb_ds1.floor_len * sizeof(CELL_F_S);
        glb_ds1.floor_buff     = (CELL_F_S *) malloc(glb_ds1.floor_buff_len);
        if(glb_ds1.floor_buff == NULL){
            free(ds1_buff);
            FATAL_EXIT("not enough mem (%i bytes) for floor buffer\n", glb_ds1.floor_buff_len);
        }
        memset(glb_ds1.floor_buff, 0, glb_ds1.floor_buff_len);


        // shadow buffer
        glb_ds1.shadow_line     = new_width * glb_ds1.shadow_num;
        glb_ds1.shadow_len      = glb_ds1.shadow_line * new_height;
        glb_ds1.shadow_buff_len = glb_ds1.shadow_len * sizeof(CELL_S_S);
        glb_ds1.shadow_buff     = (CELL_S_S *) malloc(glb_ds1.shadow_buff_len);
        if(glb_ds1.shadow_buff == NULL){
            free(ds1_buff);
            FATAL_EXIT("not enough mem (%i bytes) for shadow buffer\n", glb_ds1.shadow_buff_len);
        }
        memset(glb_ds1.shadow_buff, 0, glb_ds1.shadow_buff_len);


        // tag buffer
        glb_ds1.tag_line     = new_width * glb_ds1.tag_num;
        glb_ds1.tag_len      = glb_ds1.tag_line * new_height;
        glb_ds1.tag_buff_len = glb_ds1.tag_len * sizeof(CELL_T_S);
        glb_ds1.tag_buff     = (CELL_T_S *) malloc(glb_ds1.tag_buff_len);
        if (glb_ds1.tag_buff == NULL) {
            free(ds1_buff);
            FATAL_EXIT("not enough mem (%i bytes) for tag buffer\n", glb_ds1.tag_buff_len);
        }
        memset(glb_ds1.tag_buff, 0, glb_ds1.tag_buff_len);


        // wall buffer
        glb_ds1.wall_line     = new_width * glb_ds1.wall_num;
        glb_ds1.wall_len      = glb_ds1.wall_line * new_height;
        glb_ds1.wall_buff_len = glb_ds1.wall_len * sizeof(CELL_W_S);
        glb_ds1.wall_buff     = (CELL_W_S *) malloc(glb_ds1.wall_buff_len);
        if(glb_ds1.wall_buff == NULL){
            free(ds1_buff);
            FATAL_EXIT("not enough mem (%i bytes) for wall buffer\n", glb_ds1.wall_buff_len);
        }
        memset(glb_ds1.wall_buff, 0, glb_ds1.wall_buff_len);


        // read tiles of layers

        // set pointers
        for (x=0; x<FLOOR_MAX_LAYER; x++){
            f_ptr[x] = glb_ds1.floor_buff + x;
        }

        for (x=0; x<SHADOW_MAX_LAYER; x++){
            s_ptr[x] = glb_ds1.shadow_buff + x;
        }

        for (x=0; x<TAG_MAX_LAYER; x++){
            t_ptr[x] = glb_ds1.tag_buff + x;
        }

        for (x=0; x<WALL_MAX_LAYER; x++){
            o_ptr[x] = w_ptr[x] = glb_ds1.wall_buff + x;
        }

        bptr  = (UBYTE *) ptr;

        // how to fill all blocks {{{
        // 首先  不同的版本下layer的层数不同
        // 我看到的这种floor为2层，wall为4层
        //
        // 上面先根据版本决定nb_layer的值。
        // 然后，最外层循环是nb_layer，并且根据下面这个表：
        //    lay_stream[0]  = 1
        //    lay_stream[1]  = 5
        //    lay_stream[2]  = 2
        //    lay_stream[3]  = 6
        //    lay_stream[4]  = 3
        //    lay_stream[5]  = 7
        //    lay_stream[6]  = 4
        //    lay_stream[7]  = 8
        //    lay_stream[8]  = 9
        //    lay_stream[9]  = 10
        //    lay_stream[10] = 11
        //    lay_stream[11] = N
        //    lay_stream[12] = N
        //    lay_stream[13] = N
        //
        // 下面的循环简写成如下：
        //foreach( nb_layer ){
        //    foreach( tiles块 ){
        //        case 1 2 3 4:
        //            填wall
        //        case 5 6 7 8:
        //            填orientation
        //        case 9 10:
        //            填floor
        //        case 11:
        //            填shadow
        //        case 12:
        //            填tag
        //    }
        //}
        //
        //
        //  再根据上面lay_stream[n]的值表，可以看出
        //  第一轮填wall
        //  第二轮填orientation
        //  第三轮wall
        //  第四轮orientation
        //  交替
        //  ....
        //  第九轮和第十轮填floor
        //  第十一轮填shadow
        //  第十二轮填tag
        //
        //  有可能并不能持续到第十二轮，因为nb_layer并不一定到12
        //
        //  因此数据在ds1中的格式就是：
        //  所有第一层wall数据
        //  所有第一层orientation数据
        //  所有第二层wall数据
        //  所有第二层orientation数据
        //  ....
        //  ....
        //  ....
        //  所有floor第一层数据
        //  所有floor第二层数据
        //  所有shadow数据
        //  所有tag数据
        //  over
        //  每层wall数据都对应的有一层orientation数据的哦~
        //
        // }}}
        for(n=0; n < nb_layer; n++){
            for(y=0; y < glb_ds1.height; y++){
                for(x=0; x < glb_ds1.width; x++){

                    //			这是lay_stream[]的来历..是直接强行赋值的 {{{
                    //			 nb_layer = 0;
                    //			 for (x=0; x<w_num; x++)
                    //			 {
                    //				 lay_stream[nb_layer++] = 1 + x; // wall x
                    //				 lay_stream[nb_layer++] = 5 + x; // orientation x
                    //			 }
                    //			 for (x=0; x<f_num; x++)
                    //				 lay_stream[nb_layer++] = 9 + x; // floor x
                    //			 if (s_num)
                    //				 lay_stream[nb_layer++] = 11;    // shadow
                    //			 if (t_num)
                    //				 lay_stream[nb_layer++] = 12;    // tag
                    //				 }}}
                    switch(lay_stream[n]){
                        //lay_stream[]的值如下:{{{
                        //    lay_stream[0]  = 1
                        //    lay_stream[1]  = 5
                        //    lay_stream[2]  = 2
                        //    lay_stream[3]  = 6
                        //    lay_stream[4]  = 3
                        //    lay_stream[5]  = 7
                        //    lay_stream[6]  = 4
                        //    lay_stream[7]  = 8
                        //    lay_stream[8]  = 9
                        //    lay_stream[9]  = 10
                        //    lay_stream[10] = 11
                        //    }}}

                        // walls
                        case  1:
                        case  2:
                        case  3:
                        case  4:
                            // 这里的1234源自于这一句~{{{
                            //			 for (x=0; x<w_num; x++)
                            //			 {
                            //				 lay_stream[nb_layer++] = 1 + x; // wall x
                            //				 lay_stream[nb_layer++] = 5 + x; // orientation x
                            //			 }
                            //
                            //}}}

                            // ptr = (long *)ds1_buff; ptr是指向的ds1直接读取到内存的数据 {{{
                            // 每次指向一块cell_w_s, 最外层循环一次后, 4块cell_w_s都填满
                            // 然后cell_w_s的前4个分量(如下)连续存放, 所以通过bptr++来访问到并填入
                            // 然后w_ptr += w_num, 注意不是w_ptr++
                            // 这是因为w_ptr是一个 cell_w_s *w_ptr[N], 也可以理解很cell_w_s**变量
                            // 而每个tile有w_num(4)个cell_w_s, 这里最外层循环是n, 内层才是w和h
                            // 相邻两次之间的wptr是跳了一个tile的, 这就需要跳w_num个cell_w_s, 才能到下一个tile的cell_w_s
                            // 如果最内层循环是n, 就是wptr++
                            //
                            //        typedef struct CELL_W_S
                            //        {
                            //           UBYTE prop1;
                            //           UBYTE prop2;
                            //           UBYTE prop3;
                            //           UBYTE prop4;
                            //           ...
                            //         }CELL_W_S;
                            //   }}}
                            if((x < new_width) && (y < new_height)){
                                p                 =   lay_stream[n] - 1; // 1234->0123
                                w_ptr[p]->prop1   =   * bptr;
                                bptr++;
                                w_ptr[p]->prop2   =   * bptr;
                                bptr++;
                                w_ptr[p]->prop3   =   * bptr;
                                bptr++;
                                w_ptr[p]->prop4   =   * bptr;
                                bptr++;
                                w_ptr[p]         +=   w_num;
                            } else{
                                bptr += 4;
                            }
                            break;

                            // orientations
                            // 这里5678也源自于这一句~{{{
                            //			 for (x=0; x<w_num; x++)
                            //			 {
                            //				 lay_stream[nb_layer++] = 1 + x; // wall x
                            //				 lay_stream[nb_layer++] = 5 + x; // orientation x
                            //			 }
                            //    lay_stream[0]  = 1
                            //    lay_stream[1]  = 5
                            //    lay_stream[2]  = 2
                            //    lay_stream[3]  = 6
                            //    lay_stream[4]  = 3
                            //    lay_stream[5]  = 7
                            //    lay_stream[6]  = 4
                            //    lay_stream[7]  = 8
                            //    lay_stream[8]  = 9
                            //    lay_stream[9]  = 10
                            //    lay_stream[10] = 11
                            //    lay_stream[11] = N
                            //    lay_stream[12] = N
                            //    lay_stream[13] = N
                            //    }}}
                        case  5:
                        case  6:
                        case  7:
                        case  8:
                            if((x < new_width) && (y < new_height)){
                                p = lay_stream[n] - 5;
                                if (glb_ds1.version < 7){
                                    o_ptr[p]->orientation = dir_lookup[* bptr];
                                }else{
                                    //执行的这里...
                                    o_ptr[p]->orientation = * bptr;
                                }
                                o_ptr[p] += w_num;
                            }
                            bptr += 4;
                            break;

                            // floors
                            // 9 10是单独的赋值
                        case  9:
                        case 10:
                            if((x < new_width) && (y < new_height)){
                                p               = lay_stream[n] - 9;
                                f_ptr[p]->prop1 = * bptr;
                                bptr++;
                                f_ptr[p]->prop2 = * bptr;
                                bptr++;
                                f_ptr[p]->prop3 = * bptr;
                                bptr++;
                                f_ptr[p]->prop4 = * bptr;
                                bptr++;
                                f_ptr[p]       += f_num;
                            }else{
                                bptr += 4;
                            }
                            break;

                            // shadow
                        case 11:
                            if((x < new_width) && (y < new_height)){
                                p               = lay_stream[n] - 11;
                                s_ptr[p]->prop1 = * bptr;
                                bptr++;
                                s_ptr[p]->prop2 = * bptr;
                                bptr++;
                                s_ptr[p]->prop3 = * bptr;
                                bptr++;
                                s_ptr[p]->prop4 = * bptr;
                                bptr++;
                                s_ptr[p]       += s_num;
                            }else{
                                bptr += 4;
                            }
                            break;

                            // tag
                        case 12:
                            if ((x < new_width) && (y < new_height)){
                                p = lay_stream[n] - 12;
                                t_ptr[p]->num = (UDWORD) * ((UDWORD *) bptr);
                                t_ptr[p] += t_num;
                            }
                            bptr += 4;
                            break;
                    }
                }

                // in case of bigger width
                p = new_width - glb_ds1.width;
                //我调试的时候, 这里面从来没有执行过~
                if(p > 0){
                    switch (lay_stream[n]){
                        // walls
                        case  1:
                        case  2:
                        case  3:
                        case  4:
                            w_ptr[lay_stream[n] - 1] += p * w_num;
                            break;

                            // orientations
                        case  5:
                        case  6:
                        case  7:
                        case  8:
                            o_ptr[lay_stream[n] - 5] += p * w_num;
                            break;

                            // floors
                        case  9:
                        case 10:
                            f_ptr[lay_stream[n] - 9] += p * f_num;
                            break;

                            // shadow
                        case 11:
                            s_ptr[lay_stream[n] - 11] += p * s_num;
                            break;

                            // tag
                        case 12:
                            t_ptr[lay_stream[n] - 12] += p * t_num;
                            break;
                    }
                }
            }
        }

        // update new size of the ds1
        glb_ds1.width  = new_width;
        glb_ds1.height = new_height;

        // now we're on the objects data
        ptr = (long *) bptr;

        // we van get the object nb from the ds1 file
        glb_ds1.obj_num = 0;
        if (glb_ds1.version >= 2){
            glb_ds1.obj_num = *ptr;
            ptr++;

            printf("objects         : %li\n", glb_ds1.obj_num);

            if(glb_ds1.obj_num > glb_ds1.current_obj_max){
                // 太多肿么办, 增加一些内存
                incr = 1 + glb_ds1.obj_num - glb_ds1.current_obj_max;
                if (misc_increase_ds1_objects_max(ds1_idx, incr) != 0){
                    free(ds1_buff);
                    FATAL_EXIT("too many objects (%ld), editor max is <%i>\n", glb_ds1.obj_num, glb_ds1.current_obj_max);
                }
            }

            current_valid_obj_idx = 0;
            // width和height都是以tile为单位
            // 而一个tile有5个subtile..所以这里这样
            max_subtile_width     = new_width * 5;
            max_subtile_height    = new_height * 5;
            for(n=0; n < glb_ds1.obj_num; n++){
                //初始化object的都在这里..貌似一个一个的填写~
                glb_ds1.obj[current_valid_obj_idx].type  = *ptr;
                ptr++;
                glb_ds1.obj[current_valid_obj_idx].id    = *ptr;
                ptr++;
                x = glb_ds1.obj[current_valid_obj_idx].x = *ptr;
                ptr++;
                y = glb_ds1.obj[current_valid_obj_idx].y = *ptr;
                ptr++;

                // 我的调试, 测试x是全局还是当前tile内部 输出结果如下, 这个结果让我有点迷茫, 显然不是全局的 {{{
                // 但是也不像tile内部的啊~ 因为tile的大小为160*80, 但是这里有( 152, 97 )出现
                // 奇怪 这里的glb_ds1.obj[m].x和y到底是相对哪里的，很奇怪
                // 反正不该是tile（就是cell？）的像素的
                // 难道是相对于subtile的恒纵坐标的？看下面的比较关系，因该是的
                //
                // if((x >= 0) && (x < max_subtile_width) && (y >= 0) && (y < max_subtile_height)):
                // x =   55, y =   33
                // x =   55, y =   47
                // x =  155, y =   97
                // x =  152, y =   97
                // x =  147, y =   72
                // x =  143, y =   72
                // x =  129, y =   84
                // x =  128, y =   79
                // x =  125, y =   68
                // x =  121, y =   67
                // x =   92, y =   43
                // x =   90, y =   39
                // x =   84, y =   46
                // x =   81, y =   46
                // x =   97, y =   63
                // x =   94, y =   64
                // x =  135, y =   90
                // x =  130, y =   45
                // x =   83, y =   58
                // x =  104, y =   52
                // x =  104, y =   39
                // x =   90, y =   18
                // x =   80, y =   34
                // x =   77, y =   40
                // x =   77, y =   50
                // x =   80, y =   55
                // x =   37, y =   25
                // x =   37, y =   35
                // x =   37, y =   45
                // x =   37, y =   55
                // x =   21, y =   28
                // x =   83, y =   23
                // x =   44, y =   19
                // x =   22, y =    7
                // x =   19, y =   40
                // x =   20, y =   20
                // x =   23, y =    7
                // x =   17, y =   55
                // x =   17, y =   45
                // x =   17, y =   35
                // x =   17, y =   25
                // x =   82, y =   62
                // x =   83, y =   66
                // x =   80, y =   43
                // x =   86, y =   40
                // x =  126, y =   72
                // x =  142, y =   68
                //
                //  }}}
                printf( "x = %4d, y = %4d\n", x, y);

                if (glb_ds1.version > 5){
                    // flags
                    glb_ds1.obj[current_valid_obj_idx].ds1_flags = * ptr;
                    ptr++;
                }

                // integrity check (not done by the game I believe) 看来相对关系是相对于subtile的xy坐标的
                if((x >= 0) && (x < max_subtile_width) && (y >= 0) && (y < max_subtile_height)){
                    // some init for the paths of this object
                    glb_ds1.obj[current_valid_obj_idx].path_num = 0;
                    glb_ds1.obj[current_valid_obj_idx].desc_idx = -1;
                    glb_ds1.obj[current_valid_obj_idx].flags    = 0;

                    glb_ds1.obj[current_valid_obj_idx].frame_delta = rand()%256;

                    // 我现在突然明白, 难道subtile指的是那种菱形方块,subcell指的是正方形小块? {{{
                    //
                    // 也就是说tile和cell是同级的, 只不过一个为菱形, 一个为正方形而已?
                    //typedef struct OBJ_LABEL_S
                    //{
                    //   int rx, ry; // upper/left corner of the label, relative to the sub-cell
                    //               // (in pixels, at zoom of 1:1)
                    //   int w, h;   // width & height (pixels)
                    //   int x0, y0; // pixels position on screen
                    //   int flags;
                    //   // for moving
                    //   int old_rx;
                    //   int old_ry;
                    //} OBJ_LABEL_S;
                    // }}}
                    label = & glb_ds1.obj[current_valid_obj_idx].label;
                    label->rx = label->ry = label->w = label->h = label->flags = 0;

                    //     我吧 editobj_make_obj_desc(ds1_idx, current_valid_obj_idx)这个函数的内容复制到这里来 好好看把 {{{
                    //         这个函数的功能就是在obj.txt中，根据current_valid_obj_idx寻找到在obj.txt中的desc_id
                    //         那个:
                    //              x = glb_ds1.obj[current_valid_obj_idx].x = *ptr;
                    //              y = glb_ds1.obj[current_valid_obj_idx].y = *ptr;
                    //              是相对于subtile的xy坐标的，而每个obj貌似还有个label->rx和ry偏移
                    //                  这个偏移量是相对于subtile内部像素的么？？？？？

                    //     act  = glb_ds1.act;
                    //     type = glb_ds1.obj[obj_idx].type;
                    //     id   = glb_ds1.obj[obj_idx].id;

                    //     // first try : search the exact act/type/id in data\obj.txt
                    //     for(i=0; i < glb_ds1edit.obj_desc_num; i++){
                    //         if( (glb_ds1edit.obj_desc[i].act  == act)  && (glb_ds1edit.obj_desc[i].type == type) && (glb_ds1edit.obj_desc[i].id   == id) ){
                    //             // end
                    //             glb_ds1.obj[obj_idx].desc_idx = i;
                    //             return;
                    //         }
                    //     }
                    //
                    // }}}
                    editobj_make_obj_desc(ds1_idx, current_valid_obj_idx);
                    current_valid_obj_idx++;
                }else{
                    // don't use that object
                    memset( & glb_ds1.obj[current_valid_obj_idx], 0, sizeof(glb_ds1.obj[current_valid_obj_idx]) );
                }
            }
        }else{
            printf("no objects\n");
        }

        // update the new number of objects in that ds1
        glb_ds1.obj_num = current_valid_obj_idx;

        test_add_objects();




        // warning :
        // In fact there can be less groups than expected
        // like in data\global\tiles\act1\outdoors\trees.ds1
        // where the file stop right after the last tile_x group data
        // leaving the other datas unknown (tile_y, width, height), and npc paths unknown.

        if ( (glb_ds1.version >= 12)  && ((glb_ds1.tag_type == 1) || (glb_ds1.tag_type == 2))) {
            // 我调试的时候这一块没有被执行
            // 主要是因为tag_type=0
            // skip 1 dword ?
            if (glb_ds1.version >= 18){
                ptr++;
            }

            glb_ds1.group_num = n = * ptr;
            ptr++;

            printf("groups          : %d\n", n);

            // malloc
            glb_ds1.group_size = size = n * sizeof(GROUP_S);
            glb_ds1.group = (GROUP_S *) malloc(size);
            if(glb_ds1.group == NULL){
                free(ds1_buff);
                FATAL_EXIT("not enough mem (%i bytes) for groups\n", size);
            }
            memset(glb_ds1.group, 0, size);

            // fill it
            for(x=0; x<n; x++){
                if ((UDWORD) ptr < (((UDWORD) ds1_buff) + ds1_len)){
                    glb_ds1.group[x].tile_x = * ptr;
                }
                ptr++;
                if ((UDWORD) ptr < (((UDWORD) ds1_buff) + ds1_len)){
                    glb_ds1.group[x].tile_y = * ptr;
                }
                ptr++;
                if ((UDWORD) ptr < (((UDWORD) ds1_buff) + ds1_len)){
                    glb_ds1.group[x].width  = * ptr;
                }
                ptr++;
                if ((UDWORD) ptr < (((UDWORD) ds1_buff) + ds1_len)){
                    glb_ds1.group[x].height = * ptr;
                }
                ptr++;
                if(glb_ds1.version >= 13){
                    if((UDWORD) ptr < (((UDWORD) ds1_buff) + ds1_len)){
                        glb_ds1.group[x].unk = * ptr;
                    }
                    ptr++;
                }
            }
        }else{
            printf("no groups\n");
        }


        // 这一段是读取NPC PATH的数据的, I can commit all of this with effects  {{{
        // no other important data after this
        // 最好的办法就是不要动着一块，正常的把npc的数据skip掉
        if (glb_ds1.version >= 14) {
            // 大于14的才有npc path
            if ((UDWORD) ptr < (((UDWORD) ds1_buff) + ds1_len))
                npc = * ptr;
            else
                npc = 0;
            ptr++;
            printf("npc paths       : %li\n", npc);
            for (n=0; n<npc; n++)
            {
                path = * ptr;
                ptr++;
                x = * ptr;
                ptr++;
                y = * ptr;
                ptr++;

                // search of which object are these paths datas
                o = last_o = nb = 0;
                done = FALSE;
                while (! done)
                {
                    if (o < glb_ds1.obj_num)
                    {
                        if ((glb_ds1.obj[o].x == x) && (glb_ds1.obj[o].y == y))
                        {
                            last_o = o;
                            nb++;
                            if (nb >= 2)
                                done = TRUE;
                        }
                        o++; // next object
                    }
                    else
                        done = TRUE;
                }

                if (nb >= 2)
                {
                    // there are a least 2 objects at the same coordinates

                    // put a warning
                    if (obj_path_warn_wrote != TRUE)
                    {
                        obj_path_warn_wrote = TRUE;
                        printf("\n" "ds1_read() : WARNING, there are at least 2 objects at the same coordinates for some paths datas.\n");
                    }
                    printf("   * Removing %ld paths points of 1 object at coordinates (%ld, %ld)\n", path, x, y);
                    fflush(stdout);

                    // first, delete already assigned paths
                    for (o=0; o < glb_ds1.obj_num; o++)
                    {
                        if ((glb_ds1.obj[o].x == x) && (glb_ds1.obj[o].y == y) &&
                                (glb_ds1.obj[o].path_num != 0))
                        {
                            for (p=0; p < glb_ds1.obj[o].path_num; p++)
                            {
                                glb_ds1.obj[o].path[p].x      = 0;
                                glb_ds1.obj[o].path[p].y      = 0;
                                glb_ds1.obj[o].path[p].action = 0;
                                glb_ds1.obj[o].path[p].flags  = 0;
                            }
                            glb_ds1.obj[o].path_num = 0;
                        }
                    }

                    // now, skip these paths
                    if (glb_ds1.version >= 15)
                    {
                        for (p=0; p < path; p++)
                            ptr += 3; // skip 3 dwords per path
                    }
                    else
                    {
                        for (p=0; p < path; p++)
                            ptr += 2; // skip 2 dwords only per path, no 'action'
                    }
                }
                else
                {
                    // only 1 object at these coordinates for paths, it's ok
                    o = last_o;

                    // does these paths are pointing to a valid object position ?
                    if (o >= glb_ds1.obj_num)
                    {
                        // nope
                        // the game don't alert the user, so why me ?
                        // but we'll skip them
                        if (glb_ds1.version >= 15)
                        {
                            for (p=0; p < path; p++)
                                ptr += 3; // skip 3 dwords per path
                        }
                        else
                        {
                            for (p=0; p < path; p++)
                                ptr += 2; // skip 2 dwords only per path, no 'action'
                        }
                    }
                    else
                    {
                        // yep, valid object

                        if (path > WINDS1EDIT_PATH_MAX)
                        {
                            free(ds1_buff);
                            FATAL_EXIT("object %d have too much paths (%ld), editor max is %ld\n", o, path, WINDS1EDIT_PATH_MAX);
                        }

                        // all ok for assigning the paths to this object
                        glb_ds1.obj[o].path_num = path;
                        for (p=0; p < path; p++)
                        {
                            glb_ds1.obj[o].path[p].x = * ptr;
                            ptr++;
                            glb_ds1.obj[o].path[p].y = * ptr;
                            ptr++;
                            if (glb_ds1.version >= 15)
                            {
                                glb_ds1.obj[o].path[p].action = * ptr;
                                ptr++;
                            }
                            else
                                glb_ds1.obj[o].path[p].action = 1; // default action
                        }
                    }
                }
            }
            editobj_make_obj_label(ds1_idx);
        }else{
            printf("no npc paths\n");
        }
        // }}}
        // sort objects, for drawing purpose
        // 很关键的函数~
        editobj_set_drawing_order(ds1_idx);

        // internal
        glb_ds1.cur_zoom           = ZM_11;
        glb_ds1.tile_w             = 160;
        glb_ds1.tile_h             = 80;
        glb_ds1.height_mul         = 1;
        glb_ds1.height_div         = 1;
        glb_ds1.cur_scroll.keyb.x  = glb_config.scroll.keyb.x;
        glb_ds1.cur_scroll.keyb.y  = glb_config.scroll.keyb.y;
        glb_ds1.cur_scroll.mouse.x = glb_config.scroll.mouse.x;
        glb_ds1.cur_scroll.mouse.y = glb_config.scroll.mouse.y;

        // center it
        cx = glb_ds1.width/2 + 1;
        cy = glb_ds1.height/2;
        dx = (cy * -glb_ds1.tile_w / 2) + (cx * glb_ds1.tile_w / 2);
        dy = (cy *  glb_ds1.tile_h / 2) + (cx * glb_ds1.tile_h / 2);

        glb_ds1.own_wpreview.x0 = dx - glb_config.screen.width  / 2;
        glb_ds1.own_wpreview.y0 = dy - glb_config.screen.height / 2;
        glb_ds1.own_wpreview.w  = glb_config.screen.width;
        glb_ds1.own_wpreview.h  = glb_config.screen.height;

        change_zoom(ds1_idx, ZM_14); // start with zoom of 1:4 in this ds1

        // some inits
        cptr   = glb_ds1.file_buff;
        tile_x = tile_y = zoom = -1;

        // workspace (user environment)
        if(glb_config.workspace_enable){
            // for all filenames
            //这个双层循环没看明白~
            for(x=0; x<glb_ds1.file_num; x++){
                // 我调试的时候file_num为9
                // for all workspace datas

                for(y=0; y < WRKSPC_MAX; y++){
                    // check if it's one of our datas

                    // for all characters
                    o = 0;
                    // cptr指向ds1文件读入内存后的内存头部
                    //
                    // typedef struct WRKSPC_DATAS_S{
                    //    char * id;
                    // } WRKSPC_DATAS_S;
                    // 可见glb_wrkspc_datas就为一个字符串数组而已
                    //
                    //
                    // 这个wile的意思为: cptr不到file_buf的末尾
                    //             并且: glb_wrkspc_datas字符串数组部位当前不到某个字符串末尾
                    //             并且: cptr指向的字符和当前字符串中的字符相同
                    while( (cptr[o] != 0x00) && (glb_wrkspc_datas[y].id[o] != 0x00) && (cptr[o] == glb_wrkspc_datas[y].id[o]) ){
                        // continue on next character
                        o++;
                    }
                    if((glb_wrkspc_datas[y].id[o] == 0x00) && (cptr[o]== '=')){
                        // this is our data, read its value
                        // 如果当前glb_wrkspc_datas到结尾, 且cptr指向"="
                        o++;
                        if(cptr[o] != 0x00){
                            switch(y){
                                case WRKSPC_TILE_X:
                                    sscanf(cptr + o, "%li", & tile_x);
                                    break;
                                case WRKSPC_TILE_Y:
                                    sscanf(cptr + o, "%li", & tile_y);
                                    break;
                                case WRKSPC_ZOOM:
                                    sscanf(cptr + o, "%li", & zoom);
                                    break;
                                case WRKSPC_VERSION:
                                    // just ignore it
                                    break;
                                case WRKSPC_SAVE_COUNT:
                                    sscanf(cptr + o, "%lu", & glb_ds1.save_count);
                                    break;
                            }
                        }
                    }
                }

                // next filename
                cptr += (strlen(cptr) + 1);
            }

            // update ds1 values with workspace datas
            if(zoom != -1){
                change_zoom(ds1_idx, zoom);
            }
            if((tile_x != -1) && (tile_y != -1)){
                if(tile_x < 0){
                    tile_x = 0;
                }
                if(tile_x >= glb_ds1.width){
                    tile_x = glb_ds1.width - 1;
                }
                if(tile_y < 0){
                    tile_y = 0;
                }
                if (tile_y >= glb_ds1.height){
                    tile_y = glb_ds1.height - 1;
                }

                tile_x++;
                dx = (tile_y * -glb_ds1.tile_w / 2) + (tile_x * glb_ds1.tile_w / 2);
                dy = (tile_y *  glb_ds1.tile_h / 2) + (tile_x * glb_ds1.tile_h / 2);
                tile_x--;

                if(ds1_idx == 0){
                    glb_ds1edit.win_preview.x0 = dx - glb_ds1edit.win_preview.w / 2;
                    glb_ds1edit.win_preview.y0 = dy - glb_ds1edit.win_preview.h / 2;
                }
                glb_ds1.own_wpreview.x0 = dx - glb_ds1edit.win_preview.w / 2;
                glb_ds1.own_wpreview.y0 = dy - glb_ds1edit.win_preview.h / 2;
                glb_ds1.own_wpreview.w  = glb_ds1edit.win_preview.w;
                glb_ds1.own_wpreview.h  = glb_ds1edit.win_preview.h;
            }
        }

        // end, some last init
        free(ds1_buff);
        strncpy(glb_ds1.undo.tag, "UNDO", sizeof(glb_ds1.undo.tag));
        glb_ds1.undo.cur_buf_num = -1;
        glb_ds1.path_edit_win.obj_idx = -1;

        glb_map_test=1;
        ds1_save(0,TRUE);
        glb_map_test=0;

    }

    return 0;
}
