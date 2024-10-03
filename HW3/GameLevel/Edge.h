#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Node.h"
#include "gameTile.h"
//#include "util.h"

//#include "util.h"

//#include "Graph.h"
class Edge{
    public:
        float distance;
        int idTo;
        int  idFrom;
        std::string linename;
        std::string nameFrom;
        std::string nameTo;


        Edge(std::string line, Node &from, Node &to);
        Edge(std::string line, gameTile *from, gameTile *to);
        Edge(std::string line, Node *from, Node *to);
        //Edge(std::string line, int idfrom, std::string namefrom, int idTo, std::string nameTo);

};