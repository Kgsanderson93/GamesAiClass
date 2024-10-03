#include <SFML/Graphics.hpp>
#include "gameTile.h"
#include <vector>

class GameWorld{
    public:
        std::vector<std::vector<gameTile *>> tiles;
        int gridx;
        GameWorld();
        gameTile getTile(sf::Vector2f);
    private:
        sf::Vector2i exitPos;
        void setUpGameWorld();
        void setUpTiles();
};
