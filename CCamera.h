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
#include "CWindow.h"

typedef struct CameraOffset {
    float x, y;
    int w;
    int h;
} CameraOffset;

class CCamera {
    
public:
    CCamera();
    
    void onInit(CWindow* window);
    void onLoop();
    
    void setTarget(CEntity* target);
    bool collision(CEntity* entity);
    bool collision(int x, int y, int w, int h);
    
    int offsetX();
    int offsetY();
    
    int cameraSway;
    
private:
    CameraOffset offset;
    
    CEntity* target;
    
    
};

#endif /* defined(__Third__CCamera__) */
