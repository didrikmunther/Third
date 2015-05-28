//
//  Cenemy.cpp
//  Fifth-Client
//
//  Created by Lucas Karlsson on 27/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "Cenemy.h"
#include <iostream>


Cenemy::Cenemy(sf::IntRect rect, sf::Color color) : CMovable(rect, color){
    initValues();
}

Cenemy::Cenemy(sf::IntRect rect, std::string spriteKey) : CMovable(rect, spriteKey) {
    initValues();
}


void Cenemy::initValues() {
    maxSpeed = 10.0f;
    accelerationX = 1.5f;
    accelerationY = 100.0f;
    stoppingAccelerationX = accelerationX * 2;
    sneakSpeed = (float)maxSpeed / 2.0f;
    isSneaking = false;
    hasWalkedX = false;
    hasWalkedY = false;
}


void Cenemy::_doLogic() {
    
    goLeft();

    //checkMovement();
    
}



