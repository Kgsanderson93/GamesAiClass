// 
// @author Derek Martin (dmartin7@ncsu.edu)
//
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>



class crumb : sf::CircleShape
{
    public :
        crumb();
        sf::Color color=sf::Color::Blue;
        void draw(sf::RenderWindow* window);
        void drop(float x, float y);
        void drop(sf::Vector2f position);
        sf::Vector2f query();

    private:
};