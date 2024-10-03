#pragma once
#include <SFML/Graphics.hpp>
#include "gameTile.h"
#include <vector>
#include "../Graph/Edge.h"
#include <bits/stdc++.h>
#include "../utilities/util.h"
#include "fruit.h"
#include "../character/kinematic.h"

typedef std::pair<int, fruit *> piRefFru;

class GameWorld
{
public:
    int GameId;
    std::vector<std::vector<gameTile *>> tiles;
    std::vector<gameTile *> tileList;
    std::list<fruit *>fruitList;
    std::vector<Edge *> edges;
    std::unordered_map<int, std::vector<Edge *>> map;
    std::priority_queue<piRefFru, std::vector<piRefFru>, std::greater<piRefFru>> goals;
    int fruitIdinit;
    int gridx;
    float timesincedrop;
    float droptime=2.0f;
    bool test;
    GameWorld( int id, bool test);
    gameTile *getTile(sf::Vector2f pos);
    void updategoals(sf::Vector2f pos, float time);
    void addFruit(sf::String label);
    gameTile *randomTile();
    gameTile *getTileStrict(sf::Vector2f pos);
    gameTile *getTileNearby(sf::Vector2f pos);

private:
    sf::Vector2i exitPos;
    void setUpGameWorld();
    void setUpTiles();
};
