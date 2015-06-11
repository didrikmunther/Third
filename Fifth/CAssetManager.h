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
#include "CSpriteSheet.h"
#include <SFML/Graphics.hpp>
#include "CSprite.h"
#include <map>
#include <string>

class CSpriteContainer;

class CAssetManager {
    
public:
    CAssetManager();
    
    static CSprite* addSprite(std::string name, std::string spriteSheetKey, Box source);
    static std::string addSprite(CSprite* sprite, std::string name = "");
    static CSpriteContainer* addSpriteContainer(std::string name, std::string spriteKey, Area area = {-1, -1});
    static std::string addSpriteContainer(CSpriteContainer* spriteContainer, std::string name = "");
    static CSpriteSheet* addSpriteSheet(std::string name, std::string fileName);
    static sf::Font* addFont(std::string name, std::string fileName);
    static sf::Shader* addShader(std::string name, std::string fileName, sf::Shader::Type type);
    
    static CSprite* getSprite(std::string key);
    static CSpriteContainer* getSpriteContainer(std::string key);
    static CSpriteSheet* getSpriteSheet(std::string key);
    static sf::Font* getFont(std::string key);
    static sf::Shader* getShader(std::string key);
    
    static void removeSpriteContainer(std::string key);
    
    static void onCleanup();
    
private:
    static std::map<std::string, CSprite*> _SpriteVector;
    static std::map<std::string, CSpriteContainer*> _SpriteContainerVector;
    static std::map<std::string, CSpriteSheet*> _SpriteSheetVector;
    static std::map<std::string, sf::Font> _FontVector;
    static std::map<std::string, sf::Shader*> _ShaderVector;
    static int _assetId;
    
};

#endif /* defined(__Third__CAssetManager__) */
