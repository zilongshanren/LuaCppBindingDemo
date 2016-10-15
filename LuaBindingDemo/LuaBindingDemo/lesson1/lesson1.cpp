//
//  main.cpp
//  LuaBindingDemo
//
//  Created by guanghui on 10/15/16.
//  Copyright © 2016 guanghui. All rights reserved.
//

#include "lua.hpp"


int main(int argc, const char * argv[]) {
    //1. 初始化Lua虚拟机
    lua_State *lua_state;
    lua_state = luaL_newstate();
    //2.设置待注册的Lua标准库，这个库是给你的Lua脚本用的
    //因为接下来我们只想在Lua脚本里面输出hello world，所以只引入基本库就可以了
    static const luaL_Reg lualibs[] =
    {
        { "base", luaopen_base },
        { NULL, NULL}
    };
    //3.注册Lua标准库并清空栈
    const luaL_Reg *lib = lualibs;
    for(; lib->func != NULL; lib++)
    {
        luaL_requiref(lua_state, lib->name, lib->func, 1);
        lua_pop(lua_state, 1);
    }
    //4、运行hello.lua脚本
    luaL_dofile(lua_state, "hello.lua");
    //5. 关闭Lua虚拟机
    lua_close(lua_state);
    return 0;
}
