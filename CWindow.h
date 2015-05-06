//
//  CWindow.h
//  Third
//
//  Created by Didrik Munther on 19/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CWindow__
#define __Third__CWindow__

#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <string>

class CWindow {
    
public:
    CWindow();
    
    int onInit(std::string title, int width, int height);
    int newWindow(std::string title, int width, int height);
    void setTitle(std::string title);
    void onCleanup();
    
    int getWidth();
    int getHeight();
    
    sf::RenderWindow* getWindow();
    sf::RenderTexture* getRenderTexture();
    sf::Sprite* getSprite();
    
private:
    sf::RenderWindow _window;
    sf::RenderTexture _renderTexture;
    sf::Sprite _sprite;
    
    int _screenHeight, _screenWidth;
    
    
    
    
};

#endif /* defined(__Third__CWindow__) */
