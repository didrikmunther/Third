//
//  Cpython.cpp
//  Fifth-Client
//
//  Created by Lucas Karlsson on 28/05/15.
//  Copyright (c) 2015 Didrik Munther. All rights reserved.
//

#include "Cpython.h"
#include <iostream>
#include <Python/python.h>
#include <string>
#include <fstream>


Cpython::Cpython() {  }

using namespace std;

void Cpython::onInit(){
    Py_Initialize();
    
    if (!Py_Initialize){
        cout << "Could not initialize python\n";
    }
    
    if (Py_Initialize){
        cout << "Python initialized\n";
    }
   
    
    
    
    
    PyRun_SimpleString("print('Hello')");
    
    
    
    
}