//
//  Lesson3.cpp
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

int main()
{
    lua_State *lua_state = luaL_newstate();
    
    static const luaL_Reg lualibs[] =
    {
        {"base", luaopen_base},
        {"io", luaopen_io},
        {NULL, NULL}
    };
    const luaL_Reg *lib = lualibs;
    for(; lib->func != NULL; lib++)
    {
        luaL_requiref(lua_state, lib->name, lib->func, 1);
        lua_pop(lua_state,1);
    }
    
    std::string scriptPath = "lesson3.lua";
    int status = luaL_loadfile(lua_state, scriptPath.c_str());
    
    
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
    
    
    //从Lua里面取得me这个table，并压入栈
    lua_getglobal(lua_state, "me");
    if (!lua_istable(lua_state, -1)) {
        std::cout << "error! me is not a table";
    }
    //往栈里面压入一个key:name
    lua_pushstring(lua_state, "name");
    //取得-2位置的table，然后把栈顶元素弹出，取出table[name]的值并压入栈
    lua_gettable(lua_state, -2);
    //输出栈顶的name
    std::cout << "name = " << lua_tostring(lua_state, -1);
    stackDump(lua_state);
    //把栈顶元素弹出去
    lua_pop(lua_state, 1);
    //压入另一个key:age
    lua_pushstring(lua_state, "age");
    //取出-2位置的table,把table[age]的值压入栈
    lua_gettable(lua_state, -2);
    stackDump(lua_state);
    std::cout << "age = " << lua_tointeger(lua_state, -1);

}


