#pragma once
#include "gameTile.h"

class fruit
{
    public:
        int id;
        gameTile *location;
        sf::Texture texture;
        sf::IntRect frame;
        sf::Sprite sprite;
        sf::String fruitlabel;
        sf::Vector2f coords;
        float timesinceupdate;
        fruit(int id, gameTile *tile, sf::String label); 
};