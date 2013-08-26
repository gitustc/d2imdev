#include "lua.h"  
#include "lualib.h"  
#include "lauxlib.h"  

static int _lib_func1(lua_State* L)  
{  
    return 0;  
}  

static int _lib_func2(lua_State* L)  
{  
    return 0;  
}  

static const luaL_reg _lib_func_list[] =  
{  
    {"func1",_lib_func1},  
    {"func2",_lib_func2},  
    {NULL, NULL}  
};  


int __cdecl __declspec(dllexport) luaopen_clib(lua_State* L)  
{  
    luaL_openlib(L, "clib", _lib_func_list, 0);  
    return 1;  
}  



//package.loadlib("clib.so","luaopen_clib")();
//clib.func1();
//clib.func2();
