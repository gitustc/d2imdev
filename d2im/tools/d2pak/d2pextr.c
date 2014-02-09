/*
 * =====================================================================================
 *
 *       Filename: d2pextr.c
 *        Created: 02/08/2014 03:49:09 PM
 *  Last Modified: 02/09/2014 02:34:02 AM
 *
 *    Description: extract files from package
 *
 *        Version: 1.0
 *       Revision: none
 *       Compiler: gcc
 *
 *         Author: ANHONG
 *          Email: anhonghe@gmail.com
 *   Organization: USTC
 *
 * =====================================================================================
 */

#include "d2pak.h"
#include <stdio.h>
#include <stdint.h>
int main(int argc, char *argv[]){

    d2pak   *f  = NULL;
    void    *p  = NULL;
    FILE    *fp = NULL;

    uint32_t     size = 0;


    if(argc<4){
        goto _main_fail;
    }

    /* fprintf(stderr, "here..%s\n", argv[3]); */
    fp = fopen(argv[3],"w+");
    if(!fp){
        goto _main_fail;
    }

    f = d2pak_open(argv[1]);
    if(!f){
        goto _main_fail;
    }


   /* fprintf(stdout, "file count = %u\n", f->blk_num); */
   /* fprintf(stdout, "capacity   = %u\n", f->capacity); */
   /* fprintf(stdout, "seed0      = %u\n", f->seed0); */
   /* fprintf(stdout, "seed1      = %u\n", f->seed1); */
   /* fprintf(stdout, "seed2      = %u\n", f->seed2); */


   /* { */
   /*     int k; */
   /*     for(k=0;k<f->capacity;k++){ */
   /*         printf("slot %d: crc1=0X%08X crc2=0X%08X idx=%u\n", k, f->hash[3*k], f->hash[3*k+1], f->hash[3*k+2]); */
   /*     } */
   /* } */

   /* { */
   /*     int k; */
   /*     for(k=0;k<f->blk_num;k++){ */
   /*         printf("file %d: start=%lu len=%lu\n", k, f->entries[2*k+1], f->entries[2*k]); */
   /*     } */
   /* } */



    p = d2pak_fread(f, argv[2], &size);
    if(!p){
        goto _main_fail;
    }


    /* printf("0X%08X\n%lu\n", p, size); */

    if(fwrite(p, size, 1, fp)){
        goto _main_fail;
    }

    return 0;

_main_fail:

    if(f){
        d2pak_close(f);
    }
    if(p){
        free(p);
    }
    if(fp){
        fclose(fp);
    }
    return 1;
}
