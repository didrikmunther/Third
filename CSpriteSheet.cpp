//
//  CSpriteSheet.cpp
//  Third
//
//  Created by Didrik Munther on 24/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "CSpriteSheet.h"

CSpriteSheet::CSpriteSheet(std::string fileName) {
    openFile(fileName);
}

void CSpriteSheet::openFile(std::string fileName) {
    texture.loadFromFile(fileName);
}

sf::Texture* CSpriteSheet::getTexture() {
    return &texture;
}

void CSpriteSheet::onCleanup() {
}