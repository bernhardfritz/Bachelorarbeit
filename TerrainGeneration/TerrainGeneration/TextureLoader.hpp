//
//  TextureManager.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 16/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef TextureLoader_hpp
#define TextureLoader_hpp

#include "Texture.hpp"
#include <string>

using namespace std;

class TextureLoader {
private:
public:
    Texture loadTexture(string filename, int forceChannels = 4);
};

#endif /* TextureLoader_hpp */
