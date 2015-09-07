////
////  CServer.h
////  Fifth-Server
////
////  Created by Didrik Munther on 21/05/15.
////  Copyright (c) 2015 Didrik Munther. All rights reserved.
////
//
//#ifndef __Fifth_Server__CServer__
//#define __Fifth_Server__CServer__
//
//#include <stdio.h>
//#include "CServerInstance.h"
//
//class CServer {
//    
//public:
//    CServer();
//    
//    int onExecute();
//    
//private:
//    int _onInit();
//    int _onRestart();
//    
//    void _onLoop();
//    
//    int _onCleanup();
//    
//    void _handleInput();
//    
//    bool _isRunning;
//    
//    // Timer
//    
//    float _lastTime;
//    float _timer;
//    const float _ns;
//    float _delta;
//    int _frames;
//    int _updates;
//    
//    // Misc
//    
//    //CInstance instance;
//    std::map<std::string, CServerInstance*> _instanceVector;
//    
//    sf::Thread _inputThread;
//    
//    
//    
//};
//
//#endif /* defined(__Fifth_Server__CServer__) */