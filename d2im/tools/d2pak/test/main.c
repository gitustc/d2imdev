/*
 * =====================================================================================
 *
 *       Filename: main.c
 *        Created: 02/08/2014 03:49:09 PM
 *  Last Modified: 02/08/2014 06:11:25 PM
 *
 *    Description: for test
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
int main(){

    d2pak   *f;
    void    *p;
    FILE    *fp;

    int     size;

    /* fp = fopen("test_copy.bin","w+"); */

    f = d2pak_open("./test.d2p");

    if(!f){
        fprintf(stderr, "error!\n");
        return 1;
    }


   fprintf(stdout, "file count = %u\n", f->blk_num);
   fprintf(stdout, "capacity   = %u\n", f->capacity);
   fprintf(stdout, "seed0      = %u\n", f->seed0);
   fprintf(stdout, "seed1      = %u\n", f->seed1);
   fprintf(stdout, "seed2      = %u\n", f->seed2);


   {
       int k;
       for(k=0;k<f->blk_num;k++){
           printf("file %d: start=%lu len=%lu\n", k, f->entries[2*k+1], f->entries[2*k]);
       }
   }

   {
       int k;
       for(k=0;k<f->capacity;k++){
           printf("slot %d: crc1=%lu crc2=%lu idx=%lu\n", k, f->hash[3*k], f->hash[3*k+1], f->hash[3*k+2]);
       }
   }

   d2pak_close(f);

   return 0;


    /* p = d2pak_fread(f, "test/test.bin", &size); */

    /* fwrite(p, size, 1, fp); */


}
