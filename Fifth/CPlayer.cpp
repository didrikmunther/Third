//
//  CPlayer.cpp
//  Third
//
//  Created by Didrik Munther on 14/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CPlayer.h"
#include "Define.h"
#include <iostream>

CPlayer::CPlayer(Box rect, sf::Color color) :
    CLiving(rect, color) {
        _initMovementValues();
}

CPlayer::CPlayer(Box rect, std::string spriteKey) :
    CLiving(rect, spriteKey) {
        CPlayer(rect, sf::Color{255, 255, 255, 255});
        _initMovementValues();
}

void CPlayer::_initMovementValues() {
    jumpPower = 20.0f;
}

void CPlayer::_doLogic() {
    CLiving::_doLogic();
}

void CPlayer::_collisionLogic(CEntity* target) {
    CLiving::_collisionLogic(target);
    
}