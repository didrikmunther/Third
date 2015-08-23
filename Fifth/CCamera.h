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
#include "CWindow.h"

class CEntity;

typedef struct CameraOffset {
    float   x, y;
    int     w, h;
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
    
    void addCameraShake(float cameraShake);
    
    int cameraSway;
    
private:
    CameraOffset _offset;
    float _cameraShake;
    float _cameraShakeStep;
    
    CEntity* _target;
    bool _noTarget;
};

#endif /* defined(__Third__CCamera__) */
