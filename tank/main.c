#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

void f1()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    luaL_dofile(L, "scripts/test.lua");
    lua_close(L);
}

int main(void)
{
    f1();
    f1();
    return 0;
}
