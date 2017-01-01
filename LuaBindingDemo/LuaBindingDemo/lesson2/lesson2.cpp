//
//  lesson2.cpp
//  LuaBindingDemo
//
//  Created by guanghui on 10/15/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include<iostream>
#include <sstream>
#include "lua.hpp"

using namespace std;

int luaAdd(lua_State *lua_state , int x, int y)
{
    int sum;
    //获取lua里面的add函数并把它放到lua的栈顶
    lua_getglobal(lua_state, "add");
    //往lua栈里面压入两个参数
    lua_pushnumber(lua_state, x);
    lua_pushnumber(lua_state, y);
    //调用lua函数,这里的2是参数的个数，1是返回值的个数
    lua_call(lua_state, 2, 1);
    //从栈顶读取返回值,注意这里的参数是-1
    sum = lua_tonumber(lua_state, -1);
    //最后我们把返回值从栈顶拿掉
    lua_pop(lua_state, 1);
    return sum;
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
    
    std::string scriptPath = "lesson2.lua";
    int status = luaL_loadfile(lua_state, scriptPath.c_str());
    
    //change global before pcall
    lua_pushstring(lua_state, "World");
    lua_setglobal(lua_state, "myname");
    
    lua_createtable(lua_state, 2, 0);
    lua_pushnumber(lua_state, 1);
    lua_pushnumber(lua_state, 49);
    lua_rawset(lua_state, -3);
    lua_pushnumber(lua_state, 2);
    lua_pushstring(lua_state, "Life is a beach");
    lua_rawset(lua_state, -3);
    lua_setglobal(lua_state, "arg");
    
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
    
    std::cout<< "2 + 1= " << luaAdd(lua_state,4,1)<<std::endl;

    
    lua_getglobal(lua_state, "myname");
    std::string myname = lua_tostring(lua_state, -1);
    lua_pop(lua_state, 1);
    std::cout<<"Hello: "<<myname<<std::endl;
    
    
    std::stringstream str_buf;
    while(lua_gettop(lua_state))
    {
        str_buf.str(std::string());
        str_buf << " ";
        switch(lua_type(lua_state, lua_gettop(lua_state)))
        {
            case LUA_TNUMBER:
                str_buf << "script returned the number: "
                << lua_tonumber(lua_state, lua_gettop(lua_state));
                break;
            case LUA_TTABLE:
                str_buf << "script returned a table";
                break;
            case LUA_TSTRING:
                str_buf << "script returned the string: "
                << lua_tostring(lua_state, lua_gettop(lua_state));
                break;
            case LUA_TBOOLEAN:
                str_buf << "script returned the boolean: "
                << lua_toboolean(lua_state, lua_gettop(lua_state));
                break;
            default:
                str_buf << "script returned an unknown-type value";
        }
        lua_pop(lua_state, 1);
        std::cout << str_buf.str() << std::endl;
    }
    
   

    return 0;
}
