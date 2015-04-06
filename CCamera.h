//
//  CCamera.h
//  Third
//
//  Created by Didrik Munther on 02/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CCamera__
#define __Third__CCamera__

#include <stdio.h>
#include <SDL2/SDL.h>
#include "CEntity.h"
#include <memory>

class CCamera {
    
public:
    CCamera(std::shared_ptr<CEntity> target, int WIDTH, int HEIGHT);
    CCamera(int WIDTH, int HEIGHT);
    
    void setTarget(std::shared_ptr<CEntity> target);
    bool collision(CEntity* entity);
    
    int offsetX();
    int offsetY();
    
private:
    SDL_Rect offset;
    SDL_Rect oldOffset;
    
    std::shared_ptr<CEntity> target;
    
    
};

#endif /* defined(__Third__CCamera__) */
