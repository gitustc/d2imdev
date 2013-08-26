#include "game.h"




int init();
int loop();


int init()
{

    game_state_init();
    set_game_state( GS_INIT );
    return 0;
}
int loop()
{
    ge_event    evt;

    while( !get_game_state() ){

        while( ge_event_queue_empty() ){

            ge_get_event( &evt );
            get_game_state()->process_event( &evt );

        }

    }

}
