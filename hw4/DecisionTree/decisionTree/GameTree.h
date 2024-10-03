#pragma once
#include "GameTreeNode.h"

class GameTree
{
public:
    Kinematic *cur;
    GameWorld *game;
    GameTreeDecision *root;
    GameTreeAction *lastAction;
    std::vector<GameTreeDecision *> decisions;
    std::vector<GameTreeAction *> actions;
    GameTree(Kinematic *curr, GameWorld *game);
    void updateTree(Kinematic *cur, GameWorld *game, float time);
};