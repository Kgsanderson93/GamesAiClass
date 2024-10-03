// #include "Node.h"
#include <bits/stdc++.h>
#include "utility/util.h"
// #include "Node.h"
//  #include "Graph.h"
#include "character/kinematic.h"
#include "steering/steeringbehavior.h"
#include "steering/SteeringData.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "world/gameWorld.h"

int main()
{
  std::srand(time(NULL));
  bool initializeSanFran = true;
  std::vector<std::vector<std::string>> csvRows;

  if (initializeSanFran)
  {

    std::string stopListSanFran = "graphText/Stops.txt";
    std::string routeListSanFran = "graphText/Edgesalt.txt";

    ReadFile(stopListSanFran, csvRows);
    printf("%ld\n", csvRows.size());
    csvRows.clear();
    ReadFile(stopListSanFran, csvRows);
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
      std::string name = row.at(1);
      // name=trim(name," \t");
      // name=reduce(name, " ", " \t");

      NodeListBig.emplace_back(count++, row.at(1), std::stof(row.at(2)), std::stof(row.at(3)));
    }
    printf("Rows made %ld\n", NodeListBig.size());
    csvRows.clear();
    ReadFile(routeListSanFran, csvRows);
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
      std::cout << "Cant find " << row.at(0) << "\n";

      // std::cout << "Looking for " << row.at(0) << "\n";
      Node *from = FindNode(NodeListBig, row.at(0));
       printf("found id from%d\n", from->id);
      if (from == nullptr)
      {
        std::cout << "Cant find " << row.at(0) << "\n";
        continue;
      }

       printf("from %d\n",from->getID());
      Node *to = FindNode(NodeListBig, row.at(1));
      if (to == nullptr)
      {
        std::cout << "Cant find " << row.at(1) << "\n";
        continue;
      }
      std::cout << "Cant find " << row.at(1) << "\n";

       printf("to %d\n",to->getID());

      if (from->getName().compare(to->getName()) != 0) //&& edgeExist == false)
      {
        // NodeListSanFran.at(from->getID()).Edges++;
        NodeListBig.at(to->getID()).Edges++;

        EdgeListBig.emplace_back(walking, from, to);
        printf("1\n");
        EdgeListBig.back().distance = CalcGPSDistance(from->getCoord().x, from->getCoord().y, to->getCoord().x, to->getCoord().y);
                printf("2\n");

        map[from->id].emplace_back(walking, from, to);
                        printf("3\n");

        map[from->id].back().distance = CalcGPSDistance(from->getCoord().x, from->getCoord().y, to->getCoord().x, to->getCoord().y);
                        printf("4\n");

      }
    }

    for (Node &n : NodeListBig)
    {
      int a = rand() % (NodeListBig.size());
      while (a == n.id)
      {
        a = rand() % (NodeListBig.size());
      }
      NodeListBig.at(n.getID()).Edges++;
      // printf("%d now has %d edges\n", n.getID(), n.Edges);
      EdgeListBig.emplace_back(walking, &NodeListBig.at(a), &NodeListBig.at(n.getID()));
      EdgeListBig.back().distance = CalcGPSDistance(NodeListBig.at(a).getCoord().x, NodeListBig.at(a).getCoord().y, n.getCoord().x, n.getCoord().y);
      map[a].emplace_back(walking, &NodeListBig.at(a), &NodeListBig.at(n.getID()));
      map[a].back().distance = CalcGPSDistance(NodeListBig.at(a).getCoord().x, NodeListBig.at(a).getCoord().y, n.getCoord().x, n.getCoord().y);
    }
     printf("Edges made %ld\n",map.size());

    // make graph
    std::ofstream file;
    file.open("output.txt");
    for (std::pair<int, std::vector<Edge>> p : map)
    {
      file << p.first << " Node has " << p.second.size() << " outgoing edges and " << NodeListBig.at(p.first).Edges << " incoming edges\n";
      for (Edge e : p.second)
      {
        file << "\t->" << e.idTo << " distance: " << e.distance << "\n";
      }
    }
    file.close();
    int trials = 100;

    testSearch(NodeListBig, map, trials, true);
  }

  bool initializelitte = false;
  if (initializelitte)
  {
    std::string stopListLittle = "graphText/Nodes_1.txt";
    std::string routeListLittle = "graphText/Edges_1.txt";
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
    printf("Nodes made %ld\n", NodeListLittle.size());
    csvRows.clear();
    printf("Rows after clear %ld\n", csvRows.size());

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
        std::cout << "From Cant find " << row.at(0) << "\n";
        continue;
      }

      printf("from %d %d\n",from->getID(), stoi(row.at(0)));
      Node *to = FindNode(NodeListLittle, row.at(1));
      if (to == nullptr)
      {
        // row[1];
        std::cout << "To Cant find " << row.at(0) << ", " << row.at(1) << "\n";
        continue;
      }
       printf("to %d %d\n", to->getID(), stoi(row.at(1)));

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
        EdgeListLittle.back().distance = CalcGPSDistance(from->getCoord().x, from->getCoord().y, to->getCoord().x, to->getCoord().y);
        map[stoi(row.at(0))].emplace_back(walking, from, to);
        map[stoi(row.at(0))].back().distance =CalcGPSDistance(from->getCoord().x, from->getCoord().y, to->getCoord().x, to->getCoord().y);

        printf("stoi 0 %d, Node list at row 0 %d, idfrom %d id to %d ",stoi(row.at(0)),NodeListLittle.at(stoi(row.at(0))).id,map[stoi(row.at(0))].back().idFrom, map[stoi(row.at(0))].back().idTo);
      }
    }
    printf("Edges made %ld\n", EdgeListLittle.size());

    int trials = 40;

    testSearch(NodeListLittle, map, trials, true);
  }

  bool initializebig = false;
  if (initializebig)
  {
    std::string stopListBig = "graphText/Nodes_2.txt";
    std::string routeListBig = "graphText/Edges_2.txt";
    
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
        // NodeListSanFran.at(from->getID()).Edges++;
        NodeListBig.at(to->getID()).Edges++;

        EdgeListBig.emplace_back(walking, from, to);
        EdgeListBig.back().distance = CalcGPSDistance(to->getCoord().x, to->getCoord().y, from->getCoord().x, from->getCoord().y);
        map[stoi(row.at(0))].emplace_back(walking, from, to);
        map[stoi(row.at(0))].back().distance = CalcGPSDistance(to->getCoord().x, to->getCoord().y, from->getCoord().x, from->getCoord().y);
      }
    }

    for (Node &n : NodeListBig)
    {
      int a = rand() % (NodeListBig.size());
      while (a == n.id)
      {
        a = rand() % (NodeListBig.size());
      }
      NodeListBig.at(n.getID()).Edges++;
      // printf("%d now has %d edges\n", n.getID(), n.Edges);
      EdgeListBig.emplace_back(walking, &NodeListBig.at(a), &NodeListBig.at(n.getID()));
      EdgeListBig.back().distance = CalcGPSDistance(NodeListBig.at(a).getCoord().x, NodeListBig.at(a).getCoord().y, n.getCoord().x, n.getCoord().y);
      map[a].emplace_back(walking, &NodeListBig.at(a), &NodeListBig.at(n.getID()));
      map[a].back().distance = CalcGPSDistance(NodeListBig.at(a).getCoord().x, NodeListBig.at(a).getCoord().y, n.getCoord().x, n.getCoord().y);
    }
    // printf("Edges made %ld\n",map.size());

    // make graph
    std::ofstream file;
    file.open("output.txt");
    for (std::pair<int, std::vector<Edge>> p : map)
    {
      file << p.first << " Node has " << p.second.size() << " outgoing edges and " << NodeListBig.at(p.first).Edges << " incoming edges\n";
      for (Edge e : p.second)
      {
        file << "\t->" << e.idTo << " distance: " << e.distance << "\n";
      }
    }
    file.close();
    int trials = 100;

    testSearch(NodeListBig, map, trials, true);
  }

  bool createroom = false;
  if (createroom)
  {
    create_room_outline();
  }
  
  return 0;
}
