//
//  CMovable.h
//  Fifth-Client
//
//  Created by Lucas Karlsson on 27/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth_Client__CMovable__
#define __Fifth_Client__CMovable__

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
    float accelerationX, accelerationY;
    float stoppingAccelerationX;
    float sneakSpeed;
    
    void checkMovement();
    
    
    
private:

    
};

#endif /* defined(__Fifth_Client__CMovable__) */
