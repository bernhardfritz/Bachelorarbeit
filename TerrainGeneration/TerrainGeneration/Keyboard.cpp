//
//  Keyboard.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 30/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#include <iostream>
#include "Keyboard.hpp"

void Keyboard::update(int key, bool state) {
    keys[key] = state;
}

bool Keyboard::getState(int key) {
    if(keys.find(key) != keys.end()) {
        return keys[key];
    } else return false;
}

void Keyboard::print() {
    for(map<int, bool>::iterator i = keys.begin(); i != keys.end(); i++) {
        cout << i->first << ": " << i->second << endl;
    }
}