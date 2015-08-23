////
////  CCollidable.cpp
////  Fifth
////
////  Created by Didrik Munther on 11/06/15.
////  Copyright (c) 2015 Didrik Munther. All rights reserved.
////
//
//#include "CCollidable.h"
//#include "CBody.h"
//
//CCollidable::CCollidable(Box rect) : body(rect) {
//    
//}
//
//bool CCollidable::hasProperty(int property) {
//    return properties & property;
//}
//
//void CCollidable::toggleProperty(int property) {
//    properties ^= property;
//}
//
//void CCollidable::addProperty(int property) {
//    properties |= property;
//}
//
//void CCollidable::removeProperty(int property) {
//    if(hasProperty(property)) toggleProperty(property);
//}