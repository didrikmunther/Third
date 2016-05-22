//
//  CSound.cpp
//  Tetris
//
//  Created by Didrik Munther on 22/05/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include "CSound.h"
#include "Define.h"


CSound::CSound()
    : sound(nullptr)
{
    
}

void CSound::loadSound(std::string fileName) {
    onCleanup();
    sound = Mix_LoadWAV(fileName.c_str());
}

void CSound::playSound(int channel, int loops /* = 0 */) {
    if(sound == nullptr || !PLAY_MUSIC) return;
    
    Mix_PlayChannel(channel, sound, loops);
}

void CSound::onCleanup() {
    if(sound == nullptr) return;
    
    Mix_FreeChunk(sound);
    sound = nullptr;
}