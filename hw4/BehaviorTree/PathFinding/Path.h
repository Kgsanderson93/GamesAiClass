#pragma once
#include "../World/gameTile.h"
#include "../character/kinematic.h"
#include "../World/gameWorld.h"

class Path{
    public:
    std::vector<gameTile *> returnedpath;
    int idx;
    void makePath(Kinematic *cur, gameTile *t, GameWorld *game);

    std::unordered_map<gameTile *, gameTile *> came_from;
    std::unordered_map<gameTile *, gameTile *> *came_fromptr;
    std::unordered_map<gameTile *, float> cost_so_far;
    std::unordered_map<gameTile *, float> *cost_so_farptr = &cost_so_far;
    //std::vector<gameTile *> returnedpath = std::vector<gameTile *>();
};