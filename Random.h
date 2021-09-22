#pragma once
#include <random>

class Random {

    static std::mt19937 random;
// all instances of this class share this one variable, impossible to duplicate this
public:
    static int Int(int min, int max);
    static int Float(float min, float max);

};