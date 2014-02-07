/*
 * =====================================================================================
 *
 *       Filename: collision.c
 *        Created: 02/06/2014 09:53:44 PM
 *  Last Modified: 02/07/2014 01:39:17 AM
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

    FILE *fp; 
    char StrLine[1024];

    int file_count;
    int capacity;
    int i;


    uint32_t  *fv;
    uint32_t  *cv;



    file_count = atoi(argv[1]);
    capacity   = atoi(argv[2]);

    

    fv = (uint32_t *)malloc(file_count*sizeof(uint32_t));
    if(!fv){
        fprintf(stderr, "malloc failed...\n");
        return -1;
    }

    cv = (uint32_t *)malloc(capacity*sizeof(uint32_t));
    if(!cv){
        fprintf(stderr, "malloc failed...\n");
        free(fv);
        return -1;
    }



    if((fp = fopen(argv[3],"r")) == NULL)
    { 
        printf("error!"); 
        free(fv);
        free(cv);
        return -1; 
    } 

    i = 0;
    while (!feof(fp)) 
    { 
        fgets(StrLine,1024,fp);
        StrLine[10] = '\0';
        fv[i] = (uint32_t)strtoul(StrLine, NULL, 16);
        i++;
        /* printf("%s %lu\n", StrLine, fv[i]); */
    } 

    for(i=0;i<capacity;i++){
        cv[i] = file_count; /*mark as default*/
    }

    for(i=0;i<file_count;i++){

        int k;
        k = fv[i]%capacity;
        while(1){
            /* here if file_count > capacity */
            /* trap in dead loop */
            if(cv[k] == file_count){
                cv[k] = fv[i];
                break;
            }
            k = (k+1)%capacity;
        }
    }


    /* for(i=0;i<capacity;i++){ */
    /*     printf("%d: %lu\n", i, cv[i]); */
    /* } */


    {
        int sum_count = 0;
        int tmp_count = 0;
        int k;
        for(i=0;i<file_count;i++){
            k = fv[i]%capacity;
            tmp_count = 1;
            while(1){
                if(cv[k] == fv[i]){
                    sum_count += tmp_count;
                    break;
                }
                k = (k+1)%capacity;
                tmp_count++;
            }

        }

        printf("%d", sum_count*100/file_count);
    }


    fclose(fp);
    free(fv);
    free(cv);

    return 0; 
}
