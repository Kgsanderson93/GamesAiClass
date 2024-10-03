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
void Route::addEdge(const Edge &n){
    //printf("adding edge\n");
    edges.reserve(sizeof(edges.front()));
    edges.push_back(n);
}