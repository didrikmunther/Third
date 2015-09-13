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


std::map<std::string, CSprite*> CAssetManager::_SpriteVector;
std::map<std::string, CSpriteContainer*> CAssetManager::_SpriteContainerVector;
std::map<std::string, CSpriteSheet*> CAssetManager::_SpriteSheetVector;
std::map<std::string, TTF_Font*> CAssetManager::_FontVector;
//std::map<std::string, sf::Shader*> CAssetManager::_ShaderVector;
int CAssetManager::_assetId = 0;

CAssetManager::CAssetManager() { }


CSprite* CAssetManager::addSprite(std::string name, std::string spriteSheetKey, Box source) {
    if(_SpriteVector.find(name) != _SpriteVector.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite: \"", name, "\", because it already exists.");
        return _SpriteVector[name];
    } else if(_SpriteSheetVector.find(spriteSheetKey) == _SpriteSheetVector.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite: \"", name, "\", because the spritesheet \"", spriteSheetKey, "\" didn't exist.");
        return nullptr;
    } else {
        _SpriteVector[name] = new CSprite(_SpriteSheetVector[spriteSheetKey], source);
        return _SpriteVector[name];
    }
}

std::string CAssetManager::addSprite(CSprite* sprite, std::string name /* = "" */) {
    if(name == "")
        name = "reserved:" + std::to_string(_assetId++);
    if(_SpriteVector.find(name) != _SpriteVector.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite: \"", name, "\", because it already exists.");
        return name;
    } else {
        _SpriteVector[name] = sprite;
        return name;
    }
}

CSpriteContainer* CAssetManager::addSpriteContainer(std::string name, std::string spriteKey, Area area /* = {-1, -1} */) {
    if(_SpriteContainerVector.find(name) != _SpriteContainerVector.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite container \"", name, "\", becuase it already exists.");
        return _SpriteContainerVector[name];
    } else if(_SpriteVector.find(spriteKey) == _SpriteVector.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite container \"", name, "\", becuase the sprite \"", spriteKey, "\" didn't exist.");
        return nullptr;
    } else {
        if(area.w > 0 || area.h > 0)
            _SpriteContainerVector[name] = new CSpriteContainer(spriteKey, area);
        else
            _SpriteContainerVector[name] = new CSpriteContainer(spriteKey);
            
        return _SpriteContainerVector[name];
    }
}

std::string CAssetManager::addSpriteContainer(CSpriteContainer* spriteContainer, std::string name /* = "" */) {
    if(name == "")
        name = "reserved:" + std::to_string(_assetId++);
    if(_SpriteContainerVector.find(name) != _SpriteContainerVector.end()) {
        NFile::log(LogType::WARNING, "Couldn't add sprite container: \"", name, "\", because it already exists.");
        return name;
    } else {
        _SpriteContainerVector[name] = spriteContainer;
        return name;
    }
}

CSpriteSheet* CAssetManager::addSpriteSheet(std::string name, SDL_Renderer* renderer, std::string fileName) {
    //fileName = resourcePath() + fileName;
    if(_SpriteSheetVector.find(name) != _SpriteSheetVector.end()) {
        NFile::log(LogType::WARNING, "Couldn't add spritesheet: \"", name, "\", because it already exists.");
        return _SpriteSheetVector[name];
    } else {
        CSpriteSheet* temp = new CSpriteSheet();
        if(!temp->openFile(renderer, fileName)) {
            NFile::log(LogType::WARNING, "Couldn't add spritesheet: \"", name, "\", could not open file \"", fileName, "\".");
            return nullptr;
        } else {
            NFile::log(LogType::SUCCESS, "Loaded asset: \"", fileName, "\" as \"", name, "\"");
            _SpriteSheetVector[name] = temp;
            return _SpriteSheetVector[name];
        }
    }
}

TTF_Font* CAssetManager::addFont(std::string name, std::string fileName, int size) {
    if(_FontVector.find(name) != _FontVector.end()) {
        NFile::log(LogType::WARNING, "Couldn't add font: \"", name, "\", because it already exists.");
        return _FontVector[name];
    } else {
        TTF_Font *temp = TTF_OpenFont(fileName.c_str(), size);
        if(temp == nullptr) {
            NFile::log(LogType::WARNING, "Couldn't add font: \"", name, "\", could not open file \"", fileName, "\".");
            return nullptr;
        } else {
            NFile::log(LogType::SUCCESS, "Loaded font: \"", fileName, "\" as \"", name, "\"");
            _FontVector[name] = temp;
            return _FontVector[name];
        }
    }
}

//sf::Shader* CAssetManager::addShader(std::string name, std::string fileName, sf::Shader::Type type) {
//    if(_ShaderVector.find(name) != _ShaderVector.end()) {
//        NFile::log(LogType::WARNING, "Couldn't add shader: \"", name, "\", because it already exists.");
//        return _ShaderVector[name];
//    } else {
//        sf::Shader* temp = new sf::Shader;
//        if(!temp->loadFromFile(fileName.c_str(), type)) {
//            NFile::log(LogType::WARNING, "Couldn't add shader: \"", name, "\", could not open file \"", fileName, "\".");
//            return nullptr;
//        } else {
//            NFile::log(LogType::SUCCESS, "Loaded shader: \"", fileName, "\" as \"", name, "\"");
//            _ShaderVector[name] = temp;
//            return _ShaderVector[name];
//        }
//    }
//}

CSprite* CAssetManager::getSprite(std::string key) {
    auto it = _SpriteVector.find(key);
    if(it == _SpriteVector.end())
        return nullptr;
    else
        return it->second;
}

CSpriteContainer* CAssetManager::getSpriteContainer(std::string key) {
    auto it = _SpriteContainerVector.find(key);
    if(it == _SpriteContainerVector.end())
        return nullptr;
    else
        return it->second;
}

CSpriteSheet* CAssetManager::getSpriteSheet(std::string key) {
    auto it = _SpriteSheetVector.find(key);
    if(it == _SpriteSheetVector.end())
        return nullptr;
    else
        return it->second;
}

TTF_Font* CAssetManager::getFont(std::string key) {
    auto it = _FontVector.find(key);
    if(it == _FontVector.end())
        return nullptr;
    else
        return it->second;
}

//sf::Shader* CAssetManager::getShader(std::string key) {
//    auto it = _ShaderVector.find(key);
//    if(it == _ShaderVector.end())
//        return nullptr;
//    else
//        return it->second;
//}

void CAssetManager::removeSpriteContainer(std::string key) {
    auto it = _SpriteContainerVector.find(key);
    if(it != _SpriteContainerVector.end()) {
        delete it->second;
        _SpriteContainerVector.erase(it->first);
    }
}

void CAssetManager::onCleanup() {
    
    NFile::log(LogType::ALERT, "Unloading assets!");
    
    {
        auto i = _SpriteVector.begin();
        while(i != _SpriteVector.end()) {
            delete i->second;
            i->second = nullptr;
            _SpriteVector.erase(i++->first);
        }
        _SpriteVector.clear();
    }
    
    {
        auto i = _SpriteContainerVector.begin();
        while(i != _SpriteContainerVector.end()) {
            delete i->second;
            i->second = nullptr;
            _SpriteContainerVector.erase(i++->first);
        }
        _SpriteContainerVector.clear();
    }
    
    {
        std::string toWrite = "";
        auto i = _SpriteSheetVector.begin();
        while(i != _SpriteSheetVector.end()) {
            i->second->onCleanup();
            delete i->second;
            i->second = nullptr;
            toWrite += " \"" + i->first + "\",";
            _SpriteSheetVector.erase(i++->first);
        }
        _SpriteSheetVector.clear();
        
        if(toWrite != "")
            NFile::log(LogType::SUCCESS, "Unloaded asset: ", toWrite);
    }
    
    {
        std::string toWrite = "";
        auto i = _FontVector.begin();
        while(i != _FontVector.end()) {
            toWrite += " \"" + i->first + "\",";
            _FontVector.erase(i++->first);
        }
        _FontVector.clear();
        if(toWrite != "")
            NFile::log(LogType::SUCCESS, "Unloaded asset: ", toWrite);
    }
    
//    {
//        std::string toWrite = "";
//        auto i = _ShaderVector.begin();
//        while(i != _ShaderVector.end()) {
//            toWrite += " \"" + i->first + "\",";
//            delete i->second;
//            _ShaderVector.erase(i++->first);
//        }
//        _ShaderVector.clear();
//        if(toWrite != "")
//            NFile::log(LogType::SUCCESS, "Unloaded asset: ", toWrite);
//    }
    
}