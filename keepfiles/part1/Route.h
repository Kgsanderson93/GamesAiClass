#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Edge.h"
//#include "Node.h"
//#include "Route.h"

class Route {
    public:
        int id;
        std::string name;
        std::vector <Edge> edges;
        Route();
        Route(int id, std::string name);
        Edge getEdge(int i );
        Edge &getEdgeRef(int i);
        void addEdge(Node *to, Node *From);
};