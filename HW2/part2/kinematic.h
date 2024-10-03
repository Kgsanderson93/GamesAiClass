
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "SteeringData.h"
#include "crumb.h"
#include "fixedQ.h"


class Kinematic{

    public:
    const sf::Vector2f TOP_RIGHT = sf::Vector2f(550, 0);
    const sf::Vector2f BOT_RIGHT = sf::Vector2f(550, 550);
    const sf::Vector2f BOT_LEFT = sf::Vector2f(0, 550);
    const sf::Vector2f TOP_LEFT = sf::Vector2f(0, 0);
    float MAX_VEL=7;
    float MAX_ROT=7;
    float RAD_DEC=10;
    float RAD_SAT=5;
    float MAX_ACC=.2;
    //float MAX_DECEL=.50;
    float TIME_TO_TARGET=.1;
    

    sf::Sprite sprite;
    sf::Texture texture;
    bool isVisible;
    sf::Vector2f _position;
    float _orientation;
    sf::Vector2f _velocity;
    float _rotation;
    bool _satisfied;
    bool _satisfiedRotation;
    FixedQueue<crumb, 5> crumbs;
    //indice variables
    int target_idx=1;
    int crumb_idx=0;
    float last_time;
    float drop_timer=.5f;
    
    Kinematic (int windowx, int windowy);
    void update(SteeringData steering, float time);
    void trim();
    void trimRotation();
    

};


