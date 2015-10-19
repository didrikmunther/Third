//
//  CAssetManager.cpp
//  Third
//
//  Created by Didrik Munther on 17/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CAssetManager.h"
#include "NFile.h"
#include "CSpriteContainer.h"
#include "CSpriteSheet.h"
#include "CLuaScript.h"


std::map<std::string, CSprite*> CAssetManager::_Sprites;
std::map<std::string, CSpriteContainer*> CAssetManager::_SpriteContainers;
std::map<std::string, CSpriteSheet*> CAssetManager::_SpriteSheets;
std::map<std::string, TTF_Font*> CAssetManager::_Fonts;
std::map<std::string, CLuaScript*> CAssetManager::_LuaScripts;
int CAssetManager::_assetId = 0;

CAssetManager::CAssetManager() { }


CSprite* CAssetManager::addSprite(std::string name, std::string spriteSheetKey, Box source) {
    if(_Sprites.find(name) != _Sprites.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite: \"", name, "\", because it already exists.");
        return _Sprites[name];
    } else if(_SpriteSheets.find(spriteSheetKey) == _SpriteSheets.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite: \"", name, "\", because the spritesheet \"", spriteSheetKey, "\" didn't exist.");
        return nullptr;
    } else {
        _Sprites[name] = new CSprite(_SpriteSheets[spriteSheetKey], source);
        return _Sprites[name];
    }
}

std::string CAssetManager::addSprite(CSprite* sprite, std::string name /* = "" */) {
    if(name == "")
        name = "reserved:" + std::to_string(_assetId++);
    if(_Sprites.find(name) != _Sprites.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite: \"", name, "\", because it already exists.");
        return name;
    } else {
        _Sprites[name] = sprite;
        return name;
    }
}

CSpriteContainer* CAssetManager::addSpriteContainer(std::string name, std::string spriteKey, Area area /* = {-1, -1} */) {
    if(_SpriteContainers.find(name) != _SpriteContainers.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite container \"", name, "\", becuase it already exists.");
        return _SpriteContainers[name];
    } else if(_Sprites.find(spriteKey) == _Sprites.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite container \"", name, "\", becuase the sprite \"", spriteKey, "\" didn't exist.");
        return nullptr;
    } else {
        if(area.w > 0 || area.h > 0)
            _SpriteContainers[name] = new CSpriteContainer(spriteKey, area);
        else
            _SpriteContainers[name] = new CSpriteContainer(spriteKey);
            
        return _SpriteContainers[name];
    }
}

std::string CAssetManager::addSpriteContainer(CSpriteContainer* spriteContainer, std::string name /* = "" */) {
    if(name == "")
        name = "reserved:" + std::to_string(_assetId++);
    if(_SpriteContainers.find(name) != _SpriteContainers.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite container: \"", name, "\", because it already exists.");
        return name;
    } else {
        _SpriteContainers[name] = spriteContainer;
        return name;
    }
}

CSpriteSheet* CAssetManager::addSpriteSheet(std::string name, SDL_Renderer* renderer, std::string fileName) {
    if(_SpriteSheets.find(name) != _SpriteSheets.end()) {
        NFile::log(LogType::WARNING, "Couldn't add spritesheet: \"", name, "\", because it already exists.");
        return _SpriteSheets[name];
    } else {
        CSpriteSheet* temp = new CSpriteSheet();
        if(!temp->openFile(renderer, fileName)) {
            NFile::log(LogType::WARNING, "Couldn't add spritesheet: \"", name, "\", could not open file \"", fileName, "\".");
            return nullptr;
        } else {
            NFile::log(LogType::SUCCESS, "Loaded asset: \"", fileName, "\" as \"", name, "\"");
            _SpriteSheets[name] = temp;
            return _SpriteSheets[name];
        }
    }
}

TTF_Font* CAssetManager::addFont(std::string name, std::string fileName, int size) {
    if(_Fonts.find(name) != _Fonts.end()) {
        NFile::log(LogType::WARNING, "Couldn't add font: \"", name, "\", because it already exists.");
        return _Fonts[name];
    } else {
        TTF_Font *temp = TTF_OpenFont(fileName.c_str(), size);
        if(temp == nullptr) {
            NFile::log(LogType::WARNING, "Couldn't add font: \"", name, "\", could not open file \"", fileName, "\".");
            return nullptr;
        } else {
            NFile::log(LogType::SUCCESS, "Loaded font: \"", fileName, "\" as \"", name, "\".");
            _Fonts[name] = temp;
            return temp;
        }
    }
}

CLuaScript* CAssetManager::addLuaScript(lua_State* L, std::string path) {
    CLuaScript* script = new CLuaScript(L, path);
    auto name = script->getName();
    
    if(_LuaScripts.find(name) != _LuaScripts.end()) {
        NFile::log(LogType::WARNING, "Couldn't add script: \"", name, "\", because it already exists");
        delete script;
        return _LuaScripts[name];
    } else {
        _LuaScripts[name] = script;
        NFile::log(LogType::SUCCESS, "Loaded script: \"", path, "\" as \"", name, "\".");
        return script;
    }
}

CSprite* CAssetManager::getSprite(std::string key) {
    auto it = _Sprites.find(key);
    if(it == _Sprites.end())
        return nullptr;
    else
        return it->second;
}

CSpriteContainer* CAssetManager::getSpriteContainer(std::string key) {
    auto it = _SpriteContainers.find(key);
    if(it == _SpriteContainers.end())
        return nullptr;
    else
        return it->second;
}

CSpriteSheet* CAssetManager::getSpriteSheet(std::string key) {
    auto it = _SpriteSheets.find(key);
    if(it == _SpriteSheets.end())
        return nullptr;
    else
        return it->second;
}

TTF_Font* CAssetManager::getFont(std::string key) {
    auto it = _Fonts.find(key);
    if(it == _Fonts.end())
        return nullptr;
    else
        return it->second;
}

CLuaScript* CAssetManager::getLuaScript(std::string key) {
    auto it = _LuaScripts.find(key);
    if(it == _LuaScripts.end())
        return nullptr;
    else
        return it->second;
}

void CAssetManager::removeSpriteContainer(std::string key) {
    auto it = _SpriteContainers.find(key);
    if(it != _SpriteContainers.end()) {
        delete it->second;
        _SpriteContainers.erase(it->first);
    }
}

void CAssetManager::onCleanup() {
    
    NFile::log(LogType::ALERT, "Unloading assets!");
    
    {
        auto i = _Sprites.begin();
        while(i != _Sprites.end()) {
            delete i->second;
            i->second = nullptr;
            _Sprites.erase(i++->first);
        }
        _Sprites.clear();
    }
    
    {
        auto i = _SpriteContainers.begin();
        while(i != _SpriteContainers.end()) {
            delete i->second;
            i->second = nullptr;
            _SpriteContainers.erase(i++->first);
        }
        _SpriteContainers.clear();
    }
    
    {
        std::string toWrite = "";
        auto i = _SpriteSheets.begin();
        while(i != _SpriteSheets.end()) {
            i->second->onCleanup();
            delete i->second;
            i->second = nullptr;
            toWrite += "\"" + i->first + "\", ";
            _SpriteSheets.erase(i++->first);
        }
        _SpriteSheets.clear();
        
        if(toWrite != "")
            NFile::log(LogType::SUCCESS, "Unloaded assets: ", toWrite);
    }
    
    {
        std::string toWrite = "";
        auto i = _Fonts.begin();
        while(i != _Fonts.end()) {
            toWrite += "\"" + i->first + "\", ";
            TTF_CloseFont(i->second);
            _Fonts.erase(i++->first);
        }
        _Fonts.clear();
        if(toWrite != "")
            NFile::log(LogType::SUCCESS, "Unloaded assets: ", toWrite);
    }
    
    {
        std::string toWrite = "";
        auto i = _LuaScripts.begin();
        while(i != _LuaScripts.end()) {
            toWrite += "\"" + i->first + "\", ";
            delete i->second;
            _LuaScripts.erase(i++->first);
        }
        _LuaScripts.clear();
        if(toWrite != "")
            NFile::log(LogType::SUCCESS, "Unloaded scripts: ", toWrite);
    }
    
}