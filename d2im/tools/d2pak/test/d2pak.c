#include <stdint.h>
#include <stdio.h>
#include "d2pak.h"
#include <string.h>


d2pak *d2pak_open(const char *str)
{
    d2pak  *f;
    int     crcs;

    f = malloc(sizeof(d2pak));
    memset(f, 0, sizeof(d2pak));

    if( !f ){
        goto _d2pk_open_failed;
    }

    f->fp = fopen(str, "r");
    if( !(f->fp) ){
        goto _d2pk_open_failed;
    }

    {
        uint8_t ch[4];
        fread(ch, 1, 4, f->fp);

        if(strncmp(ch, "D2PK", 4)){
            goto _d2pk_open_failed;
        }
    }

    fread(&(f->blk_num),  4, 1, f->fp);
    fread(&(f->capacity), 4, 1, f->fp);
    fread(&(f->seed0),    4, 1, f->fp);
    fread(&(f->seed1),    4, 1, f->fp);
    fread(&(f->seed2),    4, 1, f->fp);

    f->hash = (uint32_t*)malloc(f->capacity*3*4);
    if(! f->hash){
        goto _d2pk_open_failed;
    }
    fread(f->hash, 4*3, f->capacity, f->fp);

    f->entries = (uint32_t*)malloc(f->blk_num*2*4);
    if(! f->entries){
        goto _d2pk_open_failed;
    }
    fread(f->entries, 4*2, f->blk_num, f->fp);

    return f;



_d2pk_open_failed:
    if(f){
        if(f->fp){
            fclose(f->fp);
        }
        if(f->hash){
            free(f->hash);
        }
        if(f->entries){
            free(f->entries);
        }
        free(f);
    }
    return NULL;
}


int d2pak_find(d2pak *f, d2pakfd *fd, const char *str){

    if(!f || !fd || !str || !strlen(str)){
        return 1;
    }

    /* fd->fp   = f; */
    fd->crc0 = mpqhash(str, f->seed0);
    fd->crc1 = mpqhash(str, f->seed1);
    fd->crc2 = mpqhash(str, f->seed2);

    {
        uint32_t k;
        uint32_t start_k;

        k = fd->crc0 % f->capacity;
        start_k = k;
        while(1){
            if(f->hash[k*3+2]!=f->blk_num){
                if(f->hash[k*3]==fd->crc1 && f->hash[k*3+1]==fd->crc2){
                    fd->start = f->entries[2 * f->hash[k*3+2] + 0];
                    fd->len   = f->entries[2 * f->hash[k*3+2] + 1];
                    return 0;
                }else{
                    k = (k+1)%f->capacity;
                    if(k==start_k){
                        return 1;
                    }
                }
            }else{
                return 1;
            }
        }
    }

}

int d2pak_seek(d2pak *f, d2pakfd *fd){
    return fseek(f->fp, 24+f->capacity*3+f->blk_num*2+fd->start, SEEK_SET);
}

void *d2pak_read(d2pak *f, d2pakfd *fd){

    void *p;

    p = malloc(fd->len);
    if(!p){
        goto _d2pak_read_failed;
    }
    fread(p, fd->len, 1, f->fp);
    return p;

_d2pak_read_failed:
    if(p){
        free(p);
    }
    return NULL;
}

void *d2pak_fread(d2pak *f, const char *str, int *size){
    d2pakfd fd;

    if( !f || !str || !size ){
        return NULL;
    }

    d2pak_find(f, &fd, str);
    d2pak_seek(f, &fd);
    return d2pak_read(f, &fd);
}



void d2pak_close(d2pak *f){
    if(!f){
        return;
    }
    if(f->fp){
        fclose(f->fp);
    }
    if(f->hash){
        free(f->hash);
    }
    if(f->entries){
        free(f->entries);
    }
    free(f);
}
