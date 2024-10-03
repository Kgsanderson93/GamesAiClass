#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Node.h"
//#include "Graph.h"
class Edge{
    public:
        float distance;
        int idTo;
        int  idFrom;
        std::string linename;
        std::string nameFrom;
        std::string nameTo;



        Edge(std::string line, sf::Vector2f to, sf::Vector2f from, int idto, int idfrom, std::string nameTo, std::string nameFrom);
};