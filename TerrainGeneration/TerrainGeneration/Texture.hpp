//
//  Texture.hpp
//  TerrainGeneration
//
//  Created by Bernhard Fritz on 16/02/16.
//  Copyright © 2016 Bernhard Fritz. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

class Texture {
private:
    unsigned char* data;
    int width, height, channels;
    
public:
    Texture(unsigned char* data, int width, int height, int channels);
    ~Texture();
    
    unsigned char* getData();
    int getWidth();
    int getHeight();
    int getChannels();
    void assignToSlot(int slot);
};

#endif /* Texture_hpp */
