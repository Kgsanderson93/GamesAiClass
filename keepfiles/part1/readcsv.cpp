#include "Graph.h"
int main(){
  std::string stopList="stops.txt";
  std::string routeList="routesc.txt";
  Graph x;
  
  x.initialize(stopList, routeList);
 
  printf("Got out\n");
  printf("%d\n",x.getStop(6).id.front());
  
  
  printf("%ld",x.stops.size());
  printf("%ld",x.routes.size());

  return 0;
}

