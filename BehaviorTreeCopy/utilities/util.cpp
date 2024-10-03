
#include "../utilities/util.h"
#include <SFML/Graphics.hpp>
#include "../Graph/Edge.h"

#define PI 3.14159265358979323846
#define EARTHS_RADIUS_KM 6372.79756085
#define EARTHS_RADIUS_M 6372797.56085
#define TO_RADIANS PI / 180



float VecToDegAngle(sf::Vector2f vec)
{
    float angle = 0;
    if (vec.x == 0 && vec.y > 0)
    {
        return 90.0f;
    }
    if (vec.x == 0 && vec.y < 0)
    {
        return -90.0f;
    }
    if (vec.x < 0)
    {
        return radiansToDegrees(atan(vec.y / vec.x)) + 180.0f;
    }
    else if (vec.x > 0)
    {
        return radiansToDegrees(atan(vec.y / vec.x));
    }

    return angle;
}

float length(sf::Vector2f vec)
{
    float x = (vec.x * vec.x);
    float y = (vec.y * vec.y);
    float total = x + y;
    float sqrt = sqrtf(total);
    return sqrt;
}
float distancelength(sf::Vector2f veca, sf::Vector2f vecb)
{

    float x = veca.x - vecb.x;
    x = x * x;
    float y = (veca.y - vecb.y);
    y = y * y;
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
    return (distance <= 20);
}

bool inRadiusSat(float distance)
{
    return (distance <= 10);
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

    //std::cout<<("IN MAP %f\n", rotation);
    if(rotation>180){
    while (rotation > 180)
    {
        rotation -= 360;
    }
    }
    else if(rotation<-180){
        while(rotation<-180){
            rotation+=360;
        }
    }
   //std::cout<<("out MAP %f\n", rotation);

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
       //std::cout << "Couldn't read file: " << filename << "\n";
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
            //std::cout << value;
            row.push_back(std::move(value));
            //std::cout << value;
        }
        csvRows.push_back(std::move(row));
    }
}

Node *FindNode(std::vector<Node> &List, std::string name)
{
    //std::cout<<("%ld\n", List.size());
    for (Node &n : List)
    {
        //std::cout<<"name of node: "<<n.getName()<<"\n"<<"Looking for:"<<name<<"\n";
        int compare = n.getName().compare(name);
        //std::cout<<("%d\n", compare);
        if (n.getName().compare(name) == 0)
        {
            //std::cout<<("Found it!\n");
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

    latitud1 = latitud1 * TO_RADIANS;
    longitud1 = longitud1 * TO_RADIANS;
    latitud2 = latitud2 * TO_RADIANS;
    longitud2 = longitud2 * TO_RADIANS;

    haversine = (pow(sin((1.0 / 2) * (latitud2 - latitud1)), 2)) + ((cos(latitud1)) * (cos(latitud2)) * (pow(sin((1.0 / 2) * (longitud2 - longitud1)), 2)));
    temp = 2 * asin(std::min(1.0, sqrt(haversine)));
    distancia_puntos = EARTHS_RADIUS_KM * temp;

    return distancia_puntos;
}
typedef std::pair<int, Node> pi;
void dijkstra_search(std::vector<Node> &Nodelist, std::unordered_map<int, std::vector<Edge>> &map, int start, int goal, std::unordered_map<Node, Node> &came_from, std::unordered_map<Node, float> &cost_so_far, bool early, bool tracking)
{
    //std::cout<<"\nStart:"<<adj_list.at(start).first.id<<"\n";
    // for (Edge e:adj_list.at(start).second){
    //std::cout<<"Start Edge From: "<<e.idFrom<< " To: "<<e.idTo <<"\n";
    //std::cout<<"Start Edge From: "<< e.idFrom<< " To: "<<  e.idTo<<"\n";

    //}
    //std::cout<<"Goal: "<<adj_list.at(goal).first.id<<"\n";

    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> toconsider;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> toconsiderbackup;
    int fringe = 0;
    toconsider.push(std::make_pair(0.f, Nodelist.at(start)));
    cost_so_far.insert(std::make_pair(Nodelist.at(start), 0.0f));
    came_from.insert(std::make_pair(Nodelist.at(start), Nodelist.at(start)));

    while (!toconsider.empty())
    {
        if (tracking)
        {
           //std::cout << "Current fringe: \n";
            int possiblefringe = 0;
            while (toconsider.empty() == false)
            {
               //std::cout << toconsider.top().first << " id: " << toconsider.top().second.id << "\n";
                toconsiderbackup.push(toconsider.top());
                toconsider.pop();
                possiblefringe++;
            }
            if (possiblefringe > fringe)
            {
                fringe = possiblefringe;
            }
            while (toconsiderbackup.empty() == false)
            {
                toconsider.push(toconsiderbackup.top());
                toconsiderbackup.pop();
            }
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
    //    //std::cout<<("from: %d\n",a.first.getID());
    //    //std::cout<<("To: %d\n",a.second.getID());

    //}

   //std::cout << "\n\nA * fill: " << came_from.size() << " fringe: " << fringe << "\n";
}

typedef std::pair<int, gameTile *> piRef;
void dijkstra_searchRef(std::vector<gameTile *> &Nodelist, std::unordered_map<int, std::vector<Edge *>> &map, int start, int goal, std::unordered_map<gameTile *, gameTile *> &came_from, std::unordered_map<gameTile *, float> &cost_so_far, bool early, bool tracking)
{
    //std::cout<<"\nStart:"<<adj_list.at(start).first.id<<"\n";
    // for (Edge e:adj_list.at(start).second){
    //std::cout<<"Start Edge From: "<<e.idFrom<< " To: "<<e.idTo <<"\n";
    //std::cout<<"Start Edge From: "<< e.idFrom<< " To: "<<  e.idTo<<"\n";

    //}
    //std::cout<<"Goal: "<<adj_list.at(goal).first.id<<"\n";

    std::priority_queue<piRef, std::vector<piRef>, std::greater<piRef>> toconsider;
    std::priority_queue<piRef, std::vector<piRef>, std::greater<piRef>> toconsiderbackup;
    int fringe = 0;
    toconsider.push(std::make_pair(0.f, Nodelist.at(start)));
    cost_so_far.insert(std::make_pair(Nodelist.at(start), 0.0f));
    came_from.insert(std::make_pair(Nodelist.at(start), Nodelist.at(start)));

    while (!toconsider.empty())
    {
        if (tracking)
        {
           //std::cout << "Current fringe: \n";
            int possiblefringe = 0;
            while (toconsider.empty() == false)
            {
               //std::cout << toconsider.top().first << " id: " << toconsider.top().second->id << "\n";
                toconsiderbackup.push(toconsider.top());
                toconsider.pop();
                possiblefringe++;
            }
            if (possiblefringe > fringe)
            {
                fringe = possiblefringe;
            }
            while (toconsiderbackup.empty() == false)
            {
                toconsider.push(toconsiderbackup.top());
                toconsiderbackup.pop();
            }
        }
        gameTile *current = toconsider.top().second;
        toconsider.pop();

        if (current->getID() == goal && early == true)
        {
            break;
        }
        for (Edge *next : map[current->id])
        {
            float newcost = cost_so_far[current] + next->distance;
            // if end then not in cost so far
            if (cost_so_far.find(Nodelist.at(next->idTo)) == cost_so_far.end() || newcost < cost_so_far[Nodelist.at(next->idTo)])
            {
                cost_so_far[Nodelist.at(next->idTo)] = newcost;
                came_from[Nodelist.at(next->idTo)] = current;
                toconsider.push(std::make_pair(newcost, Nodelist.at(next->idTo)));
            }
        }
    }
    // for(std::pair<Node, Node> a:came_from){
    //    //std::cout<<("from: %d\n",a.first.getID());
    //    //std::cout<<("To: %d\n",a.second.getID());

    //}

   //std::cout << "\n\nA * fill: " << came_from.size() << " fringe: " << fringe << "\n";
}

void make_path(Node &start, Node &end, std::unordered_map<Node, Node> &came_from, std::vector<Node> &path)
{
    Node current = end;
    if (came_from.find(end) == came_from.end())
    {
       //std::cout << "NO PATH: Node " << start.getID() << "Node " << end.getID() << "\n";

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
void make_pathRef(gameTile *start, gameTile *end, std::unordered_map<gameTile *, gameTile *> &came_from, std::vector<gameTile *> &path)
{
    gameTile *current = end;
    if (came_from.find(end) == came_from.end())
    {
       //std::cout << "NO PATH: Node " << start->getID() << "Node " << end->getID() << "\n";

        // throw __EXCEPTION__;
    }
    while (current->id != start->id)
    {
        path.push_back(current);
        current = came_from[current];
    }
    path.push_back(start);
    std::reverse(path.begin(), path.end());
}

void aStar(std::vector<Node> &Nodelist, std::unordered_map<int, std::vector<Edge>> &map, int start, int goal, std::unordered_map<Node, Node> &came_from, std::unordered_map<Node, float> &cost_so_far, bool early, bool admissable, bool tracking)
{
   //std::cout<<("start: %d end:%d", start, goal);
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> toconsiderbackup;
    int fringe = 0;
    std::priority_queue<pi, std::vector<pi>, std::greater<pi>> toconsider;
    toconsider.push(std::make_pair(0.f, Nodelist.at(start)));
    cost_so_far.insert(std::make_pair(Nodelist.at(start), 0.0f));
    came_from.insert(std::make_pair(Nodelist.at(start), Nodelist.at(start)));
    while (!toconsider.empty())
    {
        if (tracking)
        {
           //std::cout << "Current fringe: \n";
            int possiblefringe = 0;
            while (toconsider.empty() == false)
            {
               //std::cout << toconsider.top().first << " id: " << toconsider.top().second.id << "\n";
                toconsiderbackup.push(toconsider.top());
                toconsider.pop();
                possiblefringe++;
            }
            if (possiblefringe > fringe)
            {
                fringe = possiblefringe;
            }
            while (toconsiderbackup.empty() == false)
            {
                toconsider.push(toconsiderbackup.top());
                toconsiderbackup.pop();
            }
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
            if (cost_so_far.find(Nodelist.at(next.idTo)) == cost_so_far.end())
            {
                cost_so_far[Nodelist.at(next.idTo)] = newcost;
                came_from[Nodelist.at(next.idTo)] = current;
                float dist = CalcGPSDistance(Nodelist.at(next.idTo).coords.x, Nodelist.at(next.idTo).coords.y, Nodelist.at(next.idTo).coords.x, Nodelist.at(next.idTo).coords.y);
                float prior = newcost + dist;
                if (admissable == false)
                {
                    float numneighbors = map[next.idTo].size();

                    prior -= numneighbors;
                    if (prior < 0.0f)
                    {
                        prior = 1.f;
                    }
                }

                toconsider.push(std::make_pair(prior, Nodelist.at(next.idTo)));
            }
        }
    }
   //std::cout << "\n\nA * fill: " << came_from.size() << " fringe: " << fringe << "\n";
}

// void dijkstra_searchRobers(std::vector<std::pair<Node, std::vector<Edge>>> adj_list, int start, int goal, std::unordered_map<Node, Node> &came_from, std::unordered_map<Node, float> &cost_so_far, bool early);
void prettyprint(std::vector<Node> &path)
{
    for (int i = 0; i < path.size() - 1; i++)
    {
       //std::cout << path.at(i).getID() << "->";
    }
   //std::cout << path.at(path.size() - 1).getID() << "\n";
}

void printNodes(std::vector<Node> &NodeList)
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
void printEdges(std::vector<Edge> &adj_list, std::vector<Node> &NodeList)
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

void testSearch(std::vector<Node> &Nodelist, std::unordered_map<int, std::vector<Edge>> &map, int trials, bool tracking)
{
    std::ofstream out("out.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); // save old buf
    std::cout.rdbuf(out.rdbuf());                // redirect std::cout to out.txt!

    int de = 0;
    int dne = 0;
    int astarnoearlynoadmiss = 0;
    int astarnoearlyyadmiss = 0;
    int astaryearlynoadmiss = 0;
    int astaryearlyyadmiss = 0;

    std::unordered_map<int, int> no_edge = std::unordered_map<int, int>();
    std::unordered_map<Node, Node> came_from = std::unordered_map<Node, Node>();
    std::unordered_map<Node, Node> *came_fromptr = &came_from;
    std::unordered_map<Node, float> cost_so_far = std::unordered_map<Node, float>();
    std::unordered_map<Node, float> *cost_so_farptr = &cost_so_far;
    std::vector<Node> returnedpath = std::vector<Node>();
    // dijkstra_search(adj_listSanFran, a, b, came_from, cost_so_far, false);
    for (int i = 0; i < trials; i++)
    {
        // for (int a = 0; a < NodeListSanFran.size(); a++)
        //{
        // for (int b = 0; b < NodeListSanFran.size(); b++)
        //{
       //std::cout << "\n\n\nRound :" << i << "\n";
        int a = rand() % (Nodelist.size());
        int b = rand() % (Nodelist.size());
       //std::cout << "from: " << a << " to: " << b << "\n";
        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        dijkstra_search(Nodelist, map, a, b, came_from, cost_so_far, false, tracking);
        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Djiskstra no early " << returnedpath.size() << "\n";
        dne += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprint(returnedpath);
        }
        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        dijkstra_search(Nodelist, map, a, b, came_from, cost_so_far, true, tracking);
        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Djiskstra w/ early " << returnedpath.size() << "\n";
        de += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprint(returnedpath);
        }

        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        aStar(Nodelist, map, a, b, came_from, cost_so_far, true, false, tracking);
        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Astar w/ early Not Admissable " << returnedpath.size() << "\n";
        astaryearlynoadmiss += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprint(returnedpath);
        }

        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        aStar(Nodelist, map, a, b, came_from, cost_so_far, true, true, tracking);
        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Astar w/ early and Admissable " << returnedpath.size() << "\n";
        astaryearlyyadmiss += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprint(returnedpath);
        }
        else
        {
           //std::cout<<("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }

        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        aStar(Nodelist, map, a, b, came_from, cost_so_far, false, true, tracking);
        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Astar w/o early and Admissable " << returnedpath.size() << "\n";
        astarnoearlyyadmiss += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprint(returnedpath);
        }
        else
        {
           //std::cout<<("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }
        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        aStar(Nodelist, map, a, b, came_from, cost_so_far, false, false, tracking);
        make_path(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Astar w/0 early and No Admissable " << returnedpath.size() << "\n";
        astarnoearlynoadmiss += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprint(returnedpath);
        }
        else
        {
           //std::cout<<("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }

       //std::cout << "\n";
    }
    de /= trials;
    dne /= trials;
    astarnoearlynoadmiss /= trials;
    astarnoearlyyadmiss /= trials;
    astaryearlynoadmiss /= trials;
    astaryearlyyadmiss /= trials;

   //std::cout<<("Avg path length d no e %d\nAvg path length d w/ e %d\nAvg path length a* no e no admiss %d\nAvg path length a* no e yes admiss %d\nAvg path length a* w/ e yes admiss %d\nAvg path length a* w/ e no admiss %d\n", dne, de, astarnoearlynoadmiss, astarnoearlyyadmiss, astaryearlyyadmiss, astaryearlynoadmiss);
    if (!no_edge.empty())
    {
        for (std::pair<int, int> p : no_edge)
        {
           //std::cout << "Edge " << p.first << " lacks connections to " << p.second << "\n";
        }
    }
    out.close();
   //std::cout.rdbuf(coutbuf);
}

void testSearchRef(std::vector<gameTile *> &Nodelist, std::unordered_map<int, std::vector<Edge *>> &map, int trials, bool tracking)
{
    std::ofstream out("out.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); // save old buf
    std::cout.rdbuf(out.rdbuf());                // redirect std::cout to out.txt!

    int de = 0;
    int dne = 0;
    int astarnoearlynoadmiss = 0;
    int astarnoearlyyadmiss = 0;
    int astaryearlynoadmiss = 0;
    int astaryearlyyadmiss = 0;

    std::unordered_map<int, int> no_edge = std::unordered_map<int, int>();
    std::unordered_map<gameTile *, gameTile *> came_from = std::unordered_map<gameTile *, gameTile *>();
    std::unordered_map<gameTile *, gameTile *> *came_fromptr = &came_from;
    std::unordered_map<gameTile *, float> cost_so_far = std::unordered_map<gameTile *, float>();
    std::unordered_map<gameTile *, float> *cost_so_farptr = &cost_so_far;
    std::vector<gameTile *> returnedpath = std::vector<gameTile *>();
    // dijkstra_search(adj_listSanFran, a, b, came_from, cost_so_far, false);
    for (int i = 0; i < trials; i++)
    {
        // for (int a = 0; a < NodeListSanFran.size(); a++)
        //{
        // for (int b = 0; b < NodeListSanFran.size(); b++)
        //{
       //std::cout << "\n\n\nRound :" << i << "\n";
        int a = rand() % (Nodelist.size());
        int b = rand() % (Nodelist.size());
       //std::cout << "from: " << a << " to: " << b << "\n";
        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        dijkstra_searchRef(Nodelist, map, a, b, came_from, cost_so_far, false, tracking);
        make_pathRef(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Djiskstra no early " << returnedpath.size() << "\n";
        dne += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprintRef(returnedpath);
        }
        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        dijkstra_searchRef(Nodelist, map, a, b, came_from, cost_so_far, true, tracking);
        make_pathRef(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Djiskstra w/ early " << returnedpath.size() << "\n";
        de += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprintRef(returnedpath);
        }

        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        aStarRef(Nodelist, map, a, b, came_from, cost_so_far, true, false, tracking);
        make_pathRef(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Astar w/ early Not Admissable " << returnedpath.size() << "\n";
        astaryearlynoadmiss += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprintRef(returnedpath);
        }

        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        aStarRef(Nodelist, map, a, b, came_from, cost_so_far, true, true, tracking);
        make_pathRef(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Astar w/ early and Admissable " << returnedpath.size() << "\n";
        astaryearlyyadmiss += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprintRef(returnedpath);
        }
        else
        {
           //std::cout<<("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }

        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        aStarRef(Nodelist, map, a, b, came_from, cost_so_far, false, true, tracking);
        make_pathRef(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Astar w/o early and Admissable " << returnedpath.size() << "\n";
        astarnoearlyyadmiss += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprintRef(returnedpath);
        }
        else
        {
           //std::cout<<("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }
        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        aStarRef(Nodelist, map, a, b, came_from, cost_so_far, false, false, tracking);
        make_pathRef(Nodelist.at(a), Nodelist.at(b), came_from, returnedpath);
       //std::cout << "size of path from Astar w/0 early and No Admissable " << returnedpath.size() << "\n";
        astarnoearlynoadmiss += returnedpath.size();
        if (returnedpath.size() > 0)
        {
            prettyprintRef(returnedpath);
        }
        else
        {
           //std::cout<<("%d and %d have no path\n", a, b);
            no_edge[b] = no_edge[b] + 1;
        }

       //std::cout << "\n";
    }
    de /= trials;
    dne /= trials;
    astarnoearlynoadmiss /= trials;
    astarnoearlyyadmiss /= trials;
    astaryearlynoadmiss /= trials;
    astaryearlyyadmiss /= trials;

   //std::cout<<("Avg path length d no e %d\nAvg path length d w/ e %d\nAvg path length a* no e no admiss %d\nAvg path length a* no e yes admiss %d\nAvg path length a* w/ e yes admiss %d\nAvg path length a* w/ e no admiss %d\n", dne, de, astarnoearlynoadmiss, astarnoearlyyadmiss, astaryearlyyadmiss, astaryearlynoadmiss);
    if (!no_edge.empty())
    {
        for (std::pair<int, int> p : no_edge)
        {
           //std::cout << "Edge " << p.first << " lacks connections to " << p.second << "\n";
        }
    }
    out.close();
   std::cout.rdbuf(coutbuf);
}

void create_room_outline()
{

    std::ofstream out("roomoutline.txt");
    std::streambuf *coutbuf = std::cout.rdbuf(); // save old buf
    std::cout.rdbuf(out.rdbuf());                // redirect std::cout to out.txt!
    // size of window in x direction
    int windowx = 640 / 4 + 2;
    // size of window in y direction
    int windowy = 400 / 4 + 2;

    windowx /= 3;
    windowy /= 3;

    for (int i = 0; i < windowx; i++)
    {
       //std::cout << "x";
    }
   //std::cout << "\n";
    for (int i = 0; i < windowy - 2; i++)
    {
       //std::cout << "x";

        for (int j = 0; j < (windowx - 2) / 2; j++)
        {
           //std::cout << "o";
        }
       //std::cout << "x";
        for (int j = 0; j < (windowx - 2) / 2; j++)
        {
           //std::cout << "o";
        }
       //std::cout << "x";

       //std::cout << "\n";
    }
    for (int i = 0; i < windowx; i++)
    {
       //std::cout << "x";
    }
    out.close();
   //std::cout.rdbuf(coutbuf);
}

void aStarRef(std::vector<gameTile *> &Nodelist, std::unordered_map<int, std::vector<Edge *>> &map, int start, int goal, std::unordered_map<gameTile *, gameTile *> &came_from, std::unordered_map<gameTile *, float> &cost_so_far, bool early, bool admissable, bool tracking)
{
   //std::cout << "In A* start: " << start << " end:" << goal << "\n";\
    std::cout<<"1\n";

    std::priority_queue<piRef, std::vector<piRef>, std::greater<piRef>> toconsiderbackup;
    int fringe = 0;
    std::priority_queue<piRef, std::vector<piRef>, std::greater<piRef>> toconsider;
    toconsider.push(std::make_pair(0.f, Nodelist.at(start)));
    cost_so_far.insert(std::make_pair(Nodelist.at(start), 0.0f));
    came_from.insert(std::make_pair(Nodelist.at(start), Nodelist.at(start)));
       //std::cout<<"2\n";

    while (!toconsider.empty())
    {
                //std::cout<<"3\n";

        

        if (tracking)
        {
            //std::cout << "Current fringe: \n";
            int possiblefringe = 0;
            while (toconsider.empty() == false)
            {
                //std::cout << "distance" << toconsider.top().first << " id: " << toconsider.top().second->id << "\n";
                toconsiderbackup.push(toconsider.top());
                toconsider.pop();
                possiblefringe++;
            }
            if (possiblefringe > fringe)
            {
                fringe = possiblefringe;
            }
            while (toconsiderbackup.empty() == false)
            {
                toconsider.push(toconsiderbackup.top());
                toconsiderbackup.pop();
            }
        }
        gameTile *current = toconsider.top().second;
        toconsider.pop();
                //std::cout<<"3\n";

        if (current->getID() == goal && early == true)
        {
            break;
        }
        for (Edge *next : map[current->id])
        {
            float newcost = cost_so_far[current] + next->distance;
            if (cost_so_far.find(Nodelist.at(next->idTo)) == cost_so_far.end())
            {
                cost_so_far[Nodelist.at(next->idTo)] = newcost;
                came_from[Nodelist.at(next->idTo)] = current;
                float dist = CalcGPSDistance(Nodelist.at(next->idTo)->coords.x, Nodelist.at(next->idTo)->coords.y, Nodelist.at(next->idTo)->coords.x, Nodelist.at(next->idTo)->coords.y);
                float prior = newcost + dist;
                if (admissable == false)
                {
                    float numneighbors = map[next->idTo].size();

                    prior -= numneighbors;
                    if (prior < 0.0f)
                    {
                        prior = 1.f;
                    }
                }
        //std::cout<<"4\n";

                toconsider.push(std::make_pair(prior, Nodelist.at(next->idTo)));
            }
        }
    }
           //std::cout<<"Leaving A*\n";

    //std::cout << "\n\nA * fill: " << came_from.size() << " fringe: " << fringe << "\n";
}

void prettyprintRef(std::vector<gameTile *> &path)
{
    for (int i = 0; i < path.size() - 1; i++)
    {
       //std::cout << path.at(i)->getID() << "(" << path.at(i)->coords.x << "," << path.at(i)->coords.y << ")" << "->";
    }
   //std::cout << path.at(path.size() - 1)->getID() << "\n";
}