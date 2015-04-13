//
//  CEntity.cpp
//  Third
//
//  Created by Didrik Munther on 19/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CEntity.h"
#include "CSurface.h"
#include <iostream>
#include "CCamera.h"

CEntity::CEntity(SDL_Rect rect, SDL_Color color) : body(rect), color(color), toRemove(false){
    
}

void CEntity::onLoop(std::vector<CEntity*>* entities) {
    
    doLogic();
    move(entities);
    
    if(body.rect.y > 10000) {
        toRemove = true;
        std::cout << body.rect.y;
    }
    
    //body.rect.x += body.velX;
    //body.rect.y += body.velY;
}

void CEntity::onRender(SDL_Renderer *renderer, CCamera* camera) {
    if(camera->collision(this))
        CSurface::renderRect(body.getX() - camera->offsetX(), body.getY() - camera->offsetY(),
                         body.getWidth(), body.getHeight(),
                         renderer, color.r, color.g, color.b);
}

bool CEntity::collision(int x, int y, std::vector<CEntity*>* entities) {
    
    for (auto &i: *entities) {
        if (&*i == this) continue;
    
        if(x > i->body.getX() + i->body.getWidth())
            continue;
        if(x + body.getWidth() < i->body.getX())
            continue;
        if(y > i->body.getY() + i->body.getHeight())
            continue;
        if(y + body.getHeight() < i->body.getY())
            continue;
        else
            return true;
    }
    
    return false;
}

void CEntity::move(std::vector<CEntity*>* entities) {
    
    int MoveX = body.velX;
    int MoveY = body.velY;
    
    int StopX = body.getX();
    int StopY = body.getY();
    
    int NewX = 0;
    int NewY = 0;
    
    if(MoveX != 0) {
        if(MoveX >= 0) 	NewX =  1;
        else 			NewX = -1;
    }
    
    if(MoveY != 0) {
        if(MoveY >= 0) 	NewY =  1;
        else 			NewY = -1;
    }
    
    //body.rect.x += MoveX;
    //body.rect.y += MoveY;
    //return;
    
    while(true) {
        if(!collision(StopX + NewX, StopY, entities)) {
            StopX += NewX;
            body.rect.x += NewX;
        } else {
            body.velX = 0;
        }
        
        if(!collision(StopX, StopY + NewY, entities)) {
            StopY += NewY;
            body.rect.y += NewY;
        } else {
            body.velY = 0;
        }
        
        MoveX += -NewX;
        MoveY += -NewY;
        
        if(NewX > 0 && MoveX <= 0) NewX = 0;
        if(NewX < 0 && MoveX >= 0) NewX = 0;
        
        if(NewY > 0 && MoveY <= 0) NewY = 0;
        if(NewY < 0 && MoveY >= 0) NewY = 0;
        
        if(MoveX == 0) NewX = 0;
        if(MoveY == 0) NewY = 0;
        
        if(MoveX == 0 && MoveY == 0) 	break;
        if(NewX == 0 && NewY == 0) 		break;
    }
    
}

void CEntity::doLogic() {
    body.velY += 0.5;
}

