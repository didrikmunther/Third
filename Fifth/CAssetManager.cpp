//
//  CAssetManager.cpp
//  Third
//
//  Created by Didrik Munther on 17/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CAssetManager.h"
#include <iostream>
#include "NFile.h"
#include "ResourcePath.hpp"

std::map<std::string, CSprite*> CAssetManager::_SpriteVector;
std::map<std::string, CSpriteSheet*> CAssetManager::_SpriteSheetVector;
std::map<std::string, sf::Font> CAssetManager::_FontVector;
std::map<std::string, sf::Shader*> CAssetManager::_ShaderVector;
int CAssetManager::_assetId = 0;

CAssetManager::CAssetManager() {
}


CSprite* CAssetManager::addSprite(std::string name, std::string spriteSheetKey, sf::IntRect source) {
    if(_SpriteVector.find(name) != _SpriteVector.end()) {
        std::cout << "!: Couldn't add sprite: \"" << name << "\", because it already exists.\n";
        return _SpriteVector[name];
    } else if(_SpriteSheetVector.find(spriteSheetKey) == _SpriteSheetVector.end()) {
        std::cout << "!: Couldn't add sprite: \"" << name << "\", because the spritesheet \"" << spriteSheetKey << "\" didn't exist.\n";
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
        std::cout << "!: Couldn't add sprite: \"" << name << "\", because it already exists.\n";
        return name;
    } else {
        _SpriteVector[name] = sprite;
        return name;
    }
}

CSpriteSheet* CAssetManager::addSpriteSheet(std::string name, std::string fileName) {
    //fileName = resourcePath() + fileName;
    if(_SpriteSheetVector.find(name) != _SpriteSheetVector.end()) {
        std::cout << "!: Couldn't add spritesheet: \"" << name << "\", because it already exists.\n";
        return _SpriteSheetVector[name];
    } else {
        CSpriteSheet* temp = new CSpriteSheet();
        if(!temp->openFile(fileName)) {
            std::cout << "!: Couldn't add spritesheet: \"" << name << "\", could not open file \"" << fileName << "\".\n";
            return nullptr;
        } else {
            std::cout << "Loaded asset: \"" << fileName << "\" as \"" << name << "\"\n";
            _SpriteSheetVector[name] = temp;
            return _SpriteSheetVector[name];
        }
    }
}

sf::Font* CAssetManager::addFont(std::string name, std::string fileName) {
    //fileName = resourcePath() + fileName;
    if(_FontVector.find(name) != _FontVector.end()) {
        std::cout << "!: Couldn't add font: \"" << name << "\", because it already exists.\n";
        return &_FontVector[name];
    } else {
        sf::Font temp;
        if(!temp.loadFromFile(fileName)) {
            std::cout << "!: Couldn't add font: \"" << name << "\", could not open file \"" << fileName << "\".\n";
            return nullptr;
        } else {
            std::cout << "Loaded font: \"" << fileName << "\" as \"" << name << "\"\n";
            _FontVector[name] = temp;
            return &_FontVector[name];
        }
    }
}

sf::Shader* CAssetManager::addShader(std::string name, std::string fileName, sf::Shader::Type type) {
    if(_ShaderVector.find(name) != _ShaderVector.end()) {
        std::cout << "!: Couldn't add shader: \"" << name << "\", because it already exists.\n";
        return _ShaderVector[name];
    } else {
        sf::Shader* temp = new sf::Shader;
        if(!temp->loadFromFile(fileName.c_str(), type)) {
            std::cout << "!: Couldn't add shader: \"" << name << "\", could not open file \"" << fileName << "\".\n";
            return nullptr;
        } else {
            std::cout << "Loaded shader: \"" << fileName << "\" as \"" << name << "\"\n";
            _ShaderVector[name] = temp;
            return _ShaderVector[name];
        }
    }
}

CSprite* CAssetManager::getSprite(std::string key) {
    auto it = _SpriteVector.find(key);
    if(it == _SpriteVector.end())
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

sf::Font* CAssetManager::getFont(std::string key) {
    auto it = _FontVector.find(key);
    if(it == _FontVector.end())
        return nullptr;
    else
        return &it->second;
}

sf::Shader* CAssetManager::getShader(std::string key) {
    auto it = _ShaderVector.find(key);
    if(it == _ShaderVector.end())
        return nullptr;
    else
        return it->second;
}

void CAssetManager::onCleanup() {
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
        std::cout << "Unloaded asset: ";
        auto i = _SpriteSheetVector.begin();
        while(i != _SpriteSheetVector.end()) {
            i->second->onCleanup();
            delete i->second;
            i->second = nullptr;
            std::cout << " \"" << i->first << "\",";
            _SpriteSheetVector.erase(i++->first);
        }
        _SpriteSheetVector.clear();
        std::cout << "\n";
    }
    
    {
        std::cout << "Unloaded font: ";
        auto i = _FontVector.begin();
        while(i != _FontVector.end()) {
            std::cout << "\"" << i->first << "\",";
            _FontVector.erase(i++->first);
        }
        _FontVector.clear();
        std::cout << "\n";
    }
    
    {
        std::cout << "Unloaded shader: ";
        auto i = _ShaderVector.begin();
        while(i != _ShaderVector.end()) {
            std::cout << "\"" << i->first << "\",";
            delete i->second;
            _ShaderVector.erase(i++->first);
        }
        _ShaderVector.clear();
        std::cout << "\n";
    }
    
}