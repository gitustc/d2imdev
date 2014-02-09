/*
 * =====================================================================================
 *
 *       Filename: hashmain.c
 *        Created: 02/08/2014 05:54:35 PM
 *  Last Modified: 02/08/2014 06:05:34 PM
 *
 *    Description: mpqhash
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

#include "mpqhash.h"
#include <stdio.h>
#include <stdint.h>
int main(int argc, char *argv[]){
    printf("0X%08X\n", mpqhash(argv[2], (uint32_t)strtoul(argv[1],NULL,10)));
    return 0;
}
