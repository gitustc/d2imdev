/*
 * =====================================================================================
 *
 *       Filename: d2pak.h
 *        Created: 02/08/2014 03:42:43 PM
 *  Last Modified: 02/09/2014 12:20:48 AM
 *
 *    Description: d2pak.h
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

#ifndef _D2PAK_H_
#define _D2PAK_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    FILE        *fp;

    uint32_t    blk_num;
    uint32_t    capacity;

    uint32_t    seed0;
    uint32_t    seed1;
    uint32_t    seed2;

    uint32_t    *hash;
    uint32_t    *entries;

}d2pak;

typedef struct {
    uint32_t    crc0;
    uint32_t    crc1;
    uint32_t    crc2;

    uint32_t    start;
    uint32_t    len;
    uint32_t    idx;

}d2pakfd;


d2pak *d2pak_open(const char *);
int d2pak_find(d2pak *, d2pakfd *, const char *);
int d2pak_seek(d2pak *, d2pakfd *);
void *d2pak_read(d2pak *, d2pakfd *);
void *d2pak_fread(d2pak *, const char *, int *);

void d2pak_close(d2pak *);


#endif
