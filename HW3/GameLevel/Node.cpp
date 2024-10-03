#include "Node.h"

Node::Node(){
    id=-99;
    name="";
    coords.x=0;
    coords.y=0;
    Edges=0;
}
Node::Node( int id, std::string name,float x, float y){
    this->id=id;
    this->name=std::move(name);
    this->coords=sf::Vector2f(x,y);
    this->Edges=0;
    //this->edges=std::vector<Edge> ();


}

sf::Vector2f Node::getCoord(){
    return coords;
}
int Node::getID(){
    return id;
}
std::string Node::getName(){
    return name;
}
bool Node::operator==(const Node& a) const
  {
    return(this->name.compare(a.name)==0);
  }

bool Node::operator<(const Node& a) const
  {
    return std::tie(this->coords.x, this->coords.y)<std::tie(a.coords.x, a.coords.y);
  }



  