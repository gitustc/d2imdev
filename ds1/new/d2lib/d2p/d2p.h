/*
 * =====================================================================================
 *
 *       Filename: d2p.h
 *        Created: 10/10/2013 05:44:36 PM
 *  Last Modified: 10/10/2013 05:52:28 PM
 *
 *    Description: self-defined package for read-only file system.
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


#ifndef  _included_d2p_inc__
#define  _included_d2p_inc__




struct D2P {
    uint32_t        capacity;
};
typedef struct D2P D2P;

D2P     *d2p_open(const char *);
int32_t  d2p_close(D2P *);






#endif   /* ----- #ifndef _included_d2p_inc__  ----- */
