#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
//#include "Edge.h"

class Node{
    public:
        std::string name;
        std::vector<int> id;
        int Edges;
        sf::Vector2f coords;
        Node();
        Node( int id, std::string name, float x, float y);
        sf::Vector2f getCoord();
        void addAltId(int i);
        void addEdge();
        
};

