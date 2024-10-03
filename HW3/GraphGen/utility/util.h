#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "../graph/Edge.h"
//#include "Pq.h"
#include <math.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <tuple>
#include <algorithm>

#include <unordered_set>
#include <array>
#include <utility>
#include <queue>
#include <cstdlib>
float manhatten(sf::Vector2f a, sf::Vector2f b);
float length(sf::Vector2f vec);
bool inRadiusDecel(float distance);
bool inRadiusSat(float distance);
sf::Vector2f normalize(sf::Vector2f vec);
float mapToRange(float roation);
float sqrMag(sf::Vector2f vec);
// Utility functions for degree-radian conversion
float degreesToRadians(float degrees);

float radiansToDegrees(float radians);
float randomBinomial();
float randomDirection();
sf::Vector2f getOrientationAsVector(float orientation);

float dotProduct(sf::Vector2f a, sf::Vector2f b);
sf::Vector2f randomPosition(int windowx, int windowy);
void ReadFile(std::string filename, std::vector<std::vector<std::string>> &csvRows);
Node *FindNode(std::vector<Node> &List, std::string name);
float CalcGPSDistance(float latitud1, float longitud1, float latitud2, float longitud2);
void dijkstra_search(std::vector<Node> &Nodelist,std::unordered_map<int, std::vector<Edge>> &map, int start, int goal,
                     std::unordered_map<Node, Node> &came_from,
                     std::unordered_map<Node, float> &cost_so_far, bool early, bool tracking);
void aStar(std::vector<Node>& Nodelist, std::unordered_map<int, std::vector<Edge>> &map, int start, int goal, std::unordered_map<Node, Node> &came_from, std::unordered_map<Node, float> &cost_so_far, bool early, bool neighborNeg, bool NeighbornPos, bool manhattendist, bool tracking);
void prettyprint(std::vector<Node> &path);
void printNodes(std::vector<Node> &NodeList);
void printEdges(std::vector<Edge> &adj_list, std::vector<Node> &NodeList);
void testSearch(std::vector<Node> &Nodelist,std::unordered_map<int, std::vector<Edge>> & map, int trials, bool tracking);
void make_path(Node &Start, Node &End, std::unordered_map<Node, Node> &came_from, std::vector<Node> &returnedpath);
void create_room_outline();
