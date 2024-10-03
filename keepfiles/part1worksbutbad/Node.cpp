#include "Node.h"


Node::Node( int id, std::string name,float x, float y){
    this->id=std::vector<int>();
    this->id.push_back(id);
    this->name=std::move(name);
    this->coords=sf::Vector2f(x,y);
    //this->edges=std::vector<Edge> ();
    this->Edges=0;

}

void Node::addAltId(int i){
    this->id.reserve(sizeof(i));
    this->id.push_back(i);
}
void Node::addEdge(){
    Edges++;
}