#include "TreeFunctions.h"
#include "../utilities/util.h"
#include "../World/gameTile.h"
typedef std::pair<int, fruit *> piRef;

bool FruitVis::makeDecision(Kinematic *cur, GameWorld *game)
{
    //std::cout << ("In Fruit Vis\n");
    std::unordered_map<gameTile *, gameTile *> came_from = std::unordered_map<gameTile *, gameTile *>();
    std::unordered_map<gameTile *, gameTile *> *came_fromptr = &came_from;
    std::unordered_map<gameTile *, float> cost_so_far = std::unordered_map<gameTile *, float>();
    std::unordered_map<gameTile *, float> *cost_so_farptr = &cost_so_far;
    std::vector<gameTile *> returnedpath = std::vector<gameTile *>();
    while (!game->goals.empty())
    {
        //std::cout << ("In Fruit Vis 1\n");
        came_from.clear();
        cost_so_far.clear();
        returnedpath.clear();
        gameTile *goal = game->goals.top().second->location;
        gameTile *start = game->getTile(cur->_position);
        if(start->getID()== goal->getID()){
            return true;
        }
        aStarRef(game->tileList, game->map, start->getID(), goal->getID(), came_from, cost_so_far, true, true, true);
        //std::cout << ("In Fruit Vis 2\n");

        make_pathRef(start, goal, came_from, returnedpath);
        //std::cout << ("in fruit vis 3\n");
        if (!returnedpath.empty())
        {
            //std::cout << ("%ld\n", returnedpath.size());
            if (returnedpath.size() < 15)
            {
                //std::cout << "Found fruit vis \n";
                return true;
            }
        }
        game->goals.pop();
    }
    //std::cout << ("In Fruit Vis 4\n");

    return false;
}
bool DoneEating::makeDecision(Kinematic *cur, GameWorld *game)
{
    //std::cout << ("In Done Eating \n");
     if (cur->doneEating == true)
    {
        //std::cout << "done eating true\n";

        return true;
    }
    else
    {
        //std::cout << "done eating false\n";

        return false;
    }
}
bool CanReachFruit::makeDecision(Kinematic *cur, GameWorld *game)
{
    //std::cout << ("in can reach\n");
    if (game->goals.empty())
    {
        //std::cout << ("no goals shouldnt be here\n");
        return false;
    }
    //std::cout<<"dist to fruit: "<<distancelength(cur->_position, game->goals.top().second->coords)<<"\n";
    if (inRadiusSat(distancelength(cur->_position, game->goals.top().second->coords)))
    {

        //std::cout << ("yes goals and close enough\n");
        cur->isEating = true;
        cur->targetFruit = game->goals.top().second;

        cur->targetFruit->timesinceupdate = 0;
        return true;
    }
    else
    {
        //std::cout << ("not close enough yet\n");
        return false;
    }
}
bool ActionComplete::makeDecision(Kinematic *cur, GameWorld *game)
{
    //std::cout << ("In action complete?\n");
    if (game->goals.empty())
    {
        //std::cout << ("no goals?\n");
    }
    else
    {
        //std::cout << ("yes goals\n");
    }

    if (cur->isEating)
    {
        //std::cout << ("eating\n");
    }
    else
    {
        //std::cout << ("not eating\n");
    }

    if (cur->isEating)
    {
        if (cur->targetFruit->frame.left < 900)
        {

            //std::cout << ("not done ")<<cur->targetFruit->frame.left<<"\n";
            return false;
        }else{
            cur->isEating=false;
            int i=0;
            int offset;
            
            game->fruitList.remove(cur->targetFruit);
            cur->doneEating=true;
            //std::cout<<"done eating\n";
            return true;
        }
    }
    else if (cur->dancing > 0)
    {
        //std::cout << ("not done 1\n");

        return false;
    }
    //std::cout << ("all done\n");

    return true;
}
bool FruitExist::makeDecision(Kinematic *cur, GameWorld *game)
{
    //std::cout << ("in fruit exist\n");
    if (game->fruitList.empty())
    {
        //std::cout << ("no fruit\n");
        return false;
    }
    else
        //std::cout << "FruitExist true\n";

    return true;
}

bool InCorner::makeDecision(Kinematic *cur, GameWorld *game)
{
    //std::cout << ("in corner\n");
    gameTile *t = game->getTile(cur->_position);
    if (t->idCol < 3 || t->idCol > 29 || t->idRow < 3 || t->idCol > 29)
    {
        //std::cout << ("oh yeah in corner\n");
        return true;
    }
    else
        //std::cout << ("not in corner\n");

    return false;
}
