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
#include "CSprite.h"
#include <SDL2_ttf/SDL_ttf.h>
#include <map>
#include <string>

class CAssetManager {
    
public:
    CAssetManager();
    
    CSprite* addSprite(std::string name, std::string spriteSheetKey, SDL_Rect source);
    CSpriteSheet* addSpriteSheet(std::string name, std::string fileName, SDL_Renderer* renderer);
    TTF_Font* addFont(std::string name, std::string fileName, int size);
    
    CSprite* getSprite(std::string key);
    CSpriteSheet* getSpriteSheet(std::string key);
    TTF_Font* getFont(std::string key);
    
    void onCleanup();
    
private:
    std::map<std::string, CSprite*> SpriteVector;
    std::map<std::string, CSpriteSheet*> SpriteSheetVector;
    std::map<std::string, TTF_Font*> FontVector;
    
};

#endif /* defined(__Third__CAssetManager__) */
