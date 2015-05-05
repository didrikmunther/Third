//
//  CGame.h
//  Third
//
//  Created by Didrik Munther on 12/03/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#ifndef __Third__CGame__
#define __Third__CGame__

#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <sstream>
#include <string>
#include "CEntityManager.h"
#include "CAssetManager.h"
#include "CWindow.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "CChatBubble.h"

class CGame {
    
public:
    
    CGame();
    ~CGame();
    
    int onExecute();
    
private:
    
    // Main functions
    
    int _onInit();
    
    void _onEvent(sf::Event* event);
    
    void _handleKeyStates();
    
    void _onLoop();
    
    void _onRender();
    
    int _onCleanup();
    
    // Main variables
    
    bool _running;
    
    CCamera _camera;
    CPlayer* _player;
    
    // Timer variables
    
    sf::Clock _clock;
    
    float _lastTime;
    float _timer;
    const float _ns;
    float _delta;
    int _frames;
    int _updates;
    
    // Misc variables
    
    std::stringstream _title;
    std::string _intro;
    
    // Game variables
    
    CEntityManager _entityManager;
    CAssetManager _assetManager;
    CWindow _window;
    
    // Temp variables
    
    CEntity* _block;
    
};

#endif /* defined(__Third__CGame__) */
