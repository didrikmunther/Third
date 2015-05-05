//
//  CSpriteSheet.cpp
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CSpriteSheet.h"

CSpriteSheet::CSpriteSheet() {
}

int CSpriteSheet::openFile(std::string fileName) {
    return texture.loadFromFile(fileName);
}

sf::Texture* CSpriteSheet::getTexture() {
    return &texture;
}

void CSpriteSheet::onCleanup() {
}