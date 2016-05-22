//
//  CMusic.cpp
//  Tetris
//
//  Created by Didrik Munther on 22/05/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#include "CMusic.h"
#include "Define.h"


CMusic::CMusic()
    : music(nullptr)
{
    
}

CMusic::~CMusic() {
    onCleanup();
}

Mix_Music* CMusic::loadMusic(std::string fileName) {
    onCleanup();
    
    music = Mix_LoadMUS(fileName.c_str());
    
    return nullptr;
}

void CMusic::playMusic(int loops /* = 0 */) {
    if(music == nullptr || !PLAY_MUSIC) return;
    
    if(Mix_PlayingMusic())
        Mix_HaltMusic();
    
    Mix_PlayMusic(music, loops);
}

void CMusic::pauseMusic() {
    if(music == nullptr) return;
    
    if(Mix_PlayingMusic())
        Mix_PauseMusic();
}

void CMusic::resumeMusic() {
    if(music == nullptr) return;
    
    if(Mix_PausedMusic())
        Mix_ResumeMusic();
}

void CMusic::haltMusic() {
    if(music == nullptr) return;
    
    Mix_HaltMusic();
}

void CMusic::onCleanup() {
    if(music == nullptr) return;
    
    Mix_FreeMusic(music);
    music = nullptr;
}