#include "main.h"

int main(){

	game g;
	game_init( &g );
	game_run ( &g );
	game_over( &g );

	return 0;
}
