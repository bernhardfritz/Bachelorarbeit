//
//  Mouse.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 08/03/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Mouse_hpp
#define Mouse_hpp

#include <stdio.h>
#include <map>

using namespace std;

class Mouse {
private:
    map<int,bool> buttons;
    
public:
    void update(int button, bool state);
    bool getState(int button);
    void print();
};

#endif /* Mouse_hpp */
