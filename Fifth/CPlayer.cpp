//
//  CPlayer.cpp
//  Third
//
//  Created by Didrik Munther on 14/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CPlayer.h"


CPlayer::CPlayer(Box rect, SDL_Color color) :
    CLiving(rect, color) {
        _init();
}

CPlayer::CPlayer(Box rect, std::string spriteKey) :
    CLiving(rect, spriteKey) {
        CPlayer(rect, SDL_Color{255, 255, 255, 255});
        _init();
}

void CPlayer::_init() {
    jumpPower = 15.0f;
    accelerationX = 4.0;
}

void CPlayer::renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
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