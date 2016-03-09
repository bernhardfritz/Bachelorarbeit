//
//  Mouse.cpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 08/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#include <iostream>
#include "Mouse.hpp"

void Mouse::update(int button, bool state) {
    buttons[button] = state;
}

bool Mouse::getState(int button) {
    if(buttons.find(button) != buttons.end()) {
        return buttons[button];
    } else return false;
}

void Mouse::print() {
    for(map<int, bool>::iterator i = buttons.begin(); i != buttons.end(); i++) {
        cout << i->first << ": " << i->second << endl;
    }
}