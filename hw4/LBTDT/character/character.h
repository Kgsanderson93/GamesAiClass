#include "kinematic.h"
#include "../decisionTree/GameTree.h"
#include "../PathFinding/Path.h"
#include "../behavior/steeringbehavior.h"
class character
{
public:
    Kinematic *charmove;
    GameTree *tree;
    GameWorld *world;
    int pathidx;
    Path *path;
    void followPath();
    void aquirePath(gameTile *g);
    std::string movementName;
    void newBehavior(float time);
    character(Kinematic *charmove, int id, bool test);
    void updateChar(float time);
    Arrive arrive;
    Align align;
    EatFruit eatFruit;
    Dance dance;
    

    
};