//
//  EEnemy.cpp
//  Fifth
//
//  Created by Didrik Munther on 22/09/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "EEnemy.h"
#include "EUtility.h"
#include "EMovable.h"
#include "ELiving.h"

#include "CEntity.h"
#include "CWindow.h"


EEnemy::EEnemy(CEntity* parent) : EComponent(parent) {
    EMovable* movable = parent->getComponent<EMovable>();
    if(movable) {
        movable->setMovementState(MovementState::SNEAKING_MOVEMENT);
        movable->jumpPower = 15.0f;
        movable->accelerationX = 0.1f;
    }
    
    _target = nullptr;
    _shootTimer = SDL_GetTicks();
    _renderTriangle = false;
}

void EEnemy::onLoop(CInstance* instance) {
    
    if(_target == nullptr)
        return;
    
    EMovable* movable = parent->getComponent<EMovable>();
    if(movable) {
        if(parent->body.getX() > _target->body.getX())
            movable->goLeft();
        else if(parent->body.getX() < _target->body.getX())
            movable->goRight();
        
        if(parent->collisionSides.right || parent->collisionSides.left)
            movable->jump();
    }

    int fireRate = 300; // Bullets per minute
    int fireDelay = (60.0f / fireRate) * 1000;
    
    if(SDL_GetTicks() - _shootTimer > fireDelay || _renderTriangle) {
        
        _triangles.clear();
        
        int targetX = _target->body.getCenter().x;
        int targetY = _target->body.getCenter().y;
        float targetVelX = _target->body.velX;
        float targetVelY = _target->body.velY;
        
        int thisX = parent->body.getX();
        int thisY = parent->body.getY() - 100;
        float thisVelX = parent->body.velX;
        float thisVelY = parent->body.velY;
        
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
        
        Color distanceColor = Color{0, 0, 255, 255};
        Color velocityColor = Color{0, 255, 0, 255};
        Color targetColor   = Color{255, 0, 0, 255};
        
        _triangles.push_back(Triangle{
            Line{targetX, targetY, thisX, thisY, distanceColor},
            Line{(int)(targetX + targetVelX * impactTime), (int)(targetY + targetVelY * impactTime), thisX, thisY, targetColor},
            Line{targetX, targetY, (int)(targetX + targetVelX * impactTime), (int)(targetY + targetVelY * impactTime), velocityColor}
        });
        
        if(SDL_GetTicks() - _shootTimer > fireDelay) {
            
            parent->shoot(radian, BasicUtilities::DAMAGE);
            
            _shootTimer += fireDelay;
        }
    }
}

void EEnemy::onRenderAdditional(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    if(renderFlags & RenderFlags::ENEMY_TRIANGLE && !*isDead()) {
        _renderTriangle = true;
        
        for(auto &i: _triangles)
            NSurface::renderTriangle(i, window->getRenderer(), camera);
        
    } else {
        _renderTriangle = false;
    }
}

bool EEnemy::onCollision(CEntity* target, CollisionSides* collisionSides) {
    
    ELiving* living = parent->getComponent<ELiving>();
    
    if(living && living->_hasTakenFallDamage) {
        float angle = 0;
        int particles = 90;
        
        for(int i = 0; i < particles; i++) {
            angle += (360.0f / particles) / (360 / (2 * M_PI)); // convert raidans to degrees
            
            parent->shoot(angle, BasicUtilities::DAMAGE);
        }
    }
    
    return true;
}

void EEnemy::setTarget(CEntity* target) {
    _target = target;
}

CEntity* EEnemy::getTarget() {
    return _target;
}
