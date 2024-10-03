#pragma once
#include "kinematic.h"
class flock{
    public:
    int numBoid;
    std::vector<Kinematic> boids;

    Kinematic getBoid(int i);
    Kinematic &getBoidRef(int i);
    void addBoid(const Kinematic& b);
    void flocking(float time);
    void getGoing(float time);
};