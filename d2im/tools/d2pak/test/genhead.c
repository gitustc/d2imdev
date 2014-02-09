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


    uint32_t  *file_crc;
    uint32_t  *hash_table;
    uint32_t  *flag;



    file_count = atoi(argv[1]);
    capacity   = atoi(argv[2]);

    

    file_crc = (uint32_t *)malloc(file_count*3*sizeof(uint32_t));
    if(!file_crc){
        fprintf(stderr, "malloc failed...\n");
        return -1;
    }

    hash_table = (uint32_t *)malloc(capacity*2*sizeof(uint32_t));
    if(!hash_table){
        fprintf(stderr, "malloc failed...\n");
        free(file_crc);
        return -1;
    }


    flag = (uint32_t *)malloc(capacity*sizeof(uint32_t));
    if(!flag){
        fprintf(stderr, "malloc failed...\n");
        free(file_crc);
        free(hash_table);
        return -1;
    }

    if((fp = fopen(argv[3],"r")) == NULL)
    { 
        printf("error!"); 
        free(file_crc);
        free(hash_table);
        free(flag);
        return -1; 
    } 

    i = 0;
    while (!feof(fp)) 
    { 
        fgets(StrLine,1024,fp);
        StrLine[10] = '\0';
        file_crc[i] = (uint32_t)strtoul(StrLine, NULL, 16);
        i++;
        /* printf("%s %lu\n", StrLine, file_crc[i]); */
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
                /* hash_table[k] = file_crc[i]; */
                hash_table[k*2] = file_crc[file_count+i];
                hash_table[k*2+1] = file_crc[file_count*2+i];
                flag[k] = i;
                break;
            }
            k = (k+1)%capacity;
        }
    }

    {
        /* output */
        for(i=0;i<capacity;i++){
            printf("%u\t\t%u\t\t%u", hash_table[i*2], hash_table[i*2+1], flag[i]);
            printf("\n");
            /* printf("%u\n", flag[i]); */
        }
    }

    /* for(i=0;i<capacity;i++){ */
    /*     printf("%d: %lu\n", i, flag[i]); */
    /* } */


    /* { */
    /*     int sum_count = 0; */
    /*     int tmp_count = 0; */
    /*     int k; */
    /*     for(i=0;i<file_count;i++){ */
    /*         k = file_crc[i]%capacity; */
    /*         tmp_count = 1; */
    /*         while(1){ */
    /*             if(hash_table[k*2] == file_crc[i+file_count] && hash_table[k*2+1]==file_crc[i+file_count*2]){ */
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


    fclose(fp);
    free(file_crc);
    free(hash_table);
    free(flag);

    return 0; 
}
