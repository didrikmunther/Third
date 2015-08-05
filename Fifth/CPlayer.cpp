//
//  CPlayer.cpp
//  Third
//
//  Created by Didrik Munther on 14/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CPlayer.h"
#include "CEnemy.h"
#include "Define.h"
#include <iostream>

CPlayer::CPlayer(Box rect, sf::Color color) :
    CLiving(rect, color) {
        _init();
}

CPlayer::CPlayer(Box rect, std::string spriteKey) :
    CLiving(rect, spriteKey) {
        CPlayer(rect, sf::Color{255, 255, 255, 255});
        _init();
}

void CPlayer::_init() {
    entityType = EntityTypes::Player;
    
    jumpPower = 10.0f;
    maxSpeed = 10;
    accelerationX = 4.0;
}

void CPlayer::renderAdditional(CWindow* window, CCamera* camera, int renderFlags) {
    CLiving::renderAdditional(window, camera, renderFlags);
    
}

void CPlayer::_doLogic() {
    CLiving::_doLogic();

}

bool CPlayer::_collisionLogic(CEntity* target, CollisionSides collisionSides) {
    bool parentCollision = CLiving::_collisionLogic(target, collisionSides);
    bool collision = true;
    
//    if(collisionSides.collisionBottom) {              // Jumping on enemy head kills them
//        CEnemy* enemy = dynamic_cast<CEnemy*>(target);
//        if(enemy != nullptr && !toRemove()) {
//            enemy->dealDamage(_values[ValueTypes::HEALTH]);
//        }
//    }
    
    return parentCollision && collision;
}