//
//  CPlayer.h
//  Third
//
//  Created by Didrik Munther on 14/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CPlayer__
#define __Third__CPlayer__

#include <stdio.h>
#include "CMovable.h"
#include "CSprite.h"
#include "CAssetManager.h"

class CPlayer : public CMovable {
    
public:
    CPlayer(sf::IntRect rect, sf::Color color);
    CPlayer(sf::IntRect rect, std::string spriteKey);
    
private:
    virtual void _initMovementValues();
    virtual void _doLogic();
    
    
};

#endif /* defined(__Third__CPlayer__) */