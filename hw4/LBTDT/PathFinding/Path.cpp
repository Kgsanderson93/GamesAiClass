
#include "Path.h"

void Path::makePath(Kinematic *curr, gameTile *g, GameWorld *game)
{
    came_from.clear();
    cost_so_far.clear();
    cost_so_farptr = &cost_so_far;
    came_fromptr = &came_from;
    returnedpath.clear();
    idx=1;
    //std::cout<<"Make path\n";
    gameTile *k=game->getTile(curr->sprite.getPosition());
    aStarRef(game->tileList, game->map, k->getID(), g->getID(), came_from, cost_so_far, true, true, false);
    //std::cout<<"path maker\n";
    make_pathRef(k, g, came_from, returnedpath);
    //std::cout<<"return from path\n";
    //prettyprintRef(returnedpath);


    
}