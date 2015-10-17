//
//  LuaScript.h
//  LuaTest4
//
//  Created by Didrik Munther on 16/10/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __LuaTest4__LuaScript__
#define __LuaTest4__LuaScript__

#include <stdio.h>
#include <string>
#include <iostream>

extern "C" {
# include "lua/lua.h"
# include "lua/lauxlib.h"
# include "lua/lualib.h"
}

#include "LuaBridge/LuaBridge.h"


class Entity;

class LuaScript {
    
public:
    LuaScript(lua_State* L, std::string path);
    
    luabridge::LuaRef init(lua_State* L, std::string path);
    
    void doFile();
    
    luabridge::LuaRef* getObjectCreation();
    
    lua_State* getState();
    std::string getPath();
    std::string getName();
    
private:
    lua_State* _L;
    std::string _path;
    luabridge::LuaRef _objectCreationFunction;
    
};

#endif /* defined(__LuaTest4__LuaScript__) */
