//
//  Lesson5.cpp
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

void readLuaArray(lua_State *L)
{
    lua_settop(L,0); //这样确保我们的array是放在当前栈的栈顶。
    lua_getglobal(L, "array");
    //如果前面不调用lua_settop(L,0),那我们必须要使用luaL_len(L,-1)
    auto n = luaL_len(L, 1);   //luaL_len可以获得table的元素个数
    for (int i = 1; i <= n; ++i) {
        lua_pushnumber(L, i);  //往栈里面压入i
        lua_gettable(L, -2);  //读取table[i]，table位于-2的位置。
        //lua_rawget(L, -2);  //lua_gettable也可以用lua_rawget来替换
        cout<<lua_tostring(L, -1)<<endl;
        lua_pop(L, 1);
    }
}

int writeLuaArray(lua_State *L)
{
    lua_settop(L, 0);
    lua_getglobal(L, "array");
    luaL_checktype(L, 1, LUA_TTABLE);
    auto n = luaL_len(L,1);
    for (int i = 1; i <= n; ++i) {
        lua_pushnumber(L, i);
        char buf[256];
        sprintf(buf, "hehe%d", i);
        lua_pushstring(L, buf);
        lua_rawset(L, -3);
    }
    return 0;
}

void readLuaArray2(lua_State *L)
{
    lua_getglobal(L, "array");
    auto n = luaL_len(L, -1);
    for (int i = 1; i <= n; ++i) {
        lua_rawgeti(L, 1, i);
        cout<<"readLuaArray2: "<<lua_tostring(L, -1)<<endl;
        lua_pop(L, 1);
    }
}

int writeLuaArray2(lua_State *L)
{
    lua_settop(L, 0);
    lua_getglobal(L, "array");
    //确保第一个函数一个要是一个table
    luaL_checktype(L, 1, LUA_TTABLE);
    auto n = luaL_len(L,1);
    for (int i = 1; i <= n; ++i) {
        char buf[256];
        sprintf(buf, "hehe%d", i);
        lua_pushstring(L, buf);
        lua_rawseti(L, 1, i);
    }
    return 0;
}

static int l_myconcat(lua_State* L){
    luaL_checktype(L, -1, LUA_TNUMBER);
    int n = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_concat(L, n);
    return 1;
}

static const luaL_Reg mylibs[]=
{
    {"myconcat", l_myconcat},
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
    
    std::string scriptPath = "lesson5.lua";
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
    
//    writeLuaArray(lua_state);
//    readLuaArray(lua_state);
    writeLuaArray2(lua_state);
    readLuaArray2(lua_state);
}
