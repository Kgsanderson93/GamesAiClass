#include "Edge.h"
#include "util.h"

Edge::Edge(std::string linename, sf::Vector2f to, sf::Vector2f from, int idto, int idfrom){
    this->linename=linename;
    this->idTo=idto;
    this->idFrom=idfrom;
    this->distance=length(sf::Vector2f(to.x-from.x,to.y-from.y));
}