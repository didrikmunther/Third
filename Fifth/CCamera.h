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


class CEntity;
class CWindow;

struct CameraOffset {
    float   x, y;
    int     w, h;
};

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
    
    int getWidth() { return _offset.w; }
    int getHeight() { return _offset.h; }
    
    void addCameraShake(int interval);
    
    int cameraSway;
    
    float getZoom() { return _cameraZoom; }
    
private:
    CameraOffset _offset;
    float _cameraShakeInterval;
    float _cameraShake;
    
    CEntity* _target;
    
    float _cameraZoom;
};

#endif /* defined(__Third__CCamera__) */
