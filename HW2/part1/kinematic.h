
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "SteeringData.h"


class Kinematic{

    public:
    int windowX;
    int windowY;
    float MAX_VEL=5;
    float MAX_ROT=10;
    float RAD_DEC=10;
    float MAX_DECEL=.25;
    float TIME_TO_TARGET=1;

    sf::Sprite sprite;
    sf::Texture texture;
    bool isVisible;
    sf::Vector2f _position;
    float _orientation;
    sf::Vector2f _velocity;
    float _rotation;
    bool _satisfied;
    
    Kinematic (int windowx, int windowy);
    void update(SteeringData steering, float time);

};


