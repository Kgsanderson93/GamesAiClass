#include "Edge.h"
#include "util.h"
Edge::Edge(std::string linename, Node *from, Node *to){
    this->nameFrom=from->getName();
    this->nameTo=to->getName();
    this->linename=std::move(linename);
    this->idTo=to->getID();
    this->idFrom=from->getID();
    this->distance=CalcGPSDistance(to->getCoord().x, to->getCoord().y,from->getCoord().x, from->getCoord().y);
}
Edge::Edge(std::string linename, Node &from, Node &to){
    this->nameFrom=from.getName();
    this->nameTo=to.getName();
    this->linename=std::move(linename);
    this->idTo=to.getID();
    this->idFrom=from.getID();
    this->distance=CalcGPSDistance(to.getCoord().x, to.getCoord().y,from.getCoord().x, from.getCoord().y);
}
Edge::Edge(std::string line, gameTile *from, gameTile *to){
    this->nameFrom=from->getName();
    this->nameTo=to->getName();
    this->linename=std::move(linename);
    this->idTo=to->getID();
    this->idFrom=from->getID();
    this->distance=distancelength(to->getCoord(), from->getCoord());
}
