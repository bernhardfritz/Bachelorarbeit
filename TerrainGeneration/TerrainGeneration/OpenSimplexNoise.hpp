//
//  OpenSimplexNoise.hpp
//  TerrainGeneration
//
//  OpenSimplex Noise written in Java by Kurt Spencer
//  Translated from Java to C++ by Bernhard Fritz on 27/02/16.
//

#ifndef OpenSimplexNoise_hpp
#define OpenSimplexNoise_hpp

class OpenSimplexNoise {
private:
    const double STRETCH_CONSTANT_2D = -0.211324865405187;    //(1/Math.sqrt(2+1)-1)/2;
    const double SQUISH_CONSTANT_2D = 0.366025403784439;      //(Math.sqrt(2+1)-1)/2;
    const double NORM_CONSTANT_2D = 47;
    const static long DEFAULT_SEED = 0;
    
    //Gradients for 2D. They approximate the directions to the
    //vertices of an octagon from the center.
    short gradients2D[16] = {
        5,  2,    2,  5,
        -5,  2,   -2,  5,
        5, -2,    2, -5,
        -5, -2,   -2, -5,
    };
    
    short perm[256];
    short permGradIndex3D[256];
    
    double extrapolate(int xsb, int ysb, double dx, double dy);
    static int fastFloor(double x);
public:
    OpenSimplexNoise() : OpenSimplexNoise(DEFAULT_SEED) {};
    OpenSimplexNoise(long seed);
    
    double eval(double x, double y);
};

#endif /* OpenSimplexNoise_hpp */
