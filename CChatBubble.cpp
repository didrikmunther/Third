//
//  CChatBubble.cpp
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CChatBubble.h"
#include <sstream>
#include "NSurface.h"
#include "Define.h"
#include <iostream>

CChatBubble::CChatBubble(std::string text, CEntity* target, std::string fontKey, CAssetManager* assetManager, int type) :
    _target(target), _type(type), CGuiText(0, 0, text, fontKey, assetManager), _creationTime(_clock.getElapsedTime().asMilliseconds()),
    _r(0), _g(0), _b(0), _rB(220), _gB(220), _bB(220) {
    
        int textSize = 20;
        
    switch(type) {
        case ChatBubbleType::SAY:
            _r = _g = _b = 0;        // Black
            break;
        case ChatBubbleType::YELL:
            _r = 255;                // Red
            break;
        case ChatBubbleType::WHISPER:
            _g = _b = 255;            // Cyan
            break;
    }
    
    std::vector<std::string> splittedText;              // All this basically splits the text up so that it becomes several lines instead of a long one
    
    std::stringstream ss(text);
    std::string item;
    const char* delim = " ";
    while (std::getline(ss, item, *delim)) {
        splittedText.push_back(item);
    }
    
    int currentSize = 0;
    std::string currentString = "";
    for(int i = 0; i < splittedText.size(); i++) {
        if(currentSize > 10) {
            _TextVector.push_back(CText(currentString, textSize, fontKey, assetManager, sf::Color{(sf::Uint8)_r,(sf::Uint8)_g,(sf::Uint8)_b,255}));
            currentString = "";
            currentSize = 0;
        }
        currentSize += (int)splittedText[i].length();
        currentString += splittedText[i] + " ";
    }
    if(currentSize > 0)                 // For when the loop quits but there is still text that should be added
        _TextVector.push_back(CText(currentString, textSize, fontKey, assetManager, sf::Color{(sf::Uint8)_r,(sf::Uint8)_g,(sf::Uint8)_b,255}));
        
    int letterPerSecond = 5;
    _livingTime = (int)text.length() / letterPerSecond;
    
}

void CChatBubble::onLoop() {
    if(_clock.getElapsedTime().asMilliseconds() > _creationTime + _livingTime * 1000)
        toRemove = true;
}

void CChatBubble::onRender(sf::RenderWindow* window, CCamera* camera) {
    
    if(_TextVector.size() <= 0)
        return;
    if(_TextVector[0].getFont() == nullptr)
        return;
    
    int width, height;
    
    int widestLine = 0;
    int totalHeight = 0;
    auto i = _TextVector.begin();
    while(i != _TextVector.end()) {
        sf::Text tempText(i->getText()->c_str(), *i->getFont(), i->getSize());
        width = tempText.getLocalBounds().width;
        height = tempText.getLocalBounds().height;
        if(width > widestLine)
            widestLine = width;
        totalHeight += height;
        i++;
    }
    
    int marginX = 4;
    int marginY = 2;
    
    int floatOverHead = 20;
    
    if(camera->collision(_target->body.getX() + _target->body.getW() / 2 - widestLine / 2,
                          _target->body.getY() - totalHeight - floatOverHead,
                          widestLine,
                          totalHeight))
        NSurface::renderRect(_target->body.getX() + _target->body.getW() / 2 - widestLine / 2 - camera->offsetX(),
                             _target->body.getY() - totalHeight - floatOverHead - camera->offsetY(),
                             widestLine,
                             totalHeight,
                             window, _rB, _gB, _bB);
    
    int currentLine = 0;
    i = _TextVector.begin();
    while(i != _TextVector.end()) {
        sf::Text tempText(i->getText()->c_str(), *i->getFont(), i->getSize());
        width = tempText.getLocalBounds().width;
        height = tempText.getLocalBounds().height;
        int posX = _target->body.getX() + _target->body.getW() / 2 - width / 2 + marginX;
        int posY = _target->body.getY() - totalHeight + height * currentLine - marginY * 2;
        if(!camera->collision(posX, posY, width + marginX, height + marginY * 2)) {
            currentLine++;
            i++;
            continue;
        }
        i++->onRender(posX - camera->offsetX(),
                      posY - camera->offsetY(),
                      window);
        currentLine++;
    }
    
}







