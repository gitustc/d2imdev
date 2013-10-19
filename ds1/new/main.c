/*
 * =====================================================================================
 *
 *       Filename: main.c
 *        Created: 10/09/2013 10:28:40 PM
 *  Last Modified: 10/19/2013 03:40:12 PM
 *
 *    Description: entry of game
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



#include "main.h"
#include <stdlib.h>
#include <stdio.h>
/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  main
 *  Description:  
 * =====================================================================================
 */
int main ( int argc, char *argv[] )
{
    d2im_init();
    d2im_run();
    d2im_quit();
    return 0;
}
