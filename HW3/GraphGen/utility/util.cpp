
#include "util.h"
#include <SFML/Graphics.hpp>
#include "../graph/Edge.h"

#define PI 3.14159265358979323846
#define EARTHS_RADIUS_KM 6371;
#define EARTHS_RADIUS_M 6372797.56085
#define TO_RADIANS PI / 180
float manhatten(sf::Vector2f a, sf::Vector2f b){
    float sum=abs(degreesToRadians(a.x)-degreesToRadians(b.x))+abs(degreesToRadians(a.y)-degreesToRadians(b.y));
    return sum;
}
float length(sf::Vector2f vec)
{
    float x = (vec.x * vec.x);
    float y = (vec.y * vec.y);
    float total = x + y;
    float sqrt = sqrtf(total);
    return sqrt;
}
float sqrMag(sf::Vector2f vec)
{
    float x = (vec.x * vec.x);
    float y = (vec.y * vec.y);
    float total = x + y;
    return total;
}
bool inRadiusDecel(float distance)
{
    return (distance <= 100);
}

bool inRadiusSat(float distance)
{
    return (distance <= 5);
}
sf::Vector2f normalize(sf::Vector2f vec)
{
    sf::Vector2f normed = sf::Vector2f(0, 0);
    float l = length(vec);
    if (l > 0)
    {
        normed.x = vec.x / l;
        normed.y = vec.y / l;
    }
    return normed;
}

float mapToRange(float rotation)
{
    // printf("IN MAP %f\n", rotation);
    rotation = fmodf(rotation, 360.f);
    if (rotation >= 180.f)
    {
        rotation -= 360.f;
    }
    if (rotation <= -180.f)
    {
        rotation += 360.f;
    }
    // printf("OUTMAP %f\n", rotation);
    return rotation;
}

// Utility functions for degree-radian conversion
float degreesToRadians(float degrees)
{
    return degrees * M_PI / 180.0f;
}

float radiansToDegrees(float radians)
{
    return radians * 180.0f / M_PI;
}
// namespautil

float randomBinomial()
{
    float rand = fmod(random(), 3);
    rand = rand - 1;
    return rand;
}
float randomDirection()
{
    float rand = fmod(random(), 360);
    rand = mapToRange(rand);
    return rand;
}

sf::Vector2f getOrientationAsVector(float orientation)
{
    return sf::Vector2f(sin(orientation), cos(orientation));
}

float VecToDegAngle(sf::Vector2f vec){
    float angle=0;
    if(vec.x==0 && vec.y>0){
        return 90.0f;
    }
    if(vec.x==0 && vec.y<0){
        return -90.0f;
    }
    if(vec.x<0){
        return radiansToDegrees(atan(vec.y/vec.x))+180.0f;
    }else if(vec.x>0){
        return radiansToDegrees(atan(vec.y/vec.x));
    }

    return angle;
}

float dotProduct(sf::Vector2f a, sf::Vector2f b)
{
    int prod = 0;

    prod += a.x * b.x;
    prod += a.y * b.y;
    return prod;
}

sf::Vector2f randomPosition(int windowx, int windowy)
{
    float randx = fmod(random(), windowx);
    float randy = fmod(random(), windowy);

    return sf::Vector2f(randx, randy);
}

void ReadFile(std::string filename, std::vector<std::vector<std::string>> &csvRows)
{
    std::ifstream input(filename);
    if (!input.is_open())
    {
        std::cout << "Couldn't read file: " << filename << "\n";
        throw __EXCEPTION__;
    }
    for (std::string line; std::getline(input, line, input.widen('\n'));)
    {
        std::istringstream ss(std::move(line));
        std::vector<std::string> row;
        if (!csvRows.empty())
        {
            // We expect each row to be as big as the first row
            row.reserve(csvRows.front().size() * 2);
        }
        // std::getline can split on other characters, here we use ','
        for (std::string value; std::getline(ss, value, ',');)
        {
            // std::cout << value;
            row.push_back(std::move(value));
            // std::cout << value;
        }
        csvRows.push_back(std::move(row));
    }
}

Node *FindNode(std::vector<Node> &List, std::string name)
{
    // printf("%ld\n", List.size());
    for (Node &n : List)
    {
        // std::cout<<"name of node: "<<n.getName()<<"\n"<<"Looking for:"<<name<<"\n";
        int compare = n.getName().compare(name);
        // printf("%d\n", compare);
        if (n.getName().compare(name) == 0)
        {
            // printf("Found it!\n");
            return &n;
        }
    }
    return nullptr;
}
/**https://stackoverflow.com/questions/27126714/c-latitude-and-longitude-distance-calculator*/
float CalcGPSDistance(float latitud1, float longitud1, float latitud2, float longitud2)
{
    double haversine;
    double temp;
    double distancia_puntos;

    latitud1 = latitud1 / (180/M_PI);
    longitud1 = longitud1  / (180/M_PI);
    latitud2 = latitud2  / (180/M_PI);
    longitud2 = longitud2  / (180/M_PI);

    float dlong=longitud2-longitud1;
    float dlat=latitud2-latitud1;

   float ans=pow(sin(dlat / 2), 2) + 
                          cos(latitud1) * cos(latitud2) * 
                          pow(sin(dlong / 2), 2);
        ans = 2 * asin(sqrt(ans));
        float Radius=6371.f;
        ans =Radius*ans;
        return ans;
}
typedef std::pair<int, Node> pi;
void dijkstra_search(std::vector<Node>& Nodelist, std::unordered_map<int, std::vector<Edge>> &map, int start, int goal, std::unordered_map<Node, Node> &came_from, std::unordered_map<Node, float> &cost_so_far, bool early, bool tracking)
{
    // std::cout<<"\nStart:"<<adj_list.at(start).first.id<<"\n";
    // for (Edge e:adj_list.at(start).second){
    // std::cout<<"Start Edge From: "<<e.idFrom<< " To: "<<e.idTo <<"\n";
    // std::cout<<"Start Edge From: "<< e.idFrom<< " To: "<<  e.idTo<<"\n";

    //}
    // std::cout<<"Goal: "<<adj_list.at(goal).first.id<<"\n";

    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> toconsider;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> toconsiderbackup;
    int fringe=0;
    toconsider.push(std::make_pair(0.f, Nodelist.at(start)));
    cost_so_far.insert(std::make_pair(Nodelist.at(start), 0.0f));
    came_from.insert(std::make_pair(Nodelist.at(start), Nodelist.at(start)));

    while (!toconsider.empty())
    {
        if(tracking){
            int possiblefringe=toconsider.size();
            //std::cout<<"Current fringe: "<<possiblefringe<<"\n";

            // while(toconsider.empty()==false){
            //     std::cout<<toconsider.top().first<<" id: "<<toconsider.top().second.id<<"\n";
            //     toconsiderbackup.push(toconsider.top());
            //     toconsider.pop();
            //     possiblefringe++;

            // }
            if(possiblefringe>fringe){
                fringe=possiblefringe;
            }
            // while(toconsiderbackup.empty()==false){
            //     toconsider.push(toconsiderbackup.top());
            //     toconsiderbackup.pop();
            // }
        }
        Node current = toconsider.top().second;
        toconsider.pop();
        
        if (current.getID() == goal && early == true)
        {
            
            break;

        }
        for (Edge next : map[current.id])
        {
            float newcost = cost_so_far[current] + next.distance;
            // if end then not in cost so far
            if (cost_so_far.find(Nodelist.at(next.idTo)) == cost_so_far.end() || newcost < cost_so_far[Nodelist.at(next.idTo)])
            {
                cost_so_far[Nodelist.at(next.idTo)] = newcost;
                came_from[Nodelist.at(next.idTo)] = current;
                toconsider.push(std::make_pair(newcost, Nodelist.at(next.idTo)));
            }   
        }
    }
    // for(std::pair<Node, Node> a:came_from){
    //     printf("from: %d\n",a.first.getID());
    //     printf("To: %d\n",a.second.getID());

    //}
    std::string f="False";
    std::string t="True";
    std::string ear;
    if(early){
        ear=t;
    }else{
        ear=f;
    }
    std::cout<<"Dijkstra,"<<ear<<",False,False,False"<<","<<came_from.size()<<","<<fringe<<","<<cost_so_far[Nodelist.at(goal)]<<",0,";

}

void make_path(Node& start, Node& end, std::unordered_map<Node, Node> &came_from, std::vector<Node>& path )
{
    Node current = end;
    if (came_from.find(end) == came_from.end())
    {
        std::cout << "NO PATH: Node " << start.getID() << "Node " << end.getID() << "\n";

        // throw __EXCEPTION__;
    }
    while (current.id != start.id)
    {
        path.push_back(current);
        current = came_from[current];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
}

void aStar(std::vector<Node>& Nodelist, std::unordered_map<int, std::vector<Edge>> &map, int start, int goal, std::unordered_map<Node, Node> &came_from, std::unordered_map<Node, float> &cost_so_far, bool early, bool neighborNeg, bool neighborPos, bool manhattendist, bool tracking)
{
    std::unordered_map<Node, float> costSoFarImpure;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> toconsiderbackup;
    int fringe=0;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> toconsider;
    toconsider.push(std::make_pair(0.f, Nodelist.at(start)));
    cost_so_far.insert(std::make_pair(Nodelist.at(start), 0.0f));
    costSoFarImpure.insert(std::make_pair(Nodelist.at(start), 0.0f));

    came_from.insert(std::make_pair(Nodelist.at(start), Nodelist.at(start)));
    while (!toconsider.empty())
    {
        if(tracking){
            //std::cout<<"Current fringe: \n";
            int possiblefringe=possiblefringe=toconsider.size();
            //std::cout<<"Current fringe: "<<possiblefringe<<"\n";

            // while(toconsider.empty()==false){
            //     //std::cout<<toconsider.top().first<<" id: "<<toconsider.top().second.id<<"\n";
            //     toconsiderbackup.push(toconsider.top());
            //     toconsider.pop();
            //     possiblefringe++;

            // }
            if(possiblefringe>fringe){
                fringe=possiblefringe;
            }
            // while(toconsiderbackup.empty()==false){
            //     toconsider.push(toconsiderbackup.top());
            //     toconsiderbackup.pop();
            // }
        }
        Node current = toconsider.top().second;
        toconsider.pop();
        if (current.getID() == goal && early == true)
        {
            break;
        }
        for (Edge next : map[current.id])
        {
            float newcost = cost_so_far[current]+next.distance;
            float newcostimpure=costSoFarImpure[current]+manhatten(current.coords, Nodelist.at(next.idTo).coords);
            

            if (cost_so_far.find(Nodelist.at(next.idTo)) == cost_so_far.end())
            {
                cost_so_far[Nodelist.at(next.idTo)] = newcost;
                costSoFarImpure[Nodelist.at(next.idTo)]=newcostimpure;
                came_from[Nodelist.at(next.idTo)] = current;
                float dist = CalcGPSDistance(Nodelist.at(next.idTo).coords.x, Nodelist.at(next.idTo).coords.y, Nodelist.at(goal).coords.x, Nodelist.at(goal).coords.y);
                float prior = newcost + dist;
                
                if(manhattendist){
                    prior=manhatten(Nodelist.at(next.idTo).coords,Nodelist.at(goal).coords)*.5*EARTHS_RADIUS_KM+newcost;
                }
                if (neighborPos == true)
                {
                    float numneighbors = map[next.idTo].size();

                    prior += 5.0f*numneighbors;
                    if (prior < 0.0f)
                    {
                        prior = 1.f;
                    }
                }
                if (neighborNeg == true)
                {
                    float numneighbors = map[next.idTo].size();

                    prior -= 5.0f*numneighbors;
                    if (prior < 0.0f)
                    {
                        prior = 1.f;
                    }
                }

                toconsider.push(std::make_pair(prior, Nodelist.at(next.idTo)));
            }
        }
    }
   std::string f="False";
    std::string t="True";
    std::string ear=f;
    std::string man=f;
    std::string neigh=f;
    std::string neighN=f;

    if(early){
        ear=t;
    }
    if(manhattendist){
        man=t;
    }
    if(neighborPos){
        neigh=t;
    }
    if(neighborNeg){
        neighN=t;
    }
    std::cout<<"Astar,"<<ear<<","<<man<<","<<neigh<<","<<neighN<<","<<came_from.size()<<","<<fringe<<","<<cost_so_far[Nodelist.at(goal)]<<","<<costSoFarImpure[Nodelist.at(goal)]<<",";

    

}

// void dijkstra_searchRobers(std::vector<std::pair<Node, std::vector<Edge>>> adj_list, int start, int goal, std::unordered_map<Node, Node> &came_from, std::unordered_map<Node, float> &cost_so_far, bool early);
void prettyprint(std::vector<Node> &path)
{
    for (int i = 0; i < path.size() - 1; i++)
    {
        std::cout << path.at(i).getID() << "->";
    }
    std::cout << path.at(path.size() - 1).getID() << "\n";
}

void printNodes(std::vector<Node>& NodeList)
{
    std::ofstream file;
    file.open("Stops.txt");
    file << "id, name, lat, long\n";
    for (Node &n : NodeList)
    {
        bool found = false;

        // if(found==false){
        file << n.getID() << "," << n.getName() << "," << n.getCoord().x << ","
             << n.getCoord().y << "\n";
        //}
    }
    file.close();
}
void printEdges(std::vector<Edge>& adj_list, std::vector<Node>& NodeList)
{
    std::ofstream myFile3;
    myFile3.open("Edgesalt.csv");
    myFile3 << std::fixed << std::setprecision(8);
    // myFile2 << "to,from,Coords\n";
    myFile3 << "to,from,CoordsFromLat,CoordsFromLong,CoordsToLat,CoordsToLong\n";

    for (Edge e : adj_list)
    {
        // myFile2<<e.nameFrom<<","<<e.nameTo<<",\"LineString([("<<NodeListSanFran.at(e.idFrom).coords.x<<","<<NodeListSanFran.at(e.idFrom).coords.y<<"),("<<NodeListSanFran.at(e.idTo).coords.x<<","<<NodeListSanFran.at(e.idTo).coords.y<<")])\"\n";
        myFile3 << e.nameFrom << "," << e.nameTo << "," << NodeList.at(e.idFrom).coords.x << "," << NodeList.at(e.idFrom).coords.y << "," << NodeList.at(e.idTo).coords.x << "," << NodeList.at(e.idTo).coords.y << "\n";
    }
    // myFile2.close();
    myFile3.close();
}

void testSearch(std::vector<Node>& Nodelist, std::unordered_map<int, std::vector<Edge>>& map, int trials, bool tracking)
{
    bool pretty=false;
    bool early=true;
    std::ofstream out("out.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!

    int de = 0;
    int dne = 0;
    int astarnoearlynoadmiss = 0;
    int astarnoearlyyadmiss=0;
    int astaryearlynoadmiss = 0;
    int astaryearlyyadmiss=0;
    sf::Clock timer;
    float deltaTime;
    std::unordered_map<int, int> no_edge = std::unordered_map<int, int>();
    std::unordered_map<Node, Node> came_from = std::unordered_map<Node, Node>();
    std::unordered_map<Node, Node> *came_fromptr = &came_from;
    std::unordered_map<Node, float> cost_so_far = std::unordered_map<Node, float>();
    std::unordered_map<Node, float> *cost_so_farptr = &cost_so_far;
    std::vector<Node> returnedpath = std::vector<Node> ();
    float time;
    // dijkstra_search(adj_listSanFran, a, b, came_from, cost_so_far, false);
    for (int i = 0; i < trials; i++)
    {
        // for (int a = 0; a < NodeListSanFran.size(); a++)
        //{
        // for (int b = 0; b < NodeListSanFran.size(); b++)
        //{
        //std::cout<<"\n\n\nRound :"<<i<<"\n";
        int a = rand() % (Nodelist.size());
        int b = rand() % (Nodelist.size());
        while (b==a)
        {
             b = rand() % (Nodelist.size());
        }
       
        //std::cout << "from: " << a << " to: " << b << "\n";
        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        timer.restart();
        dijkstra_search(Nodelist, map, a, b, came_from, cost_so_far, false, tracking);
        time=timer.restart().asSeconds();
        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
        std::cout <<time <<","<< i<<","<<returnedpath.size()<< "\n";
        dne += returnedpath.size();
        if (returnedpath.size() > 0&& pretty)
        {
            prettyprint(returnedpath);
        }
        else if(returnedpath.empty())
        {
            printf("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }
        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        timer.restart();
        dijkstra_search(Nodelist, map, a, b, came_from, cost_so_far, true, tracking);
        time=timer.restart().asSeconds();

        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
        std::cout <<time <<","<< i<<","<<returnedpath.size()<< "\n";
        de += returnedpath.size();
        if (returnedpath.size() > 0&& pretty)
        {
            prettyprint(returnedpath);
        }
        else if(returnedpath.empty())
        {
            printf("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }

        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        timer.restart();
        //early and just dist to goal
        aStar(Nodelist, map, a, b, came_from, cost_so_far, early, false, false,false, tracking);
        time=timer.restart().asSeconds();

        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
        std::cout <<time <<","<< i<<","<<returnedpath.size()<< "\n";
        astaryearlynoadmiss += returnedpath.size();
        if (returnedpath.size() > 0&& pretty)
        {
            prettyprint(returnedpath);
        }
        else if(returnedpath.empty())
        {
            printf("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }

        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        timer.restart();
        //early and favor more neighbors
        aStar(Nodelist, map, a, b, came_from, cost_so_far, early, true, false,false, tracking);
        time=timer.restart().asSeconds();

        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
        std::cout <<time <<","<< i<<","<<returnedpath.size()<< "\n";
        astaryearlyyadmiss += returnedpath.size();
        if (returnedpath.size() > 0&& pretty)
        {
            prettyprint(returnedpath);
        }
        else if(returnedpath.empty())
        {
            printf("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }

        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        timer.restart();
        //early and manhatten
        aStar(Nodelist, map, a, b, came_from, cost_so_far, early, true, true,false, tracking);
        time=timer.restart().asSeconds();
        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
        std::cout <<time <<","<< i<<","<<returnedpath.size()<< "\n";
        astarnoearlyyadmiss+= returnedpath.size();
        if (returnedpath.size() > 0&& pretty)
        {
            prettyprint(returnedpath);
        }
        else if(returnedpath.empty())
        {
            printf("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }
        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        timer.restart();
        //no  early no nothing
        aStar(Nodelist, map, a, b, came_from, cost_so_far, early, false, true,false, tracking);
        time=timer.restart().asSeconds();

        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
        std::cout <<time <<","<< i<<","<<returnedpath.size()<< "\n";
        astarnoearlynoadmiss += returnedpath.size();
        if (returnedpath.size() > 0&& pretty)
        {
            prettyprint(returnedpath);
        }
        else if(returnedpath.empty())
        {
            printf("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }

        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        timer.restart();
        //no  early no nothing
        aStar(Nodelist, map, a, b, came_from, cost_so_far, early, false, false,true, tracking);
        time=timer.restart().asSeconds();

        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
        std::cout <<time <<","<< i<<","<<returnedpath.size()<< "\n";
        astarnoearlynoadmiss += returnedpath.size();
        if (returnedpath.size() > 0&& pretty)
        {
            prettyprint(returnedpath);
        }
        else if(returnedpath.empty())
        {
            printf("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }

         came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        timer.restart();
        //no  early no nothing
        aStar(Nodelist, map, a, b, came_from, cost_so_far, early, true, false,true, tracking);
        time=timer.restart().asSeconds();

        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
        std::cout <<time <<","<< i<<","<<returnedpath.size()<< "\n";
        astarnoearlynoadmiss += returnedpath.size();
        if (returnedpath.size() > 0&& pretty)
        {
            prettyprint(returnedpath);
        }
        else if(returnedpath.empty())
        {
            printf("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }

 came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        timer.restart();
        //no  early no nothing
        aStar(Nodelist, map, a, b, came_from, cost_so_far, early, true, false,false, tracking);
        time=timer.restart().asSeconds();

        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
        std::cout <<time <<","<< i<<","<<returnedpath.size()<< "\n";
        astarnoearlynoadmiss += returnedpath.size();
        if (returnedpath.size() > 0&& pretty)
        {
            prettyprint(returnedpath);
        }
        else if(returnedpath.empty())
        {
            printf("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }
        //std::cout << "\n";
        
    }
    de /= trials;
    dne /= trials;
    astarnoearlynoadmiss  /= trials;
    astarnoearlyyadmiss /= trials;
    astaryearlynoadmiss  /= trials;
    astaryearlyyadmiss /= trials;

    //printf("Avg path length d no e %d\nAvg path length d w/ e %d\nAvg path length a* no e no admiss %d\nAvg path length a* no e yes admiss %d\nAvg path length a* w/ e yes admiss %d\nAvg path length a* w/ e no admiss %d\n", dne, de, astarnoearlynoadmiss, astarnoearlyyadmiss, astaryearlyyadmiss, astaryearlynoadmiss);
    if(!no_edge.empty()){
    for (std::pair<int, int> p : no_edge)
    {
        std::cout << "Edge " << p.first << " lacks connections to " << p.second << "\n";
    }
    }
    out.close();
    std::cout.rdbuf(coutbuf);

}

void create_room_outline(){

    std::ofstream out("roomoutline.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); //save old buf
    std::cout.rdbuf(out.rdbuf()); //redirect std::cout to out.txt!
    //size of window in x direction
    int windowx = 640/4+2;
    //size of window in y direction
    int windowy = 400/4+2;

    windowx/=3;
    windowy/=3;

    for (int i =0; i < windowx; i++){
        std::cout<<"x";
    }
    std::cout<<"\n";
    for(int i =0; i <windowy-2; i++){
        std::cout<<"x";

        for(int j=0; j<(windowx-2)/2; j++){
            std::cout<<"o";

        }
        std::cout<<"x";
        for(int j=0; j<(windowx-2)/2; j++){
            std::cout<<"o";

        }
        std::cout<<"x";

        std::cout<<"\n";

    }
    for (int i =0; i < windowx; i++){
        std::cout<<"x";
    }
    out.close();
    std::cout.rdbuf(coutbuf);

}



