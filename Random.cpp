#include "Random.h"
#include <random>
#include <ctime>
std::mt19937 Random::random(time(0));
//redefine static vars in global space

int Random:: Int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(random);

}

int Random:: Float(float min, float max) {
    std::uniform_real_distribution<float> dist (min, max);
    return dist(random);
}