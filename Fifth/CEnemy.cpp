//
//  CEnemy.cpp
//  Fifth
//
//  Created by Didrik Munther on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CEnemy.h"
#include "CUtilityParticle.h"
#include "CChatBubble.h"
#include "CWindow.h"


CEnemy::CEnemy(Box rect, SDL_Color color) :
    CNpc(rect, color) {
        _init();
}

CEnemy::CEnemy(Box rect, std::string spriteKey) :
    CNpc(rect, spriteKey) {
        _init();
}

void CEnemy::_init() {
    target = nullptr;
    setMovementState(MovementState::SNEAKING_MOVEMENT);
    jumpPower = 15.0f;
    accelerationX = 0.1f;
    _shootTimer = SDL_GetTicks();
}

void CEnemy::renderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    CNpc::renderAdditional(window, camera, renderFlags);
    
    for(auto &i: triangles)
        NSurface::renderTriangle(i, window->getRenderer(), camera);
    
}

void CEnemy::setTarget(CEntity *target) {
    this->target = target;
}

CEntity* CEnemy::getTarget() {
    return target;
}

void CEnemy::_doLogic() {
    CNpc::_doLogic();
    
    if(target != nullptr) {
        if(body.getX() > target->body.getX())
            goLeft();
        else if(body.getX() < target->body.getX())
            goRight();
        
        if(collisionSides.right || collisionSides.left)
            jump();
        
        int fireRate = 300; // Bullets per minute
        int fireDelay = (60.0f / fireRate) * 1000;
        
        { // add this to the firerate limiter for better performance, but the triangels limited to fireRate
            
            triangles.clear();
            
            int targetX = target->body.getCenter().x;
            int targetY = target->body.getCenter().y;
            float targetVelX = target->body.velX;
            float targetVelY = target->body.velY;
            
            int thisX = body.getX();
            int thisY = body.getY() - 100;
            float thisVelX = body.velX;
            float thisVelY = body.velY;
            
            float bulletVelocity = 50.0f;
            
            double maxError = 0.000000001;
            
            double dx = targetX - thisX;
            double dy = targetY - thisY;
            double targetDistance = sqrt(dx * dx + dy * dy);
            double impactTime = targetDistance / bulletVelocity;
            double radian = atan2(dy, dx);
            
            int maxIterations = 100;
            while(maxIterations--) {
                dx = targetX - thisX + (targetVelX - thisVelX) * impactTime;
                dy = targetY - thisY + (targetVelY - thisVelY) * impactTime;
                
                targetDistance = sqrt(dx * dx + dy * dy);
                impactTime = targetDistance / bulletVelocity;
                double newRadian = atan2(dy, dx);
                
                double curError = fabs(newRadian - radian);
                radian = newRadian;
                if(curError <= maxError) break;
            }
            
            triangles.push_back(Triangle{
                Line{targetX, targetY, thisX, thisY},
                Line{(int)(targetX + targetVelX * impactTime), (int)(targetY + targetVelY * impactTime), thisX, thisY},
                Line{targetX, targetY, (int)(targetX + targetVelX * impactTime), (int)(targetY + targetVelY * impactTime)}
            });
            
            if(SDL_GetTicks() - _shootTimer > fireDelay) {
                
                shoot(radian, BasicUtilities::HEAL);
                
                _shootTimer += fireDelay;
            }
        }
    }
}

bool CEnemy::_collisionLogic(CEntity* target, CollisionSides collisionSides) {
    bool parentCollision = CNpc::_collisionLogic(target, collisionSides);
    bool collision = true;
    
    return parentCollision && collision;
}