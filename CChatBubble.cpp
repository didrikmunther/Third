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

CChatBubble::CChatBubble(std::string text, CEntity* target, TTF_Font* font, int type) : target(target), type(type), CGuiText(0, 0, text, font) {
    
    int r = 0; int g = 0; int b = 0;
    switch(type) {
        case ChatBubbleType::SAY:
            b = 255;        // Black
            break;
        case ChatBubbleType::YELL:
            r = 255;                // Red
            break;
        case ChatBubbleType::WHISPER:
            g = b = 255;            // Cyan
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
            TextVector.push_back(CText(currentString, font, SDL_Color{(Uint8)r,(Uint8)g,(Uint8)b,255}));
            currentString = "";
            currentSize = 0;
        }
        currentSize += (int)splittedText[i].length();
        currentString += splittedText[i] + " ";
    }
    if(currentSize > 0)                 // For when the loop quits but there is still text that should be added
        TextVector.push_back(CText(currentString, font, SDL_Color{(Uint8)r,(Uint8)g,(Uint8)b,255}));
    
}

void CChatBubble::onLoop() {
    
}

void CChatBubble::onRender(SDL_Renderer *renderer, CCamera* camera) {
    NSurface::renderRect(target->body.getX() - (200 / 2) - camera->offsetX(), target->body.getY() - 225 - camera->offsetY(), 200, 200, renderer, 0, 255, 0);
    
    int currentLine = 0;
    auto i = TextVector.begin();
    while(i != TextVector.end()) {
        i++->onRender(target->body.getX() - (200 / 2), target->body.getY() + (target->body.getY() / 10) * currentLine - 200, renderer, camera);
        currentLine++;
    }
    
}







