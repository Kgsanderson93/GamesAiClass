
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "SteeringData.h"
#include "crumb.h"
#include "fixedQ.h"


class Kinematic{

    public:
    float MAX_VEL=6;
    float MIN_VEL=3;

    float MAX_ROT=15;
    float RAD_DEC=10;
    float RAD_SAT=5;
    float MAX_ACC=.1;
    float MAX_WANDER=15;
    float TIME_TO_TARGET=.05;
    
    //sf::RenderWindow window;
    sf::Sprite sprite;
    //sf::Texture texture;
    //bool isVisible;

    sf::Vector2f _position;
    float _orientation;
    sf::Vector2f _velocity;
    float _rotation;

    SteeringData updateData;

    bool _satisfied;
    int windowx;
    int windowy;
    FixedQueue<crumb, 10> crumbs;
    //indice variables
    int target_idx;
    int crumb_idx=0;
    float last_time;
    float drop_timer=.1f;
    sf::Vector2f spriteSize;
    Kinematic();
    const Kinematic operator=(const Kinematic &);
    Kinematic (int windowx, int windowy,  int index);
    void update(SteeringData steering, float time);
    void trim();
    void expand();
    void rdmspdAdjust();
    void trimRotation();
    void inBounds();


};
//constexpr Kinematic &Kinematic::operator=(const Kinematic &);



