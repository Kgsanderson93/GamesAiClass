#pragma once
#include <SFML/Graphics.hpp>
#include "gameTile.h"
#include <vector>
#include "Edge.h"
#include <bits/stdc++.h>
#include "util.h"


class GameWorld{
    public:
        std::vector<std::vector<gameTile *>> tiles;
        //std::vector<gameTile *> tileList;
        std::vector<Edge *> edges;
        std::unordered_map<int, std::vector<Edge *>> map;

        int gridx;
        GameWorld();
        gameTile *getTile(sf::Vector2f);

    private:
        sf::Vector2i exitPos;
        void setUpGameWorld();
        void setUpTiles();
};
