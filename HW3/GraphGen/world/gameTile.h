#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#pragma once

class gameTile {

public:
    bool ispassable;
    bool isExit;
    sf::Vector2f pos;
    sf::Texture texture;
    sf::Sprite sprite;
    gameTile(std::string name, float x, float y, bool ispassable, bool isexit);
    bool setUpSprite(std::string newtexture);
};