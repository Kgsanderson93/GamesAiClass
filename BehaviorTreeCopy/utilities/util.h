#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "../Graph/Edge.h"
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
#include "../World/gameWorld.h"

float VecToDegAngle(sf::Vector2f vec);
float length(sf::Vector2f vec);
float distancelength(sf::Vector2f vecA, sf::Vector2f vecB);
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
void aStar(std::vector<Node>& Nodelist,std::unordered_map<int, std::vector<Edge>> &map, int start, int goal,  std::unordered_map<Node, Node> &came_from,
                     std::unordered_map<Node, float> &cost_so_far, bool early, bool admissable, bool tracking);
void prettyprint(std::vector<Node> &path);
void prettyprintTile(std::vector<gameTile *> &path);
void printNodes(std::vector<Node> &NodeList);
void printEdges(std::vector<Edge> &adj_list, std::vector<Node> &NodeList);
void testSearch(std::vector<Node> &Nodelist,std::unordered_map<int, std::vector<Edge>> & map, int trials, bool tracking);
void make_path(Node &Start, Node &End, std::unordered_map<Node, Node> &came_from, std::vector<Node> &returnedpath);
void create_room_outline();
void testSearchRef(std::vector<gameTile *> &Nodelist,std::unordered_map<int, std::vector<Edge *>> & map, int trials, bool tracking);
void aStarRef(std::vector<gameTile *>& Nodelist, std::unordered_map<int, std::vector<Edge*>> &map, int start, int goal, std::unordered_map<gameTile*, gameTile*> &came_from, std::unordered_map<gameTile*, float> &cost_so_far, bool early, bool admissable, bool tracking);
void prettyprintRef(std::vector<gameTile*> &path);
void make_pathRef(gameTile* start, gameTile* end, std::unordered_map<gameTile*, gameTile*> &came_from, std::vector<gameTile*>& path );

void dijkstra_searchRef(std::vector<gameTile *>& Nodelist, std::unordered_map<int, std::vector<Edge *>> &map, int start, int goal, std::unordered_map<gameTile *, gameTile *> &came_from, std::unordered_map<gameTile *, float> &cost_so_far, bool early, bool tracking);


