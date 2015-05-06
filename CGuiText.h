//
//  CGuiText.h
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CGuiText__
#define __Third__CGuiText__

#include <stdio.h>
#include "CCamera.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "CWindow.h"

class CGuiText {
public:
    CGuiText(int x, int y, std::string text, std::string fontKey, CAssetManager* assetManager);
    
    virtual void onLoop();                                          // Pure virtual
    virtual void onRender(CWindow* window, CCamera* camera); //
    
    bool toRemove;
    
protected:
    std::string _fontKey;
    CAssetManager* _assetManager;
    std::string _text;
    int _x, _y;
};

#endif /* defined(__Third__CGuiText__) */
