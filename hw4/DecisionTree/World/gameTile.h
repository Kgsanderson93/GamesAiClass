#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "../Graph/Node.h"
#pragma once

class gameTile :public Node{

public:
    int idRow;
    int idCol;
    bool ispassable;
    bool isExit;
    sf::Vector2f pos;
    sf::Texture texture;
    sf::Sprite sprite;
    bool hasfruit;
    int fruitid;
    gameTile(std::string name, float x, float y, bool ispassable, bool isexit, int Row, int Col, int id);
    bool setUpSprite(std::string newtexture);
};