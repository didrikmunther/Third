//
//  CBackground.h
//  Fifth
//
//  Created by Didrik Munther on 08/07/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Fifth__CBackground__
#define __Fifth__CBackground__

#include <stdio.h>
#include <string>


struct BackgroundOffset {
    int x, y;
    float scale;
    BackgroundOffset(int x, int y, float scale = 1)
    : x(x), y(y), scale(scale) {  }
};

class CWindow;
class CSpriteContainer;
class CCamera;

class CBackground {
    
public:
    CBackground(std::string spriteContainerKey, float parallax, BackgroundOffset backgroundOffset = {0, 0});
    
    void onRender(CWindow* window, CCamera* camera);
    
    float getParallax();
    void setParallax(float parallax);
    
    std::string getSpriteContainerKey();
    void setSpriteContainerKey(std::string spriteContainerKey);
    
private:
    float _parallax;
    std::string _spriteContainerKey;
    BackgroundOffset _backgroundOffset;
    
};

#endif /* defined(__Fifth__CBackground__) */
