#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

// #include "Edge.h"

class Node
{
public:
       int id;
       std::string name;
       sf::Vector2f coords;

       //sf::Vector2f coords;
       int Edges;
       Node();
       Node(int id, std::string name, float x, float y);
       sf::Vector2f getCoord();
       int getID();
       std::string getName();
       bool operator==(const Node &a) const;
       bool operator<(const Node &a) const;

};

namespace std
{
       /* implement hash function so we can put GridLocation into an unordered_set */
       template <>
       struct hash<Node>
       {
              std::size_t operator()(const Node &id) const noexcept
              {
                     // NOTE: better to use something like boost hash_combine
                     return std::hash<int>()(id.id ^ (id.Edges<< 16));
              }
       };
}