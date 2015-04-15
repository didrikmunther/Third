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

typedef struct CameraOffset {
    float x, y;
    int w, h;
} CameraOffset;

class CCamera {
    
public:
    CCamera(CEntity* target, int WIDTH, int HEIGHT);
    CCamera(int WIDTH, int HEIGHT);
    
    void onLoop();
    
    void setTarget(CEntity* target);
    bool collision(CEntity* entity);
    
    int offsetX();
    int offsetY();
    
private:
    CameraOffset offset;
    
    CEntity* target;
    
    
};

#endif /* defined(__Third__CCamera__) */
