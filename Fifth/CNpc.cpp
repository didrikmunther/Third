//
//  CNpc.cpp
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CNpc.h"

CNpc::CNpc(Box rect, sf::Color color) :
CLiving(rect, color) {
    _init();
}

CNpc::CNpc(Box rect, std::string spriteKey) :
CLiving(rect, spriteKey) {
    _init();
}

void CNpc::_init() {
    entityType = EntityTypes::Npc;
    
    maxSpeed = 5.0f;
}

void CNpc::renderAdditional(CWindow* window, CCamera* camera, int renderFlags) {
    CLiving::renderAdditional(window, camera, renderFlags);
    
}

void CNpc::_doLogic() {
    CLiving::_doLogic();
}

bool CNpc::_collisionLogic(CEntity* target, CollisionSides collisionSides) {
    bool parentCollision = CLiving::_collisionLogic(target, collisionSides);
    bool collision = true;
    
    return parentCollision && collision;
}