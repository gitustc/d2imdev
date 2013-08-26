#ifndef script_h
#define script_h

typedef struct _script {

        /***************************
         * private
         */
        void*       game;
        stack*      call_stack;

    
}script;

int script_init( script* scr, void* g ){
#ifdef ARG_CHECK
        if( !scr || !g ){
                return 1;
        }
#endif
        scr->game = g;
        stack_init( &(scr->stack) );
}

int script_run( script* scr ){
        if( scr->state != SCR_ACTIVITE ){
                return 1;
        }else{
                stack_top( &(scr->stack) )( scr );
                return 0;
        }
}


int script_run( srcipt* );


/*
 * *********************all script function************************
 */



#endif
