//
//  CMusic.h
//  Tetris
//
//  Created by Didrik Munther on 22/05/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#ifndef CMusic_h
#define CMusic_h

#include <stdio.h>
#include <string>

#include <SDL2_mixer/SDL_mixer.h>


class CMusic {
    
    friend class CAssetManager;
    
public:
    CMusic();
    ~CMusic();
    
    Mix_Music* loadMusic(std::string fileName);
    void pauseMusic();
    void resumeMusic();
    void haltMusic();
    void onCleanup();
    
    Mix_Music* music;
    
private:
    void playMusic(int loops = 0);
    
};

#endif /* CMusic_h */
