//
//  Keyboard.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 30/10/15.
//  Copyright © 2015 Bernhard Fritz. All rights reserved.
//

#ifndef Keyboard_hpp
#define Keyboard_hpp

#include <map>

using namespace std;

class Keyboard {
private:
    map<int,bool> keys;
    map<int,double> timestamps;
    
public:
    void update(int key, bool state);
    bool getState(int key);
    void setTimestamp(int key, double timestamp);
    double getTimestamp(int key);
    void print();
};

#endif /* Keyboard_hpp */
