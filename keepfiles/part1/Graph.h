#pragma once
//#include <SFML/Graphics.hpp>
#include <vector>
#include "Edge.h"
//#include "Node.h"
#include "Route.h"

class Graph{
    public:
        std::vector<Node> stops;
        std::vector<Edge> edges;
        std::vector<Route> routes;

        void initialize( std::string stopList, std::string routelist);
        void addStop(int id, std::string name, int x, int y);
        void addRoute(int id, std::string name);
        void addEdge(Route *r, Node *to, Node *From);
        Node getStop(int i);
        Node &getStopRef(int i);
        Route getRoute(int i );
        Route &getRouteRef(int i);
        Edge getEdge(int i );
        Edge &getEdgeRef(int i);

        Node* findStop(const std::string& name);
        Node* findStop(int id);

        Route* findRoute(const std::string& name);
        Route* findRoute(int id);

        Node* findEdge(const std::string& name);
        Node* findEdge(int id);



};