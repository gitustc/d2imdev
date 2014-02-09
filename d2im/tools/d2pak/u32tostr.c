/*
 * =====================================================================================
 *
 *       Filename: str2u32.c
 *        Created: 02/08/2014 07:42:39 PM
 *  Last Modified: 02/08/2014 10:10:52 PM
 *
 *    Description: 0x00000000 -> u32
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
#include <stdint.h>


int main(int argc, char *argv[]){

    uint32_t    i;

    i = strtoul(argv[1], NULL, 10);

    printf("0X%08X", i);
    return 0;

}
