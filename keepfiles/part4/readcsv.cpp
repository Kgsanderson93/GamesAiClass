// #include "Node.h"
#include <bits/stdc++.h>
#include "util.h"
// #include "Node.h"
//  #include "Graph.h"

#include <fstream>
#include <sstream>
#include <iostream>

int main()
{
  std::srand(time(NULL));
  bool initializeSanFran = false;
  std::vector<std::vector<std::string>> csvRows;

  if (initializeSanFran)
  {

    std::string stopListSanFran = "Stops.txt";
    std::string routeListSanFran = "Edgesalt.txt";
    std::ofstream fileOUT("output.txt");
    std::cout.rdbuf(fileOUT.rdbuf());

    ReadFile(stopListSanFran, csvRows);
    printf("%ld\n", csvRows.size());
    std::vector<Node> NodeListSanFran = std::vector<Node>();

    bool first = true;
    int count = 0;
    // Make  stops
    for (const std::vector<std::string> &row : csvRows)
    {
      if (first)
      {
        first = false;
        continue;
      }
      std::string name = row.at(2);
      // name=trim(name," \t");
      // name=reduce(name, " ", " \t");
      Node *n = FindNode(NodeListSanFran, name);
      if (n == nullptr)
      {
        NodeListSanFran.emplace_back(count++, row.at(1), std::stof(row.at(2)), std::stof(row.at(3)));
      }
    }
    csvRows.clear();
    ReadFile(routeListSanFran, csvRows);
    // printf("Read in %ld rows\n", csvRows.size());

    first = true;
    std::string walking = "Walking Distance";
    std::vector<Edge> EdgeListSanFran = std::vector<Edge>();
    std::unordered_map<int, std::vector<Edge>> map;

    // make edges
    for (const std::vector<std::string> &row : csvRows)
    {
      if (first)
      {
        first = false;
        continue;
      }
      // std::cout << "Looking for " << row.at(0) << "\n";
      Node *from = FindNode(NodeListSanFran, row.at(0));
      // printf("found id from%d\n", from->id);
      if (from == nullptr)
      {
        std::cout << "Cant find " << row.at(0) << "\n";
        continue;
      }

      // printf("from %d\n",from->getID());
      Node *to = FindNode(NodeListSanFran, row.at(1));
      if (to == nullptr)
      {
        std::cout << "Cant find " << row.at(1) << "\n";
        continue;
      }
      // printf("to %d\n", to->getID());

      bool edgeExist = false;
      for (Edge e : EdgeListSanFran)
      {
        if (e.idFrom == from->getID() && e.idTo == to->getID())
        {
          edgeExist = true;
        }
      }
      if (from->getName().compare(to->getName()) != 0 && edgeExist == false)
      {
        // NodeListSanFran.at(from->getID()).Edges++;
        NodeListSanFran.at(to->getID()).Edges++;

        EdgeListSanFran.emplace_back(walking, from, to);
        EdgeListSanFran.back().distance = CalcGPSDistance(to->getCoord().x, to->getCoord().y, from->getCoord().x, from->getCoord().y);
        map[stoi(row.at(0))].emplace_back(walking, from, to);
        map[stoi(row.at(0))].back().distance=CalcGPSDistance(to->getCoord().x, to->getCoord().y, from->getCoord().x, from->getCoord().y);
      }
    }
   

    int trials = 10;

    testSearch(NodeListSanFran, map, trials);
  }



  bool initializelitte = false;
  if (initializelitte)
  {
    std::string stopListLittle = "Nodes_1.txt";
    std::string routeListLittle = "Edges_1.txt";
    csvRows.clear();
    ReadFile(stopListLittle, csvRows);
    printf("Rows read %ld\n", csvRows.size());
    std::vector<Node> NodeListLittle = std::vector<Node>();

    bool first = true;
    int count = 0;
    // Make  stops
    for (const std::vector<std::string> &row : csvRows)
    {
      if (first)
      {
        first = false;
        continue;
      }
      std::string name = row.at(0);
      // name=trim(name," \t");
      // name=reduce(name, " ", " \t");
      Node *n = FindNode(NodeListLittle, name);
      if (n == nullptr)
      {
        NodeListLittle.emplace_back(count++, row.at(0), std::stof(row.at(1)), std::stof(row.at(2)));
      }

    }
    printf("Noes made %ld\n", NodeListLittle.size());

    ReadFile(routeListLittle, csvRows);
    printf("Read in %ld rows\n", csvRows.size());

    first = true;
    std::string walking = "Walking Distance";
    std::vector<Edge> EdgeListLittle = std::vector<Edge>();
    std::unordered_map<int, std::vector<Edge>> map;

    // make edges
    for (const std::vector<std::string> &row : csvRows)
    {
      if (first)
      {
        first = false;
        continue;
      }
      // std::cout << "Looking for " << row.at(0) << "\n";
      Node *from = FindNode(NodeListLittle, row.at(0));
      // printf("found id from%d\n", from->id);
      if (from == nullptr)
      {
        std::cout << "Cant find " << row.at(0) << "\n";
        continue;
      }

      // printf("from %d\n",from->getID());
      Node *to = FindNode(NodeListLittle, row.at(1));
      if (to == nullptr)
      {
        std::cout << "Cant find " << row.at(1) << "\n";
        continue;
      }
      // printf("to %d\n", to->getID());

      bool edgeExist = false;
      for (Edge e : EdgeListLittle)
      {
        if (e.idFrom == from->getID() && e.idTo == to->getID())
        {
          edgeExist = true;
        }
      }
      if (from->getName().compare(to->getName()) != 0 && edgeExist == false)
      {
        // NodeListSanFran.at(from->getID()).Edges++;
        NodeListLittle.at(to->getID()).Edges++;

        EdgeListLittle.emplace_back(walking, from, to);
        EdgeListLittle.back().distance = CalcGPSDistance(to->getCoord().x, to->getCoord().y, from->getCoord().x, from->getCoord().y);
        map[stoi(row.at(0))].emplace_back(walking, from, to);
        map[stoi(row.at(0))].back().distance=CalcGPSDistance(to->getCoord().x, to->getCoord().y, from->getCoord().x, from->getCoord().y);

      }
    }
    printf("Edges made %ld\n", EdgeListLittle.size());

    int trials = 10;

    testSearch(NodeListLittle, map, trials);
  }


  bool initializebig=true;
  if (initializebig)
  {
    std::string stopListBig = "Nodes_2.txt";
    std::string routeListBig = "Edges_2.txt";
    csvRows.clear();
    ReadFile(stopListBig, csvRows);
    printf("Rows read %ld\n", csvRows.size());
    std::vector<Node> NodeListBig = std::vector<Node>();

    bool first = true;
    int count = 0;
    // Make  stops
    for (const std::vector<std::string> &row : csvRows)
    {
      if (first)
      {
        first = false;
        continue;
      }
      std::string name = row.at(0);
      // name=trim(name," \t");
      // name=reduce(name, " ", " \t");
      
      NodeListBig.emplace_back(count++, row.at(0), std::stof(row.at(1)), std::stof(row.at(2)));
      

    }
    printf("Rows made %ld\n", NodeListBig.size());
    csvRows.clear();
    ReadFile(routeListBig, csvRows);
    printf("Read in %ld rows\n", csvRows.size());

    first = true;
    std::string walking = "Walking Distance";
    std::vector<Edge> EdgeListBig = std::vector<Edge>();
    std::unordered_map<int, std::vector<Edge>> map;
    // make edges
    for (const std::vector<std::string> &row : csvRows)
    {
      if (first)
      {
        first = false;
        continue;
      }
      // std::cout << "Looking for " << row.at(0) << "\n";
      Node *from = &NodeListBig.at(stoi(row.at(0)));
      // printf("found id from%d\n", from->id);
      if (from == nullptr)
      {
        std::cout << "Cant find " << row.at(0) << "\n";
        continue;
      }

      // printf("from %d\n",from->getID());
      Node *to = &NodeListBig.at(stoi(row.at(1)));
      if (to == nullptr)
      {
        std::cout << "Cant find " << row.at(1) << "\n";
        continue;
      }

      
      if (from->getName().compare(to->getName()) != 0) //&& edgeExist == false)
      {
        //NodeListSanFran.at(from->getID()).Edges++;
        NodeListBig.at(to->getID()).Edges++;

        EdgeListBig.emplace_back(walking, from, to);
        EdgeListBig.back().distance = CalcGPSDistance(to->getCoord().x, to->getCoord().y, from->getCoord().x, from->getCoord().y);
        map[stoi(row.at(0))].emplace_back(walking, from, to);
        map[stoi(row.at(0))].back().distance=CalcGPSDistance(to->getCoord().x, to->getCoord().y, from->getCoord().x, from->getCoord().y);
      }
    }

    for(Node &n:NodeListBig){
       int a = rand() % (NodeListBig.size());
      while(a==n.id){
        a = rand() % (NodeListBig.size());
      }
        NodeListBig.at(n.getID()).Edges++;
        //printf("%d now has %d edges\n", n.getID(), n.Edges);
        EdgeListBig.emplace_back(walking, &NodeListBig.at(a), &NodeListBig.at(n.getID()));
        EdgeListBig.back().distance = CalcGPSDistance( NodeListBig.at(a).getCoord().x, NodeListBig.at(a).getCoord().y, n.getCoord().x, n.getCoord().y);
        map[a].emplace_back(walking, &NodeListBig.at(a), &NodeListBig.at(n.getID()));
        map[a].back().distance=CalcGPSDistance(NodeListBig.at(a).getCoord().x, NodeListBig.at(a).getCoord().y, n.getCoord().x, n.getCoord().y);
    }
    //printf("Edges made %ld\n",map.size());

    // make graph
    std::ofstream file;
    file.open("output.txt");
    for(std::pair<int, std::vector<Edge>> p: map){
      file<<p.first<<" Node has "<< p.second.size()<<" outgoing edges and "<<NodeListBig.at(p.first).Edges<<" incoming edges\n";
      for(Edge e:p.second){
        file<<"\t->"<<e.idTo<<" distance: "<<e.distance<<"\n";
      }
    }
    file.close();
    int trials = 1;

    testSearch(NodeListBig, map, trials);
    printf("WE TESTED!\n");
    printEdges(EdgeListBig, NodeListBig);
    printf("WE PRINT!\n");
    printNodes(NodeListBig);
    printf("WE print!!\n");

  }

  //try passing in a reference to read file to avoid 
  return 0;
}
