/*
 * =====================================================================================
 *
 *       Filename: scene.h
 *        Created: 10/10/2013 01:43:50 AM
 *  Last Modified: 10/10/2013 01:46:29 AM
 *
 *    Description: to discripe a scene, we may use many scenes at the same time.
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



struct D2IM_SCENE {
    UINT32          width;
    UINT32          height;

    UINT32          f_num;
    UINT32          w_num;

};
typedef struct D2IM_SCENE D2IM_SCENE;
