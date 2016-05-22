//
//  CSound.h
//  Tetris
//
//  Created by Didrik Munther on 22/05/16.
//  Copyright © 2016 Didrik Munther. All rights reserved.
//

#ifndef CSound_h
#define CSound_h

#include <stdio.h>
#include <string>
#include <SDL2_mixer/SDL_mixer.h>


class CSound {
    
    friend class CAssetManager;
    
public:
    CSound();
    
    void loadSound(std::string fileName);
    void onCleanup();
    
    Mix_Chunk* sound;
    
private:
    void playSound(int channel, int loops = 0);
    
};

#endif /* CSound_h */
