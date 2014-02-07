/*
 * =====================================================================================
 *
 *       Filename: collision.c
 *        Created: 02/06/2014 09:53:44 PM
 *  Last Modified: 02/06/2014 10:15:25 PM
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
    int i=0;
    if((fp = fopen(argv[1],"r")) == NULL)
    { 
        printf("error!"); 
        return -1; 
    } 

    while (!feof(fp)) 
    { 
        fgets(StrLine,1024,fp);
        StrLine[10] = '\0';
        printf("%s %lu\n", StrLine, (uint32_t)strtoul(StrLine, NULL, 16));
    } 
    printf("%d\n", sizeof(unsigned long));
    fclose(fp);
    return 0; 
}
