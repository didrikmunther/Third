//
//  CChatBubble.cpp
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include <sstream>

#include "CChatBubble.h"
#include "CAssetManager.h"
#include "CEntity.h"
#include "NSurface.h"
#include "CCamera.h"


CChatBubble::CChatBubble(std::string text, CEntity* target, std::string fontKey, ChatBubbleType type) :
    _target(target), _type(type), CGuiText(0, 0, text, fontKey), _widestLine(0), _totalHeight(0),
    _r(0), _g(0), _b(0), _rB(220), _gB(220), _bB(220) {
    
    if(CAssetManager::getFont(fontKey) == nullptr) {
        return;
    }
        
    int textSize = 20;
    int letterPerSecond = 100;
    bool instantText = false;
        
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
        case ChatBubbleType::INSTANT_TALK:
            instantText = true;
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
            _TextVector.push_back(CText(currentString, textSize, fontKey, Color{(Uint8)_r, (Uint8)_g, (Uint8)_b, 255}));
            currentString = "";
            currentSize = 0;
        }
        currentSize += (int)splittedText[i].length();
        currentString += splittedText[i] + " ";
    }
    if(currentSize > 0)                 // For when the loop quits but there is still text that should be added
        _TextVector.push_back(CText(currentString, textSize, fontKey, Color{(Uint8)_r, (Uint8)_g, (Uint8)_b, 255}));
        
    if(!instantText)
        _livingTime = (int)text.length() / letterPerSecond + 3;
    else
        _livingTime = 0;
        
    int width, height;
        
    auto i = _TextVector.begin();
    while(i != _TextVector.end()) {
        TTF_SizeText(i->getFont(), i->getText()->c_str(), &width, &height);
        if(width > _widestLine)
            _widestLine = width;
        _totalHeight += height;
        i++;
    }
    
}

void CChatBubble::onLoop() {
    if(SDL_GetTicks() > _creationTime + _livingTime * 1000)
        _toRemove = true;
}

void CChatBubble::onRender(CWindow* window, CCamera* camera, RenderFlags renderFlags) {
    
    if(_target == nullptr)
        return;
    if(_TextVector.size() <= 0)
        return;
    if(_TextVector[0].getFont() == nullptr)
        return;
    
    int marginX = 2;
    int marginY = 2;
    
    int floatOverHead = 32;
    
    int boxX = _target->body->getX() + _target->body->getW() / 2 - _widestLine / 2;
    int boxY = _target->body->getY() - _totalHeight - floatOverHead;
    int boxW = _widestLine + marginX;
    int boxH = _totalHeight + marginY;
    
    if(camera->collision(boxX,
                         boxY,
                         boxW,
                         boxH)) {
        NSurface::renderRect(boxX - camera->offsetX(),
                             boxY - camera->offsetY(),
                             boxW,
                             boxH,
                             window, _rB, _gB, _bB, 100);
        
        int currentLine = 0;
        auto i = _TextVector.begin();
        while(i != _TextVector.end()) {
            int width, height;
            TTF_SizeText(i->getFont(), i->getText()->c_str(), &width, &height);
            int posX = _target->body->getX() + _target->body->getW() / 2 - width / 2 + marginX;
            int posY = _target->body->getY() - _totalHeight + height * currentLine - floatOverHead + marginY * 2;
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
}







