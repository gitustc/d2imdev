#ifndef game_h
#define game_h

#include "sprite.h"
#include "map.h"


#define GAME_WIN	0
#define GAME_START	1
#define GAME_RUN	2
#define GAME_MENU	3
#define GAME_OVER	4
#define GAME_END	5


#define ENEMY_MAX 20
#define FPS 40

#define ROW_OFF		10
#define COL_OFF		8



typedef struct _game 

}game;


/*=====================================================================
 * public
 */
int	game_init( game* );
int	game_run ( game* );
int	game_over( game* );


/*=====================================================================
 * private
 */

/* process event*/
void _process_event( game* );
void _process_key_up_event( game*, al_evt* );
void _process_key_down_event( game*, al_evt* );
void _process_display_close( game*, al_evt* );
void _process_timer( game*, al_evt* );



#endif
