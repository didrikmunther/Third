//
//  CGuiText.cpp
//  Third
//
//  Created by Didrik Munther on 22/04/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CGuiText.h"
#include "NSurface.h"

CGuiText::CGuiText(int x, int y, std::string text, std::string fontKey, CAssetManager* assetManager) :
    x(x), y(y), text(text), fontKey(fontKey), assetManager(assetManager), toRemove(false) {
}

void CGuiText::onLoop() {
}
void CGuiText::onRender(sf::RenderWindow* window, CCamera* camera) {
}