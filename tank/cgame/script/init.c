#include "script.h"

ScriptBegin()

        CmdLine( 1, ClearScreen( 0, 0, 0 ) )
        CmdLine( 2, ShowLogo() )
        CmdLine( 3, MainMenu() )
        CmdLine( 4, JE( 1, 7 ) )
        CmdLine( 5, JE( 2, 8 ) )
        CmdLine( 6, Jmp( 9 ) )
        CmdLine( 7, Call( "single_player.scr" ) )
        CmdLine( 8, Call( "double_player.scr" ) )
        CmdLine( 9, Exit() )

ScriptEnd()
