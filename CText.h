//
//  CText.h
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CText__
#define __Third__CText__

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include "CCamera.h"
#include <string>
#include "CAssetManager.h"
#include "CRenderable.h"

class CText : public CRenderable {
    
public:
    CText(std::string text, int size, std::string fontKey, CAssetManager* assetManager, sf::Color color);
    
    void onRender(int x, int y, sf::RenderTarget& renderTarget, CCamera* camera);
    void onRender(int x, int y, sf::RenderTarget& renderTarget);
    
    sf::Font* getFont();
    int getSize();
    std::string* getText();
    sf::Color* getColor();
    
private:
    std::string _text;
    int _size;
    std::string _fontKey;
    CAssetManager* _assetManager;
    sf::Color _color;
    
};

#endif /* defined(__Third__CText__) */