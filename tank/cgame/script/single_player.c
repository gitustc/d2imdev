#include "script.h"

ScriptBegin()

        CmdLine( 1, InitBat() )
        CmdLine( 2, JmpEqu( 2, GameLev(), 10 ) )
        CmdLine( 3, LevBegin() )
        CmsLine( 4, Wait() )
        CmdLine( 5, LevEnd() )
        CmdLine( 6, JmpEqu( 7, GameWin(), 0 ) )
        CmdLine( 6, Jmp( 2 ) )
        CmdLine( 7, MainMenu() )

ScriptEnd()
