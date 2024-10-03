#include <SFML/Graphics.hpp>
//#include "Node.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include "Graph.h"
void Graph::addStop(int id, std::string name, int x, int y){
    stops.emplace_back(id, name, x, y);
}
void Graph::addRoute(int id, std::string name){
    routes.emplace_back(id, name);
}
void Graph::addEdge(Route *r, Node *to, Node *from){
    edges.emplace_back(r->name, to->coords, from->coords, to->id.front(), from->id.front());
}

Node Graph::getStop(int i){
    return stops[i];
}
Node & Graph::getStopRef(int i){
    return stops[i];
}
Route Graph::getRoute(int i ){
    return routes[i];
}
Route & Graph::getRouteRef(int i){
    return routes[i];
}
Edge Graph::getEdge(int i ){
    return edges[i];
}
Edge & Graph::getEdgeRef(int i){
    return edges[i];
}
void Graph::initialize(std::string stopList, std::string routeList){
    std::string filename{stopList};
    std::ifstream input{filename};
    std::ofstream out("out.txt");

    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    
    if (!input.is_open()) {
        std::cout << "Couldn't read file: " << filename << "\n";
        throw __EXCEPTION__;
    }
    std::vector<std::vector<std::string>> csvRows;
    for (std::string line; std::getline(input, line, input.widen('\n'));){
    std::istringstream ss(std::move(line));
    std::vector<std::string> row;
    if (!csvRows.empty()) {
       // We expect each row to be as big as the first row
      row.reserve(csvRows.front().size()*2);
    }
    // std::getline can split on other characters, here we use ','
    for (std::string value; std::getline(ss, value, ',');) {
      //std::cout << value;
      row.push_back(std::move(value));
      //std::cout << value;

    }
    
    csvRows.push_back(std::move(row));
  }
    printf("\n%ld\n", csvRows.size());
    //Graph *x=new Graph();

    int rowcounter=0;
    for (const std::vector<std::string>& row:csvRows){
        //std::cout <<row.front();

       if (rowcounter==0){
        rowcounter++;

       }else{
        //std::string id=std::move(row.front());
        int id=std::stoi(row.at(1));
        std::string name=std::move(row.at(2));
        float lat=std::stof(row.at(3));
        float longi=std::stof(row.at(4));
        
        Node *stop=findStop(id);
        if(stop==nullptr){
          //printf("got here!\n");
          addStop(id, name, lat, longi);
          //printf("%ld\n", stops.size());
        }else{
          stop->addAltId(id);
        }    
        rowcounter++;
       }

    }
    printf("Stops:%ld\n", stops.size());
    
    input.close();

    std::string filename2{routeList};
    std::ifstream input2{filename2};
    if (!input2.is_open()) {
      std::cout << "Couldn't read file: " << filename << "\n";
      throw __EXCEPTION__;
    }
    
    std::vector<std::vector<std::string>> csvRoutes;
    for (std::string line2; std::getline(input2, line2, input2.widen('\n'));){
    std::istringstream ss2(std::move(line2));
    std::vector<std::string> row2;
    if (!csvRoutes.empty()) {
       // We expect each row to be as big as the first row
      row2.reserve(csvRoutes.front().size());
    }
    // std::getline can split on other characters, here we use ','
    for (std::string value2; std::getline(ss2, value2, ',');) {
      //std::cout << value;
      row2.push_back(std::move(value2));
      //std::cout << value;

    }
    
    csvRoutes.push_back(std::move(row2));
  }
   printf("\n%ld\n", csvRoutes.size());
   
  rowcounter=0;
  //int lastid=-99;
 
 
    for (const std::vector<std::string>& row2:csvRoutes){
      
        //std::cout <<row.front();
      
      
      if (rowcounter==0){
        rowcounter++;
       
      }else{
        //printf("\n%ld\n", row2.size());
        int id=std::stoi(row2.front());
        //printf("id:%d\n",id );
        //if graph is the first route or a new route->
        //printf("last: %d, id:%d \n", lastid, id);
        
        //printf("%d", (int)y);
        Route *route=findRoute(id);
        if(route==nullptr){
            addRoute(id, row2.at(1));
        }
        

        std::string nameto=row2.at(2);
        //std::cout<<nameto<<"\n";

        std::string namefrom=row2.at(4);
        //std::cout<<namefrom<<"\n";

        int idto=std::stoi(row2.at(3));
        //printf("idto:%d\n",idto );


        int idfrom=std::stoi(row2.at(5));
        //printf("idfrom:%d\n",idfrom );
        Node *to=findStop(idto);
        if(to==nullptr){
          to=findStop(nameto);
          if(to==nullptr){
            printf("Supposedly we failed to find on to %d", idto);
            throw __EXCEPTION__;
          }
        }
        Node *from=findStop(idfrom);
        if(from==nullptr){
          from=findStop(namefrom);
          if(from==nullptr){
            printf("Supposedly we failed to find on from %d", idfrom);
            throw __EXCEPTION__;
          }
        }
        to->addEdge();
        from->addEdge();
        addEdge(route, to, from);
        route->addEdge(to, from);
      }
    }
}
     




Node &Graph::findStop(int id) {
    for (Node &stop : stops) {
      printf("id: %d\n", id);
      for(int idCheck :stop.id){
        printf("id checking: %d\n", idCheck);
        if (idCheck == id) {
          return &stop;
        }
      }
    }
    return nullptr;
  }

  Node* Graph::findStop(const std::string& name) { // Optional, uncomment if needed
    for (Node& stop : stops) {
      if (stop.name == name) {
        return &stop;
      }
    }
    return nullptr;
  }

  Route* Graph::findRoute(const std::string& name) { // Optional, uncomment if needed
    for (Route& stop : routes) {
      if (stop.name == name) {
        return &stop;
      }
    }
    return nullptr;
  }
  Route * Graph::findRoute(int id) {
    for (Route& stop : routes) {
      if (stop.id == id) {
        return &stop;
      }
    }
    return nullptr;
  }