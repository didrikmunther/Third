//
//  CAssetManager.h
//  Third
//
//  Created by Didrik Munther on 17/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CAssetManager__
#define __Third__CAssetManager__

#include <stdio.h>
#include <map>
#include <string>
#include <SDL2_ttf/SDL_ttf.h>

#include "CSprite.h"
#include "NFile.h"
#include "CMusic.h"
#include "CSound.h"


class CLuaScript;
class lua_State;

struct Tileset;

enum CLEAN_FLAGS {
    EVERYTHING      = 0,
    NOT_LUA_SCRIPTS = 1 << 0
};

class CAssetManager {
    
public:
    static CSprite* addSprite(std::string name, std::string spriteSheetKey, Box source);
    static std::string addSprite(CSprite* sprite, std::string name = "");
    static CSprite* createSprite(CSpriteSheet* spriteSheet, Box source);
    static CSpriteSheet* addSpriteSheet(std::string name, SDL_Renderer* renderer, std::string fileName);
    static TTF_Font* addFont(std::string name, std::string fileName, int size);
    static CLuaScript* addLuaScript(lua_State* L, std::string path);
    static Tileset* addTileset(std::string name, Tileset* tileset);
    static CMusic* addMusic(std::string name, std::string fileName);
    static CSound* addSound(std::string name, std::string fileName);
    
    static CSprite* getSprite(std::string key);
    static CSpriteSheet* getSpriteSheet(std::string key);
    static TTF_Font* getFont(std::string key);
    static CLuaScript* getLuaScript(std::string key);
    static Tileset* getTileset(std::string key);
    
    static int playMusic(std::string key, int loops = -1);
    static int playSound(std::string key, int loops = 0);
    
    static void onCleanup(CLEAN_FLAGS flags = CLEAN_FLAGS::EVERYTHING);
    
private:
    static std::map<std::string, CSprite*> _Sprites;
    static std::map<std::string, CSpriteSheet*> _SpriteSheets;
    static std::map<std::string, TTF_Font*> _Fonts;
    static std::map<std::string, CLuaScript*> _LuaScripts;
    static std::map<std::string, Tileset*> _Tilesets;
    static std::map<std::string, CMusic*> _Music;
    static std::map<std::string, CSound*> _Sounds;
    static int _assetId;
    
    template<typename T>
    static T _get(std::string key, std::map<std::string, T>* m) {
        auto it = m->find(key);
        if(it == m->end())
            return nullptr;
        else
            return it->second;
    }
    
    template<typename T>
    static void _cleanup(std::map<std::string, T>* m, bool log = true) {
        std::string toWrite = "";
        auto i = m->begin();
        while(i != m->end()) {
            i->second->onCleanup();
            delete i->second;
            toWrite += "\"" + i->first + "\", ";
            m->erase(i++->first);
        }
        m->clear();
        
        if(toWrite != "" && log)
            NFile::log(LogType::SUCCESS, "Unloaded assets: ", toWrite);
        
    }
    
};

#endif /* defined(__Third__CAssetManager__) */
