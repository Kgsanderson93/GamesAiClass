#pragma once
#include "kinematic.h"
#include "../behaviorTree/Behavior.h"

class Mob{
public:
    Kinematic *movement;
    BehaviorTree *tree;
    GameWorld *game;Kinematic *goal; Kinematic *player;
    Mob(Kinematic * movement, GameWorld *game,Kinematic *goal, Kinematic *player );
    void tick(float time, GameWorld *game, Kinematic *player);

};