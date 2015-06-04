//
//  CMovable.h
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CMovable__
#define __Fifth__CMovable__

#include <stdio.h>
#include "CEntity.h"
#include <SFML/Graphics.hpp>

class CMovable : public CEntity {
    
public:
    CMovable(sf::IntRect rect, sf::Color color);
    CMovable(sf::IntRect rect, std::string spriteKey);
    
    void goRight();
    void goLeft();
    void goUp();
    void goDown();
    bool hasWalkedX, hasWalkedY;
    
    void jump();
    bool isSneaking;
    
    float maxSpeed;
    float jumpPower;
    float accelerationX, accelerationY;
    float stoppingAccelerationX;
    float sneakSpeed;
    
protected:
    virtual void _doLogic();
    
private:
    virtual void _initMovementValues();
    
};

#endif /* defined(__Fifth__CMovable__) */
