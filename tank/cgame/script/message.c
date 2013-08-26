#include "message.h"




/*
 * **********************************************************
 */

int messagebump_post_message( messagebump* bump, message* msg ){
    
    int i;
#ifdef ARG_CHECK
    if( !bump && !msg ){
        return MESSAGEBUMP_POST_BAD_ARG;
    }
#endif

    for( i=0; i< MESSAGEBUMP_MAX; ++i ){
        if( message_list[i].type == MESSAGE_TYPE_NULL ){
            break;
        }
    }

    if( i == MESSAGEBUMP_MAX ){
        return MESSAGEBUMP_POST_OVERFLOW;
    }
    mmcpy( message_list+i,message );
    return MESSAGEBUMP_POST_OK;
}


int messagebump_process_message( messagebump* bump ){
    switch( bump->current_
