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

CAssetManager::CAssetManager() {
}


CSprite* CAssetManager::addSprite(std::string name, std::string spriteSheetKey, sf::IntRect source) {
    if(SpriteVector.find(name) != SpriteVector.end()) {
        std::cout << "!: Couldn't add sprite: \"" << name << "\", because it already exists.\n";
        return SpriteVector[name];
    } else if(SpriteSheetVector.find(spriteSheetKey) == SpriteSheetVector.end()) {
        std::cout << "!: Couldn't add sprite: \"" << name << "\", because the spritesheet \"" << spriteSheetKey << "\" didn't exist.\n";
        return nullptr;
    } else {
        SpriteVector[name] = new CSprite(SpriteSheetVector[spriteSheetKey], source);
        return SpriteVector[name];
    }
}

CSpriteSheet* CAssetManager::addSpriteSheet(std::string name, std::string fileName) {
    if(SpriteSheetVector.find(name) != SpriteSheetVector.end()) {
        std::cout << "!: Couldn't add spritesheet: \"" << name << "\", because it already exists.\n";
        return SpriteSheetVector[name];
    } else {
        CSpriteSheet* temp = new CSpriteSheet(fileName);
        if(temp->getTexture() == nullptr) {
            std::cout << "!: Couldn't add spritesheet: \"" << name << "\", could not open file \"" << fileName << "\".\n";
            return nullptr;
        } else {
            std::cout << "Loaded asset: \"" << fileName << "\" as \"" << name << "\"\n";
            SpriteSheetVector[name] = temp;
            return SpriteSheetVector[name];
        }
    }
}

sf::Font* CAssetManager::addFont(std::string name, std::string fileName) {
    if(FontVector.find(name) != FontVector.end()) {
        std::cout << "!: Couldn't add font: \"" << name << "\", because it already exists.\n";
        return &FontVector[name];
    } else {
        sf::Font temp;
        if(!temp.loadFromFile(fileName)) {
            std::cout << "!: Couldn't add font: \"" << name << "\", could not open file \"" << fileName << "\".\n";
            return nullptr;
        } else {
            std::cout << "Loaded font: \"" << fileName << "\" as \"" << name << "\"\n";
            FontVector[name] = temp;
            return &FontVector[name];
        }
    }
}

CSprite* CAssetManager::getSprite(std::string key) {
    auto it = SpriteVector.find(key);
    if(it == SpriteVector.end())
        return nullptr;
    else
        return it->second;
}

CSpriteSheet* CAssetManager::getSpriteSheet(std::string key) {
    auto it = SpriteSheetVector.find(key);
    if(it == SpriteSheetVector.end())
        return nullptr;
    else
        return it->second;
}

sf::Font* CAssetManager::getFont(std::string key) {
    auto it = FontVector.find(key);
    if(it == FontVector.end())
        return nullptr;
    else
        return &it->second;
}

void CAssetManager::onCleanup() {
    {
        auto i = SpriteVector.begin();
        while(i != SpriteVector.end()) {
            delete i->second;
            i->second = nullptr;
            SpriteVector.erase(i++->first);
        }
        SpriteVector.clear();
    }
    
    {
        std::cout << "Unloaded asset: ";
        auto i = SpriteSheetVector.begin();
        while(i != SpriteSheetVector.end()) {
            i->second->onCleanup();
            delete i->second;
            i->second = nullptr;
            std::cout << " \"" << i->first << "\",";
            SpriteSheetVector.erase(i++->first);
        }
        SpriteSheetVector.clear();
        std::cout << "\n";
    }
    
//    {
//        std::cout << "Unloaded font: ";
//        auto i = FontVector.begin();
//        while(i != FontVector.end()) {
//            TTF_CloseFont(i->second);
//            i->second = nullptr;
//            std::cout << "\"" << i->first << "\",";
//            FontVector.erase(i++->first);
//        }
//        FontVector.clear();
//        std::cout << "\n";
//    }
    
}