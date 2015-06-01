//
//  CNpc.cpp
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CNpc.h"

CNpc::CNpc(sf::IntRect rect, sf::Color color) :
CLiving(rect, color) {
    _initMovementValues();
}

CNpc::CNpc(sf::IntRect rect, std::string spriteKey) :
CLiving(rect, spriteKey) {
    _initMovementValues();
}

void CNpc::_initMovementValues() {
    maxSpeed = 5.0f;
    accelerationX = 1.5f;
    accelerationY = 100.0f;
    stoppingAccelerationX = accelerationX * 2;
    sneakSpeed = (float)maxSpeed / 2.0f;
}

void CNpc::_doLogic() {
    goRight();
}