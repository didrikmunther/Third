//
//  CSpriteContainer.h
//  Fifth
//
//  Created by Didrik Munther on 11/06/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CSpriteContainer__
#define __Fifth__CSpriteContainer__

#include <stdio.h>

#include "CAssetManager.h"


class CSprite;

class CSpriteContainer {
    
public:
    CSpriteContainer(std::string spriteKey, Area spriteArea) : spriteKey(spriteKey), spriteArea(spriteArea) {}
    CSpriteContainer(std::string spriteKey) : spriteKey(spriteKey) {
        if(CAssetManager::getSprite(spriteKey) != nullptr)
            spriteArea = Area {(int)getSprite()->getSource()->w,
                               (int)getSprite()->getSource()->h};
    }
    
    std::string getSpriteKey() { return spriteKey; }
    CSprite* getSprite() { return CAssetManager::getSprite(spriteKey); }
    void setSpriteKey(std::string spriteKey) { this->spriteKey = spriteKey; }
    
    Area spriteArea;
    
    std::string spriteKey;
    
    
};

#endif /* defined(__Fifth__CSpriteContainer__) */
