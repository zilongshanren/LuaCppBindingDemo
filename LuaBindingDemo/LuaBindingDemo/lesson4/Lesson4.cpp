//
//  Lesson4.cpp
//  LuaBindingDemo
//
//  Created by guanghui on 1/1/17.
//  Copyright © 2017 guanghui. All rights reserved.
//

#include<iostream>
#include <sstream>
#include "lua.hpp"

using namespace std;

static void stackDump(lua_State* L){
    cout<<"\nbegin dump lua stack"<<endl;
    int i = 0;
    int top = lua_gettop(L);
    for (i = 1; i <= top; ++i) {
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:
            {
                printf("'%s' ", lua_tostring(L, i));
            }
                break;
            case LUA_TBOOLEAN:
            {
                printf(lua_toboolean(L, i) ? "true " : "false ");
            }break;
            case LUA_TNUMBER:
            {
                printf("%g ", lua_tonumber(L, i));
            }
                break;
            default:
            {
                printf("%s ", lua_typename(L, t));
            }
                break;
        }
    }
    cout<<"\nend dump lua stack"<<endl;
}

int average(lua_State *L)
{
    // get number of arguments
    int n = lua_gettop(L);
    double sum = 0;
    int i;
    // loop through each argument
    for (i = 1; i <= n; i++)
    {
        // total the arguments
        sum += lua_tonumber(L, i);
    }
    // push the average
    lua_pushnumber(L, sum / n);
    // push the sum
    lua_pushnumber(L, sum);
    // return the number of results
    return 2;
}

int displayLuaFunction(lua_State *l)
{
    // number of input arguments
    int argc = lua_gettop(l);
    // print input arguments
    std::cout << "[C++] Function called from Lua with " << argc
    << " input arguments" << std::endl;
    for(int i=0; i<argc; i++)
    {
        std::cout << " input argument #" << argc-i << ": "
        << lua_tostring(l, lua_gettop(l)) << std::endl;
        lua_pop(l, 1);
    }
    // push to the stack the multiple return values
    std::cout << "[C++] Returning some values" << std::endl;
    lua_pushnumber(l, 12);
    lua_pushstring(l, "See you space cowboy");
    // number of return values
    return 2;
}

static const luaL_Reg mylibs[]=
{
    {"average", average},
    {"displayLuaFunction", displayLuaFunction},
    {NULL, NULL}
};

int lua_openmylib(lua_State *L)
{
    luaL_newlib(L, mylibs);
    return 1;
};

int main()
{
    lua_State *lua_state = luaL_newstate();
    
    static const luaL_Reg lualibs[] =
    {
        {"base", luaopen_base},
        {"io", luaopen_io},
        {"mylib", lua_openmylib},
        {NULL, NULL}
    };
    const luaL_Reg *lib = lualibs;
    for(; lib->func != NULL; lib++)
    {
        luaL_requiref(lua_state, lib->name, lib->func, 1);
        lua_pop(lua_state,1);
    }
    
    std::string scriptPath = "lesson4.lua";
    int status = luaL_loadfile(lua_state, scriptPath.c_str());
    
    //lua_register(lua_state, "average", average);
    
    // push the C++ function to be called from Lua
    //std::cout << "[C++] Pushing the C++ function" << std::endl;
    //lua_pushcfunction(lua_state, displayLuaFunction);
    //lua_setglobal(lua_state, "displayLuaFunction");

    //std::cout << " return: " << status << std::endl;
    int result = 0;
    if(status == LUA_OK)
    {
        result = lua_pcall(lua_state, 0, LUA_MULTRET, 0);
    }
    else
    {
        std::cout << " Could not load the script." << std::endl;
    }
    
    
    
    
}

