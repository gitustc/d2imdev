/*
 * =====================================================================================
 *
 *       Filename: collision.c
 *        Created: 02/06/2014 09:53:44 PM
 *  Last Modified: 02/08/2014 07:02:18 PM
 *
 *    Description: calculate the collision
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


#include <stdio.h> 
#include <stdlib.h> 
#include <stdint.h> 

int main(int argc, char *argv[]) 
{ 
    /* avgstep $file_count $capacity $crc0 $crc1 $crc2 */

    FILE *fp=NULL; 
    char strline[1024];

    int file_count;
    int capacity;
    int i;


    uint32_t  *file_crc=NULL;
    uint32_t  *hash_table=NULL;
    uint32_t  *flag=NULL;



    file_count = atoi(argv[1]);
    capacity   = atoi(argv[2]);



    file_crc = (uint32_t *)malloc(file_count*3*sizeof(uint32_t));
    if(!file_crc){
        goto _avgstep_fail;
    }

    hash_table = (uint32_t *)malloc(capacity*2*sizeof(uint32_t));
    if(!hash_table){
        goto _avgstep_fail;
    }

    flag = (uint32_t *)malloc(capacity*sizeof(uint32_t));
    if(!flag){
        goto _avgstep_fail;
    }

    if((fp = fopen(argv[3],"r")) == NULL){ 
        goto _avgstep_fail;
    } 

    i = 0;
    while (i<file_count*3){ 
        fgets(strline,1024,fp);
        strline[10] = '\0';
        file_crc[i] = (uint32_t)strtoul(strline, NULL, 16);
        /* printf("%d: %s %lu\n", i, strline, file_crc[i]); */
        i++;
    } 

    for(i=0;i<capacity;i++){
        flag[i] = file_count;
    }

    for(i=0;i<file_count;i++){

        int k;
        k = file_crc[i]%capacity;
        while(1){
            /* here if file_count > capacity */
            /* trap in dead loop */
            if(flag[k] == file_count){
                hash_table[k*2+0] = file_crc[file_count*1+i];
                hash_table[k*2+1] = file_crc[file_count*2+i];
                flag[k] = i;
                break;
            }
            k = (k+1)%capacity;
        }
    }


    for(i=0;i<capacity;i++){
        printf("%lu\t%lu\t%lu\n", hash_table[i*2], hash_table[i*2+1], flag[i]);
    }


    /* { */
    /*     int sum_count = 0; */
    /*     int tmp_count = 0; */
    /*     int k; */
    /*     for(i=0;i<file_count;i++){ */
    /*         k = file_crc[i]%capacity; */
    /*         tmp_count = 1; */
    /*         while(1){ */
    /*             if(hash_table[k*2] == file_crc[i+file_count] &&  */
    /*                     hash_table[k*2+1] == file_crc[i+file_count*2]){ */
    /*                 sum_count += tmp_count; */
    /*                 #<{(| printf("%d: tmp_cnt=%d sum_cnt=%d\n", i, tmp_count, sum_count); |)}># */
    /*                 break; */
    /*             } */
    /*             k = (k+1)%capacity; */
    /*             tmp_count++; */
    /*         } */
    /*     } */
    /*     printf("%d", sum_count*100/file_count); */
    /* } */


_avgstep_fail:

    if(fp){
        fclose(fp);
    }
    if(file_crc){
        free(file_crc);
    }
    if(hash_table){
        free(hash_table);
    }
    if(flag){
        free(flag);
    }

    return 0;
}
