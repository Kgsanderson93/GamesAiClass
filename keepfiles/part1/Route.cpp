#include "Route.h"
Route::Route(int id, std::string name){
    this->id=id;
    this->name=std::move(name);
}
Edge Route::getEdge(int i ){
    return edges[i];
}
Edge &Route::getEdgeRef(int i){
    return edges[i];
}
void Route::addEdge(Node *to, Node *from){
    //printf("adding edge\n");
    edges.reserve(sizeof(edges.front()));
    edges.emplace_back(this->name, to->coords,from->coords, to->id.front(), from->id.front());
}