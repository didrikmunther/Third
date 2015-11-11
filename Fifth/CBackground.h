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
    BackgroundOffset(int x, int y, float scale = 1.75f)
    : x(x), y(y), scale(scale) {  }
};

class CWindow;
class CSpriteContainer;
class CCamera;

class CBackground {
    
public:
    CBackground(std::string spriteKey, float parallax, BackgroundOffset backgroundOffset = {0, 0});
    
    void onRender(CWindow* window, CCamera* camera);
    
    float getParallax();
    void setParallax(float parallax);
    
private:
    float _parallax;
    std::string _spriteKey;
    BackgroundOffset _backgroundOffset;
    
};

#endif /* defined(__Fifth__CBackground__) */
