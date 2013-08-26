#include <stdio.h>
#include "gameengine.h"



#define GS_QUIT     0
#define GS_INIT     1
#define GS_LOOP     2
#define GS_MAX      (GS_LOOP+1)


typedef struct {
}game_state;
typedef struct {
}game_instance;


extern game_instance    glb_game_instance;
extern game_state       glb_game_state[GS_MAX];

extern int init();
extern int loop();

#define get_game_state()        (glb_game_instance.current_game_state)
#define set_game_state(state)   (glb_game_instance.current_game_state=glb_game_state+(state))


extern int switch_game_state( int );
