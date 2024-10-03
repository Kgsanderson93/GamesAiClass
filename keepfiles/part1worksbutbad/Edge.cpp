#include "Edge.h"
#include "util.h"

Edge::Edge(std::string linename, sf::Vector2f to, sf::Vector2f from, int idto, int idfrom, std::string nameTo, std::string nameFrom){
    this->nameFrom=nameFrom;
    this->nameTo=nameTo;
    this->linename=std::move(linename);
    this->idTo=idto;
    this->idFrom=idfrom;
    this->distance=length(sf::Vector2f(to.x-from.x,to.y-from.y));
}