#pragma once
#include "kinematic.h"
#include "../Data/LearnedTree.h"

class Mob{
public:
    Kinematic *movement;
    LearnedTree *tree;
    GameWorld *game;Kinematic *goal; Kinematic *player;
    Mob(LearningTree *lt,Kinematic * movement, GameWorld *game,Kinematic *goal, Kinematic *player );
    void tick(float time, GameWorld *game, Kinematic *player);
    LearningTree *lt;

};